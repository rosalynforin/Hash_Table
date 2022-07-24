


#include <stdio.h>

#include "hash_table_core.h"



int main(int argc, char** argv) 
{
	//自定义键值对
	int key;
	int values;

	hashmap_t* hm_hdle = NULL;

	/**************创建****************/ 
	hm_hdle = HashInit();
	if (NULL == hm_hdle) {
		printf("HashInit error!\n");
		return -1;
	}
	
	/**************插入****************/ 
	key = 1;		//插入的关键字
	values = 10;	//插入的值
	HashInsert(&hm_hdle, &key, &values);


	/*************重设大小**************/ 
	// HashSizeReset(&hm_hdle, hm_hdle->table_size * 2); //扩展为原来的2倍

	/**************查找****************/ 
	key = 1;  //输入的关键字
	values = 0;
	HashFind(&hm_hdle, &key, &values);
	printf("f(%d) = [%d]\n", key, values);


	/**************修改**************/ 
	key = 1;
	values  = 5;	//修改的值
	HashInsert(&hm_hdle, &key, &values);


	/**************删除**************/ 
	//删除指定关键字的数据
	key = 1;
	HashRemove(&hm_hdle, &key);


	/**************销毁**************/ 
	HashDestroy(&hm_hdle);

	
	return 0;
}




