/**********************************************************************
filename: cpu/baseline/bfs.cpp
author: onesuper
email: onesuperclark@gmail.com

two queues version

***********************************************************************/

#include <omp.h>
#include <stdio.h>
#include <tbb/concurrent_queue.h>
#include <sys/time.h>
#include "syncbitops.h"
#include "bitops.h"


//#define DEBUG

/*
void set_bit(unsigned int num, unsigned long* bitmap) {
     bitmap[num/32] |= ( 0x80000000 >> num%32);
}

int test_bit(unsigned int num, unsigned long* bitmap) {
     return bitmap[num/32] & (0x80000000 >> num%32);
}
*/



float bfs(int num_of_threads) 
{
	 struct timeval start, end;
	 float time_used;
	 tbb::concurrent_bounded_queue<unsigned int> current_a;
     tbb::concurrent_bounded_queue<unsigned int> current_b;

     int map_size = num_of_nodes /32 + 1;
     unsigned long* bitmap = (unsigned long*) malloc(sizeof(unsigned long)*map_size);
     for (int i=0; i<map_size; i++) bitmap[i] = 0;

	 gettimeofday(&start, 0);

     set_bit(source_node_no, bitmap);
	 current_a.push(source_node_no);
	 cost[source_node_no] = 0;


	 omp_set_num_threads(num_of_threads);
	 int k = 0;
     bool stop = false;
     
     do {
          if (k%2 == 0) {
#pragma omp parallel
               while (!current_a.empty()) {
                    unsigned int index = -1; 
                    current_a.try_pop(index);
                    if(index != -1)
                    {
                         Node cur_node = node_list[index];
                         for (int i = cur_node.start; i < (cur_node.start+cur_node.edge_num); i++)
                         {
                              unsigned int id = edge_list[i].dest;
                              if (!test_bit(id, bitmap)) {
                                   int its_color = sync_test_and_set_bit(id, bitmap);
                                   if (!its_color) {
                                        cost[id] = cost[index] + 1;
                                        current_b.push(id);
                                   }
                              }
                         }
                    } 
               } 
               if (current_b.empty()) stop =true;

          } else {
               
#pragma omp parallel
               while (!current_b.empty()) {
                    unsigned int index = -1;
                    current_b.try_pop(index);
                    if (index != -1) {
                         Node cur_node = node_list[index];
                         for (int i = cur_node.start; i < (cur_node.start+cur_node.edge_num); i++)
                         {
                              unsigned int id = edge_list[i].dest;
                              if (!test_bit(id, bitmap)) {
                                   int its_color = sync_test_and_set_bit(id, bitmap);
                                   if (!its_color) {
                                        cost[id] = cost[index] + 1;
                                        current_a.push(id);
                                   }
                              }
                         } 
                    }
               } 
               if (current_a.empty()) stop =true;
          }
          k++;
	 } while(!stop);

	 gettimeofday(&end, 0);
	 time_used = 1000000 * (end.tv_sec - start.tv_sec) +
		  end.tv_usec - start.tv_usec;
	 time_used /= 1000000;
	 printf("used time: %f\n", time_used);

     free(bitmap);
	 return time_used;
	 
}
