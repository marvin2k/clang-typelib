#ifndef TEST_HEADER_DATA_STRUCT_IN_STRUCT_H
#define TEST_HEADER_DATA_STRUCT_IN_STRUCT_H

typedef union
{
  struct __pthread_mutex_s
  {
    int __lock;
    unsigned int __count;
    int __owner;

    unsigned int __nusers;



    int __kind;

    int __spins;
  } __data;
  char __size[40];
  long int __align;
} pthread_mutex_t;

#endif /*TEST_HEADER_DATA_STRUCT_IN_STRUCT_H*/
