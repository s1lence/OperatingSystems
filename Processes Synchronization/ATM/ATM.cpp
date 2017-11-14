/*
 * `file`          ATM.cpp
 * `written`       November 14 2017 22:27:44
 * `last modified` November 14 2017 22:27:44
 * 
 * Author:         R. Neshta
 * Contact:        Ruslan.Neshta@gmail.com
 * 
 * `synopsis`      defines atm
 * 
 * Description:    heavy atm functions implemented here
 *                 
 * 
 * `note`          none
 */
 
#include "ATM.h"

bool atm::ATM::issueCash(int account, int amount){
   /* check if we have enough money to pay */
  if (getCash(amount)){
    
    m_accsDBase[account] -= amount;
    return true; /* succeeded */
  }

  return false; /* fails */
}

bool atm::ATM::getCash(int sum){



  return true;
}
