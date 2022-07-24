/*
 * @Author: Mr.Bear<https://github.com/rosalynforin>
 * @Date: 2022-06-19 17:18:20
 * @LastEditors: Mr.Bear
 * @LastEditTime: 2022-06-21 22:56:50
 * @Description: 散列表结构头文件
 * 
 * Copyright (c) 2022 by Mr.Bear, All Rights Reserved. 
 */



#ifndef HASH_MAP_H
#define HASH_MAP_H



// #ifdef __cplusplus
// extern "C" {
// #endif



/****************************************相关函数类型定义*****************************************/
/*** 
 * @description:散列映射函数类型定义  
 * @param {void*}：散列关键字地址
 * @param {size_t*}：散列表数组索引位置
 * @param {size_t}：散列表数组范围
 * @return {int}:成功：0 失败：<0
 */
typedef int (*hashmap_func)(void* , size_t* , size_t );


/*** 
 * @description:散列关键字之间的匹配函数类型定义  
 * @param {void*}：索引关键字地址
 * @param {void*}：被匹配的关键字地址
 * @return {int}:成功：0 失败：<0
 */
typedef int (*hash_key_key_match)(void* , void* );



/*** 
 * @description:散列关键字与散列映射匹配函数类型定义  
 * @param {void*}:列关键字地址
 * @param {hashmap_func*}：散列映射函数数组
 * @return {hashmap_func}: 散列函数地址
 */
typedef hashmap_func (*hash_key_func_match)(void*  , const hashmap_func* );




/*** 
 * @description:内存申请函数类型定义
 * @note: 
 * @param {size_t}:内存申请大小
 * @return {void*}:成功：返回内存申请的空间 失败：NULL
 */
typedef void* (*mem_alloc)(size_t );



/*** 
 * @description:内存申请函数类型定义
 * @note: 
 * @param {void*}:需要释放的内存地址
 * @return {void}：NULL
 */
typedef void (*mem_free)(void* );



/****************************************注册宏**************************************************/

//散列表存储数组的大小注册
#define REGISTER_USER_TABLE_SIZE(nums) \
static const size_t default_table_size = (size_t)nums;

//单节点中最大冲突键值个数阈值注册
#define REGISTER_USER_CONFLICT_SIZE(nums) \
static const size_t default_conflict_size = (size_t)nums;


//关键字类型注册
#define REGISTER_USER_KEY_TYPE(ktype) \
typedef ktype hash_key;


//值类型注册
#define REGISTER_USER_VALUE_TYPE(vtype) \
typedef vtype hash_val;


//参数个数表(目前最多10个参数，可自行修改) 
#define VARGS_NUM_FORM(_1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, __INDEX, ...) __INDEX    
//索引参数个数 
#define GET_VARGS_NUMS(...) 	VARGS_NUM_FORM(__VA_ARGS__,9,8,7,6,5,4,3,2,1)




/*** 
 * @description: 散列函数注册
 * @note: 有多个散列映射函数时，从左到右按数组从小到大的顺序排列注册。
 * @param {__hash_mapfunc}:首个散列映射函数
 * @param {...}:其次的散列映射函数
 */
#define REGISTER_USER_HASH_MAP(__hash_mapfunc, ...)                                                     \
static const hashmap_func __hashmap_func[GET_VARGS_NUMS(__VA_ARGS__)+1] = {__hash_mapfunc,##__VA_ARGS__};






/*************************************用户的散列表配置头文件**************************************/
#include "hash_table_user_config.h"


/***************************************相关缺省函数定义******************************************/

#ifndef USER_KEY_MATCH_FUNC
//默认索引关键字匹配函数实现
static int __default_key_match(void* dst_key, void* src_key)
{
    size_t sizes = sizeof(hash_key);
    char* src = (char*)dst_key;
    char* dst = (char*)src_key;

    while(sizes--) {
        if(*src++ != *dst++){
            return 0;
        }
    }
    return 1;
}
#define USER_KEY_MATCH_FUNC  __default_key_match
#endif


#ifndef USER_KEY_HASHMAP_FUNC
//默认的索引关键字与散列映射函数匹配实现
static hashmap_func __default_key_func_match(void* key, const hashmap_func* hsfunc)
{
    (void)key; 
    return (*hsfunc); //当前只定义了一个散列映射函数
}
#define USER_KEY_HASHMAP_FUNC  __default_key_func_match
#endif


#ifndef USER_MEM_ALLOC_FUNC
#include <stdlib.h>

//默认内存申请函数
static void* __default_alloc(size_t size)
{
    char* ptr = NULL;
    ptr = (char*)malloc(size);
    if(NULL == ptr) return NULL;
    
    return (void*)ptr;
}
#define USER_MEM_ALLOC_FUNC  __default_alloc
#endif


#ifndef USER_MEM_FREE_FUNC
#include <stdlib.h>
//默认内存释放函数
static void __default_free(void* ptr)
{
    if(NULL != ptr) {
        free(ptr);
    }
}
#define USER_MEM_FREE_FUNC  __default_free
#endif

/************************************************************************************************/





//返回结果值
enum hash_result {    
    hash_key_error  = -3,
    hash_hdle_null  = -2,
    hash_error      = -1,
    hash_success    = 0,
    hash_node_ovhflw= 1 //当一个散列表节点存储的链表过长时
};





/*
当存在散列冲突时，采用拉链法，即计算出到相同值时，
以链表的形式储存在相同地址的值。
*/
//键值对结构体
typedef struct hash_node hash_node_t;
struct hash_node {
    
    hash_key key;           //键
    hash_val value;         //值
    hash_node_t* next;      //链表的下个节点

};


//散列表存储数组句柄
typedef struct hash_list {

    size_t list_size; //节点链表长度。
    hash_node_t* node_list;

}hash_list_t;




//hashmap结构体
typedef struct hashmap {

    hash_list_t* table;          //散列存储数组地址
    size_t table_size;           //存储数组长度
    size_t keyval_nums;          //当前存入键值对个数
    size_t node_unused_nums;     //散列存储数组中未使用的节点个数
    size_t ovflw_node_nums;      //散列冲突中超过阈值的溢出节点数

}hashmap_t;



/******************************************实现***************************************************/


/*** 
 * @description: 内存数据拷贝
 * @param {void* dst}:目标地址
 * @param {void* src}：源地址
 *  @param {size_t count}：拷贝长度(Byte)
 * @return {void}
 */
static void __hash_memcpy(void* dst, void* src, size_t count)
{
    while(count--) {
        *(char*)dst = *(char*)src;
        dst = (char*)dst + 1;
        src = (char*)src + 1;
    }
}


/*** 
 * @description: 内存数据清零
 * @param {void* dst}:目标地址
 *  @param {size_t count}：拷贝长度(Byte)
 * @return {void}
 */
static void __hash_bzero(void* dst, size_t count) 
{
    while(count--) {
        *(char*)dst = 0;
        dst = (char*)dst + 1;
    }
}



/*** 
 * @description: 创建元素结点(因为是链表存储)
 * @param {hash_key*}:指定散列关键字地址
 * @param {hash_val*}：指定散列值地址
 * @return {hash_node_t*}：散列表节点地址：success , NULL:fail
 */
static hash_node_t* __create_node(hash_key* key, hash_val* value)
{
    hash_node_t* new_node = (hash_node_t*)USER_MEM_ALLOC_FUNC(sizeof(hash_node_t));
    if(NULL == new_node) return NULL;
  

    __hash_memcpy(&new_node->key, key, sizeof(hash_key));
    __hash_memcpy(&new_node->value, value, sizeof(hash_val));

    new_node->next = NULL;
    return new_node;
}







/*** 
 * @description: 散列表中每个数据节点销毁
 * @param {hash_node_t*}:要销毁的散列节点地址
 * @return {*}
 */
static void __hash_node_delete(hash_node_t* to_destroy)
{
    if (NULL != to_destroy) {
        __hash_node_delete(to_destroy->next);
        USER_MEM_FREE_FUNC(to_destroy);
        to_destroy = NULL;
    }
}




/**
 * @description: 散列映射
 * @param {void*}：散列关键字地址
 * @param {size_t*}：散列表数组索引位置
 * @param {size_t}：散列表数组范围
 * @return {int}:成功：0 失败：<0
 */
static int __hash_get_val(void* key, size_t* key_index, size_t tb_size)                             
{                         
    hashmap_func tmp_func = NULL;

    tmp_func = USER_KEY_HASHMAP_FUNC(key, __hashmap_func);
    if(NULL == tmp_func) {
        return -1;
    }

    return tmp_func(key, key_index, tb_size);                   
}




/***********************************************************************************************/

/*** 
 * @description:散列表存储数组大小重设置
 * @note: 注意内存空间容量大小是否足够
 * @param {hashmap_t**}：散列表控制句柄地址
 * @param {long tablesize} ：申请尺寸
 * @return {int}: hash_success:success  other:fail 
 */
static int HashSizeReset(hashmap_t** hm, size_t tablesize)
{
    size_t index = 0;
    size_t tmp_unused_nums = 0;
    size_t tmp_ovflw_nums = 0;
    hash_list_t  ptr = {0, NULL};
    hash_node_t* prv = NULL;
    hash_node_t* cur = NULL;
    hash_node_t* tmptr = NULL;
    hash_node_t* new_node = NULL;

    if(NULL == hm) return hash_error;

    //申请新的空间大小的存储数组
    hash_list_t* temp = (hash_list_t*)USER_MEM_ALLOC_FUNC(tablesize * sizeof(hash_list_t));
    if(NULL == temp)  return hash_error;
    __hash_bzero(temp, tablesize * sizeof(hash_list_t));

    //初始化散列存储数组
    if(NULL == (*hm)->table) {
        (*hm)->table = temp;
        return hash_success;
    }

    tmp_unused_nums = tablesize;
    //旧存储数组中的键值对重新散列映射到新的空间大小的存储数组中
    for (size_t i=0; i<(*hm)->table_size; i++) {
        ptr = (*hm)->table[i];
        tmptr = ptr.node_list;

        for (size_t j=0; j < ptr.list_size; j++) {
            __hash_get_val((void*)&(tmptr->key), &index, tablesize);
            index %= tablesize;  
            cur = temp[index].node_list;
            prv = NULL;

            while (NULL != cur) {
                prv = cur;
                cur = cur->next;
            }
        
            new_node = __create_node(&(tmptr->key), &(tmptr->value));
            if (NULL == new_node) {

                //先把之前申请了的全部释放
                for (size_t k=0; k < tablesize; k++) {
                    __hash_node_delete(temp[k].node_list); 
                } 

                return hash_error;
            } 

            if (NULL != prv) {
                prv->next = new_node;
            } 
            else {
                temp[index].node_list = new_node;
                tmp_unused_nums -= 1;
            } 

            ++temp[index].list_size;
            if (default_conflict_size  < temp[index].list_size) {
                tmp_ovflw_nums += 1;
            }

            tmptr = tmptr->next;
        }
    }

    //释放旧的散列存储数组的内存空间
    for(size_t i=0; i < (*hm)->table_size; i++) {
        __hash_node_delete((*hm)->table[i].node_list); 
    } 

    USER_MEM_FREE_FUNC((*hm)->table); 
    (*hm)->table = NULL;
    
    (*hm)->table_size = tablesize;
    (*hm)->node_unused_nums = tmp_unused_nums;
    (*hm)->ovflw_node_nums = tmp_ovflw_nums;
    (*hm)->table = temp;

    return hash_success;
}






/**
 * @description: 散列表的初始化
 * @param {*}
 * @return {hashmap_t*}：散列表控制句柄：success , NULL:fail
 */
static hashmap_t* HashInit(void)
{

    hashmap_t* tmp = NULL;

    tmp = (hashmap_t*)USER_MEM_ALLOC_FUNC(sizeof(hashmap_t));
    if(NULL == tmp)  return NULL;
    __hash_bzero(tmp, sizeof(hashmap_t));

    //初始化散列表数组
    if(hash_success != HashSizeReset(&tmp, default_table_size))  return NULL;

    // tmp->table = (hash_list_t*)USER_MEM_ALLOC_FUNC(default_table_size * sizeof(hash_list_t));
    // if(NULL == tmp->table)  return NULL;
    // __hash_bzero(tmp->table, default_table_size * sizeof(hash_list_t));

    //当前散列储存数组大小
    tmp->table_size = default_table_size;
    tmp->node_unused_nums = default_table_size;

    return tmp;
}







/*** 
 * @description:散列表控制句柄销毁 
 * @param {hashmap_t**}：散列表控制句柄地址
 * @return {*}
 */
static void HashDestroy(hashmap_t** hm)
{
    if(NULL == hm) return;
   
    for (size_t i=0; i < (*hm)->table_size; i++) {
        __hash_node_delete((*hm)->table[i].node_list); 
    } 

    USER_MEM_FREE_FUNC((*hm)->table); 
    (*hm)->table = NULL;
    USER_MEM_FREE_FUNC(*hm);
    *hm = NULL; 

    return;
}   








/*** 
 * @description: 插入/修改元素
 * 1.先通过散列函数求出当前元素在散列表中的下标 offset
 * 2.在链尾处添加新结点
 * 3.如果当前链表中存在与插入元素相同的 key 值，则直接覆盖原来的值
 * 
 * @param {hashmap_t**}：散列表控制句柄地址
 * @param {hash_key*}：指定散列关键字地址
 * @param {hash_val*}：指定散列值地址
 * @return {int}: hash_success:success  other:fail
 */
static int HashInsert(hashmap_t** hm, hash_key* key, hash_val* value)
{
    size_t offset = 0;
    hash_node_t* new_node = NULL;

    if(NULL == hm) return hash_hdle_null;


    //获取数据地址
    if(0 > __hash_get_val((void*)key, &offset, (*hm)->table_size)) {
        return hash_key_error;
    }


    offset %= (*hm)->table_size;

    hash_node_t* cur = NULL;
    hash_node_t* ptr = (*hm)->table[offset].node_list;

    while (NULL != ptr) {  
        if (USER_KEY_MATCH_FUNC(&(ptr->key), key)) {
            __hash_bzero(&(ptr->value), sizeof(hash_val)); //修改原来的值
            __hash_memcpy(&(ptr->value), value, sizeof(hash_val)); //修改原来的值
            return hash_success;
        }
        cur = ptr;
        ptr = ptr->next;
    }

    new_node = __create_node(key , value);
    if(NULL == new_node) return hash_error;


    if (NULL != cur) {
        cur->next = new_node;
    }
    else {
        (*hm)->table[offset].node_list = new_node;
        (*hm)->node_unused_nums -= 1;
    } 

    ++(*hm)->table[offset].list_size;
    if (default_conflict_size  < (*hm)->table[offset].list_size) {
        ++(*hm)->ovflw_node_nums;
    }

    ++(*hm)->keyval_nums;
    return hash_success;
}







/*** 
 * @description: 释放指定散列节点
 * @param {hash_node_t**}：指定散列节点地址
 * @return {*}
 */
inline static void DestroyNode(hash_node_t* node)
{
    USER_MEM_FREE_FUNC(node);
    node = NULL;
}






/*** 
 * @description: 删除指定元素
 * 1.先通过散列函数求出散列表的下标 offset
 * 2.在该位置的链表中找到该元素
 * 3.先保存前一个节点，才能删除当前要删除的结点 
 *  
 * @param {hashmap_t**}:散列表控制句柄地址
 * @param {hash_key*}:指定散列关键字的地址
 * @return {*}:hash_success:success  other:fail
 */
static int HashRemove(hashmap_t** hm, hash_key* key)
{
    size_t offset = 0;
    hash_node_t* prev = NULL;
    hash_node_t* cur   = NULL;

    if(NULL == hm) return hash_hdle_null;

 
    //获取数据地址
    if(0 > __hash_get_val((void*)key, &offset, (*hm)->table_size)) {
        return hash_key_error;
    }

    offset %= (*hm)->table_size;


    prev = (*hm)->table[offset].node_list; 
    cur  = (*hm)->table[offset].node_list; //起始点

    while (NULL != cur) {
        if (USER_KEY_MATCH_FUNC(&(cur->key), key)) { //找到对应节点
            if (NULL != cur->next) {
                if (prev != cur) {
                    prev->next = cur->next;
                }
                else {
                    (*hm)->table[offset].node_list = cur->next;  
                }
            }
            
            DestroyNode(cur);
            (*hm)->table[offset].list_size -= 1;
            if (default_conflict_size < (*hm)->table[offset].list_size) {
                (*hm)->ovflw_node_nums -= 1;
            }
            else if (0 == (*hm)->table[offset].list_size) {
                (*hm)->node_unused_nums += 1;
                (*hm)->table[offset].node_list = NULL;
            }
            --(*hm)->keyval_nums;
            return hash_success;
        }

        prev = cur;
        cur = cur->next;
    }  

    return hash_error;
}








//查找指定元素
/*** 
 * @description: 查找指定元素
 * 1.先通过散列函数找到当前 key 在散列表中的下标
 * 2.找到以后遍历链表，找到就返回，找不到就退出
 * @param {hashmap_t**}：散列表控制句柄地址
 * @param {hash_key*}：指定散列关键字的地址
 * @param {hash_val*}：指定散列值的地址
 * @return {*}:hash_success:success  other:fail
 */
static int HashFind(hashmap_t** hm, hash_key* key, hash_val* value)
{     
    size_t offset = 0; 
    hash_node_t* cur =NULL;

    if(NULL == hm) return hash_hdle_null;
   
    
    //获取数据地址
    if(0 > __hash_get_val((void*)key, &offset, (*hm)->table_size)) {
        return hash_key_error;
    }


    offset %= (*hm)->table_size;
    

    cur = (*hm)->table[offset].node_list;
    while(NULL != cur) {
        if (USER_KEY_MATCH_FUNC(&(cur->key), key)) {
            __hash_memcpy(value, &(cur->value), sizeof(hash_val));
            return hash_success;
        }
        cur = cur->next;
    }

    return hash_error;
}


// #ifdef __cplusplus
// }
// #endif

#endif




