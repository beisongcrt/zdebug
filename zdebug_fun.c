/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-08-04     zhb      the first version
 */
#include "zdebug.h"

#ifdef ZDEBUG_FUN

zdbeug_fun_t fun_head;

/***********************测试操作函数******************************/

unsigned int fun_set_zdebug(unsigned int debug_status)
{
	zDebug_Set(debug_status);
	return debug_status;
}

/*********************测试操作函数****************************/


/**************************************************************************
** 函数名称: fun_Return
** 功能描述: 函数返回值打印
**
** 输    入: fun_data 函数数据指针，Return_Data 返回值指针
** 输    出:
** 返    回:
**
** 作    者:
** 日    期:
**------------------------------------------------------------------------
**************************************************************************/
void fun_Return(zdbeug_fun_t *fun_data, unsigned int *return_data)
{
	rt_kprintf("fun return type %s value =", fun_data->Type);
	var_Show_Value(fun_data->Type, return_data);
}

/**************************************************************************
** 函数名称: fun_Run
** 功能描述: 函数运行
**
** 输    入: fun_data 函数数据指针，Param_Buf 参数数组指针
** 输    出:
** 返    回:
**
** 作    者:
** 日    期:
**------------------------------------------------------------------------
**************************************************************************/
void fun_Run(zdbeug_fun_t *fun_data, unsigned int *Param_Buf)
{
	unsigned int res = 0;

	switch(fun_data->Param_num)
  	{
		case 0://无参数(void类型)
			res=(*(unsigned int(*)())fun_data->pfun)();
			break;
	    case 1://有1个参数
			res=(*(unsigned int(*)())fun_data->pfun)(Param_Buf[0]);
			break;
	    case 2://有2个参数
			res=(*(unsigned int(*)())fun_data->pfun)(Param_Buf[0],Param_Buf[1]);
			break;
	    case 3://有3个参数
			res=(*(unsigned int(*)())fun_data->pfun)(Param_Buf[0],Param_Buf[1],Param_Buf[2]);
			break;
	    case 4://有4个参数
			res=(*(unsigned int(*)())fun_data->pfun)(Param_Buf[0],Param_Buf[1],Param_Buf[2],Param_Buf[3]);
			break;
	    case 5://有5个参数
			res=(*(unsigned int(*)())fun_data->pfun)(Param_Buf[0],Param_Buf[1],Param_Buf[2],Param_Buf[3],Param_Buf[4]);
			break;
	    case 6://有6个参数
			res=(*(unsigned int(*)())fun_data->pfun)(Param_Buf[0],Param_Buf[1],Param_Buf[2],Param_Buf[3],Param_Buf[4],\
			Param_Buf[5]);
			break;
	    case 7://有7个参数
			res=(*(unsigned int(*)())fun_data->pfun)(Param_Buf[0],Param_Buf[1],Param_Buf[2],Param_Buf[3],Param_Buf[4],\
			Param_Buf[5],Param_Buf[6]);
			break;
	    case 8://有8个参数
			res=(*(unsigned int(*)())fun_data->pfun)(Param_Buf[0],Param_Buf[1],Param_Buf[2],Param_Buf[3],Param_Buf[4],\
			Param_Buf[5],Param_Buf[6],Param_Buf[7]);
			break;
	    case 9://有9个参数
			res=(*(unsigned int(*)())fun_data->pfun)(Param_Buf[0],Param_Buf[1],Param_Buf[2],Param_Buf[3],Param_Buf[4],\
			Param_Buf[5],Param_Buf[6],Param_Buf[7],Param_Buf[8]);
			break;
	    case 10://有10个参数
			res=(*(unsigned int(*)())fun_data->pfun)(Param_Buf[0],Param_Buf[1],Param_Buf[2],Param_Buf[3],Param_Buf[4],\
			Param_Buf[5],Param_Buf[6],Param_Buf[7],Param_Buf[8],Param_Buf[9]);
			break;
	}

	fun_Return(fun_data, &res);
		
}



/**************************************************************************
** 函数名称: fun_Get_Param
** 功能描述: 获取函数参数
**
** 输    入: argv 参数数组列表指针，Param_Num 参数数目，
** 输    出: Param_Buf 参数数组指针
** 返    回: 0 = 错误，1 = 获取完成
**
** 作    者:
** 日    期:
**------------------------------------------------------------------------
**************************************************************************/
char fun_Get_Param(char **argv, char Param_Num, unsigned int *Param_Buf)
{
	unsigned int i;
	char *Data;
	unsigned int Value = 0;

	if(PARAM_NUM_MAX < Param_Num)
	{
	  	Param_Num = PARAM_NUM_MAX;
	}
	
	for(i = 0; Param_Num > i; i++)
	{
	  	Data = argv[i + 2];
	  	if('"' == *(Data - 1))   // 字符串参数
	    {
	        Param_Buf[i] = (unsigned int)&Data[0];
	  		Char_Escape((char *)Param_Buf[i], (char *)Param_Buf[i]);
	    }
	 	else if(('0' == Data[0]) && (('x' == Data[1]) || ('X' == Data[1])))
      	{
        	if(HexToUint32(&Data[2], rt_strlen(Data) - 2, &Value))  // 16 进制数据
        	{
          	Param_Buf[i] = Value;
        	}
  			else
  			{
  		  		return 0;
  			}
      	}
	  	else
	  	{
			if(DecToUint32(&Data[0], rt_strlen(Data), &Value))  // 10 进制数据
        	{
          		Param_Buf[i] = Value;
        	}
			else
			{
		  		return 0;
			}
	  	}

	}

	return 1;
}



/**************************************************************************
** 函数名称: fun_get_fun_from_name
** 功能描述: 通过函数名获取函数数据
**
** 输     入: name 函数名称字符串指针，
** 输     出: num 函数列表序号
** 返     回: 函数数据指针
**
** 作 	 者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
zdbeug_fun_t * fun_get_fun_from_name(const char *name, unsigned int *num)
{
	unsigned int i = 0;
	zdbeug_fun_t *fun_data;

	rt_slist_t *slist = rt_slist_first(&fun_head.slist);
	
	do
	{
		fun_data = rt_container_of(slist, zdbeug_fun_t, slist);
		if(!rt_strcmp(fun_data->Name, name))     
		{
			*num = i;
	  		return fun_data;
		}
		i++;
		slist = rt_slist_next(slist);
		
	}while(slist);

	return RT_NULL;

}


/**************************************************************************
** 函数名称: fun_get_fun_from_num
** 功能描述: 通过函数序号获取函数数据
**
** 输     入: num 函数序号
** 输     出:
** 返     回: 函数数据指针
**
** 作 	 者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
zdbeug_fun_t * fun_get_fun_from_num(unsigned int num)
{
	unsigned int i = 0;
	zdbeug_fun_t *fun_data;

	rt_slist_t *slist = rt_slist_first(&fun_head.slist);

	do
	{
		if(num == i)
		{
			fun_data = rt_container_of(slist, zdbeug_fun_t, slist);
	  		return fun_data;
		}
		i++;
		slist = rt_slist_next(slist);
		
	}while(slist);

	return RT_NULL;

}


/**************************************************************************
** 函数名称: fun_add_node
** 功能描述: 添加函数数据列表节点
**
** 输     入: Type 类型名称，Name 函数名称指针，Param 函数参数名称指针，
			 pfun 函数指针，Param_num 参数数目
** 输     出:
** 返     回:
**
** 作 	 者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
rt_slist_t * fun_add_node(char *Type, char *Name, void *Param, void *pfun, unsigned char Param_num)
{
  	zdbeug_fun_t *node;

  	node = rt_malloc(sizeof(zdbeug_fun_t));

	if(RT_NULL == node)
	{
	  	return RT_NULL;
	}

	node->Type  = Type;
	node->Name  = Name;
	node->Param = Param;
	node->pfun  = pfun;
	node->Param_num = Param_num;
	rt_slist_init(&node->slist);

	return &node->slist;
}


/**************************************************************************
** 函数名称: fun_add_fun_list
** 功能描述: 添加函数到函数列表
**
** 输     入: Type 类型序号，Name 变量名称指针，Param 函数参数名称指针，pfun 函数指针
** 输     出:
** 返     回:
**
** 作 	 者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
void fun_add_fun_list(unsigned char Type, char *Name, char *Param, void *pfun)
{
	unsigned char Param_num = 0;

	if(rt_strcmp(Param, "(void)"))
	{
	  	Param_num = get_str_count(Param, ',') + 1;
	}
		
	rt_slist_t *slist = rt_slist_tail(&fun_head.slist);
	
  	slist->next = fun_add_node(var_type_str[Type], Name, Param, pfun, Param_num);
}


/**************************************************************************
** 函数名称: fun_exe_n
** 功能描述: 函数指定执行
**
** 输    入: argc 参数数目，argv 参数数组列表指针, fun_data 函数数据指针
** 输    出:
** 返    回:
**
** 作    者:
** 日    期:
**------------------------------------------------------------------------
**************************************************************************/
void fun_exe_n(int argc, char **argv, zdbeug_fun_t *fun_data)
{
  	unsigned int Param_Buf[PARAM_NUM_MAX] = {0};
	
  	if((PARAM_NUM_MAX >= fun_data->Param_num) && (fun_data->Param_num <= (argc - 2)))
  	{
    	if(fun_Get_Param(argv, fun_data->Param_num, Param_Buf))
    	{
      		fun_Run(fun_data, Param_Buf);
    	}
  		else
  		{
  	  		rt_kprintf("--- fun param error ---\n");
  		}
  	}
	else
	{
	  	rt_kprintf("--- fun param num error ---\n");
	}
}

/**************************************************************************
** 函数名称: fun_exe
** 功能描述: 函数执行
**
** 输    入: argc 参数数目，argv 参数数组列表指针
** 输    出:
** 返    回:
**
** 作    者:
** 日    期:
**------------------------------------------------------------------------
**************************************************************************/
void fun_exe(int argc, char **argv)
{
	unsigned int i;
	zdbeug_fun_t *fun_data;

	if(Get_Num(argv[1], &i, 'f'))	// 通过变量编号设置
	{
		fun_data = fun_get_fun_from_num(i);
	
		if(fun_data)
		{
			fun_exe_n(argc, argv, fun_data);
			return;
		}
	  
	}

	fun_data = fun_get_fun_from_name(argv[1], &i);

	if(fun_data)
	{
		fun_exe_n(argc, argv, fun_data);
	  	return;
	}

	rt_kprintf("--- Unfound fun %s ---\n", argv[1]);

	
}


/**************************************************************************
** 函数名称: fun_Show_Name
** 功能描述: 显示函数名称信息
**
** 输    入: fun_data 函数数据指针
** 输    出:
** 返    回:
**
** 作    者:
** 日    期:
**------------------------------------------------------------------------
**************************************************************************/
void fun_Show_Name(zdbeug_fun_t *fun_data)
{
  	rt_kprintf("ADDR = 0x%08X  %s %s%s\n", fun_data->pfun, fun_data->Type, fun_data->Name, fun_data->Param);

}

/**************************************************************************
** 函数名称: fun_List
** 功能描述: 函数列表显示
**
** 输    入:
** 输    出:
** 返    回:
**
** 作    者:
** 日    期:
**------------------------------------------------------------------------
**************************************************************************/
void fun_List(void)
{
	unsigned int i;
	zdbeug_fun_t *fun_data;

	rt_slist_t *slist = rt_slist_first(&fun_head.slist);

	rt_kprintf("***** fun list begin *****\n");

	i = 0;
	do
	{
		fun_data = rt_container_of(slist, zdbeug_fun_t, slist);
		
		rt_kprintf("f%03d: ", i);
		fun_Show_Name(fun_data);
		
		slist = rt_slist_next(slist);
		i++;
		
	}while(slist);

	rt_kprintf("****** fun list end ******\n");
}



/**************************************************************************
** 函数名称: fun_init
** 功能描述: 函数列表初始化
**
** 输     入:
** 输     出:
** 返     回:
**
** 作 	 者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
int fun_init(void)
{
  	fun_head.Type  = RT_NULL;
	fun_head.Name  = RT_NULL;
	fun_head.Param = RT_NULL;
	fun_head.pfun  = RT_NULL;
	fun_head.Param_num = RT_NULL;
	rt_slist_init(&fun_head.slist);

	zdebug_add_fun(VAT_TYPE_U32, fun_set_zdebug, (unsigned int debug_status));
	
	return RT_EOK;
}
INIT_APP_EXPORT(fun_init);


/**************************************************************************
** 函数名称: fun
** 功能描述: 函数操作命令
**
** 输    入: argc 参数数目，argv 参数数组列表指针
** 输    出:
** 返    回:
**
** 作    者:
** 日    期:
**------------------------------------------------------------------------
**************************************************************************/
void fun(int argc, char**argv)
{	
  	if (1 == argc)
  	{
      	fun_List();
  	}
  	else if (2 <= argc)
  	{
	 	fun_exe(argc, argv);
  	}

}

MSH_CMD_EXPORT(fun, input 'fun <name(fxxx)> <param ...>');



#endif /* ZDEBUG_FUN */


