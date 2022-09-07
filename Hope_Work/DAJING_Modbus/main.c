/*
 * 
 * 　　┏┓　　　┏┓+ +
 * 　┏┛┻━━━┛┻┓ + +
 * 　┃　　　　　　　┃ 　
 * 　┃　　　━　　　┃ ++ + + +
 *  ████━████ ┃+
 * 　┃　　　　　　　┃ +
 * 　┃　　　┻　　　┃
 * 　┃　　　　　　　┃ + +
 * 　┗━┓　　　┏━┛
 * 　　　┃　　　┃　　　　　　　　　　　
 * 　　　┃　　　┃ + + + +
 * 　　　┃　　　┃
 * 　　　┃　　　┃ +  神兽保佑
 * 　　　┃　　　┃    代码无bug　　
 * 　　　┃　　　┃　　+　　　　　　　　　
 * 　　　┃　 　　┗━━━┓ + +
 * 　　　┃ 　　　　　　　┣┓
 * 　　　┃ 　　　　　　　┏┛
 * 　　　┗┓┓┏━┳┓┏┛ + + + +
 * 　　　　┃┫┫　┃┫┫
 * 　　　　┗┻┛　┗┻┛+ + + +
 * 
 * 
 * 
 * ************Copyright 2022 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2022-08-24 13:19:51
 * @LastEditors  : MCD
 * @LastEditTime : 2022-09-06 10:53:08
 * @FilePath     : /My_C_Test/Hope_Work/DAJING_Modbus/main.c
 * @Description  : 
 * 
 * ******************************************
 */
#include "adaptor_reg.h"
#include "haif_adaptor.h"
#include "common.h"

int main(int argc, char const *argv[])
{
    uint16_t dregister = 0x76f1;
    int rc = 0, i, j;
    uint8_t sta = 0, nb = 0;
    uint16_t tab_rq_registers[4] = {0x7899, 0x0000, 0x1001, 0x7788};

#if 0
    uint8_t ul = (dregister & INDOOR_MACH_WIND_SEPPD_SET_Msk) >> INDOOR_MACH_WIND_SEPPD_SET_Pos;
    printf("get ul = %d\n", ul);

    dregister = 0;
    dregister = INDOOR_MACH_CTRL_START_OR_STOP(dregister, 1) |
                INDOOR_MACH_CTRL_WIND_DIR_OR_VOL_SET_MODE(dregister, 4) |
                INDOOR_MACH_CTRL_WIND_DIR_SET(dregister, 7) |
                INDOOR_MACH_CTRL_AUTO_WIND(dregister, 1) |
                INDOOR_MACH_CTRL_AIR_OR_VENTILATION_VOL(dregister, 5);
    printf("dregister = %d, %x\n", dregister, dregister);
    short int data = -10;
    printf("data = %d, %d\n", data, abs(data));

    dregister = INDOOR_MACH_CTRL_START_OR_STOP(dregister, 0);
    printf("dregister = %d, %x\n", dregister, dregister);
#endif
    printf(" INDOOR_MACH_DIII_ADDR_0_Msk = %d\n", INDOOR_MACH_DIII_ADDR_0_Msk);
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 16; j++) {
            nb = j + i * 16;
            sta = (tab_rq_registers[i] & (INDOOR_MACH_DIII_ADDR_0_Msk << j)) >> j;
            printf("sta = %d, %d\n",tab_rq_registers[i] & (INDOOR_MACH_DIII_ADDR_0_Msk << j), (tab_rq_registers[i] & (INDOOR_MACH_DIII_ADDR_0_Msk << j)) >> j);
            if (sta == 1)
                rc++;
        }
    }

    printf("rc = %d\n", rc);

    
    return 0;
}
