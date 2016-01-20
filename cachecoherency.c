#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>

#define MEM_SIZE (1 << 16)



typedef enum {
  read_t,
  write_t
} ThreadType;


struct data {
  int num_iter;
  int *data_ptr;
  int cpu;
  ThreadType type;
};


void *readThread(void *arg)
{
  struct data *pdata = arg;
  int a = 0;
  int i;
  cpu_set_t c_set;
  volatile int *ptr = pdata->data_ptr;
  *ptr = 1;
  CPU_ZERO(&c_set);
  CPU_SET(pdata->cpu, &c_set);
  i = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &c_set);
  printf("readThread started on cpu=%d ret_affinity=%d\n", pdata->cpu, i);
  for(i = 0; i < pdata->num_iter; i++)
    a += *ptr;
  *ptr = a;
}


void *writeThread(void *arg)
{
  struct data *pdata = arg;
  int a = 0;
  int i;
  cpu_set_t c_set;
  volatile int *ptr = pdata->data_ptr;
  a = 1;
  CPU_ZERO(&c_set);
  CPU_SET(pdata->cpu, &c_set);
  i = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &c_set);
  printf("writeThread started on cpu=%d ret_affinity=%d\n", pdata->cpu, i);
  for(i = 0; i < pdata->num_iter; i++)
    *ptr += a;
}



int main(int argc, char *argv[])
{
  pthread_t rTid, wTid;
  int *array;
  int cache_line_sz = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
  if (argc < 6) {
    printf("%s: num_iter rd_offset wr_offset rd_cpu wr_cpu\n\n", argv[0]);
    return -1;
  }
  printf("Level 1 Data cache line size=%d\n", cache_line_sz);
  posix_memalign((void *)&array, cache_line_sz, MEM_SIZE);
  int num_iter = atoi(argv[1]);
  int rd_offset = atoi(argv[2]);
  int wr_offset = atoi(argv[3]);
  int rd_cpu  = atoi(argv[4]);
  int wr_cpu  = atoi(argv[5]);
  
  struct data r_data = {
    .num_iter = num_iter,
    .data_ptr = array + rd_offset,
    .type = read_t,
    .cpu = rd_cpu
  };

  struct data w_data = {
    .num_iter = atoi(argv[1]),
    .data_ptr = array + wr_offset,
    .type = write_t,
    .cpu = wr_cpu    
  };
  printf("rd_offset=%d wr_offset=%d num_iter=%d\n", rd_offset, wr_offset, num_iter);
  pthread_create(&rTid, NULL, readThread, &r_data);
  pthread_create(&wTid, NULL, writeThread, &w_data);
  pthread_join(rTid, NULL);
  pthread_join(wTid, NULL);
  printf("finished wr_data=%d rd_data=%d\n", *w_data.data_ptr, *r_data.data_ptr);
  return 0;
}
