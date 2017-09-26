/*
 * `file`          Dispatcher.cpp
 * `written`       September 26 2017 21:29:12
 * `last modified` September 26 2017 23:38:15
 * 
 * Author:         R. Neshta
 * Contact:        Ruslan.Neshta@gmail.com
 * 
 * `synopsis`      implementations of os::Dispatcher
 * 
 * Description:    heawy functions of os::Dispatcher implemented here
 *                 
 * 
 * `note`          none
 */
 
#include "Dispatcher.h"

void os::Dispatcher::initQueues(int time){
  int randomValue = getRandom();

  for (int n = 0; n < randomValue; ++n){
    if (getRandom() % 2 == 0)
      m_queues[0].push(Process(getRandom(), time));
  }

}

void os::Dispatcher::process(int time){

  m_queues[0].push(Process(getRandom())); /* initialize queue with at least one process */
  initQueues();

  for (int i = 0; i < time; i += m_clock){ /* all time range */

    /*
     *	here we need to decide if we add new process to queue
     *	and how many processes can be added at one time
     */
    initQueues(i);

    /*int randomChance = getRandom();
    for (int n = 0; n < randomChance; ++n){

      if (getRandom() % 2 == 0)
        m_queues[0].push(Process(getRandom(), time));
    }
*/
    for (int n = 0; n < 4; ++n){ /* through all queues */
      
      if (!m_queues[n].empty()){ /* process if not empty */
        m_queues[n].front()(m_clock);

        if (m_queues[n].front().solved()){ /* save result if solved */
          m_queues[n].front().shutDown();

          m_queues[4].push(m_queues[n].front()); /* last queue holds results */
          m_queues[n].pop();

        } else { /* if not solved and not the 'worst' queue */

          if (n != 3){ /* send task to 'worse' queue */
            m_queues[n + 1].push(m_queues[n].front());
            m_queues[n].pop();
          }
        }
      }
    }
  }
    /*  cycle(i);*/
}

void os::Dispatcher::report(std::ostream& stream){
  while (!m_queues[4].empty()){
    m_queues[4].front().report(stream);
  }
}

//
//void os::Dispatcher::cycle(int time){
//  /*for (int i = 0; i < 4; ++i)
//    pass(m_queues[i]);*/
//  /*for (int i = 0; i < 4; ++i){
//    m_queues[i].front()
//  }*/
//}
//
//void os::Dispatcher::pass(std::queue<os::Process>& Q){
//
//}
//
//void os::Dispatcher::halt(){
//
//}
