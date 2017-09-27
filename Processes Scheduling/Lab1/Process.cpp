/*
* `file`          Process.cpp
* `written`       September 26 2017 20:11:18
* `last modified` September 26 2017 23:38:15
*
* Author:         R. Neshta
* Contact:        Ruslan.Neshta@gmail.com
*
* `synopsis`      defines process class
*
* Description:    heawy functions of os::Process implemented here
*
*
* `note`          none
*/

#include "Process.h"

unsigned os::Process::m_counter;

os::Process::Process(int complexity, int arrived /*= 0*/) :m_complexity(complexity), m_arrived(arrived), 
m_number(++m_counter), m_delayed(0), m_solved(0), m_served(0), m_lastVisitedAt(0){}


os::Process::Process(const Process& rhs)
  :m_arrived(rhs.m_arrived), m_complexity(rhs.m_complexity), m_delayed(rhs.m_delayed), m_lastVisitedAt(rhs.m_lastVisitedAt),
  m_number(rhs.m_number), m_served(rhs.m_served), m_solved(rhs.m_solved){}

int os::Process::operator()(int time, int clock){
  m_delayed += time - m_lastVisitedAt; 
  m_served += clock;
  int delta = m_complexity < m_served ? clock - m_complexity + m_served : clock;
  m_lastVisitedAt = time + delta;
  return delta;
}

void os::Process::shutDown(){
  m_solved = m_lastVisitedAt;
  m_delayed -= m_arrived;
}

void os::Process::report(std::ostream& stream){
  stream << m_number << "\t\t" << m_arrived << "\t\t" << m_complexity << "\t\t" << m_solved << "\t\t" << m_delayed << std::endl;
}
