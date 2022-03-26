#include "my_tty.h"
#include <stdio.h>
#include "string.h"



uint16_t tty_info;


void tty_test(uint8_t port, uint16_t data, uint32_t data_len)
{
	data_len += (port + data);
}


//函数名列表初始化(用户自己添加)
//用户直接在这里输入要执行的函数名及其查找串
__func_info func_info[]=
{
	(void*)tty_test,"void tty_test(uint8_t port, uint16_t data, uint32_t data_len)",
};

__smart_info smart_info=
{
	.funs = func_info,
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

/*****************************************************************************
 * 函 数 名  : my_pow
 * 负 责 人  : 吾奶常扇赵子龙
 * 创建日期  : 2021年12月15日
 * 函数功能  : 获取一个整数的n次幂
 * 输入参数  : uint8_t base  幂函数底数
               uint8_t po    幂函数次幂
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
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

//将表示16进制数值的ascii码转换成对应的hex
void hex_ascii2hex(char* ascii, void* hex, uint8_t hex_type, uint16_t ascii_len, uint16_t anasy_method)
{

}

//将表示10进制数值的ascii码转换成对应的Hex
void dec_ascii2hex(char* ascii, void* hex, uint8_t hex_type, uint16_t ascii_len)
{
	
}

void ascii2hex_8bit(char* ascii, uint8_t* hex, uint8_t ascii_len, uint8_t overchar)
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

void ascii2hex(char* ascii, void* hex, uint8_t hex_type, uint16_t ascii_len, uint16_t anasy_method)
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


//将字符串中十进制的浮点数转换成float值
float str2float(char* str, uint8_t str_len, void* val)
{
	uint8_t i = 0, j = 0;
	float tmp_int = 0,tmp_dec = 0;
	do{
		if(*(str + i) == '.')
			break;
		tmp_int *= 10;
		tmp_int += *(str + i) - '0';
	}while(i++ < str_len);
	//跳过小数点
	++i;
	do{
		tmp_dec += (*(str + i) - '0') / my_pow(10, j);
		j++;
	}while(i++ < str_len);
	if(val)
		*(float*)val = tmp_int + tmp_dec;
    return tmp_int + tmp_dec;
}

uint32_t str2int(char* str, uint8_t str_len, void* val)
{
	uint8_t i = 0;
	uint32_t tmp = 0;
	do{
		tmp *= 10;
		tmp += (*(str + i) - '0');
	}while(++i < str_len);
	if(val)
		*(uint32_t*)val = tmp;
	return tmp;
}

/*****************************************************************************
 * 函 数 名  : ascii2float
 * 负 责 人  : 吾奶常扇赵子龙
 * 创建日期  : 2021年11月19日
 * 函数功能  : 将ascii生成float
 * 输入参数  : uint8_t* ascii         ascii源地址
               float* flo             float生成地址
               uint8_t ascii_len      ascii长度
               uint16_t anasy_method  float字节序
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 马新响

*****************************************************************************/
void ascii2float(char* ascii, float* flo, uint8_t ascii_len, uint16_t anasy_method)
{
    const uint8_t space[8] = {0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
    uint8_t i,j,hex[4],method[4],*tmp;
    for(i = 0 ; i < 4 ; i++)
    {
        method[i] = anasy_method / my_pow(10, 4 - i - 1);
        anasy_method = anasy_method % my_pow(10, 4 - i - 1);
    }
//    for(i = 0 ; i < 4 ; i++)
//    {
//        method[i] = anasy_method % my_pow(10, 1);
//        anasy_method = anasy_method / my_pow(10, 1);
//    }
    for(i = 0 ; i < ascii_len / 8 ; i++)
    {
        if(0 == memcmp(space, ascii + i * 8, 8))
            continue;
        tmp = (uint8_t*)(flo + i);
        ascii2hex(ascii + i * 8, hex, sizeof(uint8_t), 8, 1);
        for(j = 0 ; j < 4 ; j++)
        {
            tmp[j] = hex[method[j] - 1];
        }
    }
}

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
 * 函 数 名  : find_str
 * 负 责 人  : 吾奶常扇赵子龙
 * 创建日期  : 2021年12月6日
 * 函数功能  : 指定字符串中查找字符串
 * 输入参数  : char* str       字符串首地址
               char* str_lim   字符串结束位置
               char* mark_str  需要查询的字符串
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
char* find_str(char* str, char* str_lim, char* mark_str)
{
	uint8_t i,mark_num;
	mark_num = strlen(mark_str);
	if(str_lim && (str_lim - str < mark_num))
		return NULL;
    do{
		if(*str == '\0' || (str_lim && (str_lim - str < mark_num)))
            return NULL;
        for(i = 0 ; i < mark_num ; i++)
        {
            if(*(str + i) != mark_str[i])
                break;
        }
		if(i == mark_num)
			return str;
		str += mark_num;
    }while(1);
}

uint8_t str2type(char* str, uint8_t* para_type)
{
    uint8_t type_tmp;
    __para_type* type;
    if(para_type)
        type = (__para_type*)para_type;
    else
        type = (__para_type*)&type_tmp;
    
    if(memcmp("uint8_t", str, strlen("uint8_t")) == 0)
		type->type = PARA_UINT8_T;
	else if(memcmp("uint16_t", str, strlen("uint16_t")) == 0)
		type->type = PARA_UINT16_T;
	else if(memcmp("uint32_t", str, strlen("uint32_t")) == 0)
		type->type = PARA_UINT32_T;
	else if(memcmp("float", str, strlen("float")) == 0)
		type->type = PARA_FLOAT;
	else if(memcmp("void", str, strlen("void")) == 0)
		type->type = PARA_VOID;
	else
		return 0xFF;
    return type->type;
}

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
    ret_info->type = str2type(type, NULL);
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
//	while(1)
//	{
//		if(*str == '\0')
//			return NULL;
//		else if(*str == '(' && ++depth == 1)
//			str_soi = skip_mark(str + 1, NULL, ' ', 1);			//记录第一个参数的起始位置
//		else if(*str == ')' && depth > 1)
//			depth--;
//		else if((*str == ',' || *str == ')') && depth == 1)		//参数结束位置
//		{
//			str_eoi = skip_mark(str, NULL, ' ', -1);
//			if(str_eoi > str_soi)
//				para_num++;
//			else if(str_eoi <= str_soi && *str != ')')			//需要该参数，而该参数为空
//				return NULL;
//			else{}
//			if(*str == ',')
//				str_soi = skip_mark(str + 1, NULL, ' ', 1);		//记录下一个参数的起始位置
//			if(*str == ')')
//				depth--;										//最后一个')'
//		}
//		else{}
//		if(para_n == para_num)
//		{
//			*para_len = str_eoi - str_soi;
//			return str_soi;
//		}
//		str++;
//	}
    while(1)
	{
		if(*str == '\0')
            return NULL;
		else if(*str == '(')
        {
            if(++depth == 1)
            {
                str_soi = skip_mark(str + 1, NULL, ' ', 1);			//记录第一个参数的起始位置
                if(!str_soi)
                    return NULL;
            }
        }
		else if(*str == ',' || *str == ')')
		{
            if(depth == 1)                                              //参数结束位置
            {
                str_eoi = skip_mark(str, NULL, ' ', -1);
                if(str_eoi > str_soi)
                {
                    if(++para_num == para_n)
                    {
                        *para_len = str_eoi - str_soi;
                        return str_soi;
                    }
                }
                if(*str == ',')
                    str_soi = skip_mark(str + 1, NULL, ' ', 1);		//记录下一个参数的起始位置
            }
            if(*str == ')')
                --depth;
		}
		else{}
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
//	while(1)
//	{
//		if(*str == '\0')
//		{
//			if(depth != 0)
//				return 1;										//ERR；"()"使用不对成
//			else
//				return 0;
//		}
//		else if(*str == '(' && ++depth == 1)
//			str_soi = skip_mark(str + 1, NULL, ' ', 1);			//记录第一个参数的起始位置
//		else if(*str == ')' && depth > 1)
//			depth--;
//		else if((*str == ',' || *str == ')') && depth == 1)		//参数结束位置
//		{
//			str_eoi = skip_mark(str, NULL, ' ', -1);
//			if(str_eoi > str_soi)
//				(*para_num)++;
//			if(*str == ',')
//				str_soi = skip_mark(str + 1, NULL, ' ', 1);		//记录下一个参数的起始位置
//			if(*str == ')')
//				depth--;										    //最后一个')'
//		}
//		else
//        {}
//		str++;
//	}
    while(1)
	{
		if(*str == '\0')
		{
			if(depth != 0)
				return 1;										        //ERR；"()"使用不对成
			else
				return 0;
		}
		else if(*str == '(')
        {
            if(++depth == 1)
            {
                str_soi = skip_mark(str + 1, NULL, ' ', 1);			//记录第一个参数的起始位置
                if(!str_soi)
                    return 1;
            }
        }
		else if(*str == ',' || *str == ')')
		{
            if(depth == 1)                                              //参数结束位置
            {
                str_eoi = skip_mark(str, NULL, ' ', -1);
                if(str_eoi > str_soi)
                    (*para_num)++;
                if(*str == ',')
                    str_soi = skip_mark(str + 1, NULL, ' ', 1);		//记录下一个参数的起始位置
            }
            if(*str == ')')
                --depth;
		}
		else
        {}
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

/*****************************************************************************
 * 函 数 名  : get_func_para_type
 * 负 责 人  : 吾奶常扇赵子龙
 * 创建日期  : 2021年12月6日
 * 函数功能  : 根据声明中参数字符串获取参数类型
 * 输入参数  : char* str           参数的首地址
               uint8_t* para_type  存放参数类型
 * 输出参数  : 无
 * 返 回 值  : 0xFF: ERR
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint8_t get_func_para_type(char* str, uint8_t* para_type)
{
	char* point;
	uint8_t type_tmp;
	__para_type* type;
	if(para_type)
		type = (__para_type*)para_type;
	else
		type = (__para_type*)&type_tmp;
	type->is_point = 0;
	point = find_mark(str, NULL, '*', 1);
	if(point)
		type->is_point = 1;
    type->type = str2type(str, NULL);
	return *(uint8_t*)type;
}

uint8_t char2bcd(char ascii, uint8_t* val)
{
	if(val)
		*val = ascii - '0';
	return (ascii - '0');
}


//根据函数调用内容获取参数值
/*****************************************************************************
 * 函 数 名  : get_func_para_val
 * 负 责 人  : 吾奶常扇赵子龙
 * 创建日期  : 2021年12月13日
 * 函数功能  : 根据参数字符串内容获取参数值
 * 输入参数  : char* str        参数字符串起始地址
               uint8_t str_len  参数字符串长度
               void* para_val   存放参数值的起始地址
               uint8_t* type    参数类型
 * 输出参数  : 无
 * 返 回 值  : 存放的参数的结束地址
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
char* get_func_para_val(char* str, uint8_t str_len, void* para_val, uint8_t* type)
{
    uint8_t type_tmp = *type;
	char *str_soi,*str_eoi,*tmp = para_val;
	str_soi = find_marks(str, NULL, "\"\'", 1);
	if(str_soi)									//调用参数为字符串
	{
		str_soi += 1;
		str_eoi = find_marks(str_soi, NULL, "\"\'", 1);
		memcpy(tmp, str_soi, str_eoi - str_eoi);
		return (tmp + (str_eoi - str_eoi));
	}
    if(type_tmp & 0x80)
        type_tmp |= PARA_UINT32_T;
	//将ASCII码转换成十进制数字
	if(((type_tmp & 0x7F) == PARA_FLOAT) && find_mark(str, str + str_len, '.', 1))		//十进制表示的float
	{
        *(float*)tmp = str2float(str, str_len, NULL);
		return tmp + sizeof(float);
	}
	else if(find_str(str, str + str_len, "0x"))										//函数调用中的参数为16进制
	{
        str += 2;
		switch(type_tmp & 0x7F)
		{
			case PARA_UINT8_T:
				ascii2hex(str, tmp, sizeof(uint8_t), 2, 1);
				return tmp + sizeof(uint8_t);
			case PARA_UINT16_T:
				ascii2hex(str, tmp, sizeof(uint16_t), 4, 12);
				return tmp + sizeof(uint16_t);
			case PARA_UINT32_T:
				ascii2hex(str, tmp, sizeof(uint32_t), 8, 1234);
				return tmp + sizeof(uint32_t);
			case PARA_FLOAT:
				return tmp + sizeof(float);
			case PARA_VOID:
				return tmp;
		}
	}
	else
	{
		switch(type_tmp & 0x7F)
		{
			case PARA_UINT8_T:
                *(uint8_t*)tmp = str2int(str, str_len, NULL);
				return tmp + sizeof(uint8_t);
			case PARA_UINT16_T:
                *(uint16_t*)tmp = str2int(str, str_len, NULL);
				return tmp + sizeof(uint16_t);
			case PARA_UINT32_T:
                *(uint32_t*)tmp = str2int(str, str_len, NULL);
				return tmp + sizeof(uint32_t);
			case PARA_FLOAT:
                ascii2float(str, (float*)tmp, 1 * 8, 1234);
				return tmp + sizeof(float);
			case PARA_VOID:
				return tmp;
		}
	}
	return NULL;
}

/*****************************************************************************
 * 函 数 名  : get_para
 * 负 责 人  : 吾奶常扇赵子龙
 * 创建日期  : 2021年12月6日
 * 函数功能  : 根据函数调用与函数声明的内容获取参数类型
 * 输入参数  : char* str_recv      调用的字符串首地址
               char* str_decl      声明的首地址
               void* para          保存所有参数值的地址
               __para_info* info   参数信息保存地址		   
               uint8_t para_num	   函数函数个数
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
char* get_para(char* str_recv, char* str_decl, void* para, __para_info* info, uint8_t para_num)
{
	char *str_call_para, *str_decl_para, type_ascii[10], val_ascii[20];
	uint8_t i, ret_stat, call_para_len, decl_para_len;
	for(i = 1 ; i <= para_num ; i++)
	{
		//获取函数调用中的参数起始地址、参数长度
		str_call_para = skip_func_para_n(str_recv, i, &call_para_len);
		//获取函数声明中的参数起始地址、参数长度
		str_decl_para = skip_func_para_n(str_decl, i, &decl_para_len);
		if(str_call_para == NULL || str_decl_para == NULL)
			return NULL;		//ERR
        if(!decl_para_len || !call_para_len)
           return NULL;
		//读取类型字符串
		memcpy(type_ascii, str_decl_para, decl_para_len);
		//读取数值字符串
		memcpy(val_ascii , str_call_para, call_para_len);
		//根据声明中的类型字符串获取参数类型
		ret_stat = get_func_para_type(type_ascii, (uint8_t*)&info[i - 1].para_type);
		if(ret_stat == 0xFF)
			return NULL;		//ERR
		//根据调用中的数值字符串获取实际值
		info[i - 1].para_val_adr = para;
		para = get_func_para_val(val_ascii, call_para_len, para, (uint8_t*)&info[i - 1].para_type);
		if(!para)
			return NULL;
	}
	return para;
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
    for(index = 0 ; index < sizeof(func_info) / sizeof(__func_info) ; index++)
    {
		//第一次使用需要从函数声明中获取函数名指针以及函数名长度
        if(func_info[index].name == NULL)
        {
			str = func_info[index].declaration;
			//获取函数声明中函数名
			str = get_func_name(str, regi_func_name);
			if(str)
			{
				func_info[index].name = str;
				func_info[index].name_len = strlen(regi_func_name);
			}
			//函数声明中格式有误，查询不到函数名，查询下一声明
			else
				continue;
		}
		//函数名长度不同，则跳过进行下一匹配
		if(func_info[index].name_len != strlen(regi_func_name))
			continue;
		//函数名匹配判断
		if(0 == memcmp(func_info[index].name, regi_func_name, func_info[index].name_len))
		{
			*matching_index = index;
			return 0;
		}
    }
	//err, 本地注册的函数中没有指定的函数
	return 1;
}

//调用函数
void run_call_func(uint8_t func_index, __para_info para, uint8_t para_num, uint8_t* para_val)
{
}


uint8_t call_para[20];
__para_info para_info[5];
//响应函数命令
void response_func_cmd(char* str)
{
    char *str_soi;
	uint8_t	matching_index,ret_type,decl_para_num,recv_para_num;
    char recv_func_name[20];                //接收到的指令中的函数名
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
    if(get_func_para_num(str, &decl_para_num) || get_func_para_num(func_info[matching_index].declaration, &recv_para_num))
    	return;								//参数格式不正确
    //接收到的参数是否与声明的参数匹配
    if(decl_para_num != recv_para_num)
		return;
	//根据index中的函数声明获取函数返回类型
	get_func_ret_type(func_info[matching_index].declaration, &ret_type);
	//获取参数
    get_para(str, func_info[matching_index].declaration, call_para, para_info, decl_para_num);
    //运行函数
    
}












