#ifndef __MY_TTY_H__
#define __MY_TTY_H__
#include "my_cfg.h"
#include "my_usart.h"
#include "my_printf.h"

#define MAX_FNAME_LEN 		30	//��������󳤶ȣ�Ӧ������Ϊ��С����������ĳ��ȡ�											   
#define MAX_PARM 			10	//���Ϊ10������ ,�޸Ĵ˲���,�����޸�usmart_exe��֮��Ӧ.
#define PARM_LEN 			200	//���в���֮�͵ĳ��Ȳ�����PARM_LEN���ֽ�,ע�⴮�ڽ��ղ���Ҫ��֮��Ӧ(��С��PARM_LEN)


#define USMART_ENTIMX_SCAN 	1	//ʹ��TIM�Ķ�ʱ�ж���ɨ��SCAN����,�������Ϊ0,��Ҫ�Լ�ʵ�ָ�һ��ʱ��ɨ��һ��scan����.
								//ע��:���Ҫ��runtimeͳ�ƹ���,��������USMART_ENTIMX_SCANΪ1!!!!
								
#define USMART_USE_HELP		1	//ʹ�ð�������ֵ��Ϊ0�����Խ�ʡ��700���ֽڣ����ǽ������޷���ʾ������Ϣ��
#define USMART_USE_WRFUNS	1	//ʹ�ö�д����,ʹ������,���Զ�ȡ�κε�ַ��ֵ,������д�Ĵ�����ֵ.
///////////////////////////////////////////////END///////////////////////////////////////////////////////////

#define USMART_OK 			0  //�޴���
#define USMART_FUNCERR 		1  //��������
#define USMART_PARMERR 		2  //��������
#define USMART_PARMOVER 	3  //�������
#define USMART_NOFUNCFIND 	4  //δ�ҵ�ƥ�亯��

#define SP_TYPE_DEC      	0  //10���Ʋ�����ʾ
#define SP_TYPE_HEX       	1  //16���Ʋ�����ʾ


typedef struct
{   
    uint8_t type:7;
    uint8_t is_point:1;
}__para_type;

typedef enum
{
    PARA_UINT8_T,
    PARA_UINT16_T,
    PARA_UINT32_T,
    PARA_FLOAT,
    PARA_STRING,
    PARA_VOID,
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
}__para_info;

typedef struct
{
    uint16_t new_msg:1;
    uint16_t msg_len:15;
}__tty_info;
extern uint16_t tty_info;

//�������б�	 
typedef struct
{
	void*       func;                   //����ָ��
    char*       declaration;           	//��������
    char* 		name;		            //�������ں��������еĵ�ַ
    uint8_t		name_len;				//����������
}__func_info;

//usmart���ƹ�����
typedef struct
{
	__func_info *funs;	                    //������ָ��
	void                        (*init)(uint8_t);           //��ʼ��
	uint8_t                     (*cmd_rec)(uint8_t*str);    //ʶ������������
	void                        (*exe)(void); 				//ִ�� 
	void                        (*scan)(void);              //ɨ��
	uint8_t                     fnum; 				  		//��������
	uint8_t                     pnum;                       //��������
	uint8_t                     id;							//����id
	uint8_t                     sptype;						//������ʾ����(���ַ�������):0,10����;1,16����;
	uint16_t                    parmtype;					//����������
	uint8_t                     plentbl[MAX_PARM];  		//ÿ�������ĳ����ݴ��
	uint8_t                     parm[PARM_LEN];  			//�����Ĳ���
	uint8_t                     runtimeflag;			    //0,��ͳ�ƺ���ִ��ʱ��;1,ͳ�ƺ���ִ��ʱ��,ע��:�˹��ܱ�����USMART_ENTIMX_SCANʹ�ܵ�ʱ��,������
	uint32_t                    runtime;					//����ʱ��,��λ:0.1ms,�����ʱʱ��Ϊ��ʱ��CNTֵ��2��*0.1ms
}__smart_info;
extern __func_info usmart_nametab[];	//��usmart_config.c���涨��
extern __smart_info usmart_dev;				//��usmart_config.c���涨��


void usmart_init(uint8_t sysclk);//��ʼ��
uint8_t usmart_cmd_rec(uint8_t*str);	//ʶ��
void usmart_exe(void);		//ִ��
void usmart_scan(void);     //ɨ��
uint32_t read_addr(uint32_t addr);	//��ȡָ����ַ��ֵ
void write_addr(uint32_t addr,uint32_t val);//��ָ����ַд��ָ����ֵ




uint8_t usmart_get_parmpos(uint8_t num);						//�õ�ĳ�������ڲ������������ʼλ��
uint8_t usmart_strcmp(uint8_t*str1,uint8_t *str2);					//�Ա������ַ����Ƿ����
uint32_t usmart_pow(uint8_t m,uint8_t n);							//M^N�η�
uint8_t usmart_str2num(uint8_t*str,uint32_t *res);					//�ַ���תΪ����
uint8_t usmart_get_cmdname(uint8_t*str,uint8_t*cmdname,uint8_t *nlen,uint8_t maxlen);//��str�еõ�ָ����,������ָ���
uint8_t usmart_get_fname(uint8_t*str,uint8_t*fname,uint8_t *pnum,uint8_t *rval);		//��str�еõ�������
uint8_t usmart_get_aparm(uint8_t *str,uint8_t *fparm,uint8_t *ptype); 	//��str�еõ�һ����������
uint8_t usmart_get_fparam(uint8_t*str,uint8_t *parn);  			//�õ�str�����еĺ�������.





#endif


