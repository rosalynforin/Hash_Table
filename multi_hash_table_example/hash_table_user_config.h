
/***********************example_number示例**********************/
//用户1配置
//配置一个关键字是int型, 键值是float型，且有两个散列映射函数的散列数据结构。
#if defined(HASH_USER_ONE)

#include <stdlib.h>

#define KEY_MIN 0

//用户1自定义映射函数1(采用直接寻址法，需注意关键字的取值范围)
static int HashMapFunc_1(void* key, size_t* key_index, size_t table_len)
{
    int* tmp = (int*)key;

    if(*tmp < KEY_MIN) { //当超出范围时，返回-1
        return -1;
    }
    else {
        *key_index = (size_t)(*tmp % table_len);
    }
    
    return 0;
}



//用户1自定义映射函数2(求出一个大数，需注意关键字的取值范围)
static int HashMapFunc_2(void* key, size_t* key_index, size_t table_len)
{
    int* tmp = (int*)key;

    if(*tmp < KEY_MIN) { //当超出范围时，返回-1
        return -1;
    }
    else {
        
        *key_index = (size_t)((*tmp ^ 13131) % table_len);
    }   
    
    return 0;
}



//关键字匹配函数
static int keys_each_math(void* src_key, void* dst_key)
{
    int* srckey = (int*)src_key;
    int* dstkey = (int*)dst_key;

    return (int)(*srckey == *dstkey? 1 : 0);
}



//索引关键字与散列映射函数匹配（存在两个散列映射函数，需要与关键字进行匹配）
static hashmap_func key_func_match(void* key, const hashmap_func* hsfunc)
{
    int* rank = (int*)key;
    if(*rank > 10000) {
        return hsfunc[0];
    } 
    else {
        return hsfunc[1];
    }
}



//内存申请函数
static void* my_malloc(size_t size)
{
    char* ptr = NULL;
    ptr = (char*)malloc(size);
    if(NULL == ptr) return NULL;
    
    return (void*)ptr;
}




//内存释放函数
static void my_free(void* ptr)
{
    if(NULL != ptr) {
        free(ptr);
    }
}





//对应配置注册
REGISTER_USER_TABLE_SIZE( 10000 )                       //默认HASHMAP存储空间大小
REGISTER_USER_CONFLICT_SIZE( 1 )                        //散列表存储数组里单节点中最大冲突键值个数阈值
REGISTER_USER_KEY_TYPE( int )                           //用户键类型
REGISTER_USER_VALUE_TYPE( float )                       //用户值类型

REGISTER_USER_HASH_MAP( HashMapFunc_1, HashMapFunc_2)   //散列映射




#define USER_KEY_MATCH_FUNC     keys_each_math          //键与键匹配

#define USER_KEY_HASHMAP_FUNC   key_func_match          //键与散列映射匹配
         

#define USER_MEM_ALLOC_FUNC     my_malloc               //内存申请

#define USER_MEM_FREE_FUNC      my_free                 //内存释放
               




/******************************example_string示例******************************/

//用户2配置
//配置一个关键字是字符数组, 键值是自定义的info_t结构体，且只有一个散列映射函数的散列表。
#elif defined(HASH_USER_TWO)

#include <string.h>
#include <stdlib.h>

//用户函数实现


//用户关键字类型
#define STR_LEN 16U
typedef char name[STR_LEN]; //16字节大小的数组


//用户值类型
typedef struct info{

    unsigned char ages;
    float height;

}info_t;





//关键字匹配函数
static int keys_each_math(void* src_key, void* dst_key)
{
    name* sname = (name*)src_key;
    name* dname = (name*)dst_key;

    return (int)(!strcmp((char*)(*sname), (char*)(*dname))? 1 : 0);
}



//索引关键字与散列映射函数匹配
static hashmap_func key_func_match(void* key, const hashmap_func* hsfunc)
{
    (void)key; 
    return (*hsfunc); //当前只定义了一个散列映射函数
}






//用户2自定义映射函数
/*
采用BKDRHash算法
*/
static int BKDRHash(void* key, size_t* key_index, size_t table_len)
{
    char* tmpstr = (char*)key;

    long long seed = 131; // 31 131 1313 13131 131313 etc..
    long long hash = 0;
    unsigned char len_limit = STR_LEN;

    while (*tmpstr && len_limit)
    {
        hash = hash * seed + (*tmpstr++);
        --len_limit;
    }

    *key_index = hash % table_len;
    return 0;
}




//内存申请函数
static void* my_malloc(size_t size)
{
    char* ptr = NULL;
    ptr = (char*)malloc(size);
    if(NULL == ptr) return NULL;
    
    return (void*)ptr;
}




//内存释放函数
static void my_free(void* ptr)
{
    if(NULL != ptr) {
        free(ptr);
    }
}




//对应配置注册
REGISTER_USER_TABLE_SIZE( 100 )                         //HASHMAP存储空间大小
REGISTER_USER_CONFLICT_SIZE( 2 )                        //散列表存储数组里单节点中最大冲突键值个数阈值
REGISTER_USER_KEY_TYPE( name )                          //用户键类型
REGISTER_USER_VALUE_TYPE( info_t )                      //用户值类型
        
REGISTER_USER_HASH_MAP( BKDRHash )                      //散列映射


#define USER_KEY_MATCH_FUNC     keys_each_math          //键与键匹配

#define USER_KEY_HASHMAP_FUNC   key_func_match          //键与散列映射匹配
         
#define USER_MEM_ALLOC_FUNC     my_malloc               //内存申请

#define USER_MEM_FREE_FUNC      my_free                 //内存释放

/******************************测试示例******************************/

// #elif defined(HASH_USER_TRD)

//用户函数实现
// USER_FUNC_REALIZE(


    // #include <stdlib.h>

    // #define KEY_MIN -5
    // #define KEY_MAX 100
    // #define HASH_DATA_RANGE 106

    // //散列表的存储空间大小
    // #define HASH_MAP_MAX_SIZE (HASH_DATA_RANGE*10) //较为合适的范围


    // //用户关键字类型
    // typedef struct keyval{
    //     signed char src_keys;
    //     signed char dst_keys;
    // }keyval_t;
    // #define HASH_USER_KEY_TYPE keyval_t


    // //用户值类型
    // #define HASH_USER_VALUE_TYPE float




    // //关键字匹配函数
    // static int keys_each_math(void* src_key, void* dst_key)
    // {
    //     keyval_t* srckey = (keyval_t*)src_key;
    //     keyval_t* dstkey = (keyval_t*)dst_key;

    //     return (int)(srckey->src_keys == dstkey->src_keys? (srckey->dst_keys == dstkey->dst_keys? 1 : 0) : 0);
    // }




    // //索引关键字与散列映射函数匹配
    // static hashmap_func key_func_match(void* key, hashmap_func* hsfunc)
    // {
    //     keyval_t* tkey = (keyval_t*)key;

    //     if( (0 < tkey->src_keys) && (0 < tkey->dst_keys) ){           
    //         return hsfunc[0];                               
    //     }                                                               
    //     else if( (0 >= tkey->src_keys) && (0 >= tkey->dst_keys) ){    
    //         return hsfunc[1];                              
    //     }                                                               
    //     else{                                                           
    //         return hsfunc[2];                               
    //     }  
    // }




    // //散列映射函数
    // /*
    // 映射函数中三元一次方程系数
    // (当系数在一定范围内相差越大，取值范围【相同】的x,y在二维平面坐标组成的面积内，计算数值越平均，
    // 反过来，当系数相差越小，越符合高斯分布，均值处于x,y取值范围中点。
    // 相当与许多平行线以arc tan(A_COEFF/B_COEFF)反正切得到的角度切割一个正方形。)
    // */

    // #define  A_COEFF   2
    // #define  B_COEFF   19

    // //映射函数1(src_keys>0, dst_keys>0)
    // //方程： z = A_COEFF*x + B_COEFF*y + b
    // //return <0:fail >=0:success
    // //最大值出现在2098这个数值
    // static int HashMapFunc_one(void* key, size_t* key_index, size_t table_len)
    // {
    //     (void)table_len;
    //     keyval_t* tmpkey = (keyval_t*)key;

    //     if((KEY_MAX < tmpkey->src_keys) || (KEY_MAX < tmpkey->dst_keys)){
    //         return -1;
    //     }
    //     else if(tmpkey->src_keys == tmpkey->dst_keys){
    //         return -1;
    //     }

    //     //利用简单的三元一次函数作为散列计算
    //     *key_index = (A_COEFF*tmpkey->src_keys + B_COEFF*tmpkey->dst_keys)%(HASH_MAP_MAX_SIZE-36) + 36; //36为映射函数2结果范围中最大值35之后开始
    //     return 0;
    // }

    // #undef   A_COEFF
    // #undef   B_COEFF




    // #define  A_COEFF   3
    // #define  B_COEFF   23

    // //映射函数3(src_keys>0, dst_keys<=0 || src_keys<=0, dst_keys>0)
    // static int HashMapFunc_trd(void* key, size_t* key_index, size_t table_len)
    // {
    //     (void)table_len;
    //     int x = 0, y = 0;

    //     keyval_t* tmpkey = (keyval_t*)key;

    //     if((KEY_MAX < tmpkey->src_keys) || 
    //        (KEY_MAX < tmpkey->dst_keys) ||
    //        (KEY_MIN > tmpkey->src_keys) ||
    //        (KEY_MIN > tmpkey->dst_keys)){
    //         return -1;
    //     }
    //     else if(tmpkey->src_keys == tmpkey->dst_keys){
    //         return -1;
    //     }

    //     x = (0 > tmpkey->src_keys)? -1*tmpkey->src_keys : tmpkey->src_keys;
    //     y = (0 > tmpkey->dst_keys)? -1*tmpkey->dst_keys : tmpkey->dst_keys;
    //     //利用简单的三元一次函数作为散列计算
    //     *key_index = (A_COEFF*x  + B_COEFF*y)%(HASH_MAP_MAX_SIZE-36) + 36; //36为映射函数2结果范围中最大值35之后开始
    //     return 0;
    // }

    // #undef A_COEFF
    // #undef B_COEFF





    // //映射函数2(src_keys<=0, dst_keys<=0)
    // static int HashMapFunc_two(void* key, size_t* key_index, size_t table_len)
    // {
    //     (void)table_len;
    //     keyval_t* tmpkey = (keyval_t*)key;

    //     if((KEY_MIN > tmpkey->src_keys) || (KEY_MIN > tmpkey->dst_keys)){
    //         return -1;
    //     }
    //     else if(tmpkey->src_keys == tmpkey->dst_keys){
    //         return -1;
    //     }

    //     //映射表
    //     static int matux[6][6] =
    //     {
    //         {0 , 1 , 2 , 3 , 4 , 5 }, 
    //         {6 , 7 , 8 , 9 , 10, 11}, 
    //         {12, 13, 14, 15, 16, 17}, 
    //         {18, 19, 20, 21, 22, 23}, 
    //         {24, 25, 26, 27, 28, 29}, 
    //         {30, 31, 32, 33, 34, 35}
    //     };

    //     int i = 0 , j = 0;

    //     i = -1*(tmpkey->src_keys);
    //     j = -1*(tmpkey->dst_keys);

    //     *key_index = matux[i][j];

    //     return 0;
    // }





    // //内存申请函数
    // static void* my_malloc(size_t size)
    // {
    //     char* ptr = NULL;
    //     ptr = (char*)malloc(size);
    //     if(NULL == ptr) return NULL;
        
    //     return (void*)ptr;
    // }




    // //内存释放函数
    // static void my_free(void* ptr)
    // {
    //     if(NULL != ptr) {
    //         free(ptr);
    //     }
    // }

// )

// //对应配置注册
// REGISTER_USER_TABLE_SIZE( HASH_DATA_RANGE*10 )                              //HASHMAP存储空间大小
// REGISTER_USER_CONFLICT_SIZE( 5 )                                            //散列表存储数组里单节点中最大冲突键值个数阈值
// REGISTER_USER_KEY_TYPE( keyval_t )                                          //用户键类型
// REGISTER_USER_VALUE_TYPE( float )                                           //用户值类型

// REGISTER_USER_HASH_MAP( HashMapFunc_one, HashMapFunc_two, HashMapFunc_trd)  //散列映射

// REGISTER_USER_BOTH_KEY_MATCH( keys_each_math )                              //键与键匹配
// REGISTER_USER_KEY_FUNC_MATCH( key_func_match )                              //键与散列映射匹配
// #define USER_MEM_FUNC
// REGISTER_USER_MEM_ALLOC( my_malloc )                                        //内存申请
// REGISTER_USER_MEM_FREE( my_free )                                           //内存释放






#endif


