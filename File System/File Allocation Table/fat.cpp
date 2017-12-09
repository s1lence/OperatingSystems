/*
 * `file`          fat.cpp
 * `written`       December 09 2017 16:41:10
 * `last modified` December 09 2017 16:41:10
 * 
 * Author:         R. Neshta
 * Contact:        Ruslan.Neshta@gmail.com
 * 
 * `synopsis`      implementation of fat16
 * 
 * Description:    heavy functions of fat.h implemented here
 *                 
 * 
 * `note`          none
 */

#include "fat.h"

void fat16::Folder::remove(std::string& name)
{
  for (auto i = m_members.begin(); i != m_members.end(); ++i){
    if (**i == name){ delete *i; m_members.erase(i); }

    if (nullptr != dynamic_cast<Folder*>(*i)) dynamic_cast<Folder*>(*i)->remove(name);
  }
}

fat16::Entity* fat16::Folder::find(std::string& name)
{
  for (auto &i : m_members){
    if (nullptr != dynamic_cast<Folder*>(i)) return dynamic_cast<Folder*>(i)->find(name);
    if (*i == name) return i;
  }
  
  return nullptr;
}

fat16::Entity* fat16::HardDrive::createFile(Entity* folder, size_t size)
{

}

void fat16::File::pprint()
{

}
