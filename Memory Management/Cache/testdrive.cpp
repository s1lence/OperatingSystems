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

  const int arr_size = 32;
  win32::dword arr[arr_size];

  for (win32::dword i = 0; i < arr_size; ++i)
    arr[i] = i * 78 + 0x210;

  cache.initMemory(arr, arr_size);
  cache.stepByStepDebugTest(arr_size);

  return 0;
}
