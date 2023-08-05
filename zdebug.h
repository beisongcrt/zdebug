/*
 * 
 */



#ifndef _ZDEBUG_H_
#define _ZDEBUG_H_

#include <rtthread.h>

//
///* 使用 zdebug，如果在rtcongfig.h中配置，此处就不需要配置 */
//#define PKG_USING_ZDEBUG

#ifdef PKG_USING_ZDEBUG
//
///* 根据标志位，打印 log，如果在rtcongfig.h中配置，此处就不需要配置 */
//#define ZDEBUG_LOG
//
///* var 变量调试控制，如果在rtcongfig.h中配置，此处就不需要配置 */
//#define ZDEBUG_VAR
//
///* fun 函数执行，如果在rtcongfig.h中配置，此处就不需要配置 */
//#define ZDEBUG_FUN


#ifdef  ZDEBUG_VAR
#include "zdebug_var.h"

#define zdebug_add_var(type, var)  var_add_var_list(type, #var, &var)

#else

#define zdebug_add_var(type, var)

#endif


#ifdef  ZDEBUG_FUN
#include "zdebug_fun.h"

#define zdebug_add_fun(type, fun, param)  fun_add_fun_list(type, #fun, #param, fun)  

#else

#define zdebug_add_fun(type, fun, param)

#endif


#ifdef  ZDEBUG_LOG

#define zdebug_log(type, message)                         \
do                                                        \
{                                                         \
    if ((zDEBUG_ERR == type) || (zdebug & type))          \
        rt_kprintf message;                               \
}                                                         \
while (0)

#define zDebug_Set(Debus_Status)                          \
do                                                        \
{                                                         \
    zdebug = Debus_Status;                                \
}                                                         \
while (0)

extern unsigned int  zdebug;     // 对应位调试信息输出,


#define zDEBUG_BIT_NO                 0x00000000

#define zDEBUG_BIT_00                 0x00000001
#define zDEBUG_BIT_01                 0x00000002
#define zDEBUG_BIT_02                 0x00000004
#define zDEBUG_BIT_03                 0x00000008
#define zDEBUG_BIT_04                 0x00000010
#define zDEBUG_BIT_05                 0x00000020
#define zDEBUG_BIT_06                 0x00000040
#define zDEBUG_BIT_07                 0x00000080
#define zDEBUG_BIT_08                 0x00000100
#define zDEBUG_BIT_09                 0x00000200
#define zDEBUG_BIT_10                 0x00000400
#define zDEBUG_BIT_11                 0x00000800
#define zDEBUG_BIT_12                 0x00001000
#define zDEBUG_BIT_13                 0x00002000
#define zDEBUG_BIT_14                 0x00004000
#define zDEBUG_BIT_15                 0x00008000
#define zDEBUG_BIT_16                 0x00010000
#define zDEBUG_BIT_17                 0x00020000
#define zDEBUG_BIT_18                 0x00040000
#define zDEBUG_BIT_19                 0x00080000
#define zDEBUG_BIT_20                 0x00100000
#define zDEBUG_BIT_21                 0x00200000
#define zDEBUG_BIT_22                 0x00400000
#define zDEBUG_BIT_23                 0x00800000
#define zDEBUG_BIT_24                 0x01000000
#define zDEBUG_BIT_25                 0x02000000
#define zDEBUG_BIT_26                 0x04000000
#define zDEBUG_BIT_27                 0x08000000
#define zDEBUG_BIT_28                 0x10000000
#define zDEBUG_BIT_29                 0x20000000
#define zDEBUG_BIT_30                 0x40000000

#define zDEBUG_ERR                    0x80000000

#else  /* ZDEBUG_LOG */

#define zdebug_log(type, message)
#define zDebug_Set(Debus_Status)

#endif /* ZDEBUG_LOG */

#else /* PKG_USING_ZDEBUG */

#define zdebug_log(type, message) 
#define zDebug_Set(Debus_Status)

#define zdebug_add_var(type, var) 
#define zdebug_add_fun(type, fun, param)


#endif   /* PKG_USING_ZDEBUG */


#endif /* _ZDEBUG_H_ */


