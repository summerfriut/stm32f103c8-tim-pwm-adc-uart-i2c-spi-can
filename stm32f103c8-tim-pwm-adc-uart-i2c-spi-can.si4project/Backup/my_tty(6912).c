#include "my_tty.h"
#include <stdio.h>


uint16_t tty_info;

//�������б��ʼ��(�û��Լ����)
//�û�ֱ������������Ҫִ�еĺ�����������Ҵ�
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
	.fnum = sizeof(usmart_nametab)/sizeof(__func_info),//��������
	.pnum = 0,	  	//��������
	.id = 0,	 	//����ID
	.sptype = 1,		//������ʾ����,0,10����;1,16����
	.parmtype = 0,		//��������.bitx:,0,����;1,�ַ���	    
	.plentbl = 0,	  	//ÿ�������ĳ����ݴ��,��ҪMAX_PARM��0��ʼ��
	.parm = 0,		//�����Ĳ���,��ҪPARM_LEN��0��ʼ��
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
//����ϵͳָ��
//0,�ɹ�����;����,�������;
uint8_t usmart_sys_cmd_exe(uint8_t *str)
{
	uint8_t i;
	uint8_t sfname[MAX_FNAME_LEN];//��ű��غ�����
	uint8_t pnum;
	uint8_t rval;
	uint32_t res;  
	res = usmart_get_cmdname(str,sfname, &i, MAX_FNAME_LEN);//�õ�ָ�ָ���
	if(res)
        return USMART_FUNCERR;//�����ָ�� 
	str += i;
	for(i = 0 ; i < sizeof(sys_cmd_tab) / 4 ; i++)//֧�ֵ�ϵͳָ��
	{
		if(usmart_strcmp(sfname,sys_cmd_tab[i]) == 0)
            break;
	}
	switch(i)
	{
		case 0:
		case 1://����ָ��
			printf("\r\n");
#if USMART_USE_HELP 
			printf("------------------------USMART V3.3------------------------ \r\n");
			printf("    USMART����ALIENTEK������һ�����ɵĴ��ڵ��Ի������,ͨ�� \r\n");
			printf("��,�����ͨ���������ֵ��ó���������κκ���,��ִ��.���,���\r\n");
			printf("��������ĺ������������(֧������(10/16����,֧�ָ���)���ַ���\r\n"),
			printf("��������ڵ�ַ����Ϊ����),�����������֧��10���������,��֧��\r\n"),  
			printf("��������ֵ��ʾ.֧�ֲ�����ʾ�������ù���,֧�ֽ���ת������.\r\n");
			printf("����֧��:www.openedv.com\r\n");
			printf("USMART��7��ϵͳ����(����Сд):\r\n");
			printf("?:      ��ȡ������Ϣ\r\n");
			printf("help:   ��ȡ������Ϣ\r\n");
			printf("list:   ���õĺ����б�\r\n\n");
			printf("id:     ���ú�����ID�б�\r\n\n");
			printf("hex:    ����16������ʾ,����ո�+���ּ�ִ�н���ת��\r\n\n");
			printf("dec:    ����10������ʾ,����ո�+���ּ�ִ�н���ת��\r\n\n");
			printf("runtime:1,�����������м�ʱ;0,�رպ������м�ʱ;\r\n\n");
			printf("�밴�ճ����д��ʽ���뺯�������������Իس�������.\r\n");    
			printf("--------------------------ALIENTEK------------------------- \r\n");
#else
			printf("ָ��ʧЧ\r\n");
#endif
			break;
		case 2://��ѯָ��
			printf("\r\n");
			printf("-------------------------�����嵥--------------------------- \r\n");
			for(i=0;i<usmart_dev.fnum;i++)
                printf("%s\r\n",usmart_dev.funs[i].name);
			printf("\r\n");
			break;	 
		case 3://��ѯID
			printf("\r\n");
			printf("-------------------------���� ID --------------------------- \r\n");
			for(i=0;i<usmart_dev.fnum;i++)
			{
				usmart_get_fname((uint8_t*)usmart_dev.funs[i].name,sfname,&pnum,&rval);//�õ����غ����� 
				printf("%s id is:\r\n0X%08X\r\n",sfname,(uint32_t)usmart_dev.funs[i].func); //��ʾID
			}
			printf("\r\n");
			break;
		case 4://hexָ��
			printf("\r\n");
			usmart_get_aparm(str,sfname,&i);
			if(i==0)//��������
			{
				i = usmart_str2num(sfname,&res);	   	//��¼�ò���	
				if(i==0)						  	//����ת������
					printf("HEX:0X%X\r\n",res);	   	//תΪ16����
                else if(i!=4)
                    return USMART_PARMERR;//��������.
				else 				   				//������ʾ�趨����
				{
					printf("16���Ʋ�����ʾ!\r\n");
					usmart_dev.sptype=SP_TYPE_HEX;  
				}
			}
            else
                return USMART_PARMERR;			//��������.
			printf("\r\n"); 
			break;
		case 5://decָ��
			printf("\r\n");
			usmart_get_aparm(str,sfname,&i);
			if(i==0)//��������
			{
				i=usmart_str2num(sfname,&res);	   	//��¼�ò���	
				if(i==0)						   	//����ת������
					printf("DEC:%u\r\n",res);	   	//תΪ10����
				else if(i!=4)
                    return USMART_PARMERR;//��������.
				else 				   				//������ʾ�趨����
				{
					printf("10���Ʋ�����ʾ!\r\n");
					usmart_dev.sptype=SP_TYPE_DEC;  
				}

			}
            else 
                return USMART_PARMERR;			//��������. 
			printf("\r\n"); 
			break;	 
		case 6://runtimeָ��,�����Ƿ���ʾ����ִ��ʱ��
			printf("\r\n");
			usmart_get_aparm(str,sfname,&i);
			if(i==0)//��������
			{
				i=usmart_str2num(sfname,&res);	   		//��¼�ò���	
				if(i==0)						   		//��ȡָ����ַ���ݹ���
				{
					if(USMART_ENTIMX_SCAN==0)
                        printf("\r\nError! \r\nTo EN RunTime function,Please set USMART_ENTIMX_SCAN = 1 first!\r\n");//����
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
                    return USMART_PARMERR;   			//δ������,���߲�������	 
 			}
            else 
                return USMART_PARMERR;				//��������. 
			printf("\r\n"); 
			break;	    
		default://�Ƿ�ָ��
			return USMART_FUNCERR;
	}
	return 0;
}

//��str�л�ȡ������,id,��������Ϣ
//*str:�ַ���ָ��.
//����ֵ:0,ʶ��ɹ�;����,�������.
uint8_t usmart_cmd_rec(uint8_t* str) 
{
	uint8_t sta,i,rval;//״̬	 
	uint8_t rpnum,spnum;
	uint8_t rfname[MAX_FNAME_LEN];//�ݴ�ռ�,���ڴ�Ž��յ��ĺ�����  
	uint8_t sfname[MAX_FNAME_LEN];//��ű��غ�����
	sta = usmart_get_fname(str,rfname,&rpnum,&rval);//�õ����յ������ݵĺ���������������	  
	if(sta)
        return sta;//����
	for(i=0;i<usmart_dev.fnum;i++)
	{
		sta=usmart_get_fname((uint8_t*)usmart_dev.funs[i].name,sfname,&spnum,&rval);//�õ����غ���������������
		if(sta)
            return sta;//���ؽ�������	  
		if(usmart_strcmp(sfname,rfname)==0)//���
		{
			if(spnum>rpnum)
                return USMART_PARMERR;//��������(���������Դ����������)
			usmart_dev.id=i;//��¼����ID.
			break;//����.
		}	
	}
	if(i==usmart_dev.fnum)
        return USMART_NOFUNCFIND;	//δ�ҵ�ƥ��ĺ���
 	sta=usmart_get_fparam(str,&i);					//�õ�������������	
	if(sta)
        return sta;								//���ش���
	usmart_dev.pnum=i;								//����������¼
    return USMART_OK;
}
//usamrtִ�к���
//�ú�����������ִ�дӴ����յ�����Ч����.
//���֧��10�������ĺ���,����Ĳ���֧��Ҳ������ʵ��.�����õĺ���.һ��5�����ҵĲ����ĺ����Ѿ����ټ���.
//�ú������ڴ��ڴ�ӡִ�����.��:"������(����1������2...����N)=����ֵ".����ʽ��ӡ.
//����ִ�еĺ���û�з���ֵ��ʱ��,����ӡ�ķ���ֵ��һ�������������.
void usmart_exe(void)
{
	uint8_t id,i;
	uint32_t res;		   
	uint32_t temp[MAX_PARM];//����ת��,ʹ֧֮�����ַ��� 
	uint8_t sfname[MAX_FNAME_LEN];//��ű��غ�����
	uint8_t pnum,rval;
	id=usmart_dev.id;
	if(id>=usmart_dev.fnum)
        return;//��ִ��.
	usmart_get_fname((uint8_t*)usmart_dev.funs[id].name,sfname,&pnum,&rval);//�õ����غ�����,���������� 
	printf("\r\n%s(",sfname);//�����Ҫִ�еĺ�����
	for(i=0;i<pnum;i++)//�������
	{
		if(usmart_dev.parmtype&(1<<i))//�������ַ���
		{
			printf("%c",'"');			 
			printf("%s",usmart_dev.parm+usmart_get_parmpos(i));
			printf("%c",'"');
			temp[i]=(uint32_t)&(usmart_dev.parm[usmart_get_parmpos(i)]);
		}else						  //����������
		{
			temp[i]=*(uint32_t*)(usmart_dev.parm+usmart_get_parmpos(i));
			if(usmart_dev.sptype==SP_TYPE_DEC)
                printf("%d",temp[i]);//10���Ʋ�����ʾ
			else 
                printf("0X%X",temp[i]);//16���Ʋ�����ʾ 	   
		}
		if(i!=pnum-1)printf(",");
	}
	printf(")");
//#if USMART_ENTIMX_SCAN==1
//	usmart_reset_runtime();	//��ʱ������,��ʼ��ʱ
//#endif
	switch(usmart_dev.pnum)
	{
		case 0://�޲���(void����)											  
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)();
			break;
	    case 1://��1������
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0]);
			break;
	    case 2://��2������
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1]);
			break;
	    case 3://��3������
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2]);
			break;
	    case 4://��4������
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3]);
			break;
	    case 5://��5������
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4]);
			break;
	    case 6://��6������
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5]);
			break;
	    case 7://��7������
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6]);
			break;
	    case 8://��8������
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6],temp[7]);
			break;
	    case 9://��9������
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6],temp[7],temp[8]);
			break;
	    case 10://��10������
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6],temp[7],temp[8],temp[9]);
			break;
	}
//#if USMART_ENTIMX_SCAN==1
//	usmart_get_runtime();//��ȡ����ִ��ʱ��
//#endif
	if(rval==1)//��Ҫ����ֵ.
	{
		if(usmart_dev.sptype==SP_TYPE_DEC)printf("=%u;\r\n",res);//���ִ�н��(10���Ʋ�����ʾ)
		else printf("=0X%X;\r\n",res);//���ִ�н��(16���Ʋ�����ʾ)	   
	}else printf(";\r\n");		//����Ҫ����ֵ,ֱ���������
	if(usmart_dev.runtimeflag)	//��Ҫ��ʾ����ִ��ʱ��
	{ 
		printf("Function Run Time:%d.%1dms\r\n",usmart_dev.runtime/10,usmart_dev.runtime%10);//��ӡ����ִ��ʱ�� 
	}
}


//usmartɨ�躯��
//ͨ�����øú���,ʵ��usmart�ĸ�������.�ú�����Ҫÿ��һ��ʱ�䱻����һ��
//�Լ�ʱִ�дӴ��ڷ������ĸ�������.
//�������������ж��������,�Ӷ�ʵ���Զ�����.
//�����ALIENTEK�û�,��USART_RX_STA��USART_RX_BUF[]��Ҫ�û��Լ�ʵ��
void usmart_scan(void)
{
    uint8_t sta,len,index;
    __tty_info* tty_info_p = (__tty_info*)&tty_info;
    index = uart_port2index(printf_info.port);
    if(IS_INDEX(index) == pdFAIL)
        return;
	if(tty_info_p->new_msg)             //���ڽ�����ɣ�(���յ�\r\n)
	{
		len = tty_info_p->msg_len;	    //�õ��˴ν��յ������ݳ���
        usart_info[index].recv_buf[usart_info[index].recv_buf_len - 2] = '\0';
		sta = usmart_dev.cmd_rec(usart_info[index].recv_buf);//�õ�����������Ϣ
		if(sta == 0)
            usmart_dev.exe();	//ִ�к��� 
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
						printf("��������!\r\n");
						break;	
					case USMART_PARMERR:
						printf("��������!\r\n");   			
						break;				
					case USMART_PARMOVER:
						printf("����̫��!\r\n");   			
						break;		
					case USMART_NOFUNCFIND:
						printf("δ�ҵ�ƥ��ĺ���!\r\n");   			
						break;		
				}
			}
		}
		tty_info = 0;                   //״̬�Ĵ������	    
	} 
}
#if USMART_USE_WRFUNS==1 	//���ʹ���˶�д����
//��ȡָ����ַ��ֵ		 
uint32_t read_addr(uint32_t addr)
{
	return *(uint32_t*)addr;//	
}
//��ָ����ַд��ָ����ֵ		 
void write_addr(uint32_t addr,uint32_t val)
{
	*(uint32_t*)addr=val; 	
}
#endif


//�Ա��ַ���str1��str2
//*str1:�ַ���1ָ��
//*str2:�ַ���2ָ��
//����ֵ:0�����;1�������;
uint8_t usmart_strcmp(uint8_t *str1,uint8_t *str2)
{
	while(1)
	{
		if(*str1!=*str2)return 1;//�����
		if(*str1=='\0')break;//�Ա������.
		str1++;
		str2++;
	}
	return 0;//�����ַ������
}
//��str1������copy��str2
//*str1:�ַ���1ָ��
//*str2:�ַ���2ָ��			   
void usmart_strcopy(uint8_t*str1,uint8_t *str2)
{
	while(1)
	{										   
		*str2=*str1;	//����
		if(*str1=='\0')break;//���������.
		str1++;
		str2++;
	}
}
//�õ��ַ����ĳ���(�ֽ�)
//*str:�ַ���ָ��
//����ֵ:�ַ����ĳ���		   
uint8_t usmart_strlen(uint8_t*str)
{
	uint8_t len=0;
	while(1)
	{							 
		if(*str=='\0')break;//���������.
		len++;
		str++;
	}
	return len;
}
//m^n����
//����ֵ:m^n�η�
uint32_t usmart_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}	    
//���ַ���תΪ����
//֧��16����ת��,����16������ĸ�����Ǵ�д��,�Ҹ�ʽΪ��0X��ͷ��.
//֧�ָ��� 
//*str:�����ַ���ָ��
//*res:ת����Ľ����ŵ�ַ.
//����ֵ:0���ɹ�ת�����.����,�������.
//1,���ݸ�ʽ����.2,16����λ��Ϊ0.3,��ʼ��ʽ����.4,ʮ����λ��Ϊ0.
uint8_t usmart_str2num(uint8_t*str,uint32_t *res)
{
	uint32_t t;
	int tnum;
	uint8_t bnum=0;		//���ֵ�λ��
	uint8_t *p;		  
	uint8_t hexdec=10;	//Ĭ��Ϊʮ��������
	uint8_t flag=0;		//0,û�з��ű��;1,��ʾ����;2,��ʾ����.
	p=str;
	*res=0;//����.
	while(1)
	{
		if((*p<='9'&&*p>='0')||((*str=='-'||*str=='+')&&bnum==0)||(*p<='F'&&*p>='A')||(*p=='X'&&bnum==1))//�����Ϸ�
		{
			if(*p>='A')hexdec=16;	//�ַ����д�����ĸ,Ϊ16���Ƹ�ʽ.
			if(*str=='-'){flag=2;str+=1;}//ƫ�Ƶ�����
			else if(*str=='+'){flag=1;str+=1;}//ƫ�Ƶ�����
			else bnum++;			//λ������.
		}else if(*p=='\0')break;	//����������,�˳�.
		else return 1;				//��ȫ��ʮ���ƻ���16��������.
		p++; 
	} 
	p=str;			    //���¶�λ���ַ�����ʼ�ĵ�ַ.
	if(hexdec==16)		//16��������
	{
		if(bnum<3)return 2;			//λ��С��3��ֱ���˳�.��Ϊ0X��ռ��2��,���0X���治������,������ݷǷ�.
		if(*p=='0' && (*(p+1)=='X'))//������'0X'��ͷ.
		{
			p+=2;	//ƫ�Ƶ�������ʼ��ַ.
			bnum-=2;//��ȥƫ����	 
		}else return 3;//��ʼͷ�ĸ�ʽ����
	}else if(bnum==0)return 4;//λ��Ϊ0��ֱ���˳�.	  
	while(1)
	{
		if(bnum)bnum--;
		if(*p<='9'&&*p>='0')t=*p-'0';	//�õ����ֵ�ֵ
		else t=*p-'A'+10;				//�õ�A~F��Ӧ��ֵ	    
		*res+=t*usmart_pow(hexdec,bnum);		   
		p++;
		if(*p=='\0')break;//���ݶ�������.	
	}
	if(flag==2)//�Ǹ���?
	{	
		tnum=-*res; 
		*res=tnum;
	}
	return 0;//�ɹ�ת��
}
//�õ�ָ����
//*str:Դ�ַ���
//*cmdname:ָ����
//*nlen:ָ��������		
//maxlen:��󳤶�(������,ָ�����̫����)	
//����ֵ:0,�ɹ�;����,ʧ��.	  
uint8_t usmart_get_cmdname(uint8_t*str,uint8_t*cmdname,uint8_t *nlen,uint8_t maxlen)
{
	*nlen=0;
 	while(*str!=' '&&*str!='\0') //�ҵ��ո���߽���������Ϊ������
	{
		*cmdname=*str;
		str++;
		cmdname++;
		(*nlen)++;//ͳ�������
		if(*nlen>=maxlen)return 1;//�����ָ��
	}
	*cmdname='\0';//���������
	return 0;//��������
}
//��ȡ��һ���ַ������м��кܶ�ո��ʱ�򣬴˺���ֱ�Ӻ��Կո��ҵ��ո�֮��ĵ�һ���ַ���
//str:�ַ���ָ��	
//����ֵ:��һ���ַ�
uint8_t usmart_search_nextc(uint8_t* str)
{
	str++;
	while(*str == ' ' && str != '\0')
        str++;
	return *str;
} 
//��str�еõ�������
//*str:Դ�ַ���ָ��
//*fname:��ȡ���ĺ�������ָ��
//*pnum:�����Ĳ�������
//*rval:�Ƿ���Ҫ��ʾ����ֵ(0,����Ҫ;1,��Ҫ)
//����ֵ:0,�ɹ�;����,�������.
uint8_t usmart_get_fname(uint8_t*str,uint8_t*fname,uint8_t *pnum,uint8_t *rval)
{
	uint8_t res;
	uint8_t fover = 0;	  //�������
	uint8_t *strtemp;
	uint8_t offset = 0;  
	uint8_t parmnum = 0;
	uint8_t temp = 1;
	uint8_t fpname[6];//void+X+'/0'
	uint8_t fplcnt = 0; //��һ�������ĳ��ȼ�����
	uint8_t pcnt = 0;	 //����������
	uint8_t nchar;
	//�жϺ����Ƿ��з���ֵ
	strtemp = str;
	while(*strtemp != '\0')//û�н���
	{
		if(*strtemp != ' ' && (pcnt & 0x7F) < 5)//����¼5���ַ�
		{
			if(pcnt == 0)
                pcnt |= 0x80;//��λ���λ,��ǿ�ʼ���շ���ֵ����
			if(((pcnt & 0x7f) == 4) && (*strtemp != '*'))
                break;//���һ���ַ�,������*
			fpname[pcnt & 0x7f] = *strtemp;//��¼�����ķ���ֵ����
			pcnt++;
		}
        else if(pcnt == 0x85)
            break;
		strtemp++;
	}
	if(pcnt)//��������
	{
		fpname[pcnt & 0x7f] = '\0';//���������
		if(usmart_strcmp(fpname,(uint8_t*)"void") == 0)
            *rval = 0;//����Ҫ����ֵ
		else
            *rval = 1;							   //��Ҫ����ֵ
		pcnt = 0;
	}
	res = 0;
	strtemp = str;
	while(*strtemp != '(' && *strtemp != '\0') //�˴����ҵ���������������ʼλ��
	{
		strtemp++;
		res++;
		if(*strtemp == ' ' || *strtemp == '*')
		{
			nchar = usmart_search_nextc(strtemp);		//��ȡ��һ���ַ�
			if(nchar != '(' && nchar != '*')
                offset = res;	//�����ո��*��
		}
	}
	strtemp = str;
	if(offset)
        strtemp += offset + 1;//������������ʼ�ĵط�	   
	res = 0;
	nchar = 0;//�Ƿ������ַ�������ı�־,0�������ַ���;1�����ַ���;
	while(1)
	{
		if(*strtemp==0)
		{
			res = USMART_FUNCERR;//��������
			break;
		}
        else if(*strtemp == '(' && nchar == 0)
            fover++;//�����������һ��
		else if(*strtemp == ')' && nchar == 0)
		{
			if(fover)
                fover--;
			else 
                res = USMART_FUNCERR;//�������,û�յ�'('
			if(fover == 0)
                break;//��ĩβ��,�˳�	    
		}
        else if(*strtemp == '"')
            nchar =! nchar;
		if(fover == 0)//��������û������
		{
			if(*strtemp != ' ')//�ո����ں�����
			{
				*fname = *strtemp;//�õ�������
				fname++;
			}
		}
        else //�Ѿ��������˺�������.
		{
			if(*strtemp==',')
			{
				temp=1;		//ʹ������һ������
				pcnt++;	
			}
            else if(*strtemp!=' '&&*strtemp!='(')
			{
				if(pcnt==0&&fplcnt<5)		//����һ��������ʱ,Ϊ�˱���ͳ��void���͵Ĳ���,�������ж�.
				{
					fpname[fplcnt]=*strtemp;//��¼��������.
					fplcnt++;
				}
				temp++;	//�õ���Ч����(�ǿո�)
			}
			if(fover==1&&temp==2)
			{
				temp++;		//��ֹ�ظ�����
				parmnum++; 	//��������һ��
			}
		}
		strtemp++; 			
	}
	if(parmnum==1)//ֻ��1������.
	{
		fpname[fplcnt]='\0';//���������
		if(usmart_strcmp(fpname,(uint8_t*)"void")==0)
            parmnum=0;//����Ϊvoid,��ʾû�в���.
	}
	*pnum=parmnum;	//��¼��������
	*fname='\0';	//���������
	return res;		//����ִ�н��
}


//��str�еõ�һ�������Ĳ���
//*str:Դ�ַ���ָ��
//*fparm:�����ַ���ָ��
//*ptype:�������� 0������;1���ַ���;0XFF����������
//����ֵ:0,�Ѿ��޲�����;����,��һ��������ƫ����.
uint8_t usmart_get_aparm(uint8_t *str,uint8_t *fparm,uint8_t *ptype)
{
	uint8_t i=0;
	uint8_t enout=0;
	uint8_t type=0;//Ĭ��������
	uint8_t string=0; //���str�Ƿ����ڶ�
	while(1)
	{
		if(*str==','&& string==0)
            enout=1;			//�ݻ������˳�,Ŀ����Ѱ����һ����������ʼ��ַ
		if((*str==')'||*str=='\0')&&string==0)
            break;//�����˳���ʶ��
		if(type==0)//Ĭ�������ֵ�
		{
			if((*str>='0' && *str<='9')||*str=='-'||*str=='+'||(*str>='a' && *str<='f')||(*str>='A' && *str<='F')||*str=='X'||*str=='x')//���ִ����
			{
				if(enout)break;					//�ҵ�����һ������,ֱ���˳�.
				if(*str>='a')*fparm=*str-0X20;	//Сдת��Ϊ��д
				else *fparm=*str;		   		//Сд�������ֱ��ֲ���
				fparm++;
			}else if(*str=='"')//�ҵ��ַ����Ŀ�ʼ��־
			{
				if(enout)break;//�ҵ�,����ҵ�",��Ϊ������.
				type=1;
				string=1;//�Ǽ�STRING ���ڶ���
			}else if(*str!=' '&&*str!=',')//���ַǷ��ַ�,��������
			{
				type=0XFF;
				break;
			}
		}else//string��
		{ 
			if(*str=='"')string=0;
			if(enout)break;			//�ҵ�����һ������,ֱ���˳�.
			if(string)				//�ַ������ڶ�
			{	
				if(*str=='\\')		//����ת���(������ת���)
				{ 
					str++;			//ƫ�Ƶ�ת���������ַ�,����ʲô�ַ�,ֱ��COPY
					i++;
				}					
				*fparm=*str;		//Сд�������ֱ��ֲ���
				fparm++;
			}	
		}
		i++;//ƫ��������
		str++;
	}
	*fparm='\0';	//���������
	*ptype=type;	//���ز�������
	return i;		//���ز�������
}
//�õ�ָ����������ʼ��ַ
//num:��num������,��Χ0~9.
//����ֵ:�ò�������ʼ��ַ
uint8_t usmart_get_parmpos(uint8_t num)
{
	uint8_t temp=0;
	uint8_t i;
	for(i=0;i<num;i++)temp+=usmart_dev.plentbl[i];
	return temp;
}
//��str�еõ���������
//str:Դ�ַ���;
//parn:�����Ķ���.0��ʾ�޲��� void����
//����ֵ:0,�ɹ�;����,�������.
uint8_t usmart_get_fparam(uint8_t*str,uint8_t *parn)
{	
	uint8_t i,type;  
	uint32_t res;
	uint8_t n=0;
	uint8_t len;
	uint8_t tstr[PARM_LEN+1];//�ֽڳ��ȵĻ���,�����Դ��PARM_LEN���ַ����ַ���
	for(i=0;i<MAX_PARM;i++)usmart_dev.plentbl[i]=0;//��ղ������ȱ�
	while(*str!='(')//ƫ�Ƶ�������ʼ�ĵط�
	{
		str++;											    
		if(*str=='\0')return USMART_FUNCERR;//������������
	}
	str++;//ƫ�Ƶ�"("֮��ĵ�һ���ֽ�
	while(1)
	{
		i=usmart_get_aparm(str,tstr,&type);	//�õ���һ������  
		str+=i;								//ƫ��
		switch(type)
		{
			case 0:	//����
				if(tstr[0]!='\0')				//���յ��Ĳ�����Ч
				{					    
					i=usmart_str2num(tstr,&res);	//��¼�ò���	 
					if(i)return USMART_PARMERR;		//��������.
					*(uint32_t*)(usmart_dev.parm+usmart_get_parmpos(n))=res;//��¼ת���ɹ��Ľ��.
					usmart_dev.parmtype&=~(1<<n);	//�������
					usmart_dev.plentbl[n]=4;		//�ò����ĳ���Ϊ4  
					n++;							//��������  
					if(n>MAX_PARM)return USMART_PARMOVER;//����̫��
				}
				break;
			case 1://�ַ���	 	
				len=usmart_strlen(tstr)+1;	//�����˽�����'\0'
				usmart_strcopy(tstr,&usmart_dev.parm[usmart_get_parmpos(n)]);//����tstr���ݵ�usmart_dev.parm[n]
				usmart_dev.parmtype|=1<<n;	//����ַ��� 
				usmart_dev.plentbl[n]=len;	//�ò����ĳ���Ϊlen  
				n++;
				if(n>MAX_PARM)return USMART_PARMOVER;//����̫��
				break;
			case 0XFF://����
				return USMART_PARMERR;//��������	  
		}
		if(*str==')'||*str=='\0')break;//�鵽������־��.
	}
	*parn=n;	//��¼�����ĸ���
	return USMART_OK;//��ȷ�õ��˲���
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
        if(*(ascii + i) == ' ') //�������' '
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
 * �� �� ��  : conver_para_type
 * �� �� ��  : ���̳���������
 * ��������  : 2021��12��6��
 * ��������  : ���������в����ַ�����ȡ��������
 * �������  : char* para_str      �������׵�ַ
               uint8_t* para_type  ��Ų�������
 * �������  : ��
 * �� �� ֵ  : 0xFF: ERR
 * ���ù�ϵ  : 
 * ��    ��  : 

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

//��ȡ��������ֵ����
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
	while(1)
	{
		if(*str == '\0')
			return NULL;
		else if(*str == '(' && ++depth == 1)
			str_soi = skip_mark(str + 1, NULL, ' ', 1);			//��¼��һ����������ʼλ��
		else if(*str == ')' && depth > 1)
			depth--;
		else if((*str == ',' || *str == ')') && depth == 1)		//��������λ��
		{
			str_eoi = skip_mark(str, NULL, ' ', -1);
			if(str_eoi > str_soi)
				para_num++;
			else if(str_eoi <= str_soi && *str != ')')			//��Ҫ�ò��������ò���Ϊ��
				return NULL;
			else{}
			if(*str == ',')
				str_soi = skip_mark(str + 1, NULL, ' ', 1);		//��¼��һ����������ʼλ��
			if(*str == ')')
				depth--;										//���һ��')'
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
				return 1;										//ERR��"()"ʹ�ò��Գ�
			else
				return 0;
		}
		else if(*str == '(' && ++depth == 1)
			str_soi = skip_mark(str + 1, NULL, ' ', 1);			//��¼��һ����������ʼλ��
		else if(*str == ')' && depth > 1)
			depth--;
		else if((*str == ',' || *str == ')') && depth == 1)		//��������λ��
		{
			str_eoi = skip_mark(str, NULL, ' ', -1);
			if(str_eoi > str_soi)
				(*para_num)++;
			if(*str == ',')
				str_soi = skip_mark(str + 1, NULL, ' ', 1);		//��¼��һ����������ʼλ��
			if(*str == ')')
				depth--;										//���һ��')'
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

//���ݺ���������Ϣ��ȡ��������
char* get_func_para_type(char* str, uint8_t* para_type)
{
    
}

//���ݺ����������ݻ�ȡ����ֵ
char* get_func_para_val(char* str, void* para_val)
{
    char* str_eoi;
    uint8_t para_type;
    //
    
    str_eoi = get_func_para_type(str, &para_type);
}

/*****************************************************************************
 * �� �� ��  : get_para
 * �� �� ��  : ���̳���������
 * ��������  : 2021��12��6��
 * ��������  : ���ݺ��������뺯�����������ݻ�ȡ��������
 * �������  : char* str_recv      ���յ����ַ����е�n���������׵�ַ
               char* str_decl      �����е�n���������׵�ַ
               void* para_val      �������ֵ�ĵ�ַ
               uint8_t* para_type  ����������͵ĵ�ַ����������sizeof(��������)
               uint8_t para_num    ��������
 * �������  : ��
 * �� �� ֵ  : 
 * ���ù�ϵ  : 
 * ��    ��  : 

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
    for(index = 0 ; index < sizeof(usmart_nametab) / sizeof(__func_info) ; index++)
    {
		//��һ��ʹ����Ҫ�Ӻ��������л�ȡ������ָ���Լ�����������
        if(usmart_nametab[index].name == NULL)
        {
			str = usmart_nametab[index].declaration;
			//��ȡ���������к�����
			str = get_func_name(str, regi_func_name);
			if(str)
			{
				usmart_nametab[index].name = str;
				usmart_nametab[index].name_len = strlen(regi_func_name);
			}
			//���������и�ʽ���󣬲�ѯ��������������ѯ��һ����
			else
				continue;
		}
		//���������Ȳ�ͬ��������������һƥ��
		if(usmart_nametab[index].name_len != strlen(regi_func_name))
			continue;
		//������ƥ���ж�
		if(0 == memcmp(usmart_nametab[index].name, regi_func_name, usmart_nametab[index].name_len))
		{
			*matching_index = index;
			return 0;
		}
    }
	//err, ����ע��ĺ�����û��ָ���ĺ���
	return 1;
}

//��Ӧ��������
void response_func_cmd(char* str)
{
    char *str_soi,*str_eoi;
	uint8_t	matching_index,ret_type,decl_para_num,recv_para_num;
    char recv_func_name[20];                //���յ���ָ���еĺ�����
    char regi_func_name[20];				//�����Ѿ�ע��ĺ�����
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
    if(get_func_para_num(str, &decl_para_num) || get_func_para_num(usmart_nametab[matching_index].declaration, &recv_para_num))
    	return;								//������ʽ����ȷ
    //���յ��Ĳ����Ƿ��������Ĳ���ƥ��
    if(decl_para_num != recv_para_num)
		return;
	//����index�еĺ���������ȡ������������
	get_func_ret_type(usmart_nametab[matching_index].declaration, &ret_type);
	//��ȡ����

	
}












