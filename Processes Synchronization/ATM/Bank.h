/*
 * `file`          Bank.h
 * `written`       November 14 2017 18:54:24
 * `last modified` November 20 2017 23:33:42
 * 
 * Author:         R. Neshta
 * Contact:        Ruslan.Neshta@gmail.com
 * 
 * `synopsis`      defines bank
 * 
 * Description:    bank model
 *                 
 * 
 * `note`          none
 */
 
 #ifndef _BANK_H_
 #define _BANK_H_


#include "ATM.h"

#include<future>
#include<random>
 
 namespace bank{
 
   class Bank{
     
     /* 
      * `class           Bank
      * 
      * `in              bank
      * 
      * `synopsis        defines bank interface
      * 
      * DESCRIPTION:     bank model
      *                  handles all required bank stuff
      *                  manages atm-terminal communication
      * 
      * `notes           none
      * 
      * `author          R. Neshta
      * 
      * `version         1.0
      * 
      * `date            November 14 2017 18:55:07
      * 
      * Contact:         Ruslan.Neshta@gmail.com
      * 
      */
      
     using _atm_t = atm::ATM;
     using _trm_t = terminal::Terminal<_atm_t>;
     using _rndgn = std::mt19937;
     using _dstrb = std::uniform_int_distribution<>;

     _atm_t   m_atm;
     _trm_t   m_trm_1;
     _trm_t   m_trm_2;

   public:

      /* ctors/dtors */

     Bank() :m_atm(&m_trm_1, &m_trm_2), m_trm_1(&m_atm, 1), m_trm_2(&m_atm, 2){}
     Bank(const Bank&) = delete;

     ~Bank() = default;

      /* atm tests */

     void init(int min, int max);

     void fillAccountsDatabasse(_dstrb *distribution, _rndgn *generator, int rounds);

     void fillCashDatabase(_dstrb *distribution, _rndgn *generator, int rounds);

     void fillAccountsDatabaseInRange(int min, int max, int rounds);

     void fillCashDatabaseInRange(int min, int max, int rounds);

     void start(int min, int max, int rounds);

#ifdef _DEBUG

     void cashAvailablilityReport() const{ m_atm.reportCashDBase(std::cout); std::cout << std::endl; }

#endif // _DEBUG

   };
 
 }
 
 #endif /* _BANK_H_ */
