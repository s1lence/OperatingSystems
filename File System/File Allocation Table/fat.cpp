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

void fat16::File::print(size_t offset) const
{
  for (size_t i = 0; i < offset; ++i) std::cout << "--\--";
  std::cout << R"(Name: ")" << m_name << R"(", Type: File, Clusters allocated:)" << std::endl;
}

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

void fat16::Folder::print(size_t offset) const
{
  for (size_t i = 0; i < offset; ++i) std::cout << "--\--";
  std::cout << R"(Name: ")" << m_name << R"(", Type: Folder, Members:)";
  for (auto i : m_members) i->print(offset + 1);
}

fat16::HardDrive::HardDrive(size_t amountOfClusters, size_t amountOfDefectedClusters)
{
  for (auto &i : m_clusters) i = 0;
  auto k = amountOfClusters / amountOfDefectedClusters;
  for (size_t i = 0; i < amountOfClusters; ++i) if (i % k == 2) m_clusters[i] = 247;  //f7 means defected cluster
}

fat16::Entity* fat16::HardDrive::createFile(Entity* folder, std::string& name, size_t size)
{
  size_t count = 0, prev = 0, first = 0;

  for (size_t i = 0; count <= size && i < m_clusters.size(); ++i){
    if (m_clusters[i] == 247) continue;

    if (0 == m_clusters[i]){
      if (!first) first = i;

      if (count == size)
        m_clusters[i] = 255; /* ff means the eof symbol */

      if (prev) 
        m_clusters[prev] = i;
      
      prev = i;      
      ++count;
    }
  }

  if (count < size) return nullptr; /* not enough memory */

  Entity* ptr = new File(name, size, first);
  return ptr;
}

bool fat16::HardDrive::resizeFile(Entity* file, size_t size)
{
  if (*file <= size || nullptr == dynamic_cast<File*>(file)) return false;
  
  size_t i, count;
  for (i = dynamic_cast<File*>(file)->m_cluster, count = 0; i != 255; ++i){
    
    if (count == size){
      m_clusters[i] = 0;
      continue;
    }
    
    if (count < size)
      ++count;
    else
      m_clusters[i] = 255; /* eof symbol */
  }

  m_clusters[i] = 0;
  dynamic_cast<File*>(file)->m_size = size;
  return true;
}

void fat16::HardDrive::printFile(Entity* file, size_t offset)
{
  if (nullptr == file || !dynamic_cast<File*>(file)){
    std::cout << "Error: no such file found." << std::endl;
    return;
  }
  
  File* tmp = dynamic_cast<File*>(file);
  for (size_t i = 0, j = tmp->m_cluster; i <= tmp->m_size; ++i){
    if (i % 5 == 0){
      std::cout << std::endl;
      for (size_t i = 0; i < offset; ++i) std::cout << "--\--";
    }

    std::cout << std::setw(3) << std::hex << m_clusters[j];
  }
  
  
  /*

  File* tmp = dynamic_cast<File*>(file);
  std::cout << "Name: " << tmp->m_name << ", cluster list: ";

  for (size_t i = tmp->m_cluster, count = 0; count <= tmp->m_size; ++count, i = m_clusters[i])
    std::cout << std::setw(3) << std::hex << m_clusters[i];
*/
  std::cout << std::endl;
}

void fat16::HardDrive::print(size_t length /*= 0*/)
{
  if (!length) length = m_clusters.size();
  std::cout << "Memory map:";

  for (size_t i = 0; i < length; ++i){
    if (i % 10 == 0) std::cout << i / 10 * 10 << std::endl;
    std::cout << std::setw(3) << std::hex << m_clusters[i];
  }
}
