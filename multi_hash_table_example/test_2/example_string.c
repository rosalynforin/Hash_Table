
#include <stdio.h>
#include <sys/time.h>



#define HASH_USER_TWO
#include "hash_table_core.h"


/*
字符串型的散列表测试
1.创建散列表。
2.插入键值对。
3.查找指定的键值对。
4.修改指定键值对。
5.删除指定键值对。
6.销毁散列表。
*/
void string_example(void)
{
	int ret = 0;
	hashmap_t* hm_hdle = NULL;
	hash_key key;
	hash_val values;
	

	/**************创建****************/ 
	hm_hdle = HashInit();
	if (NULL == hm_hdle) {
		printf("HashInit error!\n");
		return;
	}
	
	/**************插入****************/ 

	hash_key key_0 = {"LiMing"};
	values.ages = 15;
	values.height = 1.67; 
	if (0 > HashInsert(&hm_hdle, &key_0, &values)) {
	printf("HashInsert:f(%s)=[%d, %f] err!!\n", key_0, values.ages, values.height);  
	}




	hash_key key_1 = {"ZhangSan"};
	values.ages = 16;
	values.height = 1.77; 
	if (0 > HashInsert(&hm_hdle, &key_1, &values)) {
	printf("HashInsert:f(%s)=[%d, %f] err!!\n", key_1, values.ages, values.height);  
	}




	hash_key key_2 = {"WangWu"};
	values.ages = 17;
	values.height = 1.72; 
	if (0 > HashInsert(&hm_hdle, &key_2, &values)) {
	printf("HashInsert:f(%s)=[%d, %f] err!!\n", key_2, values.ages, values.height);  
	}


	printf("\n---------\n[string_example]:\ntab_size:[%ld]\nkeyval_nums:[%ld]\nnode_unused_nums:[%ld]\novflw_node_nums:[%ld]\n---------\n",\
	hm_hdle->table_size, hm_hdle->keyval_nums, hm_hdle->node_unused_nums, hm_hdle->ovflw_node_nums);


	/**************查找****************/ 
	// struct timeval start;
	// struct timeval end;
	//gettimeofday(&start,NULL);
	//gettimeofday(&end,NULL);
	//(end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec)

	memset(&values, 0, sizeof(values));
	ret = HashFind(&hm_hdle, &key_0, &values);
	if (0 > ret) {
	printf("HashFind err,key:%s!\n", key_0);    
	}
	else printf("f(%s) = [%d, %f]\n", key_0, values.ages, values.height);




	memset(&values, 0, sizeof(values));
	ret = HashFind(&hm_hdle, &key_1, &values);
	if (0 > ret) {
	printf("HashFind err,key:%s!\n", key_1);    
	}
	else printf("f(%s) = [%d, %f]\n", key_1, values.ages, values.height);




	memset(&values, 0, sizeof(values));
	ret = HashFind(&hm_hdle, &key_2, &values);
	if (0 > ret) {
	printf("HashFind err,key:%s!\n", key_2);    
	}
	else printf("f(%s) = [%d, %f]\n", key_2, values.ages, values.height);


	/**************修改**************/ 
	memset(key, 0, STR_LEN);
	strcpy((char*)key, "LiMing");

	values.ages = 14;
	values.height = 1.60;
	if(0 > HashInsert(&hm_hdle, &key, &values)){
	 printf("modify err, value:[%d, %f]!\n", values.ages, values.height);  
	}


	memset(&values, 0, sizeof(values));
	ret = HashFind(&hm_hdle, &key, &values);
	if (0 > ret) {
	printf("HashFind err,key:%s!\n", key);    
	}
	else printf("after modify,f(%s) = [%d, %f]\n", key, values.ages, values.height);

	/**************删除**************/ 
	memset(key, 0, STR_LEN);
	strcpy((char*)key, "LiMing");
	HashRemove(&hm_hdle, &key);

	printf("remove key:%s\n", key);
	memset(&values, 0, sizeof(values));
	ret = HashFind(&hm_hdle, &key, &values);
	if (0 > ret) {
	printf("HashFind err,key:%s!\n", key);    
	}
	else printf("f(%s) = [%d, %f]\n", key, values.ages, values.height);


	/**************销毁**************/ 
	HashDestroy(&hm_hdle);

}


