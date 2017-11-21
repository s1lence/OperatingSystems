/*
 * `file`          entry.cpp
 * `written`       November 14 2017 15:00:25
 * `last modified` November 20 2017 23:33:42
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

#ifdef DEBUG


#include<iostream>
#include<string>

#endif /* DEBUG */

#include<conio.h>

int main(){

  bank::Bank b;
  b.fillAccountsDatabaseInRange(12, 75, 100);
  b.fillCashDatabaseInRange(3, 35, 5);
  b.fillCashDatabaseInRange(35, 200, 25);

#ifdef DEBUG

  std::cout << std::endl << "Press any key to exit.\nBank opened. Transactions history:" << std::endl;
  
#endif /* DEBUG */

  b.start(1, 20, 60);

  std::getchar();

  return 0;
}
