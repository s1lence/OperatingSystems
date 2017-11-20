/*
 * `file`          Bank.cpp
 * `written`       November 19 2017 17:20:24
 * `last modified` November 20 2017 23:33:42
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

  fillAccountsDatabasse(&distribution, &generator, max);
  fillCashDatabase(&distribution, &generator, max);
}

void bank::Bank::fillAccountsDatabasse(_dstrb *distribution, _rndgn *generator, int rounds)
{
  while (rounds--)
    m_atm.addNewAccount(rounds, (*distribution)(*generator) * RICHNESS_COEFFICIENT);
}

void bank::Bank::fillCashDatabase(_dstrb *distribution, _rndgn *generator, int rounds)
{
  std::array<int, 7> cash = { 100, 50, 20, 10, 5, 2, 1 };
  
  while (rounds -= BILLS_AVAILABILITY_COEFFICIENT, rounds > 0){

    for (auto i : cash)
      m_atm.receiveCash(i, (*distribution)(*generator) / i);
  }
}

void bank::Bank::fillAccountsDatabaseInRange(int min, int max, int rounds)
{
  _rndgn   generator;
  _dstrb   distribution(min, max);

  generator.seed(std::random_device()());

  while (rounds--)
    m_atm.addNewAccount(rounds, distribution(generator) * RICHNESS_COEFFICIENT);
}

void bank::Bank::fillCashDatabaseInRange(int min, int max, int rounds)
{
  _rndgn   generator;
  _dstrb   distribution(min, max);

  generator.seed(std::random_device()());
  std::array<int, 7> cash = { 100, 50, 20, 10, 5, 2, 1 };

  while (rounds -= BILLS_AVAILABILITY_COEFFICIENT, rounds > 0){

    for (auto i : cash)
      m_atm.receiveCash(i, distribution(generator) / i);
  }
}

void bank::Bank::start(int min, int max, int rounds)
{
  _rndgn   generator;
  _dstrb   distribution(min, max);

  generator.seed(std::random_device()());

  auto r1 = std::async(std::launch::async, [&, rounds]()mutable{while (rounds--) m_trm_1.withdrawCash(distribution(generator)*GREEDINESS_COEFFICIENT, distribution(generator), rounds + 1); });
  auto r2 = std::async(std::launch::async, [&, rounds]()mutable{while (rounds--) m_trm_2.withdrawCash(distribution(generator)*GREEDINESS_COEFFICIENT, distribution(generator), rounds + 1); });
}
