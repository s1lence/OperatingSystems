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

  std::vector<std::pair<int, int>> cash = { { 100, 0 }, { 50, 0 }, { 20, 0 }, { 10, 0 }, { 5, 0 }, { 2, 0 }, { 1, 0 } };

  for (auto &it : cash)
    sum -= (sum / it.first > m_cashDBase[it.first] ? it.second = m_cashDBase[it.first] : it.second = sum / it.first, it.first*it.second);
  
  if (sum) return false; /* not enough cash */

  for (auto &it : cash)
    m_cashDBase[it.first] -= it.second;

  return true;
}
