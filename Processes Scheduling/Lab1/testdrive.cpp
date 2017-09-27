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

  os::Dispatcher dispacher(1, 10, 5);

  dispacher.process(500);

  dispacher.reportInOrder(std::cout);
  std::cout.flush();

  return 0;
}
