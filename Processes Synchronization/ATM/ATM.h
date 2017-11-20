/*
 * `file`          ATM.h
 * `written`       November 14 2017 14:58:10
 * `last modified` November 20 2017 23:33:42
 * 
 * Author:         R. Neshta
 * Contact:        Ruslan.Neshta@gmail.com
 * 
 * `synopsis`      defines atm
 * 
 * Description:    atm model
 *                 
 * 
 * `note`          none
 */
 
 #ifndef _ATM_H_
 #define _ATM_H_

#include "Common.h"
#include "Terminal.h"

#include<array>
#include<map>

 namespace atm{
 
   class ATM{
     
     /* 
      * `class           ATM
      * 
      * `in              atm
      * 
      * `synopsis        defines atm interface
      * 
      * DESCRIPTION:     atm model
      *                  handles all required atm stuff
      * 
      * `notes           none
      * 
      * `author          R. Neshta
      * 
      * `version         1.0
      * 
      * `date            November 14 2017 15:01:23
      * 
      * Contact:         Ruslan.Neshta@gmail.com
      * 
      */
      
     using _m_terminal_t = terminal::Terminal<ATM>;
     using _m_database_t = std::map<int, int>;

     _m_terminal_t *  m_first;
     _m_terminal_t *  m_second;

     _m_database_t    m_accsDBase;
     _m_database_t    m_cashDBase;

     int              m_queue;

   public:

      /* ctors/dtors */

     ATM() = delete;

      /* atm requires at least two terminals or existing atm */
     ATM(_m_terminal_t *first, _m_terminal_t *second) :m_first(first), m_second(second), m_queue(m_first->queueState()){}
     ATM(const ATM &atm) :ATM(atm.m_first, atm.m_second){}

     ~ATM() = default;

     ATM& operator=(const ATM &atm);

      /* terminal management */

     bool isResourceBusy(_m_terminal_t const* whoAsks) const{ return (whoAsks == m_first ? m_second->attempted() : m_first->attempted()); }

      /* queue management */

     void setQueueState(int value){ m_queue = value; }
     int  getQueueState() const{ return m_queue; }
     
     int getNextQueueState(_m_terminal_t const *terminal) const{ return terminal == m_first ? m_second->queueState() : m_first->queueState(); }

      /* account management */

     void addNewAccount(int account, int cash){ m_accsDBase[account] = cash; }

     void receiveCash(int bill, int amount){ m_cashDBase[bill] += amount; }

      /* cash management */
#ifdef _DEBUG

     bool proceedRequest(int const * account, int amount);

     void reportCashDBase(std::ostream& str) const{ for (auto i : m_cashDBase) str << i.first << " UAH ~ " << i.second << " bills; "; }
#else

     bool proceedRequest(int const * account, int amount){ return m_accsDBase.find(*account) == m_accsDBase.end() || m_accsDBase[*account] < amount ? false : issueCash(*account, amount); }
#endif // _DEBUG

     bool issueCash(int account, int amount); /* handles cash issues */

   private:
     bool getCash(int sum); /* manages cash amount */

   };
 
 }
 
 #endif /* _ATM_H_ */
