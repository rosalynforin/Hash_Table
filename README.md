## C语言实现的简单的散列表数据结构

[TOC]



### 1.实现介绍：

本工程源码是由两个头文件：【hash_table_core.h 、 hash_table_user_config.h】组成。

#### hash_table_core.h：

a.负责散列表的相关操作，提供了散列表几个基本的操作，包括：
	1.散列表的申请。
	2.散列表的空间大小重新分配。
	3.散列表的销毁。
	4.数据插入。(当存在相同关键字时，数据将会覆盖)
	5.数据查询。
	6.数据修改。
	7.数据删除。

b.本工程实现的散列表采用了**拉链法**解决散列冲突。提供几个散列表属性相关变量：
	1.当前散列表键值对数量。
	2.当前散列表存储数组长度。
	3.当前散列表存储数组未使用的节点个数。
	4.当前散列表存储数组中所有超出设定阈值（散列冲突键值个数）范围的节点个数。

当散列冲突个数不断增多，搜索时间不断增大：从O(1) 不断逼近O(n)，可通过上述的属性变量加上用户自己的处理逻辑，合理的调用**散列表的空间大小重新分配函数**来缓解散列冲突带来的问题。



#### hash_table_user_config.h：

由用户实现相关自定义数据类型和映射函数等等。



------

### 2.使用介绍：

为了满足更广泛的使用，所以需要用户根据自己的需求来实现散列映射的函数，以及提供散列表必须的配置。



#### a.需要填充四个配置宏：

| 宏                                 | 释义                                           |
| ---------------------------------- | ---------------------------------------------- |
| REGISTER_USER_TABLE_SIZE( xxx )    | 散列存储数组空间大小注册                       |
| REGISTER_USER_CONFLICT_SIZE( xxx ) | 散列存储数组里单节点中最大冲突键值个数阈值注册 |
| REGISTER_USER_KEY_TYPE( xxx )      | 用户键类型注册                                 |
| REGISTER_USER_VALUE_TYPE( xxx )    | 用户值类型注册                                 |

散列表存储数组里单节点中最大冲突键值个数阈值：

因为存在散列冲突，且采用了拉链法，散列存储数组每个节点可能存在大于1个的用户数据，设定阈值，用于统计散列存储数组中超出阈值的那些节点的总共数量，从而为判断整个散列储存数组中散列冲突的严重长程度提供数据支持。



#### b.实现函数的注册：

| 宏                                  | 释义                                                         |
| ----------------------------------- | ------------------------------------------------------------ |
| REGISTER_USER_HASH_MAP( xxx, ... )  | 用户的散列映射函数注册（不可缺省）                           |
|                                     |                                                              |
| \#define USER_KEY_MATCH_FUNC   xxx  | 散列关键字匹配函数注册（可缺省）                             |
| \#define USER_KEY_HASHMAP_FUNC  xxx | 关键字与不同散列映射函数匹配函数注册(当只有一个散列函数时,可缺省) |
|                                     |                                                              |
| \#define USER_MEM_ALLOC_FUNC  xxx   | 用户自定义内存申请函数注册（可缺省）                         |
| \#define USER_MEM_FREE_FUNC   xxx   | 用户自定义内存释放函数注册（可缺省）                         |

注意：

1.可存在多个映射函数，最多10个。

2.建议由用户自定义实现散列关键字匹配函数，如果不自己实现，则采用默认的单字节逐个对比判断是否匹配，会影响搜索效率。

3.如果不使用用户自定义的内存申请与释放函数，则采用默认的标准库函数：malloc()与free()

4.尽可能的使用C99标准。

#### c.简单示例：

```c
//在hash_table_user_config.h文件中,配置一个键值对都是int型，且散列函数只有一个的散列表


//散列映射函数(采用简单的直接取值法)
//(需参考hash_table_core.h的对应的函数类型：hashmap_func)
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
    
/***************************************************************************************************/
     
//在main.c中，使用配置好的散列表    
 
#include <stdio.h>

#include "hash_table_core.h" 
    
int main(int argc, char** argv)    
{
	//自定义键值对
	int key;
	int values;
    
	/**************创建****************/
    hashmap_t* hm_hdle = NULL;
	hm_hdle = HashInit();
	if (NULL == hm_hdle) {
		return -1;
	}
    
	/**************插入****************/ 
	key = 1;		//插入的关键字
	values = 10;	//插入的值
	HashInsert(&hm_hdle, &key, &values);
    
	/**************查找****************/ 
	key = 1;  //输入的关键字
	values = 0;
	HashFind(&hm_hdle, &key, &values);

	/**************修改**************/ 
	key = 1;
	values  = 5.6;	//修改的值
	HashInsert(&hm_hdle, &key, &values);

	/**************删除**************/ 
	//删除指定关键字的数据
	key = 1;
	HashRemove(&hm_hdle, &key);

	/**************销毁**************/ 
	HashDestroy(&hm_hdle); 
    
    return 0;
}  
    
    
```

### 3.参考函数：

| 散列相关操作函数                                             | 释义                 |
| ------------------------------------------------------------ | -------------------- |
| hashmap_t* HashInit (void)                                   | 创建散列表           |
| void HashDestroy ( hashmap_t** hm )                          | 销毁散列表           |
| int HashSizeReset ( hashmap_t** hm, size_t tablesize )       | 散列存储数组重设大小 |
| int HashInsert ( hashmap_t** hm, hash_key* key, hash_val* value ) | 散列表插入元素       |
| int HashRemove ( hashmap_t** hm, hash_key* key )             | 散列表移除指定元素   |
| int HashFind ( hashmap_t** hm, hash_key* key, hash_val* value ) | 散列表查找指定元素   |

