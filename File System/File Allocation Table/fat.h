/*
 * `file`          fat.h
 * `written`       December 05 2017 16:39:13
 * `last modified` December 05 2017 16:39:13
 * 
 * Author:         R. Neshta
 * Contact:        Ruslan.Neshta@gmail.com
 * 
 * `synopsis`      declaration of FAT
 * 
 * Description:    file allocation table handles file management
 *                 
 * 
 * `note`          none
 */


#ifndef _FAT_H_
#define _FAT_H_

#include<iostream>
#include<vector>

namespace fat16{

  class Entity{
  public:
    virtual ~Entity() = 0;
  };

  class File :public Entity{

  };

  class Folder :public Entity{
    std::vector<Entity*>    m_members;
  public:

  };

  class FAT16{

  };

}

#endif /* _FAT_H_ */