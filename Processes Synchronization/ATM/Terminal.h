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

     /* console report */
     std::cout << '[' << round << ']' << "Terminal " << m_queueState << "uses atm: ";

      /* critical section */
     bool result = const_cast<AbstractATM*>(m_atm)->proceedRequest(&account, amount);

      /* release the resource */
     m_attempt = 0;
     const_cast<AbstractATM*>(m_atm)->setQueueState(m_atm->getNextQueueState(this));

     return result;
   }

 }
 
 #endif /* _TERMINAL_H_ */
