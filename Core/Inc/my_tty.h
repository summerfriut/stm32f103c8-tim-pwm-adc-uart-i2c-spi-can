#ifndef __MY_TTY_H__
#define __MY_TTY_H__
#include "my_cfg.h"
#include "my_usart.h"
#include "my_printf.h"

#define MAX_FNAME_LEN 		30	//函数名最大长度，应该设置为不小于最长函数名的长度。											   
#define MAX_PARM 			10	//最大为10个参数 ,修改此参数,必须修改usmart_exe与之对应.
#define PARM_LEN 			200	//所有参数之和的长度不超过PARM_LEN个字节,注意串口接收部分要与之对应(不小于PARM_LEN)


#define USMART_ENTIMX_SCAN 	1	//使用TIM的定时中断来扫描SCAN函数,如果设置为0,需要自己实现隔一段时间扫描一次scan函数.
								//注意:如果要用runtime统计功能,必须设置USMART_ENTIMX_SCAN为1!!!!
								
#define USMART_USE_HELP		1	//使用帮助，该值设为0，可以节省近700个字节，但是将导致无法显示帮助信息。
#define USMART_USE_WRFUNS	1	//使用读写函数,使能这里,可以读取任何地址的值,还可以写寄存器的值.
///////////////////////////////////////////////END///////////////////////////////////////////////////////////

#define USMART_OK 			0  //无错误
#define USMART_FUNCERR 		1  //函数错误
#define USMART_PARMERR 		2  //参数错误
#define USMART_PARMOVER 	3  //参数溢出
#define USMART_NOFUNCFIND 	4  //未找到匹配函数

#define SP_TYPE_DEC      	0  //10进制参数显示
#define SP_TYPE_HEX       	1  //16进制参数显示


typedef struct
{   
    uint8_t type:7;
    uint8_t is_point:1;
}__para_type;

typedef enum
{
    PARA_VOID,
    PARA_UINT8_T,
    PARA_UINT16_T,
    PARA_UINT32_T,
    PARA_FLOAT,
    PARA_STRING,
    PARA_POINT,
}__para_type_enum;
typedef union
{
    uint8_t     uint8bit;
    uint16_t    uint16bit;
    uint32_t    uint32bit;
    float       float32bit;
    char*       string;
    void*       addr_point;
}__para_type_union;

typedef struct
{
	__para_type para_type;				//参数类型
	uint8_t		para_len;				//参数以该类型为单位的长度
    void*		para_val_adr;			//参数值所在的地址
}__para_info;

typedef struct
{
    uint16_t new_msg:1;
    uint16_t msg_len:15;
}__tty_info;
extern uint16_t tty_info;

//函数名列表	 
typedef struct
{
	void*       func;                   //函数指针
    char*       declaration;           	//函数声明
    char* 		name;		            //函数名在函数声明中的地址
    uint8_t		name_len;				//函数名长度
}__func_info;

//usmart控制管理器
typedef struct
{
	__func_info *funs;	                    //函数名指针
	void        (*init)(uint8_t);           //初始化
	uint8_t     (*cmd_rec)(uint8_t*str);    //识别函数名及参数
	void        (*exe)(void); 				//执行 
	void        (*scan)(void);              //扫描
	uint8_t     fnum; 				  		//函数数量
	uint8_t     pnum;                       //参数数量
	uint8_t     id;							//函数id
	uint8_t     sptype;						//参数显示类型(非字符串参数):0,10进制;1,16进制;
	uint16_t    parmtype;					//参数的类型
	uint8_t     plentbl[MAX_PARM];  		//每个参数的长度暂存表
	uint8_t     parm[PARM_LEN];  			//函数的参数
	uint8_t     runtimeflag;			    //0,不统计函数执行时间;1,统计函数执行时间,注意:此功能必须在USMART_ENTIMX_SCAN使能的时候,才有用
	uint32_t    runtime;					//运行时间,单位:0.1ms,最大延时时间为定时器CNT值的2倍*0.1ms
}__smart_info;
extern __func_info func_info[];
extern __smart_info smart_info;	


uint32_t read_addr(uint32_t addr);	//读取指定地址的值
void write_addr(uint32_t addr,uint32_t val);//在指定地址写入指定的值


void response_func_cmd(char* str);


#endif


