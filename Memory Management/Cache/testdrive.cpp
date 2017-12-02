/*
 * `file`          testdrive.cpp
 * `written`       December 02 2017 13:38:56
 * `last modified` December 02 2017 13:38:56
 * 
 * Author:         R. Neshta
 * Contact:        Ruslan.Neshta@gmail.com
 * 
 * `synopsis`      program entry
 * 
 * Description:    testing cache model
 *                 
 * 
 * `note`          none
 */
 
#include "Cache.h"


int main(){

  win32::Cache<> cache;

  win32::dword arr[112];
  for (win32::dword i=0;i<112;++i)
    arr[i] = i;

  cache.initMemory(arr, 112);
  cache.stepByStepDebugTest(112);

  return 0;
}
