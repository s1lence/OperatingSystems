/*
 * `file`          ATM.h
 * `written`       November 14 2017 14:58:10
 * `last modified` November 14 2017 14:58:10
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
      
     typedef terminal::Terminal<ATM>  _m_terminal_t;
     using _m_database_t = std::map<int, int>;

     _m_terminal_t  m_first;
     _m_terminal_t  m_second;

     _m_database_t  m_accsDBase;
     _m_database_t  m_cashDBase;

     int            m_queue;

   public:

      /* terminal management */

     bool isMyTurn(_m_terminal_t* whoAsks) const{ return m_queue == (whoAsks == &m_first ? m_first.queueState() : m_second.queueState()); }

      /* queue management */

     void setQueueState(int value){ m_queue = value; }
     int  getQueueState() const{ return m_queue; }
     
     int getNextQueueState(_m_terminal_t *terminal) const{ return terminal == &m_first ? m_second.queueState() : m_first.queueState(); }

      /* account management */

     void addNewAccount(int account, int cash){ m_accsDBase[account] = cash; }

      /* cash management */

     bool proceedRequest(int const * account, int amount){ return m_accsDBase.find(*account) == m_accsDBase.end() || m_accsDBase[*account] < amount ? false : issueCash(amount); }

     bool issueCash(int amount);

   };
 
 }
 
 #endif /* _ATM_H_ */
