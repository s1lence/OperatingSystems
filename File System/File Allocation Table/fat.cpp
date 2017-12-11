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

fat16::File::~File()
{
  for (size_t i = m_cluster, count = 0; count <= m_size; ++i){
    if (m_p2memory->m_clusters[i] == 247) continue;
    
    m_p2memory->m_clusters[i] = 0;
    ++count;
  }
}

void fat16::File::print(size_t offset) const
{
  for (size_t i = 0; i < offset; ++i) std::cout << "   |";
  std::cout << R"(Name: ")" << m_name << R"(", Type: File, Size: )" << std::dec << m_size << R"( Clusters allocated:)";

  for (size_t i = 0, j = m_cluster; i <= m_size; ++i){
    if (i % 20 == 0){
      std::cout << std::endl;
      for (size_t i = 0; i < offset; ++i) std::cout << "   |";
    }

    std::cout << std::setw(3) << std::hex << (j = m_p2memory->m_clusters[j]);
  }

  std::cout << std::endl << std::endl;
}

fat16::Folder::~Folder()
{
  for (auto &i : m_members)
    delete i;
}

void fat16::Folder::removeEntity(std::string& name, bool recursively /*= true*/)
{
  for (auto i = m_members.begin(); i != m_members.end(); ++i){
    if (**i == name){ delete *i; m_members.erase(i); return; }

    if (recursively && nullptr != dynamic_cast<Folder*>(*i)) dynamic_cast<Folder*>(*i)->removeFile(name);
  }
}

void fat16::Folder::removeFile(std::string& name, bool recursively)
{
  for (auto i = m_members.begin(); i != m_members.end(); ++i){
    if (**i == name && nullptr != dynamic_cast<File*>(*i)){ delete *i; m_members.erase(i); return; }

    if (recursively && nullptr != dynamic_cast<Folder*>(*i)) dynamic_cast<Folder*>(*i)->removeFile(name);
  }
}

void fat16::Folder::removeFolder(std::string& name, bool recursively /*= true*/)
{
  for (auto i = m_members.begin(); i != m_members.end(); ++i){
    if (**i == name && nullptr != dynamic_cast<Folder*>(*i)){ delete *i; m_members.erase(i); return; }

    if (recursively && nullptr != dynamic_cast<Folder*>(*i)) dynamic_cast<Folder*>(*i)->removeFile(name);
  }
}

fat16::Entity* fat16::Folder::findFile(std::string& name)
{
  Entity* res = nullptr;
  for (auto &i : m_members){
    if (nullptr != dynamic_cast<Folder*>(i)) res = dynamic_cast<Folder*>(i)->findFile(name);
    if (nullptr != res) return res;
    if (*i == name) return i;
  }
  
  return nullptr;
}

fat16::Entity* fat16::Folder::findFolder(std::string& name)
{
  if (m_name == name) return this;
  
  Entity* res = nullptr;
  for (auto &i : m_members){
    if (nullptr != dynamic_cast<Folder*>(i)) res = dynamic_cast<Folder*>(i)->findFolder(name);
    if (nullptr != res) return res;
  }

  return nullptr;
}

void fat16::Folder::print(size_t offset) const
{
  for (size_t i = 0; i < offset; ++i) std::cout << "   |";
  std::cout << R"(Name: ")" << m_name << R"(", Type: Folder, Members:)" << std::endl << std::endl;
  for (auto i : m_members) i->print(offset + 1);
}

fat16::HardDrive::HardDrive(size_t amountOfClusters, size_t amountOfDefectedClusters)
{
  m_clusters.reserve(amountOfClusters);
  for (size_t i = 0; i < amountOfClusters; ++i) m_clusters.push_back(0);

  auto k = amountOfClusters / amountOfDefectedClusters;
  for (size_t i = 0; i < amountOfClusters; ++i) 
    if (i % k == 2) m_clusters[i] = 247;  //f7 means defected cluster
}

fat16::Entity* fat16::HardDrive::createFile(std::string& name, size_t size)
{
  long count = 0, prev = -1, first = -1;

  for (size_t i = 0; count <= size && i < m_clusters.size(); ++i){
    if (m_clusters[i] == 247) continue;

    if (0 == m_clusters[i]){
      if (0 > first) first = i;

      if (count == size)
        m_clusters[i] = 255; /* ff means the eof symbol */

      if (0 <= prev)
        m_clusters[prev] = i;
      
      prev = i;      
      ++count;
    }
  }

  if (count < size){  /* cleanup memory */
    for (size_t i = first, j = 0; i < m_clusters.size(); i = j){

      j = m_clusters[i];
      m_clusters[i] = 00;
    }
    
    return nullptr; /* not enough memory */
  }

  return new File(name, size, first, this);
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

  std::cout << std::endl;
}

void fat16::HardDrive::print(size_t length /*= 0*/)
{
  if (!length) length = m_clusters.size();
  std::cout << "Memory map:";

  for (size_t i = 0; i < length; ++i){
    if (i % 10 == 0) std::cout << std::endl << std::setw(4) << i / 10 * 10 << " |";
    std::cout << std::setw(4) << std::hex << m_clusters[i];
  }
}
