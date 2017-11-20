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
#include<conio.h>

int main(){

  bank::Bank b;
  b.fillAccountsDatabaseInRange(12, 75, 100);
  b.fillCashDatabaseInRange(1, 35, 10);
  b.fillCashDatabaseInRange(35, 200, 15);

  std::cout << "Bills bank:" << std::endl;
  b.cashAvailablilityReport();

  std::cout << std::endl << "Bank opened. Transactions history:" << std::endl;
  b.start(1, 20, 45);

  /*std::string buf;
  getline(std::cin, buf);*/
  

  std::getchar();
 

  return 0;
}
