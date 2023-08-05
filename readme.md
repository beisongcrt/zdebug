# zdebug

**版本：v1.0.0**

2023.08.04：初始版本

## 1、介绍

方便的调试工具，随时控制打印 log，查看、设置变量数据，执行函数。

### 1.1 zdebug 有三个调试功能。

1、ZDEBUG_LOG，根据设置的标志位，打印 log 信息。

2、ZDEBUG_VAR，对变量进行查看、设置、加载。

3、ZDEBUG_FUN，手动控制函数执行。

### 1.2 依赖

需要打开控制台 RT_USING_CONSOLE，ZDEBUG_VAR 和 ZDEBUG_FUN 需要使用堆 RT_USING_HEAP

### 1.3 查询命令

在控制台中输入 help 查询使用命令。

fun              - input 'fun \<name(fxxx)\> \<param ...\>'
var              - input 'var \<add\> \<type\> \<name(vxxx)\> \<value(addr)\>'

## 2、使用

### 2.1 ZDEBUG_LOG

对 rt_kprintf 进行简单封装。

```c
#define zdebug_log(type, message)                         \
do                                                        \
{                                                         \
    if ((zDEBUG_ERR == type) || (zdebug & type))          \
        rt_kprintf message;                               \
}                                                         \
while (0)
```

- 打开 ZDEBUG_LOG

- 在需要打印信息处添加 zdebug_log

  ```c
  zdebug_log(type, message)
  ```

  type：打印信息的类别，是32位无符号数据，可以分别设置 31 个打印信息类别。

  ​           与32位无符号变量 **zdebug** 按位与后，不为 0 则打印 log 信息。

  ​           type 最高位为错误信息位，不需要按位与 zdebug  可直接打印信息。

  ```c
  #define zDEBUG_ERR                    0x80000000     
  ```

  message：打印的信息

  **注意：打印的信息需要添加英文半角小括号，变量 zdebug 可以通过 var 命令控制其数值 **
  
  **示例：**
  
  ```c
  #define zDEBUG_BIT_00                 0x00000001
  #define LED_1                         zDEBUG_BIT_00     
  
  unsigned int LED_Time = 500;    
  zdebug_log(LED_1, ("LED_Time = %d\n", LED_Time));
  ```


### 2.2 ZDEBUG_VAR

- 打开 ZDEBUG_VAR

- 程序中可以直接添加变量到变量列表中

  ```
  #define zdebug_add_var(type, var)  var_add_var_list(type, #var, &var)
  ```

  zdebug_add_var(type, var) 

  type：变量类型，在 zdebug_var.h 中定义。如下：

  ```c
  #define VAT_TYPE_VOID    0
  #define VAT_TYPE_U8      1
  #define VAT_TYPE_U16     2
  #define VAT_TYPE_U32     3
  #define VAT_TYPE_P8      4
  #define VAT_TYPE_P16     5
  #define VAT_TYPE_P32     6
  #define VAT_TYPE_STU     7
  ```

  var：变量名称

  **例如：**

  zdebug_add_var(VAT_TYPE_U32, zdebug);

- 通过控制台可以查看、设置变量数据，也可以临时添加变量到变量列表中。

  **命令：**var <add\> \<type\> <name(vxxx)> \<value(addr)\>

  var：命令名

  add：添加变量命令

  type：变量类型，"void", "u8", "u16", "u32", "p8", "p16", "p32", "stu"，void 无类型，u 无符号类型，p 指针类型，stu 结构体类型

  name(vxxx)：变量名（变量编号）

  value(addr)：变量值，在添加变量命令时为变量地址



#### 2.2.1 查看变量列表

- **var：查看所有变量，如：var**

  **返回：**

  \*\*\*\*\* var list begin \*\*\*\*\* 

  v000: ADDR = 0x20001298 u32 zdebug = 0
  v001: ADDR = 0x2000129C  u8 hex = 0
  v002: ADDR = 0x20000204 u32 LED_Time = 500
  v003: ADDR = 0x2000020C p32 pLED = 0x20000204, * = 500

  \*\*\*\*\*  var list end \*\*\*\*\* 

  **变量说明：**

  v000: ADDR = 0x20001298 u32 zdebug = 0

  v000：是变量编号

  ADDR = 0x20001B88：变量地址

  u32：变量类型

  zdebug：变量名

  0：变量值

  **指针变量说明：**

  v003: ADDR = 0x2000020C p32 pLED = 0x20000204, * = 500

  v003：是变量编号

  ADDR = 0x2000020C：变量地址

  p32：变量类型，p 为指针类型

  pLED：指针变量名

  0x20000204：指针变量的值

  \* = 500：指针变量指向的变量的值

- **var \<type\>：查看对应类型的所有变量，如：var u32**

  **返回：**

  \*\*\*\*\* var u32 list begin \*\*\*\*\* 

  v000: ADDR = 0x20001298 u32 zdebug = 0
  v002: ADDR = 0x20000204 u32 LED_Time = 500

  \*\*\*\*\*  var u32 list end \*\*\*\*\* 

#### 2.2.2 查看单独变量

- **var \<name(vxxx)\>：查看单独变量，如：var zdebug，var v000**

  var 后可以是变量名，也可以是变量编号。

  需要输入正确的变量名称或编号，否则报错。

  --- Unfound var zdebu ---

  **注意：变量编号是变量前的序号，需加小写 v，如v000，v001。**

  **返回：**

  get var: v000 ADDR = 0x20001298 u32 zdebug = 0

- **var \<type\> \<name(vxxx)\>：查看对应类型的单独变量，如：var u32 zdebug，var u32 v000**

  var 后需要输入正确的变量类型，否则报错。

  --- Unfound var u8 zdebug ---

  type 后可以是变量名，也可以是变量编号。需要输入正确的变量名称或编号，否则报错。

  --- Unfound var u32 zdebu ---

  **注意：变量编号是变量前的序号，需加小写 v，如v000，v001。**

  **返回：**

  get var: v000 ADDR = 0x20001298 u32 zdebug = 0

#### 2.2.3 设置变量数据

- **var \<name(vxxx)\> \<value(addr)\>：设置变量数据，如：var zdebug 1，var v000 1**

  var 后可以是变量名，也可以是变量编号。

  需要输入正确的变量名称或编号，否则报错。

  --- Unfound var zdebu ---

  value 设置变量的值，可以是10进制，也可以是16进制。16进制需要在数值前添加 0x 或 0X，不区分大小写。

  **注意：变量编号是变量前的序号，需加小写 v，如v000，v001。**

  **返回：**

   set var: v000 ADDR = 0x20001298 u32 zdebug = 1

- **var \<type\> \<name(vxxx)\> \<value(addr)\>：设置变量数据，如：var u32 zdebug 1，var u32 v000 1**

  var 后需要输入正确的变量类型，否则报错。

  --- Unfound var u8 zdebug ---

  需要输入正确的变量名称或编号，否则报错。

  --- Unfound var zdebu ---

  value 设置变量的值，可以是10进制，也可以是16进制。16进制需要在数值前添加 0x 或 0X，不区分大小写。

  **注意：变量编号是变量前的序号，需加小写 v，如v000，v001。**

  **返回：**

   set var: v000 ADDR = 0x20001298 u32 zdebug = 1

- **设置指针变量的值和指针变量指向的变量的值**

  - 设置指针变量时，和普通变量设置方法相同。**这时设置的是指针变量指向的地址。**

    \*\*\*\*\* var list begin \*\*\*\*\* 

    v000: ADDR = 0x20001298 u32 zdebug = 0
    v001: ADDR = 0x2000129C  u8 hex = 0
    v002: ADDR = 0x20000204 u32 LED_Time = 200
    v003: ADDR = 0x2000020C p32 pLED = 0x20000204, * = 200
    v004: ADDR = 0x20000208 u32 LED_Time_2 = 500

    \*\*\*\*\*  var list end \*\*\*\*\* 

    

    var pLED 0x20000208
    set var: v003 ADDR = 0x2000020C p32 pLED = 0x20000208, * = 500

  - 设置指针变量指向的变量值时，**需要在数据前添加 *** 

    var pLED *300
    set var: v003 ADDR = 0x2000020C p32 pLED = 0x20000208, * = 300

    

    \*\*\*\*\* var list begin \*\*\*\*\* 

    v000: ADDR = 0x20001298 u32 zdebug = 0
    v001: ADDR = 0x2000129C  u8 hex = 0
    v002: ADDR = 0x20000204 u32 LED_Time = 200
     v003: ADDR = 0x2000020C p32 pLED = 0x20000208, * = 300
    v004: ADDR = 0x20000208 u32 LED_Time_2 = 300

    \*\*\*\*\*  var list end \*\*\*\*\* 

  

#### 2.2.4 添加变量

- **var \<add\> \<type\> \<name(vxxx)\> \<value(addr)\>：设置变量，如：var add u32 LED_Time_2 0x20000208**

  var 后需要输入 add 命令

  需要输入正确的变量类型，否则之后的操作可能会出现错误。

  变量名称可以输入任意字符，最好和程序中变量名称一致。**另不要用 v 后面跟随数字的变量名，以免和变量编号重复。**

  在此命令中 value(addr) 是设置变量的地址，可以是10进制，也可以是16进制。16进制需要在数值前添加 0x 或 0X，不区分大小写。

  **变量地址在程序源码 .map 文件中可以搜索查找。**

  **返回：**

  v004 ADDR = 0x20000208 u32 LED_Time_2 = 500
  
  查询变量列表显示：
  
  \*\*\*\*\* var list begin \*\*\*\*\* 
  
  v000: ADDR = 0x20001298 u32 zdebug = 0
  v001: ADDR = 0x2000129C  u8 hex = 0
  v002: ADDR = 0x20000204 u32 LED_Time = 500
  v003: ADDR = 0x2000020C p32 pLED = 0x20000204, * = 500
  v004: ADDR = 0x20000208 u32 LED_Time_2 = 500
  
  \*\*\*\*\*  var list end \*\*\*\*\* 

### 2.3 ZDEBUG_FUN

通过控制台可以手动控制函数执行

- 打开 ZDEBUG_FUN

- 程序中可以直接添加变量到变量列表中

  ```
  #define zdebug_add_fun(type, fun, param)  fun_add_fun_list(type, #fun, #param, fun) 
  ```

  zdebug_add_fun(type, fun, param)

  type：变量类型，在 zdebug_var.h 中定义。如下：

  ```c
  #define VAT_TYPE_VOID    0
  #define VAT_TYPE_U8      1
  #define VAT_TYPE_U16     2
  #define VAT_TYPE_U32     3
  #define VAT_TYPE_P8      4
  #define VAT_TYPE_P16     5
  #define VAT_TYPE_P32     6
  #define VAT_TYPE_STU     7
  ```

  fun：函数名称

  param：参数字符串，参数需用英文半角小括号包裹

  例如：

  zdebug_add_fun(VAT_TYPE_U32, fun_set_zdebug, (unsigned int debug_status));

- 通过控制台可以查看，执行函数

  **命令：**fun name(fxxx) <param ...>

  fun：命令名

  name(fxxx)：函数名（函数编号）

  param ：函数参数，参数之间用空格隔开，最多可以有 10 参数，没有参数可以不填。

  ​                指针参数之间填写地址，可以是10进制，也可以是16进制。16进制需要在数值前添加 0x 或 0X，不区分大小写

  ​                字符串参数需用双引号括包裹

#### 2.3.1 查看函数列表

- **fun：查看所有函数，如：fun**

  返回：

  \*\*\*\*\* fun list begin \*\*\*\*\* 
  
  f000: ADDR = 0x08007D11  u32 fun_set_zdebug(unsigned int debug_status)
  f001: ADDR = 0x08007CAD  p8 fun_test(unsigned char sw, unsigned char *p, char *Data)
  
  \*\*\*\*\*  fun list end \*\*\*\*\* 

#### 2.3.2 执行函数

- **fun name(fxxx) <param ...>：查看所有函数，如：fun fun_set_zdebug 1，fun f000 1 **

  fun 后可以是函数名，也可以是函数编号。

  需要输入正确的函数名称或编号，否则报错。

  --- Unfound fun fun_set_zdebu ---

  param 函数参数。

  **注意：函数编号是函数前的序号，需加小写 f，如f000，f001。**

  **返回：函数的返回值**

  fun return type u32 value = 1

### 2.4 默认变量

程序中自带两个变量，zdebug、hex

#### 2.4.1 zdebug

这个变量是打印 zdebug_log 的比较变量，当打印的 type 按位与 zdebug 后不为 0 则打印此条 log。

```
#define zDEBUG_BIT_00                 0x00000001
#define zDEBUG_BIT_01                 0x00000002

#define LED_1 zDEBUG_BIT_00
#define LED_2 zDEBUG_BIT_01

unsigned int LED_Time = 500;
unsigned int LED_Time_2 = 500;

rt_thread_mdelay(LED_Time);
zdebug_log(LED_1, ("LED_Time = %d\n", LED_Time));
rt_thread_mdelay(LED_Time_2);
zdebug_log(LED_2, ("LED_Time_2 = %d\n", LED_Time_2));
```

可以同时设置 31 种打印 log 位，每个位可以随意组合。如下：

var zdebug 1
set var: v000 ADDR = 0x20001298 u32 zdebug = 1
LED_Time = 500
LED_Time = 500

var zdebug 2
 set var: v000 ADDR = 0x20001298 u32 zdebug = 2
LED_Time_2 = 500
LED_Time_2 = 500

var zdebug 3
set var: v000 ADDR = 0x20001298 u32 zdebug = 3
LED_Time = 500
LED_Time_2 = 500
LED_Time = 500
LED_Time_2 = 500

#### 2.4.2 hex

这个变量是设置输出数据的显示形式，等于 0 时为 10 进制显示，不为 0 时为 16 进制显示。

**注意：这个变量不影响变量的输入形式，输入默认为 10 进制，16 进制需要在数值前添加 0x 或 0X，不区分大小写**

var hex 1
set var: v001 ADDR = 0x2000129C  u8 hex = 0x01



\*\*\*\*\* var list begin \*\*\*\*\* 

v000: ADDR = 0x20001298 u32 zdebug = 0x00000000
v001: ADDR = 0x2000129C  u8 hex = 0x01
v002: ADDR = 0x20000204 u32 LED_Time = 0x000001F4
v003: ADDR = 0x2000020C p32 pLED = 0x20000204, * = 0x000001F4

\*\*\*\*\*  var list end \*\*\*\*\* 



var hex 0
set var: v001 ADDR = 0x2000129C  u8 hex = 0



\*\*\*\*\* var list begin \*\*\*\*\* 

v000: ADDR = 0x20001298 u32 zdebug = 0
v001: ADDR = 0x2000129C  u8 hex = 0
v002: ADDR = 0x20000204 u32 LED_Time = 500
v003: ADDR = 0x2000020C p32 pLED = 0x20000204, * = 500

\*\*\*\*\*  var list end \*\*\*\*\* 
