/*
 * 
 */

#include "zdebug.h"

#ifdef ZDEBUG_VAR

/***********************操作变量******************************/

unsigned int  zdebug = 0;     // 对应位调试信息输出,
unsigned char hex = 0;        // 0 = 十进制输出，1 = 十六进制输出


/*********************操作变量结束****************************/

char * var_type_str[VAT_TYPE_NUM] = 
{
	"void",
	"u8",
	"u16",
	"u32",

	"p8",
	"p16",
	"p32",
	"stu"                     
};

zdbeug_var_t var_head;


/**************************************************************************
** 函数名称: get_str_count
** 功能描述: 获取当前字符串中出现字符 ch 的次数
**
** 输    入: str 查询的字符串，ch 查询字符
** 输    出:
** 返    回:
**
** 作    者: zhb
** 日    期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
int get_str_count(char * str, char ch)
{
	int count = 0;
	
	for (int i = 0; str[i] != '\0'; i++) 
	{
        if (str[i] == ch) 
		{
            count++;
        }
    }

	return count;
	 
}


/**************************************************************************
** 函数名称: Char_Escape
** 功能描述: 字符转义函数
**
** 输     入: scr_str 源字符串
** 输     出: des_str 待转化字符串
** 返     回:
**
** 作     者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
void Char_Escape(char *des_str, char *scr_str)
{
	unsigned int i = 0;
	unsigned int n = 0;

	while(scr_str[i])
	{
	  	if('\\' == scr_str[i])
	  	{
	    	i++;
			switch(scr_str[i])
			{
			  	case 'r':
					des_str[n] = '\r';
					break;

				case 'n':
					des_str[n] = '\n';
					break;

				case '\\':
					des_str[n] = '\\';
					break;

				case '\'':
					des_str[n] = '\'';
					break;

				case '\"':
					des_str[n] = '\"';
					break;

				default:
					des_str[n] = ' ';
					break;
			}
			i++;
			n++;
	  	}

		des_str[n] = scr_str[i];
		i++;
		n++;
		
	}

	des_str[n] = 0;

}



/**************************************************************************
** 函数名称: DecToUint32
** 功能描述: 将十进制字符串转化为整数
**
** 输     入: str,待转化字符串; len,待转化串长度
** 输     出: Data 转换变量指针
** 返     回: 0 = 错误，1 = 转换完成
**
** 作     者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
unsigned char DecToUint32(char *str, unsigned int len, unsigned int *Data)
{
	unsigned int i;

	for (i = 0; i < len; i++)
	{
		if ((str[i] < 0x30) || (str[i] > 0x39)) //检查是否为0--9
		{
			return 0;
		}
		*Data = (*Data) * 10 + str[i] - '0';
	}

	return 1;
}

/**************************************************************************
** 函数名称: HexToUint32
** 功能描述: 将十六进制字符串转化为整数
**
** 输     入: str,待转化字符串; len,待转化串长度
** 输     出: Data 转换变量指针
** 返     回: 0 = 错误，1 = 转换完成
**
** 作     者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
unsigned char HexToUint32(char *str, unsigned int len, unsigned int *Data)
{
	unsigned int  i;

	if(8 < len)
	{
	  	len = 8;
	}
	
	if(0 == len)
	{
	  	return 0;
	}

	for(i = 0; i < len; i++)
	{
		if ((str[i] >= '0') && (str[i] <= '9'))
		{
			*Data = (*Data) * 16 + str[i] - '0';
		}
		else if ((str[i] >= 'A') && (str[i] <= 'F')) /*A....F*/
		{
			*Data = (*Data) * 16 + str[i] - 'A' + 10;
		}
		else if ((str[i] >= 'a') && (str[i] <= 'f')) /*a....f */
		{
			*Data = (*Data) * 16 + str[i] - 'a' + 10;
		}
		else
		{
			return 0;
		}
		
	}

	return 1;
}


/**************************************************************************
** 函数名称: Get_Num
** 功能描述: 获取序列号
**
** 输     入: Data 转换数据字符串指针，c 标志字符
** 输     出: Num 转换数据变量指针
** 返     回: 0 = 错误，1 = 获取完成
**
** 作     者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
unsigned char Get_Num(char *Data, unsigned int *Num, char c)
{
  	unsigned int Num_Temp = 0;
	
  	if((c == Data[0]) && (4 >= rt_strlen(Data)))
  	{
    	if(DecToUint32(&Data[1], rt_strlen(Data) - 1, &Num_Temp))
    	{
      		*Num = Num_Temp;
			return 1;
    	}
  	}

	return 0;
}


/**************************************************************************
** 函数名称: var_get_var_from_name
** 功能描述: 通过变量名获取变量数据
**
** 输     入: name 变量名称字符串指针
** 输     出: num 变量列表序号
** 返     回: 变量数据指针
**
** 作 	 者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
zdbeug_var_t * var_get_var_from_name(const char *name, unsigned int *num)
{
	unsigned int i = 0;
	zdbeug_var_t *var_data;

	rt_slist_t *slist = rt_slist_first(&var_head.slist);
	
	do
	{
		var_data = rt_container_of(slist, zdbeug_var_t, slist);
		if(!rt_strcmp(var_data->Name, name))     
		{
			*num = i;
	  		return var_data;
		}
		i++;
		slist = rt_slist_next(slist);
		
	}while(slist);

	return RT_NULL;

}


/**************************************************************************
** 函数名称: var_get_var_from_num
** 功能描述: 通过变量序号获取变量数据
**
** 输     入: num 变量序号
** 输     出:
** 返     回: 变量数据指针
**
** 作 	 者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
zdbeug_var_t * var_get_var_from_num(unsigned int num)
{
	unsigned int i = 0;
	zdbeug_var_t *var_data;

	rt_slist_t *slist = rt_slist_first(&var_head.slist);

	do
	{
		if(num == i)
		{
			var_data = rt_container_of(slist, zdbeug_var_t, slist);
	  		return var_data;
		}
		i++;
		slist = rt_slist_next(slist);
		
	}while(slist);

	return RT_NULL;

}


/**************************************************************************
** 函数名称: var_get_type_num
** 功能描述: 获取变量类型序号
**
** 输     入: Type 变量类型字符串指针
** 输     出:
** 返     回: 变量类型序号，255 = 无此类型
**
** 作 	 者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
unsigned char var_get_type_num(const char *Type)
{
	unsigned char i;
	
	for(i = 0; VAT_TYPE_NUM > i; i++)
	{
		if(!rt_strcmp(var_type_str[i], Type))      // 找到变量名
		{
			return i;
		}
	}

	return VAT_TYPE_ERR;
}


/**************************************************************************
** 函数名称: var_Show_Value
** 功能描述: 显示变量值
**
** 输     入: Type 变量类型指针，var_Value 变量值指针
** 输     出:
** 返     回:
**
** 作 	 者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
void var_Show_Value(const char *Type, void *var_Value)
{
	unsigned char type;
	
	type = var_get_type_num(Type);

	switch (type)
	{
		case VAT_TYPE_VOID:
			rt_kprintf(" RT_NULL\n");
			break;

		case VAT_TYPE_U8:
			if(0 == hex)
	    	{
	      		rt_kprintf(" %d\n", *((unsigned char *)var_Value));
	    	}
			else
			{
			  	rt_kprintf(" 0x%02X\n", *((unsigned char *)var_Value));
			}
			break;

		case VAT_TYPE_U16:
			if(0 == hex)
	    	{
	      		rt_kprintf(" %d\n", *((unsigned short *)var_Value));
	    	}
			else
			{
			  	rt_kprintf(" 0x%04X\n", *((unsigned short *)var_Value));
			}
			break;

		case VAT_TYPE_U32:
			if(0 == hex)
	    	{
	      		rt_kprintf(" %d\n", *((unsigned int *)var_Value));
	    	}
			else
			{
			  	rt_kprintf(" 0x%08X\n", *((unsigned int *)var_Value));
			}
			break;

		case VAT_TYPE_P8:
			rt_kprintf(" 0x%08X, * = ", *((unsigned int *)var_Value));
			
			if(0 == hex)
	    	{
	      		rt_kprintf("%d\n", *(unsigned char *)(*((unsigned int *)var_Value)));
	    	}
			else
			{
			  	rt_kprintf("0x%02X\n", *(unsigned char *)(*((unsigned int *)var_Value)));
			}
			
			
			break;

		case VAT_TYPE_P16:
			rt_kprintf(" 0x%08X, * = ", *((unsigned int *)var_Value));
			if(0 == hex)
	    	{
	      		rt_kprintf("%d\n", *(unsigned short *)(*((unsigned int *)var_Value)));
	    	}
			else
			{
			  	rt_kprintf("0x%04X\n", *(unsigned short *)(*((unsigned int *)var_Value)));
			}
			break;

		case VAT_TYPE_P32:
			rt_kprintf(" 0x%08X, * = ", *((unsigned int *)var_Value));
			if(0 == hex)
	    	{
	      		rt_kprintf("%d\n", *(unsigned int *)(*((unsigned int *)var_Value)));
	    	}
			else
			{
			  	rt_kprintf("0x%08X\n", *(unsigned int *)(*((unsigned int *)var_Value)));
			}			
			break;

		case VAT_TYPE_STU:
			rt_kprintf(" invalid\n");
			break;

		default:
			rt_kprintf(" Value type invalid\n");
			break;
	}

}



/**************************************************************************
** 函数名称: var_Set_Value
** 功能描述: 设置变量值
**
** 输     入: Data 变量数据字符串指针，Type 变量类型指针，
** 输     出: var_Value 变量值指针
** 返     回:
**
** 作 	 者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
void var_Set_Value(char *Data, const char *Type, void *var_Value)
{
	unsigned char type;
	unsigned int Value = 0;
	
	type = var_get_type_num(Type);

	switch (type)
	{
		case VAT_TYPE_VOID:
			rt_kprintf(" Value void invalid\n");
			break;

		case VAT_TYPE_U8:
			if(('0' == Data[0]) && (('x' == Data[1]) || ('X' == Data[1])))
	    	{
	      		if(HexToUint32(&Data[2], rt_strlen(Data) - 2, &Value))
	      		{
	        		*((unsigned char *)var_Value) = (unsigned char)Value;
	      		}
	    	}
			else
			{
			  	if(DecToUint32(&Data[0], rt_strlen(Data), &Value))
	      		{
	        		*((unsigned char *)var_Value) = (unsigned char)Value;
	      		}
			}
			break;

		case VAT_TYPE_U16:
			if(('0' == Data[0]) && (('x' == Data[1]) || ('X' == Data[1])))
	    	{
	      		if(HexToUint32(&Data[2], rt_strlen(Data) - 2, &Value))
	      		{
	        		*((unsigned short *)var_Value) = (unsigned short)Value;
	      		}
	    	}
			else
			{
			  	if(DecToUint32(&Data[0], rt_strlen(Data), &Value))
	      		{
	        		*((unsigned short *)var_Value) = (unsigned short)Value;
	      		}
			}
			break;

		case VAT_TYPE_U32:
			if(('0' == Data[0]) && (('x' == Data[1]) || ('X' == Data[1])))
	      	{
		        if(HexToUint32(&Data[2], rt_strlen(Data) - 2, &Value))
		        {
		        	*((unsigned int *)var_Value) = Value;
		        }
	      	}
	  		else
	  		{
	  		  	if(DecToUint32(&Data[0], rt_strlen(Data), &Value))
	        	{
	          		*((unsigned int *)var_Value) = Value;
	        	}
	  		}
			break;

		case VAT_TYPE_P8:
			if('*' == Data[0])
			{
				if(('0' == Data[1]) && (('x' == Data[2]) || ('X' == Data[2])))
		    	{
		      		if(HexToUint32(&Data[3], rt_strlen(Data) - 3, &Value))
		      		{
		        		*(unsigned char *)(*((unsigned int *)var_Value)) = (unsigned char)Value;
		      		}
		    	}
				else
				{
				  	if(DecToUint32(&Data[1], rt_strlen(Data) - 1, &Value))
		      		{
		        		*(unsigned char *)(*((unsigned int *)var_Value)) = (unsigned char)Value;
		      		}
				}
			}
			else
			{
				if(('0' == Data[0]) && (('x' == Data[1]) || ('X' == Data[1])))
		    	{
		      		if(HexToUint32(&Data[2], rt_strlen(Data) - 2, &Value))
		      		{
		        		*((unsigned int *)var_Value) = Value;
		      		}
		    	}
				else
				{
				  	if(DecToUint32(&Data[0], rt_strlen(Data), &Value))
		      		{
		        		*((unsigned int *)var_Value) = Value;
		      		}
				}
			}
			break;

		case VAT_TYPE_P16:
			if('*' == Data[0])
			{
				if(('0' == Data[1]) && (('x' == Data[2]) || ('X' == Data[2])))
		    	{
		      		if(HexToUint32(&Data[3], rt_strlen(Data) - 3, &Value))
		      		{
		        		*(unsigned short *)(*((unsigned int *)var_Value)) = (unsigned short)Value;
		      		}
		    	}
				else
				{
				  	if(DecToUint32(&Data[1], rt_strlen(Data) - 1, &Value))
		      		{
		        		*(unsigned short *)(*((unsigned int *)var_Value)) = (unsigned short)Value;
		      		}
				}
			}
			else
			{
				if(('0' == Data[0]) && (('x' == Data[1]) || ('X' == Data[1])))
		    	{
		      		if(HexToUint32(&Data[2], rt_strlen(Data) - 2, &Value))
		      		{
		        		*((unsigned int *)var_Value) = Value;
		      		}
		    	}
				else
				{
				  	if(DecToUint32(&Data[0], rt_strlen(Data), &Value))
		      		{
		        		*((unsigned int *)var_Value) = Value;
		      		}
				}
			}
			break;

		case VAT_TYPE_P32:
			if('*' == Data[0])
			{
				if(('0' == Data[1]) && (('x' == Data[2]) || ('X' == Data[2])))
		    	{
		      		if(HexToUint32(&Data[3], rt_strlen(Data) - 3, &Value))
		      		{
		        		*(unsigned int *)(*((unsigned int *)var_Value)) = (unsigned int)Value;
		      		}
		    	}
				else
				{
				  	if(DecToUint32(&Data[1], rt_strlen(Data) - 1, &Value))
		      		{
		        		*(unsigned int *)(*((unsigned int *)var_Value)) = (unsigned int)Value;
		      		}
				}
			}
			else
			{
				if(('0' == Data[0]) && (('x' == Data[1]) || ('X' == Data[1])))
		    	{
		      		if(HexToUint32(&Data[2], rt_strlen(Data) - 2, &Value))
		      		{
		        		*((unsigned int *)var_Value) = Value;
		      		}
		    	}
				else
				{
				  	if(DecToUint32(&Data[0], rt_strlen(Data), &Value))
		      		{
		        		*((unsigned int *)var_Value) = Value;
		      		}
				}
			}
			break;

		case VAT_TYPE_STU:
			rt_kprintf(" Value stu invalid\n");
			break;

		default:
			rt_kprintf(" Value type invalid\n");
			break;
	}

}



/**************************************************************************
** 函数名称: var_Get_Type
** 功能描述: 获取列表中变量类型
**
** 输     入: Type 变量类型字符串指针
** 输     出:
** 返     回: 0 = 错误，1 = 获取完成
**
** 作 	 者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
unsigned char var_Get_Type(char *Type)
{
	zdbeug_var_t *var_data;

	rt_slist_t *slist = rt_slist_first(&var_head.slist);

	do
	{
		var_data = rt_container_of(slist, zdbeug_var_t, slist);
		if(!rt_strcmp(var_data->Type, Type))      // 找到变量名
		{
			return 1;
		}
		slist = rt_slist_next(slist);
		
	}while(slist);

	return 0;
	
}


/**************************************************************************
** 函数名称: var_Show_Name
** 功能描述: 显示变量名称信息
**
** 输     入: var_data 变量数据指针
** 输     出:
** 返     回:
**
** 作 	 者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
void var_Show_Name(zdbeug_var_t *var_data)
{
  	rt_kprintf("ADDR = 0x%08X %3s %s =", var_data->Value, var_data->Type, var_data->Name);
}

/**************************************************************************
** 函数名称: var_Get_Show
** 功能描述: 变量显示
**
** 输     入: var_data 变量数据指针
** 输     出:
** 返     回:
**
** 作 	 者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
void var_Get_Show(zdbeug_var_t *var_data)
{
  	var_Show_Name(var_data);
  	var_Show_Value(var_data->Type, var_data->Value);
}



/**************************************************************************
** 函数名称: var_Set_Show
** 功能描述: 设置变量显示
**
** 输     入: var_data 变量数据指针，Data 设置变量数据字符串指针
** 输     出:
** 返     回:
**
** 作 	 者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
void var_Set_Show(zdbeug_var_t *var_data, char *Data)
{
  	var_Show_Name(var_data);
	var_Set_Value(Data, var_data->Type, var_data->Value);
  	var_Show_Value(var_data->Type, var_data->Value);
}


/**************************************************************************
** 函数名称: var_add_node
** 功能描述: 添加变量数据列表节点
**
** 输     入: Type 类型名称，Name 变量名称指针，Value 变量地址
** 输     出:
** 返     回:
**
** 作 	 者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
rt_slist_t * var_add_node(char *Type, char *Name, void *Value)
{
  	zdbeug_var_t *node;

  	node = rt_malloc(sizeof(zdbeug_var_t));

	if(RT_NULL == node)
	{
	  	return RT_NULL;
	}

	node->Type	= Type;
	node->Name	= Name;
	node->Value = Value;
	rt_slist_init(&node->slist);

	return &node->slist;
}


/**************************************************************************
** 函数名称: var_add_var_list
** 功能描述: 添加变量到变量列表
**
** 输     入: Type 类型序号，Name 变量名称指针，Value 变量地址
** 输     出:
** 返     回:
**
** 作 	 者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
void var_add_var_list(unsigned char Type, char *Name, void *Value)
{
	rt_slist_t *var_list = rt_slist_tail(&var_head.slist);
  	var_list->next = var_add_node(var_type_str[Type], Name, Value);
}

/**************************************************************************
** 函数名称: var_List
** 功能描述: 变量列表显示
**
** 输     入:
** 输     出:
** 返     回:
**
** 作 	 者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
void var_List(void)
{
  	unsigned int i;
	zdbeug_var_t *var_data;

	rt_slist_t *slist = rt_slist_first(&var_head.slist);

	rt_kprintf("***** var list begin *****\n");

	i = 0;
	do
	{
		var_data = rt_container_of(slist, zdbeug_var_t, slist);
		
		rt_kprintf("v%03d: ", i);
		var_Get_Show(var_data);
		
		slist = rt_slist_next(slist);
		i++;
		
	}while(slist);

	rt_kprintf("****** var list end ******\n");
}


/**************************************************************************
** 函数名称: var_List_ux
** 功能描述: 变量(类型)列表显示
**
** 输     入: argv 参数数组列表指针
** 输     出:
** 返     回:
**
** 作 	 者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
void var_List_ux(char **argv)
{
  	unsigned int i;
	zdbeug_var_t *var_data;

	rt_slist_t *slist = rt_slist_first(&var_head.slist);

	rt_kprintf("***** var %s list begin *****\n", argv[1]);

	i = 0;
	do
	{
		var_data = rt_container_of(slist, zdbeug_var_t, slist);
		if(!rt_strcmp(var_data->Type, argv[1]))      // 找到变量名
		{
			rt_kprintf("v%03d: ", i);
		  	var_Get_Show(var_data);
		}
		slist = rt_slist_next(slist);
		i++;
		
	}while(slist);

	rt_kprintf("****** var %s list end ******\n", argv[1]);
}



/**************************************************************************
** 函数名称: var_get
** 功能描述: 获取变量值命令
**
** 输     入: argv 参数数组列表指针
** 输     出:
** 返     回:
**
** 作 	 者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
void var_get(char**argv)
{
  	unsigned int i;
	zdbeug_var_t *var_data;

	if(Get_Num(argv[1], &i, 'v'))   // 通过变量编号设置
	{
		var_data = var_get_var_from_num(i);

		if(var_data)
		{
			rt_kprintf("get var: ");
			rt_kprintf("v%03d ", i);
			var_Get_Show(var_data);
		  	return;
		}
	  
	}


	var_data = var_get_var_from_name(argv[1], &i);

	if(var_data)
	{
		rt_kprintf("get var: ");
		rt_kprintf("v%03d ", i);
		var_Get_Show(var_data);
	  	return;
	}

  	rt_kprintf("--- Unfound var %s ---\n", argv[1]);
		
}


/**************************************************************************
** 函数名称: var_get_ux
** 功能描述: 获取变量值命令
**
** 输     入: argv 参数数组列表指针
** 输     出:
** 返     回:
**
** 作 	 者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
void var_get_ux(char**argv)
{
  	unsigned int i;
	zdbeug_var_t *var_data;

	if(Get_Num(argv[2], &i, 'v'))   // 通过变量编号设置
	{
		var_data = var_get_var_from_num(i);

		if(var_data && (!rt_strcmp(var_data->Type, argv[1])))
		{
			rt_kprintf("get var: ");
			rt_kprintf("v%03d ", i);
			var_Get_Show(var_data);
		  	return;
		}
	  
	}

	var_data = var_get_var_from_name(argv[2], &i);

	if(var_data && (!rt_strcmp(var_data->Type, argv[1])))
	{
		rt_kprintf("get var: ");
		rt_kprintf("v%03d ", i);
		var_Get_Show(var_data);
	  	return;
	}

  	rt_kprintf("--- Unfound var %s %s ---\n", argv[1], argv[2]);
		
}


/**************************************************************************
** 函数名称: var_set
** 功能描述: 设置变量值命令
**
** 输     入: argv 参数数组列表指针
** 输     出:
** 返     回:
**
** 作 	 者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
void var_set(char **argv)
{
	unsigned int i;
	zdbeug_var_t *var_data;

	if(Get_Num(argv[1], &i, 'v'))   // 通过变量编号设置
	{
		var_data = var_get_var_from_num(i);

		if(var_data)
		{
			rt_kprintf("set var: ");
			rt_kprintf("v%03d ", i);
			var_Set_Show(var_data, argv[2]);
		  	return;
		}
	  
	}


	var_data = var_get_var_from_name(argv[1], &i);

	if(var_data)
	{
		rt_kprintf("set var: ");
		rt_kprintf("v%03d ", i);
		var_Set_Show(var_data, argv[2]);
	  	return;
	}

  	rt_kprintf("--- Unfound var %s ---\n", argv[1]);
		
}


/**************************************************************************
** 函数名称: var_set
** 功能描述: 设置变量值命令
**
** 输     入: argv 参数数组列表指针
** 输     出:
** 返     回:
**
** 作 	 者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
void var_set_ux(char **argv)
{
	unsigned int i;
	zdbeug_var_t *var_data;

	if(Get_Num(argv[2], &i, 'v'))   // 通过变量编号设置
	{
		var_data = var_get_var_from_num(i);

		if(var_data && (!rt_strcmp(var_data->Type, argv[1])))
		{
			rt_kprintf("set var: ");
			rt_kprintf("v%03d ", i);
			var_Set_Show(var_data, argv[3]);
		  	return;
		}
	  
	}

	var_data = var_get_var_from_name(argv[2], &i);

	if(var_data && (!rt_strcmp(var_data->Type, argv[1])))
	{
		rt_kprintf("set var: ");
		rt_kprintf("v%03d ", i);
		var_Set_Show(var_data, argv[3]);
	  	return;
	}

  	rt_kprintf("--- Unfound var %s %s ---\n", argv[1], argv[2]);
		
}

/**************************************************************************
** 函数名称: var_add
** 功能描述: 添加变量命令
**
** 输     入: argv 参数数组列表指针
** 输     出:
** 返     回:
**
** 作 	 者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
void var_add(char **argv)
{
	char *name;
	unsigned char type;
	unsigned int var_Value;
	unsigned int i;
	zdbeug_var_t *var_data;
	
	if(!rt_strcmp(argv[1], "add"))
	{
		type = var_get_type_num(argv[2]);		
		if(VAT_TYPE_ERR == type)
		{
		  	rt_kprintf("var error type = %s \n", argv[2]);
			return;
		}

		var_data = var_get_var_from_name(argv[3], &i);

		if(var_data)
		{
			rt_kprintf("var redo\n");
			return;
		}

		name = rt_malloc(rt_strlen(argv[3]) + 1);
		rt_memcpy(name, argv[3], rt_strlen(argv[3]) + 1);

		if(('0' == argv[4][0]) && (('x' == argv[4][1]) || ('X' == argv[4][1])))
		{
			HexToUint32(&argv[4][2], rt_strlen(argv[4]) - 2, &var_Value);
		}
		else
		{
		  	DecToUint32(&argv[4][0], rt_strlen(argv[4]), &var_Value);
		}
				
	  	var_add_var_list(type, name, (unsigned int *)var_Value);

		var_data = var_get_var_from_name(name, &i);
		if(var_data)
	  	{
	  		rt_kprintf("v%03d ", i);
	  		var_Get_Show(var_data);
	  	}
		
	}
	else
	{
		rt_kprintf("var command error\n");
	}
}


/**************************************************************************
** 函数名称: var_init
** 功能描述: 变量列表初始化
**
** 输     入:
** 输     出:
** 返     回:
**
** 作 	 者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
int var_init(void)
{
  	var_head.Type  = RT_NULL;
	var_head.Name  = RT_NULL;
	var_head.Value = RT_NULL;
	rt_slist_init(&var_head.slist);
	
	zdebug_add_var(VAT_TYPE_U32, zdebug);
	zdebug_add_var(VAT_TYPE_U8,  hex);

	return RT_EOK;
}
INIT_APP_EXPORT(var_init);

/**************************************************************************
** 函数名称: var
** 功能描述: 变量操作命令
**
** 输     入: argc 参数数目，argv 参数数组列表指针
** 输     出:
** 返     回:
**
** 作 	 者: zhb
** 日     期: 2023.07.28
**------------------------------------------------------------------------
**************************************************************************/
void var(int argc, char**argv)
{	
  	switch(argc)
  	{
    	case 1:
			var_List();
			break;

		case 2:
			if(var_Get_Type(argv[1]))
      		{
        		var_List_ux(argv);
      		}
    		else
    		{
    	  		var_get(argv);
    		}
			break;

		case 3:
			if(var_Get_Type(argv[1]))
      		{
        		var_get_ux(argv);
      		}
    		else
    		{
    	  		var_set(argv);
    		}
			break;

		case 4:
			var_set_ux(argv);
			break;

		case 5:
			var_add(argv);
			break;

		default:
			break;
  	}

}
MSH_CMD_EXPORT(var, input 'var <add> <type> <name(vxxx)> <value(addr)>');


#endif /* ZDEBUG_VAR */


