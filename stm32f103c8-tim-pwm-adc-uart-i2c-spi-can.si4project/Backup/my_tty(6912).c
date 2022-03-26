#include "my_tty.h"
#include <stdio.h>


uint16_t tty_info;

//函数名列表初始化(用户自己添加)
//用户直接在这里输入要执行的函数名及其查找串
__func_info usmart_nametab[]=
{
	(void*)uart_send_msg,"void uart_send_msg(uint8_t port,uint8_t* pdata,uint8_t msg_len)",
};

__smart_info usmart_dev=
{
	.funs = usmart_nametab,
	.cmd_rec = usmart_cmd_rec,
	.exe = usmart_exe,
	.scan = usmart_scan,
	.fnum = sizeof(usmart_nametab)/sizeof(__func_info),//函数数量
	.pnum = 0,	  	//参数数量
	.id = 0,	 	//函数ID
	.sptype = 1,		//参数显示类型,0,10进制;1,16进制
	.parmtype = 0,		//参数类型.bitx:,0,数字;1,字符串	    
	.plentbl = 0,	  	//每个参数的长度暂存表,需要MAX_PARM个0初始化
	.parm = 0,		//函数的参数,需要PARM_LEN个0初始化
};   


//系统命令
uint8_t* sys_cmd_tab[]=
{
	(uint8_t*)"?",
	(uint8_t*)"help",
	(uint8_t*)"list",
	(uint8_t*)"id",
	(uint8_t*)"hex",
	(uint8_t*)"dec",
	(uint8_t*)"runtime",	   
};
//处理系统指令
//0,成功处理;其他,错误代码;
uint8_t usmart_sys_cmd_exe(uint8_t *str)
{
	uint8_t i;
	uint8_t sfname[MAX_FNAME_LEN];//存放本地函数名
	uint8_t pnum;
	uint8_t rval;
	uint32_t res;  
	res = usmart_get_cmdname(str,sfname, &i, MAX_FNAME_LEN);//得到指令及指令长度
	if(res)
        return USMART_FUNCERR;//错误的指令 
	str += i;
	for(i = 0 ; i < sizeof(sys_cmd_tab) / 4 ; i++)//支持的系统指令
	{
		if(usmart_strcmp(sfname,sys_cmd_tab[i]) == 0)
            break;
	}
	switch(i)
	{
		case 0:
		case 1://帮助指令
			printf("\r\n");
#if USMART_USE_HELP 
			printf("------------------------USMART V3.3------------------------ \r\n");
			printf("    USMART是由ALIENTEK开发的一个灵巧的串口调试互交组件,通过 \r\n");
			printf("它,你可以通过串口助手调用程序里面的任何函数,并执行.因此,你可\r\n");
			printf("以随意更改函数的输入参数(支持数字(10/16进制,支持负数)、字符串\r\n"),
			printf("、函数入口地址等作为参数),单个函数最多支持10个输入参数,并支持\r\n"),  
			printf("函数返回值显示.支持参数显示进制设置功能,支持进制转换功能.\r\n");
			printf("技术支持:www.openedv.com\r\n");
			printf("USMART有7个系统命令(必须小写):\r\n");
			printf("?:      获取帮助信息\r\n");
			printf("help:   获取帮助信息\r\n");
			printf("list:   可用的函数列表\r\n\n");
			printf("id:     可用函数的ID列表\r\n\n");
			printf("hex:    参数16进制显示,后跟空格+数字即执行进制转换\r\n\n");
			printf("dec:    参数10进制显示,后跟空格+数字即执行进制转换\r\n\n");
			printf("runtime:1,开启函数运行计时;0,关闭函数运行计时;\r\n\n");
			printf("请按照程序编写格式输入函数名及参数并以回车键结束.\r\n");    
			printf("--------------------------ALIENTEK------------------------- \r\n");
#else
			printf("指令失效\r\n");
#endif
			break;
		case 2://查询指令
			printf("\r\n");
			printf("-------------------------函数清单--------------------------- \r\n");
			for(i=0;i<usmart_dev.fnum;i++)
                printf("%s\r\n",usmart_dev.funs[i].name);
			printf("\r\n");
			break;	 
		case 3://查询ID
			printf("\r\n");
			printf("-------------------------函数 ID --------------------------- \r\n");
			for(i=0;i<usmart_dev.fnum;i++)
			{
				usmart_get_fname((uint8_t*)usmart_dev.funs[i].name,sfname,&pnum,&rval);//得到本地函数名 
				printf("%s id is:\r\n0X%08X\r\n",sfname,(uint32_t)usmart_dev.funs[i].func); //显示ID
			}
			printf("\r\n");
			break;
		case 4://hex指令
			printf("\r\n");
			usmart_get_aparm(str,sfname,&i);
			if(i==0)//参数正常
			{
				i = usmart_str2num(sfname,&res);	   	//记录该参数	
				if(i==0)						  	//进制转换功能
					printf("HEX:0X%X\r\n",res);	   	//转为16进制
                else if(i!=4)
                    return USMART_PARMERR;//参数错误.
				else 				   				//参数显示设定功能
				{
					printf("16进制参数显示!\r\n");
					usmart_dev.sptype=SP_TYPE_HEX;  
				}
			}
            else
                return USMART_PARMERR;			//参数错误.
			printf("\r\n"); 
			break;
		case 5://dec指令
			printf("\r\n");
			usmart_get_aparm(str,sfname,&i);
			if(i==0)//参数正常
			{
				i=usmart_str2num(sfname,&res);	   	//记录该参数	
				if(i==0)						   	//进制转换功能
					printf("DEC:%u\r\n",res);	   	//转为10进制
				else if(i!=4)
                    return USMART_PARMERR;//参数错误.
				else 				   				//参数显示设定功能
				{
					printf("10进制参数显示!\r\n");
					usmart_dev.sptype=SP_TYPE_DEC;  
				}

			}
            else 
                return USMART_PARMERR;			//参数错误. 
			printf("\r\n"); 
			break;	 
		case 6://runtime指令,设置是否显示函数执行时间
			printf("\r\n");
			usmart_get_aparm(str,sfname,&i);
			if(i==0)//参数正常
			{
				i=usmart_str2num(sfname,&res);	   		//记录该参数	
				if(i==0)						   		//读取指定地址数据功能
				{
					if(USMART_ENTIMX_SCAN==0)
                        printf("\r\nError! \r\nTo EN RunTime function,Please set USMART_ENTIMX_SCAN = 1 first!\r\n");//报错
					else
					{
						usmart_dev.runtimeflag=res;
						if(usmart_dev.runtimeflag)
                            printf("Run Time Calculation ON\r\n");
						else 
                            printf("Run Time Calculation OFF\r\n"); 
					}
				}
                else 
                    return USMART_PARMERR;   			//未带参数,或者参数错误	 
 			}
            else 
                return USMART_PARMERR;				//参数错误. 
			printf("\r\n"); 
			break;	    
		default://非法指令
			return USMART_FUNCERR;
	}
	return 0;
}

//从str中获取函数名,id,及参数信息
//*str:字符串指针.
//返回值:0,识别成功;其他,错误代码.
uint8_t usmart_cmd_rec(uint8_t* str) 
{
	uint8_t sta,i,rval;//状态	 
	uint8_t rpnum,spnum;
	uint8_t rfname[MAX_FNAME_LEN];//暂存空间,用于存放接收到的函数名  
	uint8_t sfname[MAX_FNAME_LEN];//存放本地函数名
	sta = usmart_get_fname(str,rfname,&rpnum,&rval);//得到接收到的数据的函数名及参数个数	  
	if(sta)
        return sta;//错误
	for(i=0;i<usmart_dev.fnum;i++)
	{
		sta=usmart_get_fname((uint8_t*)usmart_dev.funs[i].name,sfname,&spnum,&rval);//得到本地函数名及参数个数
		if(sta)
            return sta;//本地解析有误	  
		if(usmart_strcmp(sfname,rfname)==0)//相等
		{
			if(spnum>rpnum)
                return USMART_PARMERR;//参数错误(输入参数比源函数参数少)
			usmart_dev.id=i;//记录函数ID.
			break;//跳出.
		}	
	}
	if(i==usmart_dev.fnum)
        return USMART_NOFUNCFIND;	//未找到匹配的函数
 	sta=usmart_get_fparam(str,&i);					//得到函数参数个数	
	if(sta)
        return sta;								//返回错误
	usmart_dev.pnum=i;								//参数个数记录
    return USMART_OK;
}
//usamrt执行函数
//该函数用于最终执行从串口收到的有效函数.
//最多支持10个参数的函数,更多的参数支持也很容易实现.不过用的很少.一般5个左右的参数的函数已经很少见了.
//该函数会在串口打印执行情况.以:"函数名(参数1，参数2...参数N)=返回值".的形式打印.
//当所执行的函数没有返回值的时候,所打印的返回值是一个无意义的数据.
void usmart_exe(void)
{
	uint8_t id,i;
	uint32_t res;		   
	uint32_t temp[MAX_PARM];//参数转换,使之支持了字符串 
	uint8_t sfname[MAX_FNAME_LEN];//存放本地函数名
	uint8_t pnum,rval;
	id=usmart_dev.id;
	if(id>=usmart_dev.fnum)
        return;//不执行.
	usmart_get_fname((uint8_t*)usmart_dev.funs[id].name,sfname,&pnum,&rval);//得到本地函数名,及参数个数 
	printf("\r\n%s(",sfname);//输出正要执行的函数名
	for(i=0;i<pnum;i++)//输出参数
	{
		if(usmart_dev.parmtype&(1<<i))//参数是字符串
		{
			printf("%c",'"');			 
			printf("%s",usmart_dev.parm+usmart_get_parmpos(i));
			printf("%c",'"');
			temp[i]=(uint32_t)&(usmart_dev.parm[usmart_get_parmpos(i)]);
		}else						  //参数是数字
		{
			temp[i]=*(uint32_t*)(usmart_dev.parm+usmart_get_parmpos(i));
			if(usmart_dev.sptype==SP_TYPE_DEC)
                printf("%d",temp[i]);//10进制参数显示
			else 
                printf("0X%X",temp[i]);//16进制参数显示 	   
		}
		if(i!=pnum-1)printf(",");
	}
	printf(")");
//#if USMART_ENTIMX_SCAN==1
//	usmart_reset_runtime();	//计时器清零,开始计时
//#endif
	switch(usmart_dev.pnum)
	{
		case 0://无参数(void类型)											  
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)();
			break;
	    case 1://有1个参数
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0]);
			break;
	    case 2://有2个参数
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1]);
			break;
	    case 3://有3个参数
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2]);
			break;
	    case 4://有4个参数
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3]);
			break;
	    case 5://有5个参数
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4]);
			break;
	    case 6://有6个参数
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5]);
			break;
	    case 7://有7个参数
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6]);
			break;
	    case 8://有8个参数
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6],temp[7]);
			break;
	    case 9://有9个参数
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6],temp[7],temp[8]);
			break;
	    case 10://有10个参数
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6],temp[7],temp[8],temp[9]);
			break;
	}
//#if USMART_ENTIMX_SCAN==1
//	usmart_get_runtime();//获取函数执行时间
//#endif
	if(rval==1)//需要返回值.
	{
		if(usmart_dev.sptype==SP_TYPE_DEC)printf("=%u;\r\n",res);//输出执行结果(10进制参数显示)
		else printf("=0X%X;\r\n",res);//输出执行结果(16进制参数显示)	   
	}else printf(";\r\n");		//不需要返回值,直接输出结束
	if(usmart_dev.runtimeflag)	//需要显示函数执行时间
	{ 
		printf("Function Run Time:%d.%1dms\r\n",usmart_dev.runtime/10,usmart_dev.runtime%10);//打印函数执行时间 
	}
}


//usmart扫描函数
//通过调用该函数,实现usmart的各个控制.该函数需要每隔一定时间被调用一次
//以及时执行从串口发过来的各个函数.
//本函数可以在中断里面调用,从而实现自动管理.
//如果非ALIENTEK用户,则USART_RX_STA和USART_RX_BUF[]需要用户自己实现
void usmart_scan(void)
{
    uint8_t sta,len,index;
    __tty_info* tty_info_p = (__tty_info*)&tty_info;
    index = uart_port2index(printf_info.port);
    if(IS_INDEX(index) == pdFAIL)
        return;
	if(tty_info_p->new_msg)             //串口接收完成？(接收到\r\n)
	{
		len = tty_info_p->msg_len;	    //得到此次接收到的数据长度
        usart_info[index].recv_buf[usart_info[index].recv_buf_len - 2] = '\0';
		sta = usmart_dev.cmd_rec(usart_info[index].recv_buf);//得到函数各个信息
		if(sta == 0)
            usmart_dev.exe();	//执行函数 
		else
		{
			len = usmart_sys_cmd_exe(usart_info[index].recv_buf);
			if(len != USMART_FUNCERR)
                sta = len;
			if(sta)
			{
				switch(sta)
				{
					case USMART_FUNCERR:
						printf("函数错误!\r\n");
						break;	
					case USMART_PARMERR:
						printf("参数错误!\r\n");   			
						break;				
					case USMART_PARMOVER:
						printf("参数太多!\r\n");   			
						break;		
					case USMART_NOFUNCFIND:
						printf("未找到匹配的函数!\r\n");   			
						break;		
				}
			}
		}
		tty_info = 0;                   //状态寄存器清空	    
	} 
}
#if USMART_USE_WRFUNS==1 	//如果使能了读写操作
//读取指定地址的值		 
uint32_t read_addr(uint32_t addr)
{
	return *(uint32_t*)addr;//	
}
//在指定地址写入指定的值		 
void write_addr(uint32_t addr,uint32_t val)
{
	*(uint32_t*)addr=val; 	
}
#endif


//对比字符串str1和str2
//*str1:字符串1指针
//*str2:字符串2指针
//返回值:0，相等;1，不相等;
uint8_t usmart_strcmp(uint8_t *str1,uint8_t *str2)
{
	while(1)
	{
		if(*str1!=*str2)return 1;//不相等
		if(*str1=='\0')break;//对比完成了.
		str1++;
		str2++;
	}
	return 0;//两个字符串相等
}
//把str1的内容copy到str2
//*str1:字符串1指针
//*str2:字符串2指针			   
void usmart_strcopy(uint8_t*str1,uint8_t *str2)
{
	while(1)
	{										   
		*str2=*str1;	//拷贝
		if(*str1=='\0')break;//拷贝完成了.
		str1++;
		str2++;
	}
}
//得到字符串的长度(字节)
//*str:字符串指针
//返回值:字符串的长度		   
uint8_t usmart_strlen(uint8_t*str)
{
	uint8_t len=0;
	while(1)
	{							 
		if(*str=='\0')break;//拷贝完成了.
		len++;
		str++;
	}
	return len;
}
//m^n函数
//返回值:m^n次方
uint32_t usmart_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}	    
//把字符串转为数字
//支持16进制转换,但是16进制字母必须是大写的,且格式为以0X开头的.
//支持负数 
//*str:数字字符串指针
//*res:转换完的结果存放地址.
//返回值:0，成功转换完成.其他,错误代码.
//1,数据格式错误.2,16进制位数为0.3,起始格式错误.4,十进制位数为0.
uint8_t usmart_str2num(uint8_t*str,uint32_t *res)
{
	uint32_t t;
	int tnum;
	uint8_t bnum=0;		//数字的位数
	uint8_t *p;		  
	uint8_t hexdec=10;	//默认为十进制数据
	uint8_t flag=0;		//0,没有符号标记;1,表示正数;2,表示负数.
	p=str;
	*res=0;//清零.
	while(1)
	{
		if((*p<='9'&&*p>='0')||((*str=='-'||*str=='+')&&bnum==0)||(*p<='F'&&*p>='A')||(*p=='X'&&bnum==1))//参数合法
		{
			if(*p>='A')hexdec=16;	//字符串中存在字母,为16进制格式.
			if(*str=='-'){flag=2;str+=1;}//偏移掉符号
			else if(*str=='+'){flag=1;str+=1;}//偏移掉符号
			else bnum++;			//位数增加.
		}else if(*p=='\0')break;	//碰到结束符,退出.
		else return 1;				//不全是十进制或者16进制数据.
		p++; 
	} 
	p=str;			    //重新定位到字符串开始的地址.
	if(hexdec==16)		//16进制数据
	{
		if(bnum<3)return 2;			//位数小于3，直接退出.因为0X就占了2个,如果0X后面不跟数据,则该数据非法.
		if(*p=='0' && (*(p+1)=='X'))//必须以'0X'开头.
		{
			p+=2;	//偏移到数据起始地址.
			bnum-=2;//减去偏移量	 
		}else return 3;//起始头的格式不对
	}else if(bnum==0)return 4;//位数为0，直接退出.	  
	while(1)
	{
		if(bnum)bnum--;
		if(*p<='9'&&*p>='0')t=*p-'0';	//得到数字的值
		else t=*p-'A'+10;				//得到A~F对应的值	    
		*res+=t*usmart_pow(hexdec,bnum);		   
		p++;
		if(*p=='\0')break;//数据都查完了.	
	}
	if(flag==2)//是负数?
	{	
		tnum=-*res; 
		*res=tnum;
	}
	return 0;//成功转换
}
//得到指令名
//*str:源字符串
//*cmdname:指令名
//*nlen:指令名长度		
//maxlen:最大长度(做限制,指令不可能太长的)	
//返回值:0,成功;其他,失败.	  
uint8_t usmart_get_cmdname(uint8_t*str,uint8_t*cmdname,uint8_t *nlen,uint8_t maxlen)
{
	*nlen=0;
 	while(*str!=' '&&*str!='\0') //找到空格或者结束符则认为结束了
	{
		*cmdname=*str;
		str++;
		cmdname++;
		(*nlen)++;//统计命令长度
		if(*nlen>=maxlen)return 1;//错误的指令
	}
	*cmdname='\0';//加入结束符
	return 0;//正常返回
}
//获取下一个字符（当中间有很多空格的时候，此函数直接忽略空格，找到空格之后的第一个字符）
//str:字符串指针	
//返回值:下一个字符
uint8_t usmart_search_nextc(uint8_t* str)
{
	str++;
	while(*str == ' ' && str != '\0')
        str++;
	return *str;
} 
//从str中得到函数名
//*str:源字符串指针
//*fname:获取到的函数名字指针
//*pnum:函数的参数个数
//*rval:是否需要显示返回值(0,不需要;1,需要)
//返回值:0,成功;其他,错误代码.
uint8_t usmart_get_fname(uint8_t*str,uint8_t*fname,uint8_t *pnum,uint8_t *rval)
{
	uint8_t res;
	uint8_t fover = 0;	  //括号深度
	uint8_t *strtemp;
	uint8_t offset = 0;  
	uint8_t parmnum = 0;
	uint8_t temp = 1;
	uint8_t fpname[6];//void+X+'/0'
	uint8_t fplcnt = 0; //第一个参数的长度计数器
	uint8_t pcnt = 0;	 //参数计数器
	uint8_t nchar;
	//判断函数是否有返回值
	strtemp = str;
	while(*strtemp != '\0')//没有结束
	{
		if(*strtemp != ' ' && (pcnt & 0x7F) < 5)//最多记录5个字符
		{
			if(pcnt == 0)
                pcnt |= 0x80;//置位最高位,标记开始接收返回值类型
			if(((pcnt & 0x7f) == 4) && (*strtemp != '*'))
                break;//最后一个字符,必须是*
			fpname[pcnt & 0x7f] = *strtemp;//记录函数的返回值类型
			pcnt++;
		}
        else if(pcnt == 0x85)
            break;
		strtemp++;
	}
	if(pcnt)//接收完了
	{
		fpname[pcnt & 0x7f] = '\0';//加入结束符
		if(usmart_strcmp(fpname,(uint8_t*)"void") == 0)
            *rval = 0;//不需要返回值
		else
            *rval = 1;							   //需要返回值
		pcnt = 0;
	}
	res = 0;
	strtemp = str;
	while(*strtemp != '(' && *strtemp != '\0') //此代码找到函数名的真正起始位置
	{
		strtemp++;
		res++;
		if(*strtemp == ' ' || *strtemp == '*')
		{
			nchar = usmart_search_nextc(strtemp);		//获取下一个字符
			if(nchar != '(' && nchar != '*')
                offset = res;	//跳过空格和*号
		}
	}
	strtemp = str;
	if(offset)
        strtemp += offset + 1;//跳到函数名开始的地方	   
	res = 0;
	nchar = 0;//是否正在字符串里面的标志,0，不在字符串;1，在字符串;
	while(1)
	{
		if(*strtemp==0)
		{
			res = USMART_FUNCERR;//函数错误
			break;
		}
        else if(*strtemp == '(' && nchar == 0)
            fover++;//括号深度增加一级
		else if(*strtemp == ')' && nchar == 0)
		{
			if(fover)
                fover--;
			else 
                res = USMART_FUNCERR;//错误结束,没收到'('
			if(fover == 0)
                break;//到末尾了,退出	    
		}
        else if(*strtemp == '"')
            nchar =! nchar;
		if(fover == 0)//函数名还没接收完
		{
			if(*strtemp != ' ')//空格不属于函数名
			{
				*fname = *strtemp;//得到函数名
				fname++;
			}
		}
        else //已经接受完了函数名了.
		{
			if(*strtemp==',')
			{
				temp=1;		//使能增加一个参数
				pcnt++;	
			}
            else if(*strtemp!=' '&&*strtemp!='(')
			{
				if(pcnt==0&&fplcnt<5)		//当第一个参数来时,为了避免统计void类型的参数,必须做判断.
				{
					fpname[fplcnt]=*strtemp;//记录参数特征.
					fplcnt++;
				}
				temp++;	//得到有效参数(非空格)
			}
			if(fover==1&&temp==2)
			{
				temp++;		//防止重复增加
				parmnum++; 	//参数增加一个
			}
		}
		strtemp++; 			
	}
	if(parmnum==1)//只有1个参数.
	{
		fpname[fplcnt]='\0';//加入结束符
		if(usmart_strcmp(fpname,(uint8_t*)"void")==0)
            parmnum=0;//参数为void,表示没有参数.
	}
	*pnum=parmnum;	//记录参数个数
	*fname='\0';	//加入结束符
	return res;		//返回执行结果
}


//从str中得到一个函数的参数
//*str:源字符串指针
//*fparm:参数字符串指针
//*ptype:参数类型 0，数字;1，字符串;0XFF，参数错误
//返回值:0,已经无参数了;其他,下一个参数的偏移量.
uint8_t usmart_get_aparm(uint8_t *str,uint8_t *fparm,uint8_t *ptype)
{
	uint8_t i=0;
	uint8_t enout=0;
	uint8_t type=0;//默认是数字
	uint8_t string=0; //标记str是否正在读
	while(1)
	{
		if(*str==','&& string==0)
            enout=1;			//暂缓立即退出,目的是寻找下一个参数的起始地址
		if((*str==')'||*str=='\0')&&string==0)
            break;//立即退出标识符
		if(type==0)//默认是数字的
		{
			if((*str>='0' && *str<='9')||*str=='-'||*str=='+'||(*str>='a' && *str<='f')||(*str>='A' && *str<='F')||*str=='X'||*str=='x')//数字串检测
			{
				if(enout)break;					//找到了下一个参数,直接退出.
				if(*str>='a')*fparm=*str-0X20;	//小写转换为大写
				else *fparm=*str;		   		//小写或者数字保持不变
				fparm++;
			}else if(*str=='"')//找到字符串的开始标志
			{
				if(enout)break;//找到,后才找到",认为结束了.
				type=1;
				string=1;//登记STRING 正在读了
			}else if(*str!=' '&&*str!=',')//发现非法字符,参数错误
			{
				type=0XFF;
				break;
			}
		}else//string类
		{ 
			if(*str=='"')string=0;
			if(enout)break;			//找到了下一个参数,直接退出.
			if(string)				//字符串正在读
			{	
				if(*str=='\\')		//遇到转义符(不复制转义符)
				{ 
					str++;			//偏移到转义符后面的字符,不管什么字符,直接COPY
					i++;
				}					
				*fparm=*str;		//小写或者数字保持不变
				fparm++;
			}	
		}
		i++;//偏移量增加
		str++;
	}
	*fparm='\0';	//加入结束符
	*ptype=type;	//返回参数类型
	return i;		//返回参数长度
}
//得到指定参数的起始地址
//num:第num个参数,范围0~9.
//返回值:该参数的起始地址
uint8_t usmart_get_parmpos(uint8_t num)
{
	uint8_t temp=0;
	uint8_t i;
	for(i=0;i<num;i++)temp+=usmart_dev.plentbl[i];
	return temp;
}
//从str中得到函数参数
//str:源字符串;
//parn:参数的多少.0表示无参数 void类型
//返回值:0,成功;其他,错误代码.
uint8_t usmart_get_fparam(uint8_t*str,uint8_t *parn)
{	
	uint8_t i,type;  
	uint32_t res;
	uint8_t n=0;
	uint8_t len;
	uint8_t tstr[PARM_LEN+1];//字节长度的缓存,最多可以存放PARM_LEN个字符的字符串
	for(i=0;i<MAX_PARM;i++)usmart_dev.plentbl[i]=0;//清空参数长度表
	while(*str!='(')//偏移到参数开始的地方
	{
		str++;											    
		if(*str=='\0')return USMART_FUNCERR;//遇到结束符了
	}
	str++;//偏移到"("之后的第一个字节
	while(1)
	{
		i=usmart_get_aparm(str,tstr,&type);	//得到第一个参数  
		str+=i;								//偏移
		switch(type)
		{
			case 0:	//数字
				if(tstr[0]!='\0')				//接收到的参数有效
				{					    
					i=usmart_str2num(tstr,&res);	//记录该参数	 
					if(i)return USMART_PARMERR;		//参数错误.
					*(uint32_t*)(usmart_dev.parm+usmart_get_parmpos(n))=res;//记录转换成功的结果.
					usmart_dev.parmtype&=~(1<<n);	//标记数字
					usmart_dev.plentbl[n]=4;		//该参数的长度为4  
					n++;							//参数增加  
					if(n>MAX_PARM)return USMART_PARMOVER;//参数太多
				}
				break;
			case 1://字符串	 	
				len=usmart_strlen(tstr)+1;	//包含了结束符'\0'
				usmart_strcopy(tstr,&usmart_dev.parm[usmart_get_parmpos(n)]);//拷贝tstr数据到usmart_dev.parm[n]
				usmart_dev.parmtype|=1<<n;	//标记字符串 
				usmart_dev.plentbl[n]=len;	//该参数的长度为len  
				n++;
				if(n>MAX_PARM)return USMART_PARMOVER;//参数太多
				break;
			case 0XFF://错误
				return USMART_PARMERR;//参数错误	  
		}
		if(*str==')'||*str=='\0')break;//查到结束标志了.
	}
	*parn=n;	//记录参数的个数
	return USMART_OK;//正确得到了参数
}


uint32_t my_pow(uint8_t base,uint8_t po)
{
    uint32_t tmp;
    if(po == 0 && base != 0) 
        return 1;
	if(po == 1 || base == 0) 
        return base;
	if(po > 1)
        tmp = base * my_pow(base, po - 1);
	return tmp;
}

void ascii2hex_8bit(uint8_t* ascii, uint8_t* hex, uint8_t ascii_len, uint8_t overchar)
{
    uint8_t i = 0,hex_h,hex_l;
    for(i = 0 ; i < ascii_len ; i += 2)
    {
        if(*(ascii + i) == ' ') //数据填充' '
        {
        }
        if(overchar && (*(ascii + i) == overchar))
            return;
        if(*(ascii + i) <= '9' && *(ascii + i) >= '0')
            hex_h = *(ascii + i) - '0';
        else if(*(ascii + i) <= 'F' && *(ascii + i) >= 'A')
            hex_h = *(ascii + i) - 'A' + 0x0A;
        
        if(*(ascii + i + 1) <= '9' && *(ascii + i + 1) >= '0')
            hex_l = *(ascii + i + 1) - '0';
        else if(*(ascii + i + 1) <= 'F' && *(ascii + i +1) >= 'A')
            hex_l = *(ascii + i + 1) - 'A' + 0x0A;
        
        *hex++ = (hex_h << 4 & 0xF0) | (hex_l & 0x0F);
    }
}

void ascii2hex(uint8_t* ascii, void* hex, uint8_t hex_type, uint16_t ascii_len, uint16_t anasy_method)
{
    uint8_t i,j,hex_tmp[hex_type],method[hex_type],*tmp,*hex_u8 = hex;
    if(anasy_method == 0)
    {
        //使用默认方法解析
        if(hex_type == sizeof(uint8_t))
            anasy_method = 1;
        else if(hex_type == sizeof(uint16_t))
            anasy_method = 21;
        else if(hex_type == sizeof(uint32_t))
            anasy_method = 4321;
        else
            return;
    }
//    for(i = 0 ; i < hex_type ; i++)
//    {
//        method[i] = anasy_method / my_pow(10, hex_type - i - 1);
//        anasy_method = anasy_method % my_pow(10, hex_type - i - 1);
//    }
    for(i = 0 ; i < hex_type ; i++)
    {
        method[i] = anasy_method % my_pow(10, 1);
        anasy_method = anasy_method / my_pow(10, 1);
    }
    for(i = 0 ; i < ascii_len / (hex_type * 2) ; i++)
    {
        //tmp = ((uint8_t*)hex) + i * hex_type;
        tmp = hex_u8 + i * hex_type;
        ascii2hex_8bit(ascii + i * hex_type * 2, hex_tmp, hex_type * 2, NULL);
        for(j = 0 ; j < hex_type ; j++)
        {
            tmp[j] = hex_tmp[method[j] - 1];
        }
    }
}



#include "string.h"

char* skip_mark(char* str, char* str_lim, char mark, int8_t step)
{
    while(1)
    {
		if(*str != mark)
            return str;
		else if(*str == '\0' || str == str_lim)
            return NULL;
		else;
        str += step;
    }
//	while(1)
//    {
//        if(*str == mark)
//        {
//			if(*str == '\0' || str == str_lim)
//				return NULL;
//			str += step;
//            continue;
//        }
//        return str;
//    }
//    return NULL;
}
/*****************************************************************************
 * 函 数 名  : skip_marks
 * 负 责 人  : 吾奶常扇赵子龙
 * 创建日期  : 2021年12月6日
 * 函数功能  : 跳到 str到str_lim或者字符串结束的字符串中-
                   不是marks的位置
 * 输入参数  : char* str  	      字符串首地址
               char* str_lim  字符结束限制地址
               char* marks    需要跳过的符号组成的字符串
               int8_t step    步长：±1
 * 输出参数  : 无
 * 返 回 值  : 跳到的地址
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
char* skip_marks(char* str, char* str_lim, char* marks, int8_t step)
{
	uint8_t i,mark_num;
	mark_num = strlen(marks);
    while(1)
    {
		for(i = 0 ; i < mark_num ; i++)
        {
            if(*str == marks[i])
            {
				if(*str == '\0' || str == str_lim)
					return NULL;
				str += step;
                continue;
            }
        }
        return str;
    }
}


//跳到字符串中指定字符的地址
char* find_mark(char* str, char* str_lim, char mark, int8_t step)
{
    do{
        if(*str == mark)
            return str;
		else if(*str == '\0' || str == str_lim)
            return NULL;
		else;
        str += step;
    }while(1);
}

/*****************************************************************************
 * 函 数 名  : find_marks
 * 负 责 人  : 吾奶常扇赵子龙
 * 创建日期  : 2021年12月2日
 * 函数功能  : 查询字符串中首次出现某些字符的位置
 * 输入参数  : char* str         字符串首地址
               char* marks       需要查询的字符数组
 * 输出参数  : 无
 * 返 回 值  : 查询到的字符位置
 * 调用关系  : 
 * 其    它  : 马新响

*****************************************************************************/
char* find_marks(char* str, char* str_lim, char* marks, int8_t step)
{
    uint8_t i,mark_num;
	mark_num = strlen(marks);
    do{
        for(i = 0 ; i < mark_num ; i++)
        {
            if(*str == marks[i])
                return str;
        }
		if(*str == '\0' || str == str_lim)
            return NULL;
		str += step;
    }while(1);
}

//从某段字符中获取指定字符最后一次出现的位置
char* find_last_mark(char* str_s, char* str_e, char mark)
{
    do{
        if(*str_s == mark)
            return str_s;
		if(!str_e)
		{
			if(*str_s == '\0')
            	break;

		}
		else
		{
			if(++str_s > str_e)
				break;
		}
    }while(1);
    return NULL;
}

/*****************************************************************************
 * 函 数 名  : conver_para_type
 * 负 责 人  : 吾奶常扇赵子龙
 * 创建日期  : 2021年12月6日
 * 函数功能  : 根据声明中参数字符串获取参数类型
 * 输入参数  : char* para_str      参数的首地址
               uint8_t* para_type  存放参数类型
 * 输出参数  : 无
 * 返 回 值  : 0xFF: ERR
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint8_t conver_para_type(char* para_str, uint8_t* para_type)
{
	char* point;
	uint8_t type_tmp;
	__para_type* type;
	if(para_type)
		type = (__para_type*)para_type;
	else
		type = (__para_type*)&type_tmp;
	type->is_point = 0;
	type->type = PARA_VOID;
	point = find_mark(para_str, NULL, '*', 1);
	if(point)
		type->is_point = 1;
	if(memcmp(para_str, "uint8_t", sizeof("uint8_t")) == 0)
		type->type = PARA_UINT8_T;
	else if(memcmp(para_str, "uint16_t", sizeof("uint16_t")) == 0)
		type->type = PARA_UINT16_T;
	else if(memcmp(para_str, "uint32_t", sizeof("uint32_t")) == 0)
		type->type = PARA_UINT32_T;
	else if(memcmp(para_str, "float", sizeof("float")) == 0)
		type->type = PARA_FLOAT;
	else if(memcmp(para_str, "void", sizeof("void")) == 0)
		type->type = PARA_VOID;
	else
		return 0xFF;
	return *(uint8_t*)type;
}

//获取函数返回值类型
/*****************************************************************************
 * 函 数 名  : get_func_ret_type
 * 负 责 人  : 吾奶常扇赵子龙
 * 创建日期  : 2021年12月2日
 * 函数功能  : 获取函数返回值类型
 * 输入参数  : char* str          字符串起始地址
               uint8_t* ret_type  函数返回值类型
 * 输出参数  : 无
 * 返 回 值  : 函数名字符起始地址
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
char* get_func_ret_type(char* str, uint8_t* ret_type)
{
    char *str_soi,*str_eoi,type[8];
	__para_type* ret_info = (__para_type*)ret_type;
	*ret_type = 0;
    //跳过指定符号
    str_soi = skip_mark((char*)str, NULL, ' ', 1);
    //查找返回值类型结束位置
    str_eoi = find_marks(str_soi, NULL, " *", 1);
	//获取返回类型符号
	memcpy(type, str_soi, str_eoi - str_soi);
    //返回值是否为指针
	str_eoi = skip_mark(str_eoi, NULL, ' ', 1);
	if(*str_eoi == '*')
	{
		ret_info->is_point = 1;
		//跳到函数名字位置
		str_eoi = skip_mark(str_eoi, NULL, ' ', 1);
	}
	return str_eoi;
}

//从字符串中获取函数名
//str: 该起始位置应当为返回类型结束位置(get_func_ret_type()的返回地址)
char* get_func_name_1(char* str, char* func_name)
{
    char *str_soi,*str_eoi;
	//跳过空格，到函数名起始位置
	str_soi = skip_mark(str, NULL, ' ', 1);
	//找到函数名结束位置
    str_eoi = find_marks(str_soi, NULL, " (", 1);
    if(!str_eoi)
        return NULL;
    memcpy(func_name, str_soi, str_eoi - str_soi);
	//函数名字符串加上结束标志符号
	func_name[str_eoi - str_soi] = '\0';
	//跳过空格，到函数参数的'('位置
	if(*str_eoi == ' ')
		str_eoi = skip_mark(str_eoi, NULL, ' ', 1);
    return str_eoi;                         //返回查找结束的地址
}

/*****************************************************************************
 * 函 数 名  : get_func_name
 * 负 责 人  : 吾奶常扇赵子龙
 * 创建日期  : 2021年12月6日
 * 函数功能  : 从字符串中提取函数名
 * 输入参数  : char* str        函数声明或者函数调用的字符串
               char* func_name  保存函数名的地址
 * 输出参数  : 无
 * 返 回 值  : 函数名的起始地址
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
char* get_func_name(char* str, char* func_name)
{
    char *str_soi,*str_eoi;
	uint8_t name_len;
	//找到函数名结束位置
    str_eoi = find_mark(str, NULL, '(', 1);
	//跳过函数结束位置后面的空格
	str_eoi = skip_mark(str_eoi, NULL, ' ', -1);
	//查找函数名起始位置
	str_soi = find_marks(str_eoi, str, " *", -1);
	//将str_soi指向函数名起始地址
	if(str_soi == NULL)
		str_soi = str;
	else
		str_soi++;
	name_len = str_eoi - str_soi;
	memcpy(func_name, str_soi, name_len);
	//函数名字符串加上结束标志符号
	func_name[name_len] = '\0';
	//查询失败
	if(str_eoi == str_soi)
		return NULL;
	//返回函数名起始地址
	return str_soi;
}

//定位到函数的第n个参数首地址(n: 1~x)
char* skip_func_para_n(char* str, uint8_t para_n, uint8_t* para_len)
{
	char *str_soi,*str_eoi;
	int8_t depth = 0;
	uint8_t para_num = 0;
	while(1)
	{
		if(*str == '\0')
			return NULL;
		else if(*str == '(' && ++depth == 1)
			str_soi = skip_mark(str + 1, NULL, ' ', 1);			//记录第一个参数的起始位置
		else if(*str == ')' && depth > 1)
			depth--;
		else if((*str == ',' || *str == ')') && depth == 1)		//参数结束位置
		{
			str_eoi = skip_mark(str, NULL, ' ', -1);
			if(str_eoi > str_soi)
				para_num++;
			else if(str_eoi <= str_soi && *str != ')')			//需要该参数，而该参数为空
				return NULL;
			else{}
			if(*str == ',')
				str_soi = skip_mark(str + 1, NULL, ' ', 1);		//记录下一个参数的起始位置
			if(*str == ')')
				depth--;										//最后一个')'
		}
		else{}
		if(para_n == para_num)
		{
			*para_len = str_eoi - str_soi;
			return str_soi;
		}
		str++;
	}
}

/*****************************************************************************
 * 函 数 名  : get_func_para_num
 * 负 责 人  : 吾奶常扇赵子龙
 * 创建日期  : 2021年12月6日
 * 函数功能  : 从函数声明或者函数调用中获取函数的参数个数
 * 输入参数  : char* str          函数声明或者函数调用的字符串
               uint8_t* para_num  保存函数参数个数的变量地址
 * 输出参数  : 无
 * 返 回 值  : 0：获取函数名成功
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint8_t get_func_para_num(char* str, uint8_t* para_num)
{
	char *str_soi,*str_eoi;
	int8_t depth = 0;
	*para_num = 0;
	while(1)
	{
		if(*str == '\0')
		{
			if(depth != 0)
				return 1;										//ERR；"()"使用不对成
			else
				return 0;
		}
		else if(*str == '(' && ++depth == 1)
			str_soi = skip_mark(str + 1, NULL, ' ', 1);			//记录第一个参数的起始位置
		else if(*str == ')' && depth > 1)
			depth--;
		else if((*str == ',' || *str == ')') && depth == 1)		//参数结束位置
		{
			str_eoi = skip_mark(str, NULL, ' ', -1);
			if(str_eoi > str_soi)
				(*para_num)++;
			if(*str == ',')
				str_soi = skip_mark(str + 1, NULL, ' ', 1);		//记录下一个参数的起始位置
			if(*str == ')')
				depth--;										//最后一个')'
		}
		else{}
		str++;
	}
}

/*****************************************************************************
 * 函 数 名  : get_depth_para
 * 负 责 人  : 吾奶常扇赵子龙
 * 创建日期  : 2021年12月6日
 * 函数功能  : 从str地址开始获取第一个参数
 * 输入参数  : char* str          	  字符串首地址
               char** para        记录参数在字符串中的地址,需要修改实参指针指向
               uint8_t* para_len  参数长度
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
char* get_depth_para(char* str, char** para, uint8_t* para_len)

{
	uint8_t depth = 0;
	char *str_soi,*str_eoi;
	str_soi = skip_mark(str, NULL, ' ', 1);
	*para = str_soi;
	while(1)
	{
		//参数个数错误，括号数不对等
		if(*str == '\0')
			return NULL;
		else if(*str == '(')
			depth++;
		else if(depth && *str == ')')
			depth--;
		else if((*str == ',' || *str == ')') && !depth)
		{
			str_eoi = skip_mark(str, NULL, ' ', -1);
			if(para_len)
				*para_len = str_eoi - str_soi;
			break;
		}
		else{}
		str++;
	}
	return str;
}

//根据函数声明信息获取参数类型
char* get_func_para_type(char* str, uint8_t* para_type)
{
    
}

//根据函数调用内容获取参数值
char* get_func_para_val(char* str, void* para_val)
{
    char* str_eoi;
    uint8_t para_type;
    //
    
    str_eoi = get_func_para_type(str, &para_type);
}

/*****************************************************************************
 * 函 数 名  : get_para
 * 负 责 人  : 吾奶常扇赵子龙
 * 创建日期  : 2021年12月6日
 * 函数功能  : 根据函数调用与函数声明的内容获取参数类型
 * 输入参数  : char* str_recv      接收到的字符串中第n个参数的首地址
               char* str_decl      声明中第n个参数的首地址
               void* para_val      保存参数值的地址
               uint8_t* para_type  保存参数类型的地址：参数类型sizeof(变量类型)
               uint8_t para_num    参数个数
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
char* get_para(char* str_recv, char* str_decl, void* para_val, uint8_t* para_type, uint8_t para_num)
{
	char *str_call_para, *str_decl_para, type[10], val[20];
	uint8_t i, ret_stat, call_para_len, decl_para_len;
	for(i = 0 ; i < para_num ; i++)
	{
		str_call_para = skip_func_para_n(str_recv, i, &call_para_len);
		str_decl_para = skip_func_para_n(str_decl, i, &decl_para_len);
		if(str_call_para == NULL || str_decl_para == NULL)
			return NULL;		//ERR
		memcpy(type, str_decl_para, decl_para_len);
		memcpy(val , str_call_para, call_para_len);
		ret_stat = conver_para_type(type, para_type);
		if(ret_stat == 0xFF)
			return NULL;		//ERR
		
	}
}



/*****************************************************************************
 * 函 数 名  : matching_func_name
 * 负 责 人  : 吾奶常扇赵子龙
 * 创建日期  : 2021年12月6日
 * 函数功能  : 调用的函数名与所有已注册的函数名进�-
                   �匹配
 * 输入参数  : char* func_name          接收到的函数名
               uint8_t* matching_index  该地址保存了：所有已注册声明中与给定函数名相同的声明所在的index
 * 输出参数  : 无
 * 返 回 值  : 0：匹配成功
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint8_t matching_func_name(char* func_name, uint8_t* matching_index)
{
	uint8_t index;
    char *str;
	char regi_func_name[20];				//本地已经注册的函数名
    for(index = 0 ; index < sizeof(usmart_nametab) / sizeof(__func_info) ; index++)
    {
		//第一次使用需要从函数声明中获取函数名指针以及函数名长度
        if(usmart_nametab[index].name == NULL)
        {
			str = usmart_nametab[index].declaration;
			//获取函数声明中函数名
			str = get_func_name(str, regi_func_name);
			if(str)
			{
				usmart_nametab[index].name = str;
				usmart_nametab[index].name_len = strlen(regi_func_name);
			}
			//函数声明中格式有误，查询不到函数名，查询下一声明
			else
				continue;
		}
		//函数名长度不同，则跳过进行下一匹配
		if(usmart_nametab[index].name_len != strlen(regi_func_name))
			continue;
		//函数名匹配判断
		if(0 == memcmp(usmart_nametab[index].name, regi_func_name, usmart_nametab[index].name_len))
		{
			*matching_index = index;
			return 0;
		}
    }
	//err, 本地注册的函数中没有指定的函数
	return 1;
}

//响应函数命令
void response_func_cmd(char* str)
{
    char *str_soi,*str_eoi;
	uint8_t	matching_index,ret_type,decl_para_num,recv_para_num;
    char recv_func_name[20];                //接收到的指令中的函数名
    char regi_func_name[20];				//本地已经注册的函数名
    //跳过指定符号
    str_soi = skip_mark(str, NULL, ' ', 1);
    //从接下来的数据中获取函数名
    str_soi = get_func_name(str_soi, recv_func_name);
	if(!str_soi)
		return;								//函数名提取出错
    //查找本地函数名与接收到的函数名相同的函数的index
	if(matching_func_name(recv_func_name, &matching_index))
		return;	
    //获取参数个数
    if(get_func_para_num(str, &decl_para_num) || get_func_para_num(usmart_nametab[matching_index].declaration, &recv_para_num))
    	return;								//参数格式不正确
    //接收到的参数是否与声明的参数匹配
    if(decl_para_num != recv_para_num)
		return;
	//根据index中的函数声明获取函数返回类型
	get_func_ret_type(usmart_nametab[matching_index].declaration, &ret_type);
	//获取参数

	
}












