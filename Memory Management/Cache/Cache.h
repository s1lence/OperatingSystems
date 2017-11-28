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

 namespace cache{

   /*
    *	definitions for 32-bit system
    */
   using byte  = unsigned char;
   using word  = unsigned short;
   using dword = unsigned long;
   
   /*
    *	represents all available memory space
    */
   template<class _Ty, byte _Size>
   class RAM{
     std::array<_Ty, _Size>   m_data;

   public:
     dword
   };

   /*
    *	the data sequence:
    *	holds actual data from memory
    */
   template<byte _Length>
   class DataLine{ 
     std::array<dword, _Length>   m_data;

   public:
     dword& operator[](byte offset){ return m_data[offset]; }
   };

   /*
    *	the tag sequence:
    *	holds part of the address and flags for data related to it's tag
    */
   template<byte _Length>
   class TagLine{
     dword m_tag      : _Length;
     dword m_free     : 1;
     dword m_modified : 1;

     const static byte  _Address_Offset_;

   public:
     TagLine();
     ~TagLine() = default;

     dword operator=(const dword address){ m_modified = 0, m_tag = address >> _Address_Offset_; }
     bool operator==(const dword address) const{ return address >> _Address_Offset_ == m_tag; }

     void modify(){ m_modified = 1; }
     bool free()const{ return m_free; }
   };

   template<byte _Length>
   cache::TagLine<_Length>::TagLine() :m_tag(0), m_free(0), m_modified(0)
   {
     byte tmp = ~0 >> _Length, count = 1;

     _Address_Offset_ = (while (tmp = tmp >> 1) ++count, count);
   }

   /*
    *	auxiliary class for correct modification lines and saving results into memory 
    */
   template<class _Ty, class _Ty_Aux>
   class Container{
     _Ty      * m_data;
     _Ty_Aux  * m_tag;

   public:
     Container(const _Ty *data, const _Ty_Aux *tag) :m_data(data), m_tag(tag){}
     Container(const Container&& obj) :m_data(obj.m_data), m_tag(obj.m_tag){}

     operator _Ty() const{ return *m_data; }
     _Ty operator=(_Ty&& data){ m_tag->modify(), *m_data = data; }
   };

   /*
    *	pseudo-LRU algorithm for four-way set
    *	it's simply changes states
    */
   template<class _Ty>
   struct Caller{

     _Ty operator()(_Ty& state){
       switch (state){

       case 0: /* line 0 */
         return state = 6, 0; /* 110 */

       case 6: /* line 2 */
         return state = 3, 2; /* 011 */

       case 3: /* line 1 */
         return state = 5, 1; /* 101 */

       case 5: /* line 3 */
         return state = 0, 3; /* 000 */

       }
     }

   };

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
    *	_WayNumber defines amount of directories in each set(four-way cache - four directories in each set)
    *	_TagLength is the length of tag line(how many bits of the address is the tag)
    *	
    *	address consist of(sizes are valid only for this cache model):
    *	 - offset: the lowest four bits                 ; log2(L), where L is length of data line in bytes
    *	 - set:    bits from tenth to fourth including  ; log2(N), for N-way cache
    *	 - tag:    the highest twenty one bits          ; rest of the address
    *	
    *	note: three bits enough for pseudo-LRU in four-way cache set;
    *	      _RAM object requires to have 
    */
   template<class _Ty, class _Algorithm, class _Memory, byte _Amount, byte _Size, byte _WayNumber, byte _TagLength>
   class Set{
     std::array<TagLine<_TagLength>, _WayNumber>    m_tags;
     std::array<DataLine<_Size>, _WayNumber>        m_data;
     _Ty                                            m_accesses : _Amount;

     static _Memory m_ram;

   public:
     Set(_Ty state = 0) :m_accesses(state){}

     Container<_Ty, _Ty>&& fetch(_Ty address){ _Algorithm<_Ty> alg; }

     Container<_Ty, _Ty>&& operator[](_Ty address);
   };

   template<class _Ty, class _Algorithm, byte _Amount, byte _Size, byte _WayNumber, byte _TagLength>
   Container<_Ty, _Ty>&& cache::Set<_Ty, _Algorithm, _Amount, _Size, _WayNumber, _TagLength>::operator[](_Ty address)
   {
     for (byte i = 0; i < m_tags.size(); ++i) 
       if (!m_tags[i].free() && m_tags[i] == address) return std::forward<Container<_Ty, _Ty>&&>(Container(&m_data[i], &m_tags[i]));

     return fetch(address);
   }

   /*
    *	cache is the model of physical cpu cache
    *	it's has similar structure to the real one:
    *	- whole memory divided between sets;
    *	- the address allows to decide which set should be swept;
    *	- if the set has no free tag one should be overwritten.
    */
   template<class _Ty = dword, size_t _Size = 128, byte _BitsAmountInSetAlgorithm = 3, byte _SizeOfDataLine = 4, byte _SetWayNumber = 4, byte _SetTagLength = 21>
   class Cache{
     std::array<Set<_Ty, Caller<_Ty>, RAM<_Ty, _Size*sizeof(_Ty)*_SizeOfDataLine*_SetWayNumber>, _BitsAmountInSetAlgorithm, _SizeOfDataLine, _SetWayNumber, _SetTagLength>, _Size>    m_cache;

   public:

     _Ty lookup(_Ty address);

   };

 }
 
 #endif /* _CACHE_H_ */
