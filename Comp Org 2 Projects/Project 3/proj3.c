#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

struct set;
struct block;
struct wt_cache;
struct wb_cache;

struct cache
{
     char access_type;
     int access_num;
     int byte_address;
     unsigned tag;
     unsigned offset;
     unsigned index;
};

struct wt_cache
{
     struct set *sets;
};

struct wb_cache
{
     struct set *sets;
};

struct set
{
     struct block *blocks;
};

struct block
{
     int tag;
     int validBit;
     int dirtyBit;
     int lastAccessTime;
};


//index tells us which set the instruction is in
//tag determines if the block you're looking for is in the set
//offset tells you which byte in the block you are looking for

int main (int argc, char *argv[])
{
     char buff[50];
     char *delim = " ";
     int block_num  = atoi(argv[1]);
     int set_num    = atoi(argv[2]);
     int assoc      = atoi(argv[3]);
     int offsetBits = log2(block_num);
     int indexBits  = log2(set_num);
     int word       = 32;
     int tagBits    = word - offsetBits - indexBits;
     int LRU_bit    = 0;
     int ref_num    = 0;
     int j          = 0;
     int i          = 0;
     int k          = 0;
     int offIndex   = offsetBits + indexBits;
     int tagIndex   = indexBits + tagBits;
     int offTag     = tagBits + offsetBits;
     int wb_count   = 0;
     int mask       = 0;
     int line_num   = 0;
     int wt_hits    = 0;
     int wt_misses  = 0;
     int wt_memrefs = 0;
     int wb_hits    = 0;
     int wb_misses  = 0;
     int wb_memrefs = 0;
     struct wt_cache wt;
     struct wb_cache wb;
     struct cache arr_cache[251];
     wt.sets = malloc(sizeof(struct set) * set_num);
     for(i = 0; i < set_num; i++)
          wt.sets[i].blocks = malloc(sizeof(struct block) * assoc);
     wb.sets = malloc(sizeof(struct set) * set_num);
     for(i = 0; i < set_num; i++)
          wb.sets[i].blocks = malloc(sizeof(struct block) * assoc);

i = 0;
     while(fgets(buff, 50, stdin) != NULL)
     {
          sscanf(buff, "%c %d", &(arr_cache[i].access_type), &(arr_cache[i].byte_address));
          if(arr_cache[i].access_type == 'W')
               arr_cache[i].access_num = 0;
          if(arr_cache[i].access_type == 'R')
               arr_cache[i].access_num = 1;
          arr_cache[i].offset         = arr_cache[i].byte_address << tagIndex;
          arr_cache[i].offset         = arr_cache[i].offset >> tagIndex;
          arr_cache[i].index          = arr_cache[i].byte_address << tagBits;
          arr_cache[i].index          = arr_cache[i].index >> offTag;
          arr_cache[i].tag            = arr_cache[i].byte_address >> offIndex;
          i++;
          ref_num++;

     }


     for(k = 0; k < set_num; k++)
     {
          for(i = 0; i < assoc; i++)
          {
               wt.sets[k].blocks[i].tag               = 0;
               wt.sets[k].blocks[i].validBit          = 0;
               wt.sets[k].blocks[i].dirtyBit          = 0;
               wt.sets[k].blocks[i].lastAccessTime    = 0;
               wb.sets[k].blocks[i].tag               = 0;
               wb.sets[k].blocks[i].validBit          = 0;
               wb.sets[k].blocks[i].dirtyBit          = 0;
               wb.sets[k].blocks[i].lastAccessTime    = 0;
          }
     }

     //write-through portion
     i = 0;
     while(line_num < ref_num)
     {
          if(arr_cache[line_num].access_num == 0) //checks if it is a Write
          {
               int is_hit = 0;
               for(j = 0; j < assoc; j++)
               {
                    if((wt.sets[arr_cache[line_num].index].blocks[j].tag == arr_cache[line_num].tag) && (wt.sets[arr_cache[line_num].index].blocks[j].validBit == 1))
                    {
                         is_hit = 1;
                         wt.sets[arr_cache[line_num].index].blocks[j].lastAccessTime = line_num;
                    }
               }
               if(is_hit == 1)
               {
                    wt_hits++;
                    wt_memrefs++;
               }
               if(is_hit == 0)
               {
                    wt_misses++;
                    wt_memrefs++;
               }
          }
          if(arr_cache[line_num].access_num == 1) //checks if it is a Read
          {
               int is_hit = 0;
               for(j = 0; j < assoc; j++)
               {
                    if((wt.sets[arr_cache[line_num].index].blocks[j].tag == arr_cache[line_num].tag) && (wt.sets[arr_cache[line_num].index].blocks[j].validBit == 1))
                    {
                         is_hit = 1;
                         wt.sets[arr_cache[line_num].index].blocks[j].lastAccessTime = line_num;
                    }
               }
               if(is_hit == 1)
               {
                    wt_hits++;
                    wt_memrefs++;
               }
               if(is_hit == 0)
               {
                    wt_misses++;
                    wt_memrefs++;
                    int temp = wt.sets[arr_cache[line_num].index].blocks[0].lastAccessTime;
                    LRU_bit = 0;
                    for(i = 0; i < assoc; i++)
                    {
                         if (temp > wt.sets[arr_cache[line_num].index].blocks[i].lastAccessTime)
                         {
                              temp = wt.sets[arr_cache[line_num].index].blocks[i].lastAccessTime;
                              LRU_bit = i;
                         }
                    }
                    wt.sets[arr_cache[line_num].index].blocks[LRU_bit].tag = arr_cache[line_num].tag;
                    wt.sets[arr_cache[line_num].index].blocks[LRU_bit].validBit = 1;
                    wt.sets[arr_cache[line_num].index].blocks[LRU_bit].lastAccessTime = line_num;
               }
          }
          line_num++;
     }


     line_num = 0;
     //WriteBack section of the code
     while(line_num < ref_num)
     {
          if(arr_cache[line_num].access_num == 0) //checks if it is a Write
          {
               int is_hit = 0;
               for(j = 0; j < assoc; j++)
               {
                    if((wb.sets[arr_cache[line_num].index].blocks[j].tag == arr_cache[line_num].tag) && (wb.sets[arr_cache[line_num].index].blocks[j].validBit == 1))
                    {
                         is_hit = 1;
                         wb.sets[arr_cache[line_num].index].blocks[j].lastAccessTime = line_num;
                         if(wb.sets[arr_cache[line_num].index].blocks[j].dirtyBit == 1)
                              wb_count++;
                         wb.sets[arr_cache[line_num].index].blocks[j].dirtyBit = 1;
                    }
               }
               if(is_hit == 1)
               {
                    wb_hits++;
               }
               if(is_hit == 0)
               {
                    int temp = wb.sets[arr_cache[line_num].index].blocks[0].lastAccessTime;
                    LRU_bit = 0;
                    for(i = 0; i < assoc; i++)
                    {
                         if (temp > wb.sets[arr_cache[line_num].index].blocks[i].lastAccessTime)
                         {
                              temp = wb.sets[arr_cache[line_num].index].blocks[i].lastAccessTime;
                              LRU_bit = i;
                         }
                    }
                    if(wb.sets[arr_cache[line_num].index].blocks[LRU_bit].dirtyBit == 1)
                         wb_count++;
                    wb.sets[arr_cache[line_num].index].blocks[LRU_bit].tag = arr_cache[line_num].tag;
                    wb.sets[arr_cache[line_num].index].blocks[LRU_bit].validBit = 1;
                    wb.sets[arr_cache[line_num].index].blocks[LRU_bit].dirtyBit = 1;
                    wb.sets[arr_cache[line_num].index].blocks[LRU_bit].lastAccessTime = line_num;
                    wb_misses++;
                    wb_memrefs++;

               }
          }
          if(arr_cache[line_num].access_num == 1) //checks if it is a Read
          {
               int is_hit = 0;
               for(j = 0; j < assoc; j++)
               {
                    if((wb.sets[arr_cache[line_num].index].blocks[j].tag == arr_cache[line_num].tag) && (wb.sets[arr_cache[line_num].index].blocks[j].validBit == 1))
                    {
                         is_hit = 1;
                         wb.sets[arr_cache[line_num].index].blocks[j].lastAccessTime = line_num;
                    }
               }
               if(is_hit == 1)
               {
                    wb_hits++;
                    wb_memrefs++;
               }
               if(is_hit == 0)
               {
                    wb_misses++;
                    wb_memrefs++;
                    int temp = wb.sets[arr_cache[line_num].index].blocks[0].lastAccessTime;
                    LRU_bit = 0;
                    for(i = 0; i < assoc; i++)
                    {
                         if (temp > wb.sets[arr_cache[line_num].index].blocks[i].lastAccessTime)
                         {
                              temp = wb.sets[arr_cache[line_num].index].blocks[i].lastAccessTime;
                              LRU_bit = i;
                         }
                    }
                    if(wb.sets[arr_cache[line_num].index].blocks[LRU_bit].dirtyBit == 1)
                         wb_count++;
                    wb.sets[arr_cache[line_num].index].blocks[LRU_bit].tag = arr_cache[line_num].tag;
                    wb.sets[arr_cache[line_num].index].blocks[LRU_bit].validBit = 1;
                    wb.sets[arr_cache[line_num].index].blocks[LRU_bit].lastAccessTime = line_num;
               }
          }
          line_num++;
     }

     printf("Block Size: %d\n", block_num);
     printf("Number of Sets: %d\n", set_num);
     printf("Associativity: %d\n", assoc);
     printf("Number of Offset Bits: %u\n", offsetBits);
     printf("Number of Index Bits: %d\n", indexBits);
     printf("Number of Tag Bits: %d\n", tagBits);
     printf("****************************************\n");
     printf("  Write-through with No Write Allocate  \n");
     printf("****************************************\n");
     printf("Total Number of References: %d\n", ref_num);
     printf("Hits: %d\n", wt_hits);
     printf("Misses: %d\n", wt_misses);
     printf("Memory References: %d\n", wt_memrefs);
     printf("****************************************\n");
     printf("     Write-back with Write Allocate     \n");
     printf("****************************************\n");
     printf("Total Number of References: %d\n", ref_num);
     printf("Hits: %d\n", wb_hits);
     printf("Misses: %d\n", wb_misses);
     printf("Writebacks: %d\n", wb_count);
     printf("Memory References: %d\n", wb_memrefs);

     for (i = 0; i < set_num; i++)
          free(wt.sets[i].blocks);
     free(wt.sets);
     for (i = 0; i < set_num; i++)
          free(wb.sets[i].blocks);
     free(wb.sets);


return 0;
}



//    choose the lru block to be the first block
//    for each block in the set, compare the current block's access time to the lru block's accesstime.
//    if the current block's access time is further away in time than the lru's, lru is the current block.
