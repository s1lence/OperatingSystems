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

atm::ATM& atm::ATM::operator=(const ATM &atm)
{
  m_first = atm.m_first;
  m_second = atm.m_second;
  m_accsDBase = atm.m_accsDBase;
  m_cashDBase = atm.m_cashDBase;

  return *this;
}

#ifdef _DEBUG

bool atm::ATM::proceedRequest(int const * account, int amount)
{
  std::cout << "account - '" << std::setw(3) << *account << "', money - '" << std::setw(3) << m_accsDBase[*account] << "'" << ", trying to issue - '" << std::setw(3) << amount << "'";

  if (m_accsDBase.find(*account) == m_accsDBase.end()){
    std::cout << "; transaction denied: account does not exist." << std::endl;
    return false;
  }

  if (m_accsDBase[*account] < amount){
    std::cout << "; transaction denied: not enough money." << std::endl;
    return false;
  }
  bool res = issueCash(*account, amount);
  if (res)
    std::cout << "\n; transaction succeeded: issued " << amount << " UAH." << std::endl;

  return res;
}

#endif // _DEBUG

bool atm::ATM::issueCash(int account, int amount)
{  
   /* check if we have enough money to pay */
  if (getCash(amount)){
    
    m_accsDBase[account] -= amount;
    return true; /* succeeded */
  }
#ifdef _DEBUG
  std::cout << "; transaction denied: not enough bills" << std::endl;
#endif // _DEBUG

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

#ifdef _DEBUG
#ifdef MONEY_REPORT

  std::cout << "\n\nMoney issued:" << std::endl;
  
  for(auto i:cash)
    if(i.second) std::cout << i.first << " UAH ~ " << i.second << " bills; ";

  std::cout << std::endl;

#endif // MONEY_REPORT
#endif // _DEBUG
  

  for (auto &it : cash)
    m_cashDBase[it.first] -= it.second;

  return true;
}
