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

void os::Dispatcher::initQueues(int& added, int time){
  int randomValue = getRandom();

  for (int n = 0; n < randomValue; ++n){
    if (getRandom() % 2 == 0){
      m_queues[0].push(Process(getRandom(), time));
      ++added;
    }
  }

}

void os::Dispatcher::process(int time){

  m_queues[0].push(Process(getRandom())); /* initialize queue with at least one process */
  int counter = 0;
  bool tasksLimited = false;
  initQueues(counter);

  for (int i = 0, n = 0; i < time; ){ /* all time range */

    /*
     *	here we need to decide if we add new process to queue
     *	and how many processes can be added at one time
     */
    if (counter < maximum)
      initQueues(counter, i);
    else
      tasksLimited = true;
      
    if (!m_queues[n].empty()){ /* process if not empty */
      i += m_queues[n].front()(i, m_clock);

      if (m_queues[n].front().solved()){ /* save result if solved */
        m_queues[n].front().shutDown();

        m_queues[4].push(m_queues[n].front()); /* last queue holds results */
        m_queues[n].pop();

      } else { /* if not solved and not the 'worst' queue */

        if (n != 3){ /* send task to 'worse' queue */
          m_queues[n + 1].push(m_queues[n].front());
          m_queues[n].pop();
        } else { /* send task to the end of current queue */
          m_queues[n].push(m_queues[n].front());
          m_queues[n].pop();
        }
      }

    } else {
      if (tasksLimited && !isTasksAvaliable())
        return;

      n == 3 ? n = 0 : ++n;
    }
  }
}

void os::Dispatcher::report(std::ostream& stream){
  stream << "Process number\tTime arrived\tComlexity\tTime solved\tTime delayed"<< std::endl;

  while (!m_queues[4].empty()){
    m_queues[4].front().report(stream);
    m_queues[4].pop();
  }
}

void os::Dispatcher::reportInOrder(std::ostream& stream){
  stream << "Process number\tTime arrived\tComlexity\tTime solved\tTime delayed" << std::endl;
  int sum = 0, count = m_queues[4].size();

  std::vector<os::Process> values;
  values.reserve(count);

  while (!m_queues[4].empty()){
    values.push_back(m_queues[4].front());
    m_queues[4].pop();
  }
  std::sort(values.begin(), values.end());

  for (auto i : values){
    i.report(stream);
    sum += i.getDelay();
  }

  stream << "Average delay in queues for one process is equal " << sum / count << "." << std::endl;
}

bool os::Dispatcher::isTasksAvaliable() const{
  for (int i = 0; i < 3; ++i)
    if (!m_queues[i].empty())
      return true;

  return false;
}
