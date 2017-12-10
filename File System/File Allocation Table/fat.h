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

#include<iomanip>
#include<iostream>
#include<string>
#include<vector>

namespace fat16{

  class Entity{
  public:
    virtual ~Entity() = 0;
    virtual bool operator==(std::string&){ return false; }
    virtual bool operator==(Entity*){ return false; }
    virtual bool operator<=(size_t){ return false; }
    virtual void print(size_t offset) const = 0;
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
    virtual void print(size_t offset) const override;
    friend class HardDrive;
  };

  class Folder :public Entity{
    std::vector<Entity*>    m_members;
    std::string             m_name;
  public:
    Folder(const char * cstr) :m_name(cstr){}
    Folder(std::string&& str) :m_name(str){}
    virtual ~Folder() override;
    virtual bool operator==(std::string& str) override{ return m_name == str; }
    virtual bool operator==(Entity* ent) override{ return nullptr != dynamic_cast<Folder*>(ent) ? m_name == dynamic_cast<Folder*>(ent)->m_name : false; }

    void add(Entity* ent){ m_members.push_back(ent); }

    void removeEntity(std::string& name, bool recursively = true);
    void removeFile(std::string& name, bool recursively = true);
    void removeFolder(std::string& name, bool recursively = true);

    Entity* findFile(std::string& name);
    Entity* findFolder(std::string& name);
    
    virtual void print(size_t offset) const override;
  };

  class HardDrive{
    std::vector<size_t> m_clusters;
  public:
    HardDrive(size_t amountOfClusters, size_t amountOfDefectedClusters);
    Entity* createFile(std::string&, size_t size);
    bool resizeFile(Entity* file, size_t size);
    void printFile(Entity*, size_t offsetfile);
    void print(size_t length = 0);
  };

  template<size_t _AmountOfClusters = 256, size_t _AmountOfDefectedClusters = 25>
  class FAT16{
    HardDrive   m_drive;
    Folder      m_root;
  public:
    FAT16() :m_drive(_AmountOfClusters, _AmountOfDefectedClusters), m_root("Root"){}

    bool createNewFile(std::string name, std::string folder, size_t size);
    void eraseFile(std::string name){ m_drive.resizeFile(m_root.findFile(name), 0); m_root.removeFile(name); }

    bool createNewFolder(std::string name, std::string folder);
    void eraseFolder(std::string name);

    void printMemory(size_t amount = 0){ m_drive.print(amount); }
    void printFile(std::string&& name){ m_drive.printFile(name); }
  };

  template<size_t _AmountOfClusters /*= 256*/, size_t _AmountOfDefectedClusters /*= 25*/>
  bool fat16::FAT16<_AmountOfClusters, _AmountOfDefectedClusters>::createNewFile(std::string name, std::string folder, size_t size)
  {
    Folder* fld = m_root.findFolder(folder);
    if (nullptr == fld) return false;

    fld->add(m_drive.createFile(name, size));
    return true;
  }

  template<size_t _AmountOfClusters /*= 256*/, size_t _AmountOfDefectedClusters /*= 25*/>
  bool fat16::FAT16<_AmountOfClusters, _AmountOfDefectedClusters>::createNewFolder(std::string name, std::string folder)
  {
    Folder* fld = m_root.findFolder(folder);
    if (nullptr == fld) return false;

    fld->add(new Folder(name));
    return true;
  }

  template<size_t _AmountOfClusters /*= 256*/, size_t _AmountOfDefectedClusters /*= 25*/>
  void fat16::FAT16<_AmountOfClusters, _AmountOfDefectedClusters>::eraseFolder(std::string name)
  {
    Folder* fld = m_root.findFolder(folder);
    if (nullptr == fld) return;

    delete fld;
  }

}

#endif /* _FAT_H_ */