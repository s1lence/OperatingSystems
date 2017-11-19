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

bool atm::ATM::issueCash(int account, int amount)
{
   /* check if we have enough money to pay */
  if (getCash(amount)){
    
    m_accsDBase[account] -= amount;
    return true; /* succeeded */
  }

  return false; /* fails */
}

bool atm::ATM::getCash(int sum)
{
  std::array<std::pair<int, int>, 7> cash = {
    std::make_pair(100, 0), std::make_pair(50, 0), std::make_pair(20, 0), std::make_pair(10, 0), std::make_pair(5, 0), std::make_pair(2, 0), std::make_pair(1, 0)
  };

  for (auto &it : cash)
    sum -= (sum / it.first > m_cashDBase[it.first] ? it.second = m_cashDBase[it.first] : it.second = sum / it.first, it.first*it.second);
  
  if (sum) return false; /* not enough cash */

  for (auto &it : cash)
    m_cashDBase[it.first] -= it.second;

  return true;
}
