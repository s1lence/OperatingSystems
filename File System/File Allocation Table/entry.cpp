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
#include<conio.h>

int main(){

  fat16::FAT16<> fs;
  char ch;

  fs.printAll();
  fs.printMemory();
  ch = _getch();

  fs.createNewFolder("new folder", "~");
  std::cout << std::endl << std::endl << "Creating folder: 'new folder'" << std::endl << std::endl;

  fs.printAll();
  fs.printMemory();
  ch = _getch();

  fs.createNewFile("paint.exe", "new folder", 10);
  std::cout << std::endl << std::endl << "Creating file: 'paint.exe'" << std::endl << std::endl;

  fs.printAll();
  fs.printMemory();
  ch = _getch();

  fs.createNewFile("help.txt", "new folder", 40);
  fs.createNewFile("stuff.cpp", "~", 20);
  std::cout << std::endl << std::endl << "Creating files: 'help.txt' & 'stuff.cpp'" << std::endl << std::endl;

  fs.printAll();
  fs.printMemory();
  ch = _getch();

  fs.createNewFolder("my", "~");
  fs.createNewFile("super_mario.exe", "my", 12);
  fs.createNewFile("instruction.doc", "my", 23);
  std::cout << std::endl << std::endl << "Creating folder: 'my' with two files inside: 'super_mario.exe' & 'instruction.doc'" << std::endl << std::endl;

  fs.printAll();
  fs.printMemory();
  ch = _getch();

  fs.createNewFolder("empty", "new folder");
  fs.createNewFolder("trash", "new folder");
  std::cout << std::endl << std::endl << "Creating folders: 'empty' & 'trash'" << std::endl << std::endl;

  fs.printAll();
  fs.printMemory();
  ch = _getch();

  fs.createNewFile("useless.txt", "trash", 8);
  fs.createNewFile("output.csv", "trash", 17);
  std::cout << std::endl << std::endl << "Creating files inside 'trash' folder: 'useless.txt' & 'output.csv'" << std::endl << std::endl;

  fs.printAll();
  fs.printMemory();
  ch = _getch();

  fs.eraseFile("paint.exe");
  std::cout << std::endl << std::endl << "Deleting file: 'paint.exe'" << std::endl << std::endl;

  fs.printAll();
  fs.printMemory();
  ch = _getch();

  fs.eraseFile("useless.txt");
  std::cout << std::endl << std::endl << "Deleting file: 'useless.txt'" << std::endl << std::endl;

  fs.printAll();
  fs.printMemory();
  ch = _getch();

  fs.eraseFolder("my");
  std::cout << std::endl << std::endl << "Deleting folder 'my' with files: 'super_mario.exe' & 'instruction.doc'" << std::endl << std::endl;

  fs.printAll();
  fs.printMemory();
  ch = _getch();

  fs.resizeFile("help.txt", 12);
  std::cout << std::endl << std::endl << "Resizing file 'help.txt' from 40 clusters to 12" << std::endl << std::endl;

  fs.printAll();
  fs.printMemory();
  ch = _getch();

  fs.createNewFile("new.txt", "trash", 23);
  std::cout << std::endl << std::endl << "Creating file inside 'trash' folder: 'new.txt'" << std::endl << std::endl;

  fs.printAll();
  fs.printMemory();
  ch = _getch();

  return 0;
}
