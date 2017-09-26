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

os::Process::Process(int complexity, int arrived /*= 0*/) :m_complexity(complexity), m_arrived(arrived), m_number(++m_counter), m_delayed(0), m_solved(0), m_served(0), m_lastVisitedAt(0)
{

}


void os::Process::report(std::ostream& stream)
{
  stream << m_number << " " << m_arrived << " " << m_complexity << " " << m_solved << " " << m_delayed << std::endl;
}
