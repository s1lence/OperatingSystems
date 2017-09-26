/*
 * `file`          testdrive.cpp
 * `written`       September 26 2017 23:38:47
 * `last modified` September 26 2017 23:38:47
 * 
 * Author:         R. Neshta
 * Contact:        Ruslan.Neshta@gmail.com
 * 
 * `synopsis`      entry point
 * 
 * Description:    testing process dispatcher
 *                 
 * 
 * `note`          none
 */
 
#include "Dispatcher.h"

#include<iostream>

int main(){

  os::Dispatcher dispacher(5, 12, 10);

  dispacher.initQueues();
  dispacher.process(200);

  dispacher.report(std::cout);

  return 0;
}
