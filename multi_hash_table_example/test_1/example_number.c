
#include <stdio.h>
#include <sys/time.h>




#define HASH_USER_ONE
#include "hash_table_core.h"


/*
整型数的散列表测试
1.创建散列表。
2.插入键值对。（当冲突溢出的个数超过指定的范围时，重设的散列表大小，并重映射键值对，以达到减少散列冲突，最终达到提高查找速度尽量接近O(1)）
3.查找指定的键值对。
4.修改指定键值对。
5.删除指定键值对。
6.销毁散列表。
*/
void number_example(void)
{
	int ret = 0;
	hashmap_t* hm_hdle = NULL;
	int key;
	float values;
	

	/**************创建****************/ 
	hm_hdle = HashInit();
	if (NULL == hm_hdle) {
		printf("HashInit error!\n");
		return;
	}
	
	/**************插入****************/ 
    //插入15000个键值对数据
	for (int i=0; i<15000; i++) {
		key = (hash_key)i;				//插入的关键字
		values = (hash_val)(i * 0.1);	//插入的值
		if (0 > HashInsert(&hm_hdle, &key, &values)) {
		printf("HashInsert:f(%d)=%f err!!\n", key, values);  
		}
	}

	//显示散列数组相关属性：键值对数量、散列数组剩余未使用的节点个数、冲突节点溢出个数
	printf("\n---------\n[number_example](before):\ntab_size:[%ld]\nkeyval_nums:[%ld]\nnode_unused_nums:[%ld]\novflw_node_nums:[%ld]\n---------\n",\
	hm_hdle->table_size, hm_hdle->keyval_nums, hm_hdle->node_unused_nums, hm_hdle->ovflw_node_nums);



	//假设溢出数据到达5000时，重新设置散列数组大小，以此减少散列冲突数量
	if (hm_hdle->ovflw_node_nums >= 5000) {
		ret = HashSizeReset(&hm_hdle, 20000);
		if (0 > ret) {
			printf("hash table resize err\n");
		}
	}

	printf("\n---------\n[number_example](after):\ntab_size:[%ld]\nkeyval_nums:[%ld]\nnode_unused_nums:[%ld]\novflw_node_nums:[%ld]\n---------\n",\
	hm_hdle->table_size, hm_hdle->keyval_nums, hm_hdle->node_unused_nums, hm_hdle->ovflw_node_nums);

	/**************查找****************/ 
	//计时
	// struct timeval start;
	// struct timeval end;
	//gettimeofday(&start,NULL);
	//gettimeofday(&end,NULL);
	//(end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec)

	for (int i=9995; i<10005; i++) {
		key = (hash_key)i;  //输入的关键字
		values  = 0;
		ret = HashFind(&hm_hdle, &key, &values);
		if (0 > ret) {
		printf("HashFind err,key:%d!\n", key);    
		}
		else printf("f(%d) = [%f]\n", key, values);
	}


	/**************修改**************/ 
	key = 1;
	values  = 5.6;	//修改的值
	if(0 > HashInsert(&hm_hdle, &key, &values)){
	 printf("modify err, value:%f!\n", values);  
	}
	
	// values  = 0;
	// if(0 > HashFind(&hm_hdle, &key, &values)){
	//  printf("HashFind err,key:%d\n", key);    
	// }
	// else printf("after modify,value:%f!\n", values);
	
	/**************删除**************/ 
	key = 1;
	HashRemove(&hm_hdle, &key);
	
	// values = 0;
	// if(0 > HashFind(&hm_hdle, &key, &values)){
	//  printf("HashFind err,key:%d\n", key);    
	// }
	// else printf("value:%f!\n", values);

	/**************销毁**************/ 
	HashDestroy(&hm_hdle);

	
}

