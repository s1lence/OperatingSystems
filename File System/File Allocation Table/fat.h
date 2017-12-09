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
#include<string>
#include<vector>

namespace fat16{

  class Entity{
  public:
    virtual ~Entity() = 0;
    virtual void pprint() = 0;
    virtual bool operator==(std::string&){ return false; }
    virtual bool operator==(Entity*){ return false; }
    virtual bool operator<=(size_t){ return false; }
  };

  class File :public Entity{
    std::string   m_name;
    size_t        m_size;
    size_t        m_cluster;
  public:
    File(std::string& name, size_t size, size_t cluster) :m_name(name), m_size(size), m_cluster(cluster){}
    virtual bool operator==(std::string& str) override{ return m_name == str; }
    virtual bool operator==(Entity* ent) override{ return nullptr != dynamic_cast<File*>(ent) ? m_name == dynamic_cast<File*>(ent)->m_name : false; }
    virtual bool operator<=(size_t size) override{ return m_size <= size; }
    virtual void pprint() override;

    friend class HardDrive;
  };

  class Folder :public Entity{
    std::vector<Entity*>    m_members;
  public:
    virtual void pprint() override{ for (auto i : m_members)i->pprint(); }
    void add(Entity* file){ m_members.push_back(file); }
    void remove(std::string& name);
    Entity* find(std::string& name);
  };

  class HardDrive{
    std::vector<size_t> m_clusters;
  public:
    HardDrive(size_t amountOfClusters, size_t amountOfDefectedClusters);
    Entity* createFile(Entity* folder, std::string&, size_t size);
    bool resizeFile(Entity* file, size_t size);
  };

  template<size_t _AmountOfClusters = 256, size_t _AmountOfDefectedClusters = 25>
  class FAT16{
    HardDrive   m_drive;
    Folder      m_root;
  public:
    FAT16() :m_drive(_AmountOfClusters, _AmountOfDefectedClusters){}

    void eraseFile(std::string name){ m_drive.resizeFile(m_root.find(name), 0); m_root.remove(name); }
  };

}

#endif /* _FAT_H_ */