/*
 * `file`          entry.cpp
 * `written`       November 14 2017 15:00:25
 * `last modified` November 14 2017 15:00:25
 * 
 * Author:         R. Neshta
 * Contact:        Ruslan.Neshta@gmail.com
 * 
 * `synopsis`      program entry point
 * 
 * Description:    defines test case for atm/terminal model
 *                 
 * 
 * `note`          none
 */
 
#include "Bank.h"

#include<string>

int main(){

  bank::Bank b;
  b.init(0, 100);
  b.start(0, 100, 100);

  std::string buf;
  getline(std::cin, buf);

  return 0;
}
