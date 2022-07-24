/*****************************************************************
1.在用户自定义的配置文件下，用户需要填充的四个宏，以及最少实现一个散列映射函数：


    1.散列表存储数组的大小
    REGISTER_USER_TABLE_SIZE(xxx)


    2.散列表存储数组里单节点中最大冲突键值个数阈值
    REGISTER_USER_CONFLICT_SIZE(xxx)


    3.用户关键字类型注册
    REGISTER_USER_KEY_TYPE(xxx)


    4.用户值类型注册
    REGISTER_USER_VALUE_TYPE(xxx)

    5.函数实现


    a.用户自定义散列映射函数实现（可存在多个映射函数,最多10个）
    (需参考hash_table_core.h的对应的函数类型：hashmap_func)


    b.用户自定义散列关键字之间的匹配函数实现(可缺省)
    (需参考hash_table_core.h的对应的函数类型：hash_key_key_match)


    c.用户自定义散列关键字与散列映射函数匹配实现(可缺省)
    (需参考hash_table_core.h的对应的函数类型：hash_key_func_match)


    d.用户自定义的内存申请函数实现(可缺省)
    (需参考hash_table_core.h的对应的函数类型：mem_alloc)


    e.用户自定义的内存释放函数实现(可缺省)
    (需参考hash_table_core.h的对应的函数类型：mem_free)

 

    6.函数注册
    REGISTER_USER_HASH_MAP( xxx )                //散列映射

    #define USER_KEY_MATCH_FUNC   xxx            //键与键匹配

    #define USER_KEY_HASHMAP_FUNC xxx            //键与散列映射匹配

    #define USER_MEM_ALLOC_FUNC   xxx            //内存申请
    #define USER_MEM_FREE_FUNC    xxx            //内存释放


2.用户可以自定义多套散列表配置，此时需要用到用户自定义的宏开关来区分。
（注意：一个.c文件或者.cpp文件对应一套散列表配置。）
例如：
a.在本文件中定义多套散列表配置：

#ifdef MY_HASHMAP_1

...
...
...

#ENDIF


#ifdef MY_HASHMAP_2

...
...
...

#ENDIF




b.在指定的.c文件或者.cpp文件中：

#define MY_HASHMAP_1  //选择对应的那套散列表。
#include "hash_table_core.h"

....
....

****************************************************************/
//用户函数实现
//配置一个键值对都是int型，且散列函数只有一个的散列表


//散列映射函数(采用简单的直接取值法)
static int HashMapFunc(void* key, size_t* key_index, size_t table_len)
{
    int* tmp = (int*)key;

    *key_index = (size_t)(*tmp <0? -1*(*tmp) : *tmp) % table_len;

    return 0; //成功
}




//对应配置注册
REGISTER_USER_TABLE_SIZE( 100 )                 //HASHMAP存储空间大小
REGISTER_USER_CONFLICT_SIZE( 2 )                //散列表存储数组里单节点中最大冲突键值个数阈值
REGISTER_USER_KEY_TYPE( int )                   //用户键类型
REGISTER_USER_VALUE_TYPE( int )                 //用户值类型

REGISTER_USER_HASH_MAP( HashMapFunc )           //散列映射



