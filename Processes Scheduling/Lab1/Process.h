/*
 * `file`          Process.h
 * `written`       September 26 2017 20:11:18
 * `last modified` September 26 2017 23:38:15
 * 
 * Author:         R. Neshta
 * Contact:        Ruslan.Neshta@gmail.com
 * 
 * `synopsis`      defines process class
 * 
 * Description:    process class represents system process
 *                 
 * 
 * `note`          none
 */
 

#ifndef _PROCESS_H_
#define _PROCESS_H_

#include<ostream>

namespace os{

  class Process
  {
  /* 
   * `class           Process
   * 
   * `in              os
   * 
   * `synopsis        process class
   * 
   * DESCRIPTION:     holds time of constructing/destructing etc.
   *                  
   * 
   * `notes           none
   * 
   * `author          R. Neshta
   * 
   * `version         1.0
   * 
   * `date            September 26 2017 20:17:24
   * 
   * Contact:         Ruslan.Neshta@gmail.com
   * 
   */

    static unsigned m_counter;

    int m_arrived;
    int m_complexity;
    int m_delayed;
    int m_lastVisitedAt;
    int m_number;
    int m_served;
    int m_solved;

  public:
    Process(int complexity, int arrived = 0);
    ~Process() = default;

    void operator()(int time){ m_delayed += time - m_lastVisitedAt; m_served += m_lastVisitedAt = time; }
    bool solved()const{ return m_served < m_complexity; }
    void shutDown(){ m_solved = m_lastVisitedAt; }

    void report(std::ostream& stream);
  };

}

#endif /* _PROCESS_H_ */
