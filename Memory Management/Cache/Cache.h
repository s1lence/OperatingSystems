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
 *                 
 * 
 * `note`          none
 */
 
 #ifndef _CACHE_H_
 #define _CACHE_H_

#define CACHE_LINE_DATA_FIELD_WIDTH 64


 namespace cache{
 
   struct Line{
     void * address;
     char   data[CACHE_LINE_DATA_FIELD_WIDTH];
     char   free      : 1;
     char   modified  : 1;
   };

   struct Group{
     Line data[]
   };
 
 }
 
 #endif /* _CACHE_H_ */
