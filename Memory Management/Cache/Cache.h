/*
 * `file`          Cache.h
 * `written`       November 21 2017 22:24:41
 * `last modified` November 21 2017 22:24:41
 * 
 * Author:         R. Neshta
 * Contact:        Ruslan.Neshta@gmail.com
 * 
 * `synopsis`      defines cache
 * 
 * Description:    definition of all cache required stuff
 *                 model of Intel's 486 cpu 4-way cache:
 *                 16 bytes per line;
 *                 128 sets;
 *                 8 kilobytes of total data memory.
 * 
 * `note`          none
 */
 
 #ifndef _CACHE_H_
 #define _CACHE_H_

#include<array>
#include<functional>

 namespace cache{

   /*
    *	definitions for 32-bit system
    */
   using byte  = unsigned char;
   using word  = unsigned short;
   using dword = unsigned long;
   
   /*
    *	the data sequence:
    *	holds actual data from memory
    */
   template<byte _Length>
   class dataLine{ 
     std::array<dword, _Length>   data;

   public:
     dword& operator[](byte offset){ return data[offset]; }

   };

   /*
    *	the tag sequence:
    *	holds part of the address and flags for data related to it's tag
    */
   template<byte _Length>
   class tagLine{
     dword tag      : _Length;
     dword free     : 1;
     dword modified : 1;

     const static dword _Aux_Value_ = ~(~0 >> _Length);
     const static byte  _Address_Offset_;

   public:
     tagLine();
     ~tagLine() = default;

     dword operator=(const dword address){ modified = 0, tag = address >> _Address_Offset_; }
     bool operator==(const dword& address)const{ return _Aux_Value_ & address & tag; }

     void modify(){ modified = 1; }
     
   };

   template<byte _Length>
   cache::tagLine<_Length>::tagLine() :tag(0), free(0), modified(0)
   {
     byte tmp = ~0 >> _Length, count = 1;

     while (tmp = tmp >> 1) ++count;
     _Address_Offset_ = count;
   }

   /*
    *	set is the model of cache line
    *	it consist of two main parts:
    *	  - tags;
    *	  - data.
    *	
    *	tag lines holds part of address and flags for each data section
    *	data holds data
    *	
    *	_Ty is the type of which we'll take bit fields after instantiating object
    *	_Algorithm is the algorithm for replacing existing items
    *	_Amount is the amount of bit fields
    *	_Size is width of data line
    *	_WayNumber defines amount of directories in each set(three-way cache - three directories in each set)
    *	_TagLength is the length of tag line(how many bits of the address is the tag)
    *	
    *	address consist of(sizes are valid only for this cache model):
    *	 - offset: the lowest four bits                 ; log2(L), where L is length of data line in bytes
    *	 - set:    bits from tenth to fourth including  ; log2(N), for N-way cache
    *	 - tag:    the highest twenty one bits          ; rest of the address
    *	
    *	note: three bits enough for pseudo-LRU in four-way cache set 
    */
   template<class _Ty, std::function<void(_Ty,dword)> _Algorithm, byte _Amount = 3, byte _Size = 4, byte _WayNumber = 4, byte _TagLength = 21>
   class set{
     std::array<tagLine<_TagLength>, _WayNumber>    tags;
     std::array<dataLine<_Size>, _WayNumber>        data;
     _Ty                                            accesses : _Amount;
     
   public:
     
     dword operator[](byte offset){}
     
     dword operator=(dword&& data){}
     
   };

   /*
    *	cache is the model of physical cpu cache
    *	it's has similar structure to the real one:
    *	- whole memory divided between sets;
    *	- the address allows to decide which set should be swept;
    *	- if the set has no free tag one should be overwritten.
    */
   template<class T>
   class Cache{
     std::array<set<>, 128>   m_cache;

   public:

     dword lookup(dword address);

     void fetch(dword address);
     

   };

 }
 
 #endif /* _CACHE_H_ */
