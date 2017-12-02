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

#ifdef _DEBUG
#include<conio.h>
#endif /* _DEBUG */

#include<array>
#include<iostream>

 namespace win32{

   /*
    *	definitions for 32-bit system
    */
   using byte  = unsigned char;
   using word  = unsigned short;
   using dword = unsigned long;

   /*
    *	represents all available memory space
    */
   template<class _Ty, class _DataLine, dword _Size>
   class RAM{
     std::array<_Ty, _Size>   m_data;

   public:
     _Ty& operator[](_Ty offset){ return m_data[offset]; }
     void writeBack(_DataLine & line, _Ty address){ memcpy_s((void*)m_data[address / sizeof(_Ty)], line.size(), (const void*)&line[0], line.size()); }
     void insert(_Ty* sequence, dword length){
       for (int i = 0; i < length; ++i)
         m_data[i] = i[sequence];
       /*memcpy_s((void*)m_data[0], m_data.size(), (const void*)sequence, length);*/ 
     }

     void pprint(_Ty amount) const;
   };

   template<class _Ty, class _DataLine, dword _Size>
   void win32::RAM<_Ty, _DataLine, _Size>::pprint(_Ty amount) const
   {
     for (int i = 0, j = 1, count = 0; i < amount; ++i){
       if (!j / sizeof(_Ty))
         std::cout << "\t", ++count, j = 1;
       if (count == 3) 
         std::cout << std::endl, count = 0;
       std::cout << m_data[i] << " ";
     }
   }

   /*
    *	the data sequence:
    *	holds actual data from memory
    */
   template<class _Ty, byte _Length>
   class DataLine{ 
     std::array<_Ty, _Length>   m_data;

   public:
     _Ty& operator=(_Ty& address){ memcpy_s((void*)m_data[0], m_data.size(), (void*)address, m_data.size()); return address; }
     _Ty& operator[](byte offset){ return m_data[offset]; }
     _Ty size()const{ return m_data.size(); }

     void pprint() const{ for (auto i : m_data) std::cout << i << " "; }
   };

   /*
    *	the tag sequence:
    *	holds part of the address and flags for data related to it's tag
    */
   template<class _Ty, byte _Length>
   class TagLine{
     _Ty m_tag      : _Length;
     _Ty m_free     : 1;
     _Ty m_modified : 1;

     const byte  _Address_Offset_;

     byte initOffsetVar();

   public:
     TagLine() :m_tag(0), m_free(1), m_modified(0), _Address_Offset_(initOffsetVar()){}
     ~TagLine() = default;

     _Ty operator=(const _Ty address){ m_modified = 0, m_free = 0, m_tag = address >> _Address_Offset_; return address; }
     bool operator==(const _Ty address) const{ return address >> _Address_Offset_ == m_tag; }

     operator _Ty() const{ return m_tag; }

     void modify(){ m_modified = 1; }
     bool modified() const{ return m_modified; }
     bool free() const{ return m_free; }
     void pprint() const{ std::cout << "t: " << m_tag << " f: " << m_free << " m:" << m_modified; }
   };

   template<class _Ty, byte _Length>
   byte win32::TagLine<_Ty, _Length>::initOffsetVar()
   {
     byte tmp = ~0 >> _Length, count = 1;

     while (tmp = tmp >> 1) ++count;
     return count;
   }

   /*
    *	auxiliary class for correct modification lines and saving results into memory 
    */
   template<class _Ty, class _TyData, class _TyTag>
   class Container{
     _TyData  * m_data;
     _TyTag   * m_tag;

   public:
     Container(_TyData *data, _TyTag *tag) :m_data(data), m_tag(tag){}
     Container(const Container&& obj) :m_data(obj.m_data), m_tag(obj.m_tag){}

     operator _Ty() const{ return *m_data; }
     _Ty operator=(_Ty&& data){ m_tag->modify(), *m_data = data; return data; }
     Container&& operator=(Container&& obj){ m_data = obj.m_data, m_tag = obj.m_tag, return std::forward(obj); }
   };

   /*
    *	pseudo-LRU algorithm for four-way set
    *	returns next state, sets current index
    */
   template<class _Ty>
   struct Caller{

     template<class _T>
     _Ty operator()(_T& index, _Ty state){
       switch (state){

       case 0: /* line 0 */
         index = 0;
         return 6; /* 110 */

       case 6: /* line 2 */
         index = 2;
         return 3; /* 011 */

       case 3: /* line 1 */
         index = 1;
         return 5; /* 101 */

       case 5: /* line 3 */
         index = 3;
         return 0; /* 000 */

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
    *	 - set:    bits from tenth to fourth including  ; log2(N), for N - amount of sets
    *	 - tag:    the highest twenty one bits          ; rest of the address
    *	
    *	note: three bits enough for pseudo-LRU in four-way cache set;
    *	      _RAM object requires to have 
    */
   template<class _Ty, class _Algorithm, class _Memory, byte _Amount, byte _Size, byte _WayNumber, byte _TagLength>
   class Set{

     using _tag_t  = TagLine<_Ty, _TagLength>;
     using _data_t = DataLine<_Ty, _Size>;

     using _cont_t = Container<_Ty, _data_t, _tag_t>;

     std::array<_tag_t, _WayNumber>    m_tags;
     std::array<_data_t, _WayNumber>   m_data;
     _Ty                               m_accesses : _Amount;

     static _Memory * m_p2ram;
     static const _Ty _AddressAuxiliaryConst_ = ~0 >> _TagLength;

   public:

     Set(_Ty state = 0) :m_accesses(state){}

     _cont_t&& fetch(_Ty address);

     static void setMemoryAddress(_Memory * p2ram){ m_p2ram = p2ram; }

     _cont_t&& operator[](_Ty address);

     void pprint() const;
   };

   template<class _Ty, class _Algorithm, class _Memory, byte _Amount, byte _Size, byte _WayNumber, byte _TagLength>
   _Memory * Set<_Ty, _Algorithm, _Memory, _Amount, _Size, _WayNumber, _TagLength>::m_p2ram;

   template<class _Ty, class _Algorithm, class _Memory, byte _Amount, byte _Size, byte _WayNumber, byte _TagLength>
   win32::Container<_Ty, DataLine<_Ty, _Size>, TagLine<_Ty, _TagLength>>&& win32::Set<_Ty, _Algorithm, _Memory, _Amount, _Size, _WayNumber, _TagLength>::fetch(_Ty address)
   {
     _Algorithm alg; byte index;
     m_accesses = alg(index, m_accesses);

     if (m_tags[index].modified())
       m_p2ram->writeBack(m_data[index], (_Ty)m_tags[index] & (_AddressAuxiliaryConst_ & address));

     m_tags[index] = address;
     m_data[index] = (*m_p2ram)[address];

     return std::forward<_cont_t&&>(Container<_Ty, _data_t, _tag_t>(&m_data[index], &m_tags[index]));
   }

   template<class _Ty, class _Algorithm, class _Memory, byte _Amount, byte _Size, byte _WayNumber, byte _TagLength>
   win32::Container<_Ty, DataLine<_Ty, _Size>, TagLine<_Ty, _TagLength>>&& win32::Set<_Ty, _Algorithm, _Memory, _Amount, _Size, _WayNumber, _TagLength>::operator[](_Ty address)
   {
     for (byte i = 0; i < m_tags.size(); ++i) 
       if (!m_tags[i].free() && m_tags[i] == address) 
         return std::forward<_cont_t&&>(Container<_Ty, _data_t, _tag_t>(&m_data[i], &m_tags[i]));

     return fetch(address);
   }

   template<class _Ty, class _Algorithm, class _Memory, byte _Amount, byte _Size, byte _WayNumber, byte _TagLength>
   void win32::Set<_Ty, _Algorithm, _Memory, _Amount, _Size, _WayNumber, _TagLength>::pprint() const
   {
     for (int i = 0; i < _WayNumber; ++i)
       if (!m_tags[i].free())
       {
         std::cout << "|\t";
         m_tags[i].pprint();
         std::cout << "\t|\t";
         m_data[i].pprint();
         std::cout << "\t|" << std::endl;
       }
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

     using _ram_t   = RAM<_Ty, DataLine<_Ty, _SizeOfDataLine>, _Size*_SizeOfDataLine*_SetWayNumber>;
     using _line_t  = Set<_Ty, Caller<_Ty>, _ram_t, _BitsAmountInSetAlgorithm, _SizeOfDataLine, _SetWayNumber, _SetTagLength>;

     using _data_t = DataLine<_Ty, _SizeOfDataLine>;
     using _tag_t  = TagLine<_Ty, _SetTagLength>;

     std::array<_line_t, _Size>    m_cache;
     _ram_t                        m_memory;

     _Ty initAuxConst();

   public:

     Cache(){ _line_t::setMemoryAddress(&m_memory); }

     void initMemory(_Ty *sequence, dword length){ m_memory.insert(sequence, length); }

     win32::Container<_Ty, _data_t, _tag_t>&& lookup(_Ty address);

     void stepByStepDebugTest(_Ty);

     void report(_Ty)const;
   };

   template<class _Ty /*= dword*/, size_t _Size /*= 128*/, byte _BitsAmountInSetAlgorithm /*= 3*/, byte _SizeOfDataLine /*= 4*/, byte _SetWayNumber /*= 4*/, byte _SetTagLength /*= 21*/>
   _Ty win32::Cache<_Ty, _Size, _BitsAmountInSetAlgorithm, _SizeOfDataLine, _SetWayNumber, _SetTagLength>::initAuxConst()
   {
     _Ty res = ~0, tmp = ~0;
     while (res > (_Size << 1)) res >> 1;
     while (tmp > (sizeof(_Ty)*_SizeOfDataLine << 1)) tmp >> 1;
     return res^tmp;
   }

   template<class _Ty /*= dword*/, size_t _Size /*= 128*/, byte _BitsAmountInSetAlgorithm /*= 3*/, byte _SizeOfDataLine /*= 4*/, byte _SetWayNumber /*= 4*/, byte _SetTagLength /*= 21*/>
   win32::Container<_Ty, DataLine<_Ty, _SizeOfDataLine>, TagLine<_Ty, _SetTagLength>>&& win32::Cache<_Ty, _Size, _BitsAmountInSetAlgorithm, _SizeOfDataLine, _SetWayNumber, _SetTagLength>::lookup(_Ty address)
   {
     static _Ty _SetAuxConst = initAuxConst();
     
     return m_cache[address & _SetAuxConst][address];
   }

   template<class _Ty /*= dword*/, size_t _Size /*= 128*/, byte _BitsAmountInSetAlgorithm /*= 3*/, byte _SizeOfDataLine /*= 4*/, byte _SetWayNumber /*= 4*/, byte _SetTagLength /*= 21*/>
   void win32::Cache<_Ty, _Size, _BitsAmountInSetAlgorithm, _SizeOfDataLine, _SetWayNumber, _SetTagLength>::stepByStepDebugTest(_Ty length)
   {
     for (_Ty i = 0; i < 10; ++i){

       report(length);
       auto res = lookup(i*sizeof(_Ty));
       report(length);
       std::cout << "Operation: data = " << i * 2 << std::endl;
       res = i * 2;
       report(length);
     }

   }

    /* this is a simple test case for the model: prints first set and memory enough to overwrite it's content */
   template<class _Ty /*= dword*/, size_t _Size /*= 128*/, byte _BitsAmountInSetAlgorithm /*= 3*/, byte _SizeOfDataLine /*= 4*/, byte _SetWayNumber /*= 4*/, byte _SetTagLength /*= 21*/>
   void win32::Cache<_Ty, _Size, _BitsAmountInSetAlgorithm, _SizeOfDataLine, _SetWayNumber, _SetTagLength>::report(_Ty length = 112) const
   {
     std::cout << "Set #1:" << std::endl;
     m_cache[0].pprint();
     std::cout << "Memory map:" << std::endl;
     m_memory.pprint(length); /* set is sizeof(dword)*4(four in one data line)*(4+3)(four way set + extra lines for visualise write back algorithm) = 4*4*7 = 112 */
     std::cout << std::endl;
#ifdef _DEBUG
     _getch();
#endif /* _DEBUG */
   }

 }

 #endif /* _CACHE_H_ */
