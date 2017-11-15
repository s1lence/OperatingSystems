/*
 * `file`          Bank.h
 * `written`       November 14 2017 18:54:24
 * `last modified` November 14 2017 18:54:24
 * 
 * Author:         R. Neshta
 * Contact:        Ruslan.Neshta@gmail.com
 * 
 * `synopsis`      defines bank
 * 
 * Description:    bank model
 *                 
 * 
 * `note`          none
 */
 
 #ifndef _BANK_H_
 #define _BANK_H_

#include "ATM.h"
 
 namespace bank{
 
   class Bank{
     
     /* 
      * `class           Bank
      * 
      * `in              bank
      * 
      * `synopsis        defines bank interface
      * 
      * DESCRIPTION:     bank model
      *                  handles all required bank stuff
      *                  manages atm-terminal communication
      * 
      * `notes           none
      * 
      * `author          R. Neshta
      * 
      * `version         1.0
      * 
      * `date            November 14 2017 18:55:07
      * 
      * Contact:         Ruslan.Neshta@gmail.com
      * 
      */
      
     using _atm_t = atm::ATM;
     using _trm_t = terminal::Terminal<_atm_t>;

     _atm_t   m_atm;
     _trm_t   m_trm_1;
     _trm_t   m_trm_2;


   };
 
 }
 
 #endif /* _BANK_H_ */
