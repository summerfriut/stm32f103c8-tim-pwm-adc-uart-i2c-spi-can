#include "my_tty.h"
#include <stdio.h>
#include "string.h"



uint16_t tty_info;


void tty_test(uint8_t port, uint16_t data, uint32_t data_len)
{
	data_len += (port + data);
}


//�������б��ʼ��(�û��Լ����)
//�û�ֱ������������Ҫִ�еĺ�����������Ҵ�
__func_info func_info[]=
{
	(void*)tty_test,"void tty_test(uint8_t port, uint16_t data, uint32_t data_len)",
};

__smart_info smart_info=
{
	.funs = func_info,
};   

//ϵͳ����
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
 * �� �� ��  : my_pow
 * �� �� ��  : ���̳���������
 * ��������  : 2021��12��15��
 * ��������  : ��ȡһ��������n����
 * �������  : uint8_t base  �ݺ�������
               uint8_t po    �ݺ�������
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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

//��һ��ascii��ת���ɶ�Ӧ��16����ֵ
uint8_t one_ascii2hex(char ascii, uint8_t* hex)
{
    uint8_t *hex_p,hex_tmp;
    if(hex)
        hex_p = hex;
    else
        hex_p = &hex_tmp;
    if(ascii <= '9' && ascii >= '0')
        *hex_p = ascii - '0';
    else if(ascii <= 'F' && ascii >= 'A')
        *hex_p = ascii - 'A' + 0x0A;
    else if(ascii <= 'f' && ascii >= 'a')
        *hex_p = ascii - 'a' + 0x0A;
    else
        return 0xFF;                //ERR
    return *hex_p;
}

/*****************************************************************************
 * �� �� ��  : get_default_method
 * �� �� ��  : ���̳���������
 * ��������  : 2021��12��17��
 * ��������  : ��ȡasciiת��Ϊĳ�������͵�Ĭ�Ϸ���
 * �������  : uint8_t hex_type  ��������(hex_type: __para_type_enum)
               uint16_t* method  ascii��ת���ɸ��������͵�Ĭ�Ϸ���
 * �������  : ��
 * �� �� ֵ  : Ĭ�Ϸ���
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
uint16_t get_default_method(uint8_t hex_type, uint16_t* method)
{
    uint16_t* method_p, method_tmp;
    if(method)
        method_p = method;
    else
        method_p = &method_tmp;
    switch(hex_type)
    {
        case PARA_VOID:
            *method_p = 0;
            break;
        case PARA_UINT8_T:
            *method_p = 1;
            break;
        case PARA_UINT16_T:
            *method_p = 21;
            break;
        case PARA_UINT32_T:
            *method_p = 4321;
            break;
        case PARA_FLOAT:
            *method_p = 4321;
            break;
        case PARA_STRING:
            *method_p = 1;
            break;
        case PARA_POINT:
            *method_p = 4321;
            break;
        default:
            *method_p = 0x00;
    }
    return *method_p;
}

/*****************************************************************************
 * �� �� ��  : get_type_size
 * �� �� ��  : ���̳���������
 * ��������  : 2021��12��17��
 * ��������  : ���ݱ������ͣ����ص�λ�����͵Ĵ�С
 * �������  : uint8_t hex_type  ��������(hex_type: __para_type_enum)
               uint8_t* size     ��Ÿ��������͵�sizeof()ֵ����ΪNULL
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
uint8_t get_type_size(uint8_t hex_type, uint8_t* size)
{
    uint8_t* size_p, size_tmp;
    if(size)
        size_p = size;
    else
        size_p = &size_tmp;
    switch(hex_type)
    {
        case PARA_VOID:
            *size_p = 0;
            break;
        case PARA_UINT8_T:
            *size_p = 1;
            break;
        case PARA_UINT16_T:
            *size_p = 2;
            break;
        case PARA_UINT32_T:
            *size_p = 4;
            break;
        case PARA_FLOAT:
            *size_p = 4;
            break;
        case PARA_STRING:
            *size_p = 1;
            break;
        case PARA_POINT:
            *size_p = 4;
            break;
        default:
            *size_p = 0x00;
    }
    return *size_p;
}

/*****************************************************************************
 * �� �� ��  : ascii2hex_bytes
 * �� �� ��  : ���̳���������
 * ��������  : 2021��12��17��
 * ��������  : ��ָ�����ȵ�ascii��ת��Ϊhex��Ĭ������ǰ-
                   �油0(�Ӻ���ǰ����)
 * �������  : char* ascii        ascii��ʼ��ַ
               uint8_t* hex       ���ת�����õ�hexֵ���׵�ַ
               uint8_t ascii_len  ascii����ܳ���
               uint8_t overchar   ת�������ַ�
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : ��828FE9892��--->0x08 28 FE 98 92(���ַ����������)

*****************************************************************************/
void ascii2hex_bytes(char* ascii, uint8_t* hex, uint8_t ascii_len, uint8_t overchar)
{
    uint8_t hex_tmp = 0,j = 0,hex_len = (ascii_len + (2 - 1)) / 2;
    int8_t i;
    for(i = ascii_len - 1 ; i >= 0 ; i--)
    {
        if(overchar && (*(ascii + i) == overchar))
            return;
        if(*(ascii + i) <= '9' && *(ascii + i) >= '0')
            hex_tmp = *(ascii + i) - '0';
        else if(*(ascii + i) <= 'F' && *(ascii + i) >= 'A')
            hex_tmp = *(ascii + i) - 'A' + 0x0A;
        else if(*(ascii + i) <= 'f' && *(ascii + i) >= 'a')
            hex_tmp = *(ascii + i) - 'a' + 0x0A;
        else
            return;                                 //�����������ֵģ�ֱ�ӷ���
        if(j == 0)
            hex[hex_len - 1] = 0x00;                //��ֹ�����������ݸ�λ�ֽ�Ϊ�����
        hex[hex_len - 1] &= (~(0x0F << 4 * j));
        hex[hex_len - 1] |= (hex_tmp << 4 * j);
        if(++j == 2)
        {
            j = 0;
            --hex_len;
        }
    }
}

/*****************************************************************************
 * �� �� ��  : hex_str2hex
 * �� �� ��  : ���̳���������
 * ��������  : 2021��12��17��
 * ��������  : ��(��ʾ16������ֵ��ascii��)�ַ�������ָ-
 				   �����������ͽ���������           ������
 * �������  : char* ascii            �ַ����׵�ַ
               void* hex              ���ת������ĵ�ַ
               uint8_t hex_type       ת��Ŀ����������(hex_type: __para_type_enum)
               uint16_t ascii_len     �ַ�������
               uint16_t anasy_method  �������͵Ľ���˳��
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
void hex_str2hex(char* ascii, void* hex, uint8_t hex_type, uint16_t ascii_len, uint16_t anasy_method)
{
    uint8_t i,j,*tmp,*hex_tmp = hex,type_size;
    if(anasy_method == 0)
        anasy_method = get_default_method(hex_type, NULL);
	if(anasy_method == 0)
		return;										//void ERR
    type_size = get_type_size(hex_type, NULL);
    uint8_t hex_arr[type_size],method[type_size];
    for(i = 0 ; i < type_size ; i++)
    {
        method[i] = anasy_method % my_pow(10, 1);
        anasy_method = anasy_method / my_pow(10, 1);
    }
    for(i = 0 ; i < ascii_len / (type_size * 2) ; i++)
    {
		memset(hex_arr, 0x00, type_size);
        tmp = hex_tmp + i * type_size;
        ascii2hex_bytes(ascii + i * type_size * 2, hex_arr, type_size * 2, NULL);
        for(j = 0 ; j < type_size ; j++)
        {
            tmp[j] = hex_arr[method[j] - 1];
        }
    }
}

//����ʾ10������ֵ��ascii��ת���ɶ�Ӧ��Hex
void dec_str2hex(char* ascii, void* hex, uint8_t hex_type, uint16_t ascii_len)
{
    switch(hex_type)
    {
        case PARA_VOID:
            break;
        case PARA_UINT8_T:
            break;
        case PARA_UINT16_T:
            break;
        case PARA_UINT32_T:
            break;
        case PARA_FLOAT:
            break;
        case PARA_STRING:
            break;
        case PARA_POINT:
            break;
        default:
            break;
    }
}

//ascii : 0x�����һ���ֽڵ�ַ
void ascii2hex(char* ascii, void* hex, uint8_t hex_type, int16_t ascii_len, uint16_t anasy_method)
{
    uint8_t i,j,hex_arr[hex_type],method[hex_type],*tmp,*hex_tmp = hex,type_len,offset;
//    ascii += 2;         //������ʼ"0x"
//    ascii_len -= 2;     //ȥ����ʼ�ַ�����
    if(anasy_method == 0)
    {
        //ʹ��Ĭ�Ϸ�������
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
//    for(i = 0 ; i < ascii_len / (hex_type * 2) ; i++)
//    {
//        tmp = hex_tmp + i * hex_type;
//        ascii2hex_bytes(ascii + i * hex_type * 2, hex_arr, hex_type * 2, NULL);
//        for(j = 0 ; j < hex_type ; j++)
//        {
//            tmp[j] = hex_arr[method[j] - 1];
//        }
//    }
    for(i = 0 ; 1 ; i++)
    {
        if(ascii_len < hex_type * 2)
            type_len = ascii_len;
        else
            type_len = hex_type * 2;
        ascii_len -= hex_type * 2;
        tmp = hex_tmp + i * hex_type;
        memset(hex_arr, 0x00, hex_type);
        offset = 0x00;
        if(type_len != hex_type * 2)
            offset = hex_type - (type_len + 1) / 2;
        ascii2hex_bytes(ascii + i * hex_type * 2, &hex_arr[offset], type_len, NULL);
        for(j = 0 ; j < hex_type ; j++)
        {
            tmp[j] = hex_arr[method[j] - 1];
        }
        if(ascii_len <= 0)
            break;
    }
}

//���ַ�����ʮ���Ƶĸ�����ת����floatֵ
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
	//����С����
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
 * �� �� ��  : ascii2float
 * �� �� ��  : ���̳���������
 * ��������  : 2021��11��19��
 * ��������  : ��ascii����float
 * �������  : uint8_t* ascii         asciiԴ��ַ
               float* flo             float���ɵ�ַ
               uint8_t ascii_len      ascii����
               uint16_t anasy_method  float�ֽ���
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : ������

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
 * �� �� ��  : skip_marks
 * �� �� ��  : ���̳���������
 * ��������  : 2021��12��6��
 * ��������  : ���� str��str_lim�����ַ����������ַ�����-
                   ����marks��λ��
 * �������  : char* str  	      �ַ����׵�ַ
               char* str_lim  �ַ��������Ƶ�ַ
               char* marks    ��Ҫ�����ķ�����ɵ��ַ���
               int8_t step    ��������1
 * �������  : ��
 * �� �� ֵ  : �����ĵ�ַ
 * ���ù�ϵ  : 
 * ��    ��  : 

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


//�����ַ�����ָ���ַ��ĵ�ַ
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
 * �� �� ��  : find_marks
 * �� �� ��  : ���̳���������
 * ��������  : 2021��12��2��
 * ��������  : ��ѯ�ַ������״γ���ĳЩ�ַ���λ��
 * �������  : char* str         �ַ����׵�ַ
               char* marks       ��Ҫ��ѯ���ַ�����
 * �������  : ��
 * �� �� ֵ  : ��ѯ�����ַ�λ��
 * ���ù�ϵ  : 
 * ��    ��  : ������

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

//��ĳ���ַ��л�ȡָ���ַ����һ�γ��ֵ�λ��
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
 * �� �� ��  : find_str
 * �� �� ��  : ���̳���������
 * ��������  : 2021��12��6��
 * ��������  : ָ���ַ����в����ַ���
 * �������  : char* str       �ַ����׵�ַ
               char* str_lim   �ַ�������λ��
               char* mark_str  ��Ҫ��ѯ���ַ���
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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
 * �� �� ��  : get_func_ret_type
 * �� �� ��  : ���̳���������
 * ��������  : 2021��12��2��
 * ��������  : ��ȡ��������ֵ����
 * �������  : char* str          �ַ�����ʼ��ַ
               uint8_t* ret_type  ��������ֵ����
 * �������  : ��
 * �� �� ֵ  : �������ַ���ʼ��ַ
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
char* get_func_ret_type(char* str, uint8_t* ret_type)
{
    char *str_soi,*str_eoi,type[8];
	__para_type* ret_info = (__para_type*)ret_type;
	*ret_type = 0;
    //����ָ������
    str_soi = skip_mark((char*)str, NULL, ' ', 1);
    //���ҷ���ֵ���ͽ���λ��
    str_eoi = find_marks(str_soi, NULL, " *", 1);
	//��ȡ�������ͷ���
	memcpy(type, str_soi, str_eoi - str_soi);
    ret_info->type = str2type(type, NULL);
    //����ֵ�Ƿ�Ϊָ��
	str_eoi = skip_mark(str_eoi, NULL, ' ', 1);
	if(*str_eoi == '*')
	{
		ret_info->is_point = 1;
		//������������λ��
		str_eoi = skip_mark(str_eoi, NULL, ' ', 1);
	}
	return str_eoi;
}

//���ַ����л�ȡ������
//str: ����ʼλ��Ӧ��Ϊ�������ͽ���λ��(get_func_ret_type()�ķ��ص�ַ)
char* get_func_name_1(char* str, char* func_name)
{
    char *str_soi,*str_eoi;
	//�����ո񣬵���������ʼλ��
	str_soi = skip_mark(str, NULL, ' ', 1);
	//�ҵ�����������λ��
    str_eoi = find_marks(str_soi, NULL, " (", 1);
    if(!str_eoi)
        return NULL;
    memcpy(func_name, str_soi, str_eoi - str_soi);
	//�������ַ������Ͻ�����־����
	func_name[str_eoi - str_soi] = '\0';
	//�����ո񣬵�����������'('λ��
	if(*str_eoi == ' ')
		str_eoi = skip_mark(str_eoi, NULL, ' ', 1);
    return str_eoi;                         //���ز��ҽ����ĵ�ַ
}

/*****************************************************************************
 * �� �� ��  : get_func_name
 * �� �� ��  : ���̳���������
 * ��������  : 2021��12��6��
 * ��������  : ���ַ�������ȡ������
 * �������  : char* str        �����������ߺ������õ��ַ���
               char* func_name  ���溯�����ĵ�ַ
 * �������  : ��
 * �� �� ֵ  : ����������ʼ��ַ
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
char* get_func_name(char* str, char* func_name)
{
    char *str_soi,*str_eoi;
	uint8_t name_len;
	//�ҵ�����������λ��
    str_eoi = find_mark(str, NULL, '(', 1);
	//������������λ�ú���Ŀո�
	str_eoi = skip_mark(str_eoi, NULL, ' ', -1);
	//���Һ�������ʼλ��
	str_soi = find_marks(str_eoi, str, " *", -1);
	//��str_soiָ��������ʼ��ַ
	if(str_soi == NULL)
		str_soi = str;
	else
		str_soi++;
	name_len = str_eoi - str_soi;
	memcpy(func_name, str_soi, name_len);
	//�������ַ������Ͻ�����־����
	func_name[name_len] = '\0';
	//��ѯʧ��
	if(str_eoi == str_soi)
		return NULL;
	//���غ�������ʼ��ַ
	return str_soi;
}

//��λ�������ĵ�n�������׵�ַ(n: 1~x)
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
//			str_soi = skip_mark(str + 1, NULL, ' ', 1);			//��¼��һ����������ʼλ��
//		else if(*str == ')' && depth > 1)
//			depth--;
//		else if((*str == ',' || *str == ')') && depth == 1)		//��������λ��
//		{
//			str_eoi = skip_mark(str, NULL, ' ', -1);
//			if(str_eoi > str_soi)
//				para_num++;
//			else if(str_eoi <= str_soi && *str != ')')			//��Ҫ�ò��������ò���Ϊ��
//				return NULL;
//			else{}
//			if(*str == ',')
//				str_soi = skip_mark(str + 1, NULL, ' ', 1);		//��¼��һ����������ʼλ��
//			if(*str == ')')
//				depth--;										//���һ��')'
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
                str_soi = skip_mark(str + 1, NULL, ' ', 1);			//��¼��һ����������ʼλ��
                if(!str_soi)
                    return NULL;
            }
        }
		else if(*str == ',' || *str == ')')
		{
            if(depth == 1)                                              //��������λ��
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
                    str_soi = skip_mark(str + 1, NULL, ' ', 1);		//��¼��һ����������ʼλ��
            }
            if(*str == ')')
                --depth;
		}
		else{}
		str++;
	}
}

/*****************************************************************************
 * �� �� ��  : get_func_para_num
 * �� �� ��  : ���̳���������
 * ��������  : 2021��12��6��
 * ��������  : �Ӻ����������ߺ��������л�ȡ�����Ĳ�������
 * �������  : char* str          �����������ߺ������õ��ַ���
               uint8_t* para_num  ���溯�����������ı�����ַ
 * �������  : ��
 * �� �� ֵ  : 0����ȡ�������ɹ�
 * ���ù�ϵ  : 
 * ��    ��  : 

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
				return 1;										        //ERR��"()"ʹ�ò��Գ�
			else
				return 0;
		}
		else if(*str == '(')
        {
            if(++depth == 1)
            {
                str_soi = skip_mark(str + 1, NULL, ' ', 1);			//��¼��һ����������ʼλ��
                if(!str_soi)
                    return 1;
            }
        }
		else if(*str == ',' || *str == ')')
		{
            if(depth == 1)                                              //��������λ��
            {
                str_eoi = skip_mark(str, NULL, ' ', -1);
                if(str_eoi > str_soi)
                    (*para_num)++;
                if(*str == ',')
                    str_soi = skip_mark(str + 1, NULL, ' ', 1);		//��¼��һ����������ʼλ��
            }
            if(*str == ')')
                --depth;
		}
		else{}
		str++;
	}
}

/*****************************************************************************
 * �� �� ��  : get_depth_para
 * �� �� ��  : ���̳���������
 * ��������  : 2021��12��6��
 * ��������  : ��str��ַ��ʼ��ȡ��һ������
 * �������  : char* str          	  �ַ����׵�ַ
               char** para        ��¼�������ַ����еĵ�ַ,��Ҫ�޸�ʵ��ָ��ָ��
               uint8_t* para_len  ��������
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
char* get_depth_para(char* str, char** para, uint8_t* para_len)
{
	uint8_t depth = 0;
	char *str_soi,*str_eoi;
	str_soi = skip_mark(str, NULL, ' ', 1);
	*para = str_soi;
	while(1)
	{
		//���������������������Ե�
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
 * �� �� ��  : get_func_para_type
 * �� �� ��  : ���̳���������
 * ��������  : 2021��12��6��
 * ��������  : ���������в����ַ�����ȡ��������
 * �������  : char* str           �������׵�ַ
               uint8_t* para_type  ��Ų�������
 * �������  : ��
 * �� �� ֵ  : 0xFF: ERR
 * ���ù�ϵ  : 
 * ��    ��  : 

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


//���ݺ����������ݻ�ȡ����ֵ
/*****************************************************************************
 * �� �� ��  : get_func_para_val
 * �� �� ��  : ���̳���������
 * ��������  : 2021��12��13��
 * ��������  : ���ݲ����ַ������ݻ�ȡ����ֵ
 * �������  : char* str        �����ַ�����ʼ��ַ
               uint8_t str_len  �����ַ�������
               void* para_val   ��Ų���ֵ����ʼ��ַ
               uint8_t* type    ��������
 * �������  : ��
 * �� �� ֵ  : ��ŵĲ����Ľ�����ַ
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
char* get_func_para_val(char* str, uint8_t str_len, void* para_val, __para_info* info)
{
    __para_type type_tmp = info->para_type;
	char *str_soi,*str_eoi,*tmp = para_val;
    //
    if(type_tmp.is_point)
    {
        //"" : ��ʾ�ַ���
        //() : ��ʾ�Ӻ�����������
        //[] : ��ʾ�������鲿��
        //{} : ��ʾ���������ֵ����
        str_soi = find_marks(str, NULL, "\"\'", 1);
        //�������������ָ�룬�������Ϊ�������Ű�Χ�����ָ��ָ�������Ϊ�����ڵ�����
        if(str_soi)
        {
            str_soi += 1;
            str_eoi = find_marks(str_soi, NULL, "\"\'", 1);
            memcpy(tmp, str_soi, str_eoi - str_eoi);
            return (tmp + (str_eoi - str_eoi));
        }
        //�������������ָ�룬�������Ϊ16���Ƶĵ�ַascii�룬���ָ��ָ��õ�ַ
        else
        {
        }
        info->para_len = 0;
    }
	else
        info->para_len = 1;
	if(find_str(str, str + str_len, "0x"))										//���������еĲ���Ϊ16����
	{
        str += 2;         //������ʼ"0x"
        str_len -= 2;     //ȥ����ʼ�ַ�����
		switch(type_tmp.type)
		{
			case PARA_UINT8_T:
				ascii2hex(str, tmp, sizeof(uint8_t), str_len, 1);
				return tmp + sizeof(uint8_t);
			case PARA_UINT16_T:
				ascii2hex(str, tmp, sizeof(uint16_t), str_len, 12);
				return tmp + sizeof(uint16_t);
			case PARA_UINT32_T:
				ascii2hex(str, tmp, sizeof(uint32_t), str_len, 1234);
				return tmp + sizeof(uint32_t);
			case PARA_FLOAT:
                ascii2float(str, (float*)tmp, str_len, 1234);
				return tmp + sizeof(float);
			case PARA_VOID:
				return tmp;
		}
	}
	else
	{
		switch(type_tmp.type)
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
                *(float*)tmp = str2float(str, str_len, NULL);
                return tmp + sizeof(float);
			case PARA_VOID:
				return tmp;
		}
	}
	return NULL;
}

/*****************************************************************************
 * �� �� ��  : get_para
 * �� �� ��  : ���̳���������
 * ��������  : 2021��12��6��
 * ��������  : ���ݺ��������뺯�����������ݻ�ȡ��������
 * �������  : char* str_recv      ���õ��ַ����׵�ַ
               char* str_decl      �������׵�ַ
               void* para          �������в���ֵ�ĵ�ַ
               __para_info* info   ������Ϣ�����ַ		   
               uint8_t para_num	   ������������
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
char* get_para(char* str_recv, char* str_decl, void* para, __para_info* info, uint8_t para_num)
{
	char *str_call_para, *str_decl_para, type_ascii[10], val_ascii[20];
	uint8_t i, ret_stat, call_para_len, decl_para_len;
	for(i = 1 ; i <= para_num ; i++)
	{
		//��ȡ���������еĲ�����ʼ��ַ����������
		str_call_para = skip_func_para_n(str_recv, i, &call_para_len);
		//��ȡ���������еĲ�����ʼ��ַ����������
		str_decl_para = skip_func_para_n(str_decl, i, &decl_para_len);
		if(str_call_para == NULL || str_decl_para == NULL)
			return NULL;		//ERR
        if(!decl_para_len || !call_para_len)
           return NULL;
		//��ȡ�����ַ���
		memcpy(type_ascii, str_decl_para, decl_para_len);
		//��ȡ��ֵ�ַ���
		memcpy(val_ascii , str_call_para, call_para_len);
		//���������е������ַ�����ȡ��������
		ret_stat = get_func_para_type(type_ascii, (uint8_t*)&info[i - 1].para_type);
		if(ret_stat == 0xFF)
			return NULL;		//ERR
		//���ݵ����е���ֵ�ַ�����ȡʵ��ֵ
		info[i - 1].para_val_adr = para;
		para = get_func_para_val(val_ascii, call_para_len, para, &info[i - 1]);
		if(!para)
			return NULL;
	}
	return para;
}



/*****************************************************************************
 * �� �� ��  : matching_func_name
 * �� �� ��  : ���̳���������
 * ��������  : 2021��12��6��
 * ��������  : ���õĺ�������������ע��ĺ��������1�7-
                   �1�7ƥ��
 * �������  : char* func_name          ���յ��ĺ�����
               uint8_t* matching_index  �õ�ַ�����ˣ�������ע���������������������ͬ���������ڵ�index
 * �������  : ��
 * �� �� ֵ  : 0��ƥ��ɹ�
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
uint8_t matching_func_name(char* func_name, uint8_t* matching_index)
{
	uint8_t index;
    char *str;
	char regi_func_name[20];				//�����Ѿ�ע��ĺ�����
    for(index = 0 ; index < sizeof(func_info) / sizeof(__func_info) ; index++)
    {
		//��һ��ʹ����Ҫ�Ӻ��������л�ȡ������ָ���Լ�����������
        if(func_info[index].name == NULL)
        {
			str = func_info[index].declaration;
			//��ȡ���������к�����
			str = get_func_name(str, regi_func_name);
			if(str)
			{
				func_info[index].name = str;
				func_info[index].name_len = strlen(regi_func_name);
			}
			//���������и�ʽ���󣬲�ѯ��������������ѯ��һ����
			else
				continue;
		}
		//���������Ȳ�ͬ��������������һƥ��
		if(func_info[index].name_len != strlen(regi_func_name))
			continue;
		//������ƥ���ж�
		if(0 == memcmp(func_info[index].name, regi_func_name, func_info[index].name_len))
		{
			*matching_index = index;
			return 0;
		}
    }
	//err, ����ע��ĺ�����û��ָ���ĺ���
	return 1;
}

//�Ӳ�������buf�����ݲ������ͼ������ݵ���������
void extract_para(__para_info* para_info, uint8_t para_num, __para_type_union* para)
{
	uint8_t i;
	for(i = 0 ; i < para_num ; i++)
	{
        para[i].uint32bit = 0x00;
		if(para_info[i].para_type.is_point)
		{
            para[i].addr_point = (void*)*(uint32_t*)para_info[i].para_val_adr;
            continue;
		}
		switch(para_info[i].para_type.type)
		{
            case PARA_VOID:
                break;
            case PARA_UINT8_T:
                para[i].uint8bit = *(uint8_t*)para_info[i].para_val_adr;
                break;
            case PARA_UINT16_T:
                para[i].uint16bit = *(uint16_t*)para_info[i].para_val_adr;
                break;
            case PARA_UINT32_T:
                para[i].uint32bit = *(uint32_t*)para_info[i].para_val_adr;
                break;
            case PARA_FLOAT:
                para[i].float32bit = *(float*)para_info[i].para_val_adr;
                break;
            case PARA_STRING:
                para[i].string = (char*)para_info[i].para_val_adr;
                break;
            case PARA_POINT:
                para[i].addr_point = (void*)*(uint32_t*)para_info[i].para_val_adr;
                break;
            default:
                break;
		}
	}
}

//���ú���
void run_call_func(uint8_t func_index, uint8_t ret_type, __para_info* para_info, uint8_t para_num, uint8_t* para_val)
{
    void* func_tmp = func_info[func_index].func;
	__para_type_union para_val_tmp[para_num];
    extract_para(para_info, para_num, para_val_tmp);
	switch(para_num)
	{
		case 0:
			(*(uint32_t(*)())func_tmp)();
			break;
		case 1:
			(*(uint32_t(*)())func_tmp)(para_val_tmp[0]);
			break;
		case 2:
			(*(uint32_t(*)())func_tmp)(para_val_tmp[0], para_val_tmp[1]);
			break;
		case 3:
			(*(uint32_t(*)())func_tmp)(para_val_tmp[0], para_val_tmp[1], para_val_tmp[2]);
			break;
		case 4:
			(*(uint32_t(*)())func_tmp)(para_val_tmp[0], para_val_tmp[1], para_val_tmp[2], para_val_tmp[3]);
			break;
		default:
			break;
	}
}

uint8_t call_para[20];
__para_info para_info[5];
//��Ӧ��������
void response_func_cmd(char* str)
{
    char *str_soi;
	uint8_t	matching_index,ret_type,decl_para_num,recv_para_num;
    char recv_func_name[20];                //���յ���ָ���еĺ�����
    //����ָ������
    str_soi = skip_mark(str, NULL, ' ', 1);
    //�ӽ������������л�ȡ������
    str_soi = get_func_name(str_soi, recv_func_name);
	if(!str_soi)
		return;								//��������ȡ����
    //���ұ��غ���������յ��ĺ�������ͬ�ĺ�����index
	if(matching_func_name(recv_func_name, &matching_index))
		return;	
    //��ȡ��������
    if(get_func_para_num(str, &decl_para_num) || get_func_para_num(func_info[matching_index].declaration, &recv_para_num))
    	return;								//������ʽ����ȷ
    //���յ��Ĳ����Ƿ��������Ĳ���ƥ��
    if(decl_para_num != recv_para_num)
		return;
	//����index�еĺ���������ȡ������������
	get_func_ret_type(func_info[matching_index].declaration, &ret_type);
	//��ȡ����
    get_para(str, func_info[matching_index].declaration, call_para, para_info, decl_para_num);
    //���к���
    run_call_func(matching_index, ret_type, para_info, decl_para_num, call_para);
}












