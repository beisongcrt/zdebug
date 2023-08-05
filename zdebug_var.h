/*
 * 
 */

#ifndef FINSH_CMD_VAR_H
#define FINSH_CMD_VAR_H

#ifdef ZDEBUG_VAR

#define VAT_TYPE_VOID    0
#define VAT_TYPE_U8      1
#define VAT_TYPE_U16     2
#define VAT_TYPE_U32     3

#define VAT_TYPE_P8      4
#define VAT_TYPE_P16     5
#define VAT_TYPE_P32     6
#define VAT_TYPE_STU     7

#define VAT_TYPE_NUM     8
#define VAT_TYPE_ERR     255

struct zdebug_var_node
{
  	char *Type;          // 变量类型
  	char *Name;          // 变量名称
	void *Value;         // 变量指针
  	rt_slist_t slist;
	
};

typedef struct zdebug_var_node zdbeug_var_t;


extern char * var_type_str[];

int get_str_count(char * str, char ch);

unsigned char Get_Num(char *Data, unsigned int *Num, char c);
void Char_Escape(char *des_str, char *scr_str);
unsigned char DecToUint32(char *str, unsigned int len, unsigned int *sum);
unsigned char HexToUint32(char *str, unsigned int len, unsigned int *sum);
void var_Show_Value(const char *Type, void *var_Value);

void var_add_var_list(unsigned char Type, char *Name, void *Value);


#endif /* ZDEBUG_VAR */

#endif


