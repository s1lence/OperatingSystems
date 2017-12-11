/*
 * `file`          entry.cpp
 * `written`       December 10 2017 15:05:44
 * `last modified` December 10 2017 15:05:44
 * 
 * Author:         R. Neshta
 * Contact:        Ruslan.Neshta@gmail.com
 * 
 * `synopsis`      test file for FAT
 * 
 * Description:    tests cases for FAT16 file system
 *                 
 * 
 * `note`          none
 */
 
#include "fat.h"

int main(){

  fat16::FAT16<> fs;

  fs.createNewFolder("new folder", "~");
  fs.createNewFile("paint.exe", "new folder", 10);

  fs.printMemory();


  return 0;
}
