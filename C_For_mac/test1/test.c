/*
 *
 * ************Copyright 2026 MCD************
 *
 * @version      :
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2026-05-19 13:50:27
 * @LastEditors  : MCD
 * @LastEditTime : 2026-06-22 15:18:01
 * @FilePath     : /My_C_Test/C_For_mac/test1/test.c
 * @Description  :
 *
 * ******************************************
 */
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define IO_INPUT_COREX_W (0)
#define IO_INPUT_COREX_N (1)

#define TEST_DEFINE_1 100 + 20
#define TEST_DEFINE_2 (100 + 20)

#define BIT_TEST(reg, bit) ((reg) & (1u << bit))


#define  CORE_ACTION_TYPE_COMMON1                   0x01
#define  KNIN                                       ((CORE_ACTION_TYPE_COMMON1<<4) + 0x01)

#define  STSFCFG_MEM_BLK_BYTE_SIZE            (64+64)                /*内存块大小，单位字节。*/ 
#define  BUNIT                      sizeof(STSF_MemUnit_t)           /*字节单位大小*/
#define  STSF_MEM_BOLCK_SIZE        ((STSFCFG_MEM_BLK_BYTE_SIZE+BUNIT-1)/BUNIT + (sizeof(STSF_Evt_t)+BUNIT-1)/BUNIT) /*按 STSF_MemUnit_t地址对齐*/

typedef  uint32_t  STSF_MemUnit_t;

/********************************************************************************
* 类型定义 : 事件参数: 按键                                                                        
********************************************************************************/
typedef struct 
{
  uint8_t  KeyVal;  /*键值*/
}EvtArgKey_t;

/********************************************************************************
* 类型定义 : 事件参数: 数据通信包接收                                                                       
********************************************************************************/
typedef struct 
{
  uint8_t   DAP;               /*数据访问端口*/
  uint8_t   DCIType;           /*数据包类型*/

  uint16_t  RxLen;             /*接收长度*/
  uint8_t   *pRxBuf;           /*指向接收的缓存*/

  uint16_t  AckLen;            /*数据处理后的应答长度*/
  uint8_t   *pAckBuf;          /*指向应答缓存*/
}EvtArgDCIRx_t;

/********************************************************************************
* 类型定义 : 事件参数: 数据通信包发送                                                                   
********************************************************************************/
typedef struct
{
  uint8_t   DAP;               /*数据访问端口*/
  uint8_t   DCIType;           /*数据包类型*/

  uint16_t  TxLen;             /*发送长度*/
  uint8_t   *pTxBuf;           /*指向发送的缓存*/
}EvtArgDCITx_t;

/********************************************************************************
* 类型定义 : 事件参数: 通用的事件参数                                                                    
********************************************************************************/
typedef struct
{
  uint8_t  Reserved; /*保留，有些事件不需要参数*/
}EvtArgCommon_t;


/********************************************************************************
* 类型定义 : 事件定义                                                                  
* 该定义一般用来定义指针变量，方便数据操作。所以AppendData数组大小可根据实际需要进行修改。
********************************************************************************/
typedef struct 
{ 
  uint8_t   Type;  /*事件类型*/
  
  union 
  {
    /*★ 请根据应用的需求，在这里添加事件参数的类型。 ★*/
    EvtArgKey_t             KeyScan;
    EvtArgDCITx_t           DCITx;
    EvtArgDCIRx_t           DCIRx;
    EvtArgCommon_t          Common;
  }Arg; 
  
  /*附加数据，存放大数据时使用，比如串口接收发送的数据包。结合内存块使用。
    实际操作以指针方式进行操作，方法见串口数据收发处理的相关代码。*/
  STSF_MemUnit_t            AppendData[1]; 
}STSF_Evt_t;  



typedef struct PositionCode {
  uint8_t Cur : 4; /*当前位置编码*/
  uint8_t His : 4; /*历史位置编码*/
} PositionCode_T;

void test_func() {
  uint8_t temp;
  temp = !IO_INPUT_COREX_W;
  printf("temp: %d\n", temp);
  temp <<= 1;
  printf("temp: %d\n", temp);
  temp |= 0;  //! IO_INPUT_COREX_M;
  printf("temp: %d\n", temp);
  temp <<= 1;
  printf("temp: %d\n", temp);
  temp |= !IO_INPUT_COREX_N;

  printf("temp: %d\n", temp);
}

#define LONG_TO_BIN(n)                                            \
  (((n >> 21) & 0x80) | ((n >> 18) & 0x40) | ((n >> 15) & 0x20) | \
   ((n >> 12) & 0x10) | ((n >> 9) & 0x08) | ((n >> 6) & 0x04) |   \
   ((n >> 3) & 0x02) | ((n) & 0x01))
#define BIN(n) LONG_TO_BIN(0x##n##l)
#define B(n) BIN(n)
#define b(n) BIN(n)

#define FAULT_REPORT_TL_ALWAYS 0
#define NORMAL 0
#define FALSE 0

typedef struct {
  uint8_t FaultID;              /*故障ID号*/
  uint8_t FaultReportTL;        /*故障报告时长，TL(time length)*/
  uint8_t AdditionOp;           /*附加操作 */
  uint8_t (*pFaultCheck)(void); /*故障检测函数*/

} FaultItem_t;

uint8_t FaultCheck_02() { return 1; }
uint8_t FaultCheck_03() { return 1; }

static const FaultItem_t FaultTab[] = {
    {2, FAULT_REPORT_TL_ALWAYS, B(00000000),
     FaultCheck_02}, /*主控板与手控器通信超时*/
    {3, FAULT_REPORT_TL_ALWAYS, B(00000000),
     FaultCheck_03}, /*面板快捷按键卡键*/
};

void SelfCheck_FaultCheck() {
  static uint8_t FaultCheckIndex = 1;
  uint8_t ByteOffSet, BitOffSet;

  if (FaultTab[FaultCheckIndex].pFaultCheck != NULL) {
    ByteOffSet = (FaultTab[FaultCheckIndex].FaultID - 1) >>
                 3; /*计算字节偏移。 1字节8位，>>3等效除以8*/
    BitOffSet = (FaultTab[FaultCheckIndex].FaultID - 1) & 0x07; /*计算位偏移*/

    printf("Byteoffset: %d\n", ByteOffSet);
    printf("BitOffset: %d\n", BitOffSet);
    uint8_t temp1 = 0, temp = 0;
    printf("temp = %d\n", temp |= (1 << BitOffSet));
    printf("temp1 = %d\n", temp1 &= ~(1 << BitOffSet));
  }
}

// void test_func1(void) {
//   uint8_t temp_sum = 0;
//   size_t length = 100;

//   for (size_t i = 0; i < length; i++) {
//   }
// }

int main(int argc, char const* argv[]) {
  // printf("buffer test\n");
  // printf("%d\n", 1 << 3);
  // printf("%d\n", 4 >> 3);
  // struct PositionCode test;
  // test.Cur = 1;
  // test.His = 2;
  // printf("%d, %d\n", BIT_TEST(test.Cur, 0), BIT_TEST(test.His, 2));

  // uint8_t temp = 0;

  // // temp = ~0x1;
  // temp = 7;

  // temp >>= 2;
  // printf("temp = %d\n",temp);
  // BIT_TEST(test.Cur, 0);
  // BIT_TEST(test.His, 2);

  test_func();
  printf("KNIN: %d\n", KNIN);
  uint8_t test = 0;

  uint8_t mytest = test ? 1: 0;

  printf("mytest: %d\n", mytest);
  printf("My_C_Test1 : %d\n", mytest);

  printf("STSF_MEM_BOLCK_SIZE: %lu\n", STSF_MEM_BOLCK_SIZE);
  printf("STSF_Evt_t size: %lu\n", sizeof(STSF_Evt_t));
  // printf("define test2: %d\n", TEST_DEFINE_2 * 2);
  // SelfCheck_FaultCheck();

  // uint8_t LAM = 0;

  // LAM = ~(0x01);

  // printf("LAM: %d\n", LAM);
  return 0;
}
