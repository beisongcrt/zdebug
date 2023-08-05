/*
 * 
 */

#ifndef __ZDEBUG_FUN_H_
#define __ZDEBUG_FUN_H_

#ifdef ZDEBUG_FUN

#define PARAM_NUM_MAX       10

struct zdebug_fun_node
{
  	char *Type;          // 函数返回类型
  	char *Name;          // 函数名称
  	char *Param;         // 函数参数
  
	void *pfun;          // 函数指针
	unsigned char  Param_num;     // 函数参数数目
	rt_slist_t slist;
	
};
typedef struct zdebug_fun_node zdbeug_fun_t;

#endif /* ZDEBUG_FUN */

#endif /* __ZDEBUG_FUN_H_ */


