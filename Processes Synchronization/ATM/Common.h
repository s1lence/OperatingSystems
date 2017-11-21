/*
 * `file`          Common.h
 * `written`       November 20 2017 21:21:58
 * `last modified` November 20 2017 23:33:42
 * 
 * Author:         R. Neshta
 * Contact:        Ruslan.Neshta@gmail.com
 * 
 * `synopsis`      defines common options
 * 
 * Description:    a few common definitions
 *                 
 * 
 * `note`          none
 */
 

#ifndef _COMMON_H_
#define _COMMON_H_

#define RICHNESS_COEFFICIENT 4
#define BILLS_AVAILABILITY_COEFFICIENT 7
#define GREEDINESS_COEFFICIENT 6

#ifdef _DEBUG
#define DEBUG

#define BILLS_REPORT
#define MONEY_REPORT

#endif /* _DEBUG */

#endif /* _COMMON_H_ */
