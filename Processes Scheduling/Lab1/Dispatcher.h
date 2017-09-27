/*
 * `file`          Dispatcher.h
 * `written`       September 26 2017 20:54:59
 * `last modified` September 26 2017 23:38:15
 * 
 * Author:         R. Neshta
 * Contact:        Ruslan.Neshta@gmail.com
 * 
 * `synopsis`      dispatcher interface
 * 
 * Description:    interface of dispatcher class
 *                 
 * 
 * `note`          none
 */
 
 #ifndef _DISPATCHER_H_
 #define _DISPATCHER_H_


#include "Process.h"

#include<algorithm>
#include<ostream>
#include<queue>
#include<random>
#include<vector>

 namespace os{
 
   class Dispatcher
   {
   /* 
    * `class           Dispatcher
    * 
    * `in              os
    * 
    * `synopsis        dispatcher class
    * 
    * DESCRIPTION:     manages all processes and processor time
    *                  
    * 
    * `notes           none
    * 
    * `author          R. Neshta
    * 
    * `version         1.0
    * 
    * `date            September 26 2017 20:55:48
    * 
    * Contact:         Ruslan.Neshta@gmail.com
    * 
    */
    
     int                             m_minValue;
     int                             m_maxValue;
     std::mt19937                    m_generator;
     std::uniform_int_distribution<> m_distribution;

     std::queue<os::Process>         m_queues[5];
     int                             m_clock;

     const int maximum = 100;

   public:
     Dispatcher(int min, int max, int clock = 2) :m_minValue(min), m_maxValue(max), m_clock(clock), m_distribution(m_minValue, m_maxValue){ m_generator.seed(std::random_device()()); }
     ~Dispatcher() = default;

     int getRandom(){ return m_distribution(m_generator); }
     
     void setClock(int clock){ m_clock = clock; }

     void initQueues(int&, int = 0);

     void process(int);

     void report(std::ostream& stream);

     void reportInOrder(std::ostream& stream);

     bool isTasksAvaliable()const;

   };
 
 }
 
 #endif /* _DISPATCHER_H_ */
