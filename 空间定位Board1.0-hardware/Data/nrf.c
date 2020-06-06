******************** (C) COPYRIGHT 2015 ASTO ***************************
 @* 文件名  ：24l01.c
 @* 描述    ：nRF24L01驱动程序         
 @* 开发平台：STM32F103VET6主控制MCU
 @* 硬件连接：SPI2
 @* 库版本  ：ST3.5.0
 @* 作者    ： 
 @* 公司网址：<a target=_blank href="http://www.test.com">www.test.com</a>
 @* 总部网址：<a target=_blank href="http://www.test.com">www.test.com</a>
**********************************************************************************/
 
#include "config.h"
 
#if nRF24L01_EN > 0u
 
#include "sys_temp.h"
#include "24l01.h"
#include "delay.h"
 
const u8 RX0_Address[RX_ADR_WIDTH]={0x01,0x01,0x01,0x01,0x01};	//接收方通道0地址
 
/*
const u8 RX1_Address[RX_ADR_WIDTH]={0x02,0x20,0x20,0x20,0x20};	//接收方通道1地址
const u8 RX2_Address[1] = {0x03}; 								//接收方通道2地址
const u8 RX3_Address[1] = {0x04}; 								//接收方通道3地址
const u8 RX4_Address[1] = {0x05}; 								//接收方通道4地址
const u8 RX5_Address[1] = {0x06}; 								//接收方通道5地址
*/
 
/**
	@函数名称：EXTI_Configuration()
	@函数功能：nRF24L01的IRQ中断配置（只响应接收数据的中断）
	@输入：无
	@输出：无
	@调用：内部调用
**/
static void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;  
    NVIC_InitTypeDef NVIC_InitStructure;
  
	//中断优先级设置
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
    NVIC_InitStructure.NVIC_IRQChannel = NRF24L01_INT_IRQ; 	//10-15的中断线共享一个中断处理程序 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//响应优行级
    NVIC_Init(&NVIC_InitStructure); 
	
	//中断线配置EXTI_Line11-->PB11
    GPIO_EXTILineConfig(NRF24L01_INT_SOURCE_PORT, NRF24L01_STATUS_INT_SOURCE); //设置中断源引脚 
    EXTI_InitStructure.EXTI_Line = NRF24L01_STATUS_LINE; //中断线设置 
    EXTI_InitStructure.EXTI_LineCmd = ENABLE; //中断线使能 
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //中断方式 
    EXTI_InitStructure.EXTI_Trigger = NRF24L01_STATUS_INT_MODE;//模式，这里设置为下降沿触发  
    EXTI_Init(&EXTI_InitStructure);
}
 
/**
 @* 函数名：NRF24L01_SPI_Init()
 @* 描述  ：初始化SPI2端口及基模式，用于操作nRF24L01无线2.4G模块
 @* 输入  ：无
 @* 输出  : 无
 @* 调用  ：内部调用
 */
static void NRF24L01_SPI_Init(void) {	
	
	GPIO_InitTypeDef GPIO_InitStructure;	
	SPI_InitTypeDef SPI_InitStructure;
 
	//注意将普通IO作为中断线时必须开启AFIO时钟
	RCC_APB2PeriphClockCmd(NRF24L01_SPI_Periph_CLK|RCC_APB2Periph_AFIO, ENABLE);//开启GPIOB的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//开启SPI2的时钟
	
	/*配置 SPI的SCK, MISO, MOSI引脚，GPIOB^13,GPIOB^14,GPIOB^15 */
	GPIO_InitStructure.GPIO_Pin = NRF24L01_SPI_SCK_PIN| NRF24L01_SPI_MISO_PIN | NRF24L01_SPI_MOSI_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用输出功能	
	GPIO_Init(NRF24L01_SPI_GPIO_SRC, &GPIO_InitStructure);	
 
	/*GPIOB^10为nRF24L01的CE引脚，CSN引脚GPIOB^12 */
	GPIO_InitStructure.GPIO_Pin = NRF24L01_CE_PIN|NRF24L01_CSN_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//通用推挽输出
	GPIO_Init(NRF24L01_SPI_GPIO_SRC, &GPIO_InitStructure);
	
	/* GIOB^11为nRF24L01的IRQ中断输入引脚 */
	GPIO_InitStructure.GPIO_Pin = NRF24L01_IRQ_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮动输入
	GPIO_Init(NRF24L01_SPI_GPIO_SRC, &GPIO_InitStructure);
			 
	SPI_Cmd(SPI2,DISABLE);/*先失能，然后再使能*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex ; /* 全双工 */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master ; /*当前的设备为主机模式*/
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low ;   /* 时钟极性为低，即SPI空闲时，SCK为低电平 */
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge ; /* 时钟相位，第一个时钟沿（也就是奇数边沿）捕捉数据 */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;  /* 数据宽度 */	
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB ; /* 低地址存放最高有效字节 */	
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; /*配置片选为软件控制方式*/
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; /* 时钟分频为8分频，即72MHz/8=9MHz */		
	SPI_InitStructure.SPI_CRCPolynomial = 7;  /* 校验多项式，这个不起作用 */	
	SPI_Init(SPI2, &SPI_InitStructure);
	
	//使能SPI2
	SPI_Cmd(SPI2,ENABLE);
 
	EXTI_Configuration();//配置中断线
}
 
/**
 @* 函数名：SPIx_ReadWriteByte()
 @* 描述  ：SPI2 读写一个字节
 @* 输入  ：TxData:要写入的字节
 @* 输出  : 读取到的字节
 @* 调用  ：内部调用
 */
static u8 SPIx_ReadWriteByte(u8 TxData) {		
	
	/** 原来的实现方式
	
	u8 retry=0;				 
	while((SPI2->SR&1<<1)==0) {//等待发送区空
		
		retry++;
		if(retry>200)return 0;
	}
	
	SPI2->DR=TxData;	 	  //发送一个byte 
	retry=0;
	
	while((SPI2->SR&1<<0)==0) { //等待接收完一个byte
		
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI2->DR;          //返回收到的数据	*/	
 
	/* Loop while DR register in not empty */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
 
	/* Send byte through the SPI2 peripheral */
	SPI_I2S_SendData(SPI2, TxData);
 
	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
 
	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI2);
}
 
/**
 @* 函数名：NRF24L01_Init()
 @* 描述  ：初始化24L01的相关IO口
 @* 输入  ：无
 @* 输出  : 无
 @* 调用  ：外部板级支持包调用
 */
void NRF24L01_Init(void) { 
	
	NRF24L01_SPI_Init();    //初始化SPI2
	NRF24L01_CE=0; 			//使能24L01
	NRF24L01_CSN=1;			//SPI片选取消	
}
 
/**
 @* 函数名：NRF24L01_Check()
 @* 描述  ：检测24L01是否存在
 @* 输入  ：无
 @* 输出  : 0:成功，1:失败
 @* 调用  ：外部板级支持包调用
 */	
u8 NRF24L01_Check(void) {
	
	u8 buf[5]={0x21,0x21,0x21,0x21,0x21};
	u8 buf1[5];
	u8 i;
	//SPIx_SetSpeed(SPI_SPEED_8); //spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）   	 
	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址.	
	NRF24L01_Read_Buf(TX_ADDR,buf1, 5); //读出写入的地址  	
	for(i=0;i<5;i++)if(buf1[i]!=0x21)break;	//判断读出的数据和写入的数据是否完全一致				   
	if(i!=5)return 1;//检测24L01错误	
	return 0;		 //执行到这里，表示成功检测到24L01
}
 
/**
 @* 函数名：NRF24L01_Write_Reg()
 @* 描述  ：SPI写寄存器
 @* 输入  ：reg: 指定的寄存器地址
 @*		    value: 要写入的值
 @* 输出  : 返回寄存器的状态值
 @* 调用  ：内部调用
 */
u8 NRF24L01_Write_Reg(u8 reg,u8 value) {
	
	u8 status;
   	NRF24L01_CSN=0;                 	//使能SPI传输
  	status =SPIx_ReadWriteByte(reg);	//发送寄存器号 
  	SPIx_ReadWriteByte(value);      	//写入寄存器的值
  	NRF24L01_CSN=1;                 	//禁止SPI传输	   
  	return(status);       				//返回状态值
}
 
/**
 @* 函数名：NRF24L01_Read_Reg()
 @* 描述  ：读取SPI寄存器值
 @* 输入  ：reg: 指定的寄存器地址		   
 @* 输出  : 返回寄存器的状态值
 @* 调用  ：内部调用
 */
u8 NRF24L01_Read_Reg(u8 reg) {
	
	u8 reg_val;	
 	NRF24L01_CSN = 0;          			//使能SPI传输		
  	SPIx_ReadWriteByte(reg);   			//发送寄存器号
  	reg_val=SPIx_ReadWriteByte(0XFF);	//读取寄存器内容
  	NRF24L01_CSN = 1;          			//禁止SPI传输		    
  	return(reg_val);           			//返回状态值
}
 
/**
 @* 函数名：NRF24L01_Read_Buf()
 @* 描述  ：在指定位置读出指定长度的数据
 @* 输入  ：reg:指定的寄存器位置
 @*         pBuf:数据指针用来存储读到的数据，一般是数组	
 @*		    len:数据长度
 @* 输出  : 此次读到的状态寄存器值
 @* 调用  ：内部调用
 */ 
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len) {
	
	u8 status,u8_ctr;
  	NRF24L01_CSN = 0;     //使能SPI传输
  	status=SPIx_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值   	   
 	for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=SPIx_ReadWriteByte(0xFF);//读出数据
  	NRF24L01_CSN=1;       //关闭SPI传输
  	return status;        //返回读到的状态值
}
 
/**
 @* 函数名：NRF24L01_Write_Buf()
 @* 描述  ：在指定位置写指定长度的数据
 @* 输入  ：reg:寄存器(位置)	
 @*		    pBuf:数据指针
 @*		    len:数据长度
 @* 输出  : 此次读到的状态寄存器值
 @* 调用  ：内部调用
 */
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len) {
	
	u8 status,u8_ctr;
 	NRF24L01_CSN = 0;        //使能SPI传输
  	status = SPIx_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值
  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPIx_ReadWriteByte(*pBuf++); //写入数据	 
  	NRF24L01_CSN = 1;       //关闭SPI传输
  	return status;          //返回读到的状态值
}
 
/**
 @* 函数名：NRF24L01_TxPacket()
 @* 描述  ：启动NRF24L01发送一次数据
 @* 输入  ：txbuf:待发送数据首地址
 @* 输出  : 发送完成状况
 @* 调用  ：外部板级支持包调用
 */
u8 NRF24L01_TxPacket(u8 *txbuf) {
 
	//u8 sta;	
 	//SPIx_SetSpeed(SPI_SPEED_8);//spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）
	
	NRF24L01_CE=0;
  	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  最长32个字节
 	NRF24L01_CE=1;//启动发送，10微秒后启动发送
	
		
	return 0xff;//其他原因发送失败
}
 
 
/**
 @* 函数名：NRF24L01_RxPacket()
 @* 描述  ：启动NRF24L01接收数据
 @* 输入  ：rxbuf:用来接收数据的buffer
 @*		    chl:接收到数据的通道号
 @* 输出  : 0，接收完成；其他，错误代码
 @* 调用  ：外部板级支持包调用
 */
u8 NRF24L01_RxPacket(u8 *rxbuf, u8 *chl) {
	
	u8 sta;	
	
	//SPIx_SetSpeed(SPI_SPEED_8); //spi速度为9Mhz（24L01的最大SPI时钟为10Mhz） 
	
	sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值 
	
	//状态寄存器的bit1-bit3是表示接收到数据的通道号，最大值为6
	*chl=(0x0e&sta)>>1;
	
	//清除TX_DS或MAX_RT中断标志，注意这里是写1清零，只有清零后设备才能正常通讯
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,sta);
	
	if(sta&RX_OK) { //接收到数据
		
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
		NRF24L01_Write_Reg(FLUSH_RX, 0xff);//清除RX FIFO寄存器 
		return 0; 
	}	   
	return 1;//没收到任何数据
}
 
/**
 @* 函数名：RX_Mode()
 @* 描述  ：该函数初始化NRF24L01到RX模式
 @*         设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
 @*         当CE变高后,即进入RX模式,并可以接收数据了
 @* 输入  ：无 
 @* 输出  : 无
 @* 调用  ：外部板级支持包调用
 */	   
void RX_Mode(void) {
	
	NRF24L01_CE=0;	//CE为0进入待机模式
	
	//配置通道的接收
	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(u8*)RX0_Address,RX_ADR_WIDTH);//写RX节点地址（通道0），也就是表示用哪个通道接收数据	  
	
	//======================注意以下被注释掉的内容可作参考和=========================================
	//NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P1,(u8*)RX1_Address,RX_ADR_WIDTH);//写RX节点地址（通道1），也就是表示用哪个通道接收数据	  
	//NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P2,(u8*)RX2_Address,1);//写RX节点地址（通道2），也就是表示用哪个通道接收数据
	//NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P3,(u8*)RX3_Address,1);//写RX节点地址（通道3），也就是表示用哪个通道接收数据	  	
	//NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P4,(u8*)RX4_Address,1);//写RX节点地址（通道4），也就是表示用哪个通道接收数据
	//NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P5,(u8*)RX5_Address,1);//写RX节点地址（通道5），也就是表示用哪个通道接收数据	  
  	
	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA, 0x01);    			//使能通道0的自动应答  
	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01);			//使能通道0的接收地址，共有6个通道，高两位保留固定为00
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,0);	     		//设置RF通信频率		  
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度	
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x07);//设置TX发射参数,0db增益,2Mbps,低噪声增益开启     	
	NRF24L01_Write_Reg(SPI_WRITE_REG+CONFIG, 0x0f);//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 	
	
  	NRF24L01_CE = 1; //CE为高,进入接收模式 
}
 
 
/**
 @* 函数名：TX_Mode()
 @* 描述  ：该函数初始化NRF24L01到TX模式
 @*         设置TX地址,写TX数据宽度,设置RX自动应答的地址,
 @*         填充TX发送数据,选择RF频道,波特率和LNA HCURR,PWR_UP,CRC使能
 @*         当CE变高后,即进入RX模式,并可以接收数据了
 @*		    CE为高大于10us,则启动发送
 @* 输入  ：addr，要发送数据的目标地址 
 @* 输出  : 无
 @* 调用  ：外部板级支持包调用
 */	 
void TX_Mode(u8 * addr) {
	
	NRF24L01_CE=0;	 //CE为0进入待机模式
	
  	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR, addr, TX_ADR_WIDTH);//写TX节点地址，也就是接收方的地址（目标地址）
	
	//在发送端，数据通道0被用作接收应答信号，因此数据通道0的接收地址必须要与发送端的地址相同以确保收到正确的应答信号
	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(u8*)addr, RX_ADR_WIDTH);
 
	//先单独测试发送方，看发送是否正常
	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  
	NRF24L01_Write_Reg(SPI_WRITE_REG+SETUP_RETR, 0x1a); //使能自动重发，设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
  	  	
	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,0);       //设置RF通道为40
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x07);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	NRF24L01_Write_Reg(SPI_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	
	NRF24L01_CE=1;//CE为高,10us后启动发送
}
 
#endif

--------------------- 
作者：名人堂再聚首 
来源：CSDN 
原文：https://blog.csdn.net/jmmx/article/details/43056627 
版权声明：本文为博主原创文章，转载请附上博文链接！