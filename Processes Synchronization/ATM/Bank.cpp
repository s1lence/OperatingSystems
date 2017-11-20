/*
 * `file`          Bank.cpp
 * `written`       November 19 2017 17:20:24
 * `last modified` November 19 2017 17:20:24
 * 
 * Author:         R. Neshta
 * Contact:        Ruslan.Neshta@gmail.com
 * 
 * `synopsis`      defines bank
 * 
 * Description:    heavy bank functions implemented here
 *                 
 * 
 * `note`          none
 */
 
#include "Bank.h"


void bank::Bank::init(int min, int max)
{
  _rndgn   generator;
  _dstrb   distribution(min, max);
  
  generator.seed(std::random_device()());

  fillAccountsDatabasse(&distribution, &generator,200);
  fillCashDatabase(&distribution, &generator,200);
}

void bank::Bank::fillAccountsDatabasse(_dstrb *distribution, _rndgn *generator, int rounds)
{
  while (rounds--)
    m_atm.addNewAccount((*distribution)(*generator), (*distribution)(*generator));
}

void bank::Bank::fillCashDatabase(_dstrb *distribution, _rndgn *generator, int rounds)
{
  std::array<int, 7> cash = { 100, 50, 20, 10, 5, 2, 1 };
  
  while(rounds--){

    for (auto i : cash)
      m_atm.receiveCash(i, (*distribution)(*generator) / i);
  }
}

void bank::Bank::start(int min, int max, int rounds)
{
  _rndgn   generator;
  _dstrb   distribution(min, max);

  generator.seed(std::random_device()());

  std::async(std::launch::async, [&, rounds]()mutable{while (rounds--) m_trm_1.withdrawCash(distribution(generator), distribution(generator), rounds + 1); });
  std::async(std::launch::async, [&, rounds]()mutable{while (rounds--) m_trm_2.withdrawCash(distribution(generator), distribution(generator), rounds + 1); });
}
