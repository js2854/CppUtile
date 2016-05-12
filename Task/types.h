#ifndef __TYPES_H__
#define __TYPES_H__

#include <unistd.h>
#include <sys/types.h>
#include <queue>
using std::priority_queue;

typedef signed char         SInt8;
typedef unsigned char       UInt8;
typedef signed short        SInt16;
typedef unsigned short      UInt16;
typedef signed int          SInt32;
typedef unsigned int        UInt32;
typedef signed long long    SInt64;
typedef unsigned long long  UInt64;

typedef struct T_Node
{
    UInt64 u64Priority;
    void  *pValue;
    T_Node() : u64Priority(0), pValue(NULL) {}
    T_Node(UInt64 i_u64Priority, void *i_pValue) : u64Priority(i_u64Priority), pValue(i_pValue) {}
    friend bool operator< (T_Node n1, T_Node n2)
    {
        // 默认元素越大，优先级越高; 但我们这里需要元素越小，优先级越高
        return n1.u64Priority > n2.u64Priority;
    }
}T_Node;

typedef priority_queue<T_Node> PriorityQueue;

#endif
