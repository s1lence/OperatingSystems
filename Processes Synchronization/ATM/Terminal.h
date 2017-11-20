/*
 * `file`          Terminal.h
 * `written`       November 14 2017 14:57:06
 * `last modified` November 14 2017 14:57:06
 * 
 * Author:         R. Neshta
 * Contact:        Ruslan.Neshta@gmail.com
 * 
 * `synopsis`      defines terminal class
 * 
 * Description:    terminal model
 *                 
 * 
 * `note`          templated class
 */
 
 #ifndef _TERMINAL_H_
 #define _TERMINAL_H_

#ifdef _DEBUG

#include<iomanip>

#ifdef BILLS_REPORT
#include<sstream>
#endif // BILLS_REPORT

#endif // _DEBUG


#include<iostream>
 
 namespace terminal{
 
   template<class AbstractATM>
   class Terminal{
     
     /* 
      * `class           Terminal
      * 
      * `in              terminal
      * 
      * `synopsis        defines terminal interface
      * 
      * DESCRIPTION:     terminal model
      *                  handles all required terminal stuff
      * 
      * `notes           AbstractATM represents ATM that terminal works with
      * 
      * `author          R. Neshta
      * 
      * `version         1.0
      * 
      * `date            November 14 2017 15:02:12
      * 
      * Contact:         Ruslan.Neshta@gmail.com
      * 
      */
      
     AbstractATM const *  m_atm;
     int mutable          m_attempt;
     
     const int            m_queueState;

   public:
      /* prohibited stuff */
     Terminal() = delete;
     Terminal(const Terminal&) = delete;

      /* object creation requires atm & queue state */
     Terminal(AbstractATM const *atm, int queueState) :m_atm(atm), m_attempt(0), m_queueState(queueState){}

     ~Terminal() = default;

      /* Account represents account type that m_atm can work with */
     template<class Account>
     bool withdrawCash(int amount, Account const && account, int round) const;

      /* returns true if terminal tries to process the request */
     bool attempted() const{ return 0 != m_attempt; }

      /* returns queue state for this terminal */
     int queueState() const{ return m_queueState; }
   };

   
   template<class AbstractATM>
   template<class Account>
   bool terminal::Terminal<AbstractATM>::withdrawCash(int amount, Account const && account, int round) const{
     
     m_attempt = 1;

      /* Dekker's algorithm */
     while (m_atm->isResourceBusy(this)){
       
       if (m_queueState != m_atm->getQueueState()){
         m_attempt = 0;
         
         while (m_queueState != m_atm->getQueueState());
         m_attempt = 1;
       }
     }

#ifdef _DEBUG
     /* console report */
     std::cout <<"_________________________________________________________________________________________________________________________________________\n" <<std::endl;
   
#ifdef BILLS_REPORT

     std::stringstream tmp;
     m_atm->reportCashDBase(tmp); 

#endif // BILLS_REPORT

     std::cout << std::endl << std::endl;
     std::cout << '[' << std::setw(3) << round << std::setw(2) << ']' << " Terminal #" << m_queueState << " uses atm: ";

#endif // _DEBUG

      /* critical section */
     bool result = const_cast<AbstractATM*>(m_atm)->proceedRequest(&account, amount);

#ifdef _DEBUG

#ifdef BILLS_REPORT

     std::cout << "\nBills bank before transaction:\n" << tmp.str();
     std::cout << "\n\nBills bank after transaction:\n";

     m_atm->reportCashDBase(std::cout);
     std::cout << std::endl;

#endif // BILLS_REPORT

     std::cout << "\n_________________________________________________________________________________________________________________________________________" << std::endl;
#endif // _DEBUG

      /* release the resource */
     m_attempt = 0;
     const_cast<AbstractATM*>(m_atm)->setQueueState(m_atm->getNextQueueState(this));

     return result;
   }

 }
 
 #endif /* _TERMINAL_H_ */
