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
  fs.createNewFile("help.txt", "new folder", 40);
  fs.createNewFile("stuff.cpp", "~", 20);

  fs.createNewFolder("my", "~");
  fs.createNewFile("super_mario.exe", "my", 12);
  fs.createNewFile("instruction.doc", "my", 23);

  fs.createNewFolder("empty", "new folder");
  fs.createNewFolder("trash", "new folder");
  fs.createNewFile("useless.txt", "trash", 8);
  fs.createNewFile("output.csv", "trash", 17);

  fs.printAll();

  fs.printMemory();


  return 0;
}
