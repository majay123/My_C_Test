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
 * @Date         : 2022-08-24 15:42:17
 * @LastEditors  : MCD
 * @LastEditTime : 2022-08-26 16:30:05
 * @FilePath     : /My_C_Test/DAJING_Modbus/adaptor_reg.h
 * @Description  :
 *
 * ******************************************
 */

#ifndef _ADAPTOR_REG_H
#define _ADAPTOR_REG_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* MODBUS registers */

/* ********************************************************************************************************************************** */
/* -----------------------------------------------------输入寄存器(input register)----------------------------------------------------- */
/* ********************************************************************************************************************************** */
/* 适配器状态寄存器 */
#define ADAPTRO_STATUS_REG          (30001)

/* 室内机的连接状态寄存器 */
#define INDOOR_MACH_CONN00_REG      (30002)
#define INDOOR_MACH_CONN01_REG      (INDOOR_MACH_CONN00_REG + 1)
#define INDOOR_MACH_CONN02_REG      (INDOOR_MACH_CONN00_REG + 2)
#define INDOOR_MACH_CONN03_REG      (INDOOR_MACH_CONN00_REG + 3)

/* 室内机的通信状态寄存器 */
#define INDOOR_MACH_COMM00_REG      (30006)
#define INDOOR_MACH_COMM01_REG      (INDOOR_MACH_COMM00_REG + 1)
#define INDOOR_MACH_COMM02_REG      (INDOOR_MACH_COMM00_REG + 2)
#define INDOOR_MACH_COMM03_REG      (INDOOR_MACH_COMM00_REG + 3)

#define INDOOR_MACH_ONE_ROW_SIZE                                    (16)

/* -----------------------室内机的性能信息寄存器----------------------- */
/* 1-00 ~ 1-15 */
/* 2-00 ~ 2-15 */
/* 3-00 ~ 3-15 */
/* 4-00 ~ 4-15 */
#define INDOOR_MACH_P_OFFSET                                            (3)
#define INDOOR_PERFORMANCE_INFO_OFFSET(row, addr)                       (((row - 1) * INDOOR_MACH_ONE_ROW_SIZE + addr) * INDOOR_MACH_P_OFFSET)
#define INDOOR_PERFORMANCE1_INFO_OFFSET(row, addr)                      (((row - 1) * INDOOR_MACH_ONE_ROW_SIZE + addr) * (INDOOR_MACH_P_OFFSET +  1))

/* 室内机工作模式和风量风向 */
#define INDOOR_MACH_WORK_MODE_100_INP_REG                               (31001)
#define INDOOR_MACH_WORK_MODE_INP_REG(row, addr)                        (INDOOR_MACH_WORK_MODE_100_INP_REG + INDOOR_PERFORMANCE_INFO_OFFSET(row, addr))

/* 室内机制冷设定温度上下限 */
#define INDOOR_MACH_REF_TMP_UPPER_AND_LOWER_LIMIT_100_INP_REG           (31002)
#define INDOOR_MACH_REF_TMP_UPPER_AND_LOWER_LIMIT_INP_REG(row, addr)    (INDOOR_MACH_REF_TMP_UPPER_AND_LOWER_LIMIT_100_INP_REG + INDOOR_PERFORMANCE_INFO_OFFSET(row, addr))

/* 室内机制热设定温度上下限 */
#define INDOOR_MACH_HEAT_TMP_UPPER_AND_LOWER_LIMIT_100_INP_REG          (31003)
#define INDOOR_MACH_HEAT_TMP_UPPER_AND_LOWER_LIMIT_INP_REG(row, addr)   (INDOOR_MACH_HEAT_TMP_UPPER_AND_LOWER_LIMIT_100_INP_REG + INDOOR_PERFORMANCE_INFO_OFFSET(row, addr))

/* 室内机通气模式 */
#define INDOOR_MACH_AIR_CTRL_100_INP_REG                                (31404)
#define INDOOR_MACH_AIR_CTRL_INP_REG(row, addr)                         (INDOOR_MACH_AIR_CTRL_100_INP_REG + INDOOR_PERFORMANCE1_INFO_OFFSET(row, addr))
/* ------------------------------end------------------------------ */

/* ----------------------室内机的状态信息寄存器----------------------- */
/* 1-00 ~ 1-15 */
/* 2-00 ~ 2-15 */
/* 3-00 ~ 3-15 */
/* 4-00 ~ 4-15 */
#define INDOOR_MACH_S_OFFSET                                            (6)
#define INDOOR_STATUS_INFO_OFFSET(row, addr)                            (((row - 1) * INDOOR_MACH_ONE_ROW_SIZE + addr) * INDOOR_MACH_S_OFFSET)
#define INDOOR_STATUS1_INFO_OFFSET(row, addr)                           (((row - 1) * INDOOR_MACH_ONE_ROW_SIZE + addr) * (INDOOR_MACH_S_OFFSET - 2))
#define INDOOR_STATUS2_INFO_OFFSET(row, addr)                           (((row - 1) * INDOOR_MACH_ONE_ROW_SIZE + addr) * (INDOOR_MACH_S_OFFSET - 4))

/* 室内机运行状态和风向风量信息 */
#define INDOOR_MACH_RUNING_STA_AND_AIR_STA_100_INP_REG                  (32001)
#define INDOOR_MACH_RUNING_STA_AND_AIR_STA_INP_REG(row, addr)           (INDOOR_MACH_RUNING_STA_AND_AIR_STA_100_INP_REG + INDOOR_STATUS_INFO_OFFSET(row, addr))

/* 室内机工作模式信息 */
#define INDOOR_MACH_WORKING_STA_INFO_100_INP_REG                        (32002)
#define INDOOR_MACH_WORKING_STA_INFO_INP_REG(row, addr)                 (INDOOR_MACH_WORKING_STA_INFO_100_INP_REG + INDOOR_STATUS_INFO_OFFSET(row, addr))

/* 室内机温度设定信息 */
#define INDOOR_MACH_CUR_TEMPERATURE_100_INP_REG                         (32003)
#define INDOOR_MACH_CUR_TEMPERATURE_INP_REG(row, addr)                  (INDOOR_MACH_CUR_TEMPERATURE_100_INP_REG + INDOOR_STATUS_INFO_OFFSET(row, addr))

/* 室内机异常代码信息 */
#define INDOOR_MACH_EXCEPTION_CODE_100_INP_REG                          (32004)
#define INDOOR_MACH_EXCEPTION_CODE_INP_REG(row, addr)                   (INDOOR_MACH_EXCEPTION_CODE_100_INP_REG + INDOOR_STATUS_INFO_OFFSET(row, addr))

/* 室内机室内温度信息 */
#define INDOOR_MACH_INDOOR_TEMPER_100_INP_REG                           (32005)
#define INDOOR_MACH_INDOOR_TEMPER_INP_REG(row, addr)                    (INDOOR_MACH_INDOOR_TEMPER_INP_REG + INDOOR_STATUS_INFO_OFFSET(row, addr))

/* 室内机温度传感器状态信息 */
#define INDOOR_MACH_TEMPER_SENSOR_100_INP_REG                           (32006)
#define INDOOR_MACH_TEMPER_SENSOR_INP_REG(row, addr)                    (INDOOR_MACH_TEMPER_SENSOR_INP_REG + INDOOR_STATUS_INFO_OFFSET(row, addr))

/* 室内机换气模式信息 */
#define INDOOR_MACH_VENTILATION_MODE_100_INP_REG                        (32804)
#define INDOOR_MACH_VENTILATION_MODE_INP_REG(row, addr)                 (INDOOR_MACH_VENTILATION_MODE_100_INP_REG + INDOOR_STATUS1_INFO_OFFSET(row, addr))

/* 室内机风向2信息 */
#define INDOOR_MACH_WIND_DIRECTION2_100_INP_REG                         (34001)
#define INDOOR_MACH_WIND_DIRECTION2_INP_REG(row, addr)                  (INDOOR_MACH_WIND_DIRECTION2_100_INP_REG + INDOOR_STATUS2_INFO_OFFSET(row, addr))

/* 室内机设定运转模式详细信息 */
#define INDOOR_MACH_OPERATION_DETAILS_100_INP_REG                       (34002)
#define INDOOR_MACH_OPERATION_DETAILS_INP_REG(row, addr)                (INDOOR_MACH_OPERATION_DETAILS_100_INP_REG + INDOOR_STATUS2_INFO_OFFSET(row, addr))
/* ------------------------------end------------------------------ */
/* ********************************************************************************************************************************** */
/* ----------------------------------------------------------------end--------------------------------------------------------------- */
/* ********************************************************************************************************************************** */




/* ********************************************************************************************************************************** */
/* ----------------------------------------------------保持寄存器(holding register)---------------------------------------------------- */
/* ********************************************************************************************************************************** */
#define ADAPTOR_INIT_STATUS_REG                                         (40001)

/* -------------------------室内机控制寄存器------------------------- */
/* 1-00 ~ 1-15 */
/* 2-00 ~ 2-15 */
/* 3-00 ~ 3-15 */
/* 4-00 ~ 4-15 */
#define INDOOR_MACH_C_OFFSET                                            (3)
#define INDOOR_MACH_CTRL_OFFSET(row, addr)                              (((row - 1) * INDOOR_MACH_ONE_ROW_SIZE + addr) * INDOOR_MACH_C_OFFSET)
#define INDOOR_MACH_CTRL1_OFFSET(row, addr)                             (((row - 1) * INDOOR_MACH_ONE_ROW_SIZE + addr) * (INDOOR_MACH_C_OFFSET + 1))
#define INDOOR_MACH_CTRL2_OFFSET(row, addr)                             (((row - 1) * INDOOR_MACH_ONE_ROW_SIZE + addr) * (INDOOR_MACH_C_OFFSET - 1))

/* 室内机风量风向控制寄存器 */
#define INDOOR_MACH_CTRL_WIND_100_HOLD_REG                              (42001)
#define INDOOR_MACH_CTRL_WIND_HOLD_REG(row, addr)                       (INDOOR_MACH_CTRL_WIND_100_HOLD_REG + INDOOR_MACH_CTRL_OFFSET(row, addr))

/* 室内机运转模式控制寄存器 */
#define INDOOR_MACH_OPERATION_MODE_CTRL_100_HOLD_REG                    (42002)
#define INDOOR_MACH_OPERATION_MODE_CTRL_HOLD_REG(row, addr)             (INDOOR_MACH_OPERATION_MODE_CTRL_100_HOLD_REG + INDOOR_MACH_CTRL_OFFSET(row, addr))

/* 室内机温度设定寄存器 */
#define INDOOR_MACH_TEMPER_SET_100_HOLD_REG                             (42003)
#define INDOOR_MACH_TEMPER_SET_HOLD_REG(row, addr)                      (INDOOR_MACH_TEMPER_SET_100_HOLD_REG + INDOOR_MACH_CTRL_OFFSET(row, addr))

/* 室内机换气模式切换寄存器 */
#define INDOOR_MACH_VENTILATION_MODE_100_HOLD_REG                       (42404)
#define INDOOR_MACH_ENTILATION_MODE_HOLD_REG(row, addr)                 (INDOOR_MACH_VENTILATION_MODE_100_HOLD_REG + INDOOR_MACH_CTRL1_OFFSET(row, addr))

/* 室内机风向2设定寄存器 */
#define INDOOR_MACH_WIND_DIRECTION2_100_HOLD_REG                       (44001)
#define INDOOR_MACH_WIND_DIRECTION2_HOLD_REG(row, addr)                (INDOOR_MACH_WIND_DIRECTION2_100_HOLD_REG + INDOOR_MACH_CTRL2_OFFSET(row, addr))

/* 室内机风向2设定寄存器 */
#define INDOOR_MACH_OPERATION_DETAILS_100_HOLD_REG                      (44002)
#define INDOOR_MACH_OPERATION_DETAILS_HOLD_REG(row, addr)               (INDOOR_MACH_OPERATION_DETAILS_100_HOLD_REG + INDOOR_MACH_CTRL2_OFFSET(row, addr))
/* ------------------------------end------------------------------ */
/* ********************************************************************************************************************************** */
/* ----------------------------------------------------------------end--------------------------------------------------------------- */
/* ********************************************************************************************************************************** */




/* ********************************************************************************************************************************** */
/* ---------------------------------------------------输入寄存器详情(input register Details)------------------------------------------- */
/* ********************************************************************************************************************************** */

/* -------------------------------------------------------------- */
/* REGISTER 寄存器30001 */
/* -------------------------------------------------------------- */

/* 适配器状态 0:为准备好, 1: 已准备好*/
#define ADAPTRO_STATUS_REG_Pos                  (0)                                         /* !< ADAPTOR STATUS         */
#define ADAPTRO_STATUS_REG_Msk                  (0x1ul << ADAPTRO_STATUS_REG_Pos)           /* !< ADAPTOR STATUS         */

/* DIII集中机器有无连接 0:无, 1:有 */
#define ADAPTRO_DIII_CONT_STA_REG_Pos           (1)                                         /*!< ADAPTOR DIII CONNECTION STATUS */
#define ADAPTRO_DIII_CONT_STA_REG_Msk           (0x1ul << ADAPTRO_DIII_CONT_STA_REG_Pos)    /*!< ADAPTOR DIII CONNECTION STATUS */


/* -------------------------------------------------------------- */
/* 室内机的连接状态 寄存器30002-30005 */
/* -------------------------------------------------------------- */

/* 连接状态 0:未连接，1:已连接 */
/* 室内机的通信状态30006-30009 */
/* 连接状态 0:正常，1:通信异常 */
#define INDOOR_MACH_DIII_ADDR_0_Pos             (0)                                         /* !< indoor machine 0 status */
#define INDOOR_MACH_DIII_ADDR_0_Msk             (0x1ul << INDOOR_MACH_DIII_ADDR_0_Pos)      /* !< indoor machine 0 status */

#define INDOOR_MACH_DIII_ADDR_1_Pos             (1)                                         /* !< indoor machine 1 status */
#define INDOOR_MACH_DIII_ADDR_1_Msk             (0x1ul << INDOOR_MACH_DIII_ADDR_1_Pos)      /* !< indoor machine 1 status */

#define INDOOR_MACH_DIII_ADDR_2_Pos             (2)                                         /* !< indoor machine 2 status */
#define INDOOR_MACH_DIII_ADDR_2_Msk             (0x1ul << INDOOR_MACH_DIII_ADDR_2_Pos)      /* !< indoor machine 2 status */

#define INDOOR_MACH_DIII_ADDR_3_Pos             (3)                                         /* !< indoor machine 3 status */
#define INDOOR_MACH_DIII_ADDR_3_Msk             (0x1ul << INDOOR_MACH_DIII_ADDR_3_Pos)      /* !< indoor machine  3status */

#define INDOOR_MACH_DIII_ADDR_4_Pos             (4)                                         /* !< indoor machine 4 status */
#define INDOOR_MACH_DIII_ADDR_4_Msk             (0x1ul << INDOOR_MACH_DIII_ADDR_4_Pos)      /* !< indoor machine 4 status */

#define INDOOR_MACH_DIII_ADDR_5_Pos             (5)                                         /* !< indoor machine 5 status */
#define INDOOR_MACH_DIII_ADDR_5_Msk             (0x1ul << INDOOR_MACH_DIII_ADDR_5_Pos)      /* !< indoor machine 5 status */

#define INDOOR_MACH_DIII_ADDR_6_Pos             (6)                                         /* !< indoor machine 6 status */
#define INDOOR_MACH_DIII_ADDR_6_Msk             (0x1ul << INDOOR_MACH_DIII_ADDR_6_Pos)      /* !< indoor machine 6 status */

#define INDOOR_MACH_DIII_ADDR_7_Pos             (7)                                         /* !< indoor machine 7 status */
#define INDOOR_MACH_DIII_ADDR_7_Msk             (0x1ul << INDOOR_MACH_DIII_ADDR_7_Pos)      /* !< indoor machine 7 status */

#define INDOOR_MACH_DIII_ADDR_8_Pos             (8)                                         /* !< indoor machine 8 status */
#define INDOOR_MACH_DIII_ADDR_8_Msk             (0x1ul << INDOOR_MACH_DIII_ADDR_8_Pos)      /* !< indoor machine 8 status */

#define INDOOR_MACH_DIII_ADDR_9_Pos             (9)                                         /* !< indoor machine 9 status */
#define INDOOR_MACH_DIII_ADDR_9_Msk             (0x1ul << INDOOR_MACH_DIII_ADDR_9_Pos)      /* !< indoor machine 9 status */

#define INDOOR_MACH_DIII_ADDR_10_Pos            (10)                                        /* !< indoor machine 10 status */
#define INDOOR_MACH_DIII_ADDR_10_Msk            (0x1ul << INDOOR_MACH_DIII_ADDR_10_Pos)     /* !< indoor machine 10 status */

#define INDOOR_MACH_DIII_ADDR_11_Pos            (11)                                        /* !< indoor machine 11 status */
#define INDOOR_MACH_DIII_ADDR_11_Msk            (0x1ul << INDOOR_MACH_DIII_ADDR_11_Pos)     /* !< indoor machine 11 status */

#define INDOOR_MACH_DIII_ADDR_12_Pos            (12)                                        /* !< indoor machine 12 status */
#define INDOOR_MACH_DIII_ADDR_12_Msk            (0x1ul << INDOOR_MACH_DIII_ADDR_12_Pos)     /* !< indoor machine 12 status */

#define INDOOR_MACH_DIII_ADDR_13_Pos            (13)                                        /* !< indoor machine 13 status */
#define INDOOR_MACH_DIII_ADDR_13_Msk            (0x1ul << INDOOR_MACH_DIII_ADDR_13_Pos)     /* !< indoor machine 13 status */

#define INDOOR_MACH_DIII_ADDR_14_Pos            (14)                                        /* !< indoor machine 14 status */
#define INDOOR_MACH_DIII_ADDR_14_Msk            (0x1ul << INDOOR_MACH_DIII_ADDR_14_Pos)     /* !< indoor machine 14 status */

#define INDOOR_MACH_DIII_ADDR_15_Pos            (15)                                        /* !< indoor machine 15 status */
#define INDOOR_MACH_DIII_ADDR_15_Msk            (0x1ul << INDOOR_MACH_DIII_ADDR_15_Pos)     /* !< indoor machine 15 status */


/* -------------------------------------------------------------- */
/* 模式设置，风向风量设定 寄存器31001,31004,....,31190 */
/* -------------------------------------------------------------- */

/* 0:无，1:有 */
/* 送风模式 */
#define INDOOR_MACH_AIR_SUPPLY_MODE_Pos                 (0)                                                     /* !< indoor machine set air supply mode*/
#define INDOOR_MACH_AIR_SUPPLY_MODE_Msk                 (0x1ul << INDOOR_MACH_AIR_SUPPLY_MODE_Pos)              /* !< indoor machine set air supply mode*/

/* 制冷模式 */
#define INDOOR_MACH_REFRIGERATION_MODE_Pos              (1)                                                     /* !< indoor machine Refrigeration mode */
#define INDOOR_MACH_REFRIGERATION_MODE_Msk              (0x1ul << INDOOR_MACH_REFRIGERATION_MODE_Pos)           /* !< indoor machine Refrigeration mode */

/* 制热模式 */
#define INDDOR_MACH_HEAT_MODE_Pos                       (2)                                                     /* !< indoor machine heating mode */
#define INDDOR_MACH_HEAT_MODE_Msk                       (0x1ul << INDDOR_MACH_HEAT_MODE_Pos)                    /* !< indoor machine heating mode */

/* 自动模式 */
#define INDOOR_MACH_AUTO_MODE_Pos                       (3)                                                     /* !< indoor machine automatic mode */
#define INDOOR_MACH_AUTO_MODE_Msk                       (0x1ul << INDOOR_MACH_AUTO_MODE_Pos)                    /* !< indoor machine automatic mode */

/* 除湿模式 */
#define INDOOR_MACH_DEHUMIDIFICATION_MODE_Pos           (4)                                                     /* !< indoor machine Dehumidification mode */
#define INDOOR_MACH_DEHUMIDIFICATION_MODE_Msk           (0x1ul << INDOOR_MACH_DEHUMIDIFICATION_MODE_Pos)        /* !< indoor machine Dehumidification mode */

/* 风量自动 */
#define INDOOR_MACH_AUTO_AIR_VOL_Pos                    (6)                                                     /* !< indoor machine auto air vol */
#define INDOOR_MACH_AUTO_AIR_VOL_Msk                    (0x1ul << INDOOR_MACH_AUTO_AIR_VOL_Pos)                 /* !< indoor machine auto air vol */

/* 风向自动 */
#define INDOOR_MACH_AUTO_WIND_DIR_Pos                   (7)                                                     /* !< indoor machine auto wind direction */
#define INDOOR_MACH_AUTO_WIND_DIR_Msk                   (0x1ul << INDOOR_MACH_AUTO_WIND_DIR_Pos)                /* !< indoor machine auto wind direction */

/* 风向叶片位置数设定 */
/* [10:8] */
/* ------------------------------------------------ */
/* |    值    |   0   | 1 | 2 | 3 | 4 | 5 | 6 | 7 | */
/* ------------------------------------------------ */
/* | 叶片位置数 | 固定  | 1 | 2 | 3 | 4 | 5 | - | - | */
/* ------------------------------------------------ */
#define INDOOR_MACH_POSITION_NUMBER_OF_WIND_BLADE_Pos   (8)                                                         /* !< indoor machine position number of wind blade */
#define INDOOR_MACH_POSITION_NUMBER_OF_WIND_BLADE_Msk   (0x7ul << INDOOR_MACH_POSITION_NUMBER_OF_WIND_BLADE_Pos)    /* !< indoor machine position number of wind blade */

/* 风向设定 */
#define INDOOR_MACH_WIND_DIRECTION_SET_Pos              (11)                                                    /* !< indoor machine wind direction set */
#define INDOOR_MACH_WIND_DIRECTION_SET_Msk              (0x1ul << INDOOR_MACH_WIND_DIRECTION_SET_Pos)           /* !< indoor machine wind direction set */

/* 风量/换气风量的风速设定 */
/* [14:12] */
/* ---------------------------------------- */
/* |  值  | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | */
/* ---------------------------------------- */
/* | 风速 | 0  | 1 | 2 | 3 | 4 | 5 | - | - | */
/* ---------------------------------------- */
#define INDOOR_MACH_WIND_SEPPD_SET_Pos                  (12)                                                    /* !< indoor machine wind speed set */
#define INDOOR_MACH_WIND_SEPPD_SET_Msk                  (0x7ul << INDOOR_MACH_WIND_SEPPD_SET_Pos)               /* !< indoor machine wind speed set */

/* 风量调节 */
#define INDOOR_MACH_AIR_VOLUME_REGULATION_Pos           (15)                                                    /* !< indoor machine air volume regulation */  
#define INDOOR_MACH_AIR_VOLUME_REGULATION_Msk           (0x1ul << INDOOR_MACH_AIR_VOLUME_REGULATION_Pos)        /* !< indoor machine air volume regulation */


/* -------------------------------------------------------------- */
/* 室内机的制冷设定温度上限和下限 寄存器31002,31005,....,31191*/
/* -------------------------------------------------------------- */

/* 制冷设定温度上限 (-128-127摄氏度)*/
/* [7:0] */
#define INDOOR_MACH_REF_SET_TEMPER_UPPER_LIMIT_Pos      (0)                                                     /* !< indoor machine Upper limit of temperature for refrigeration */
#define INDOOR_MACH_REF_SET_TEMPER_UPPER_LIMIT_Mak      (0xfful << INDOOR_MACH_REF_SET_TEMPER_UPPER_LIMIT_Pos)  /* !< indoor machine Upper limit of temperature for refrigeration */

/* 制冷设定温度下限 (-128-127摄氏度) */
/* [15:8] */
#define INDOOR_MACH_REF_SET_TEMPER_LOWER_LIMIT_Pos      (8)                                                     /* !< indoor machine Lower limit of temperature for refrigeration */
#define INDOOR_MACH_REF_SET_TEMPER_LOWER_LIMIT_Mak      (0xfful << INDOOR_MACH_REF_SET_TEMPER_LOWER_LIMIT_Pos)  /* !< indoor machine Lower limit of temperature for refrigeration */


/* -------------------------------------------------------------- */
/* 室内机的制热设定温度上限和下限 寄存器31003,31006,....,31192*/
/* -------------------------------------------------------------- */

/* 制热设定温度上限 (-128-127摄氏度) */
/* [7:0] */
#define INDOOR_MACH_HEAT_SET_TEMPER_UPPER_LIMIT_Pos     (0)                                                     /* !< indoor machine Upper limit of heating set temperature */
#define INDOOR_MACH_HEAT_SET_TEMPER_UPPER_LIMIT_Mak     (0xfful << INDOOR_MACH_HEAT_SET_TEMPER_UPPER_LIMIT_Pos) /* !< indoor machine Upper limit of heating set temperature */

/* 制热设定温度下限 (-128-127摄氏度) */
/* [15:8] */
#define INDOOR_MACH_HEAT_SET_TEMPER_LOWER_LIMIT_Pos     (8)                                                     /* !< indoor machine Lower limit of heating set temperature */
#define INDOOR_MACH_HEAT_SET_TEMPER_LOWER_LIMIT_Mak     (0xfful << INDOOR_MACH_HEAT_SET_TEMPER_LOWER_LIMIT_Pos) /* !< indoor machine Lower limit of heating set temperature */


/* -------------------------------------------------------------- */
/* 室内机各种空气设定 寄存器31404,31408....,31656  */
/* -------------------------------------------------------------- */

/* 通常模式 */
#define INDOOR_MACH_NORMAL_MODE_Pos                     (0)                                                     /* !< indoor machine normal mode */
#define INDOOR_MACH_NORMAL_MODE_Msk                     (0x1ul << INDOOR_MACH_NORMAL_MODE_Pos)                  /* !< indoor machine normal mode */

/* 自动换气 */
#define INDOOR_MACH_AUTO_VENTILATION_Pos                (1)                                                     /* !< indoor machine auto Ventilation */
#define INDOOR_MACH_AUTO_VENTILATION_Msk                (0x1ul << INDOOR_MACH_VENTILATION_Pos)                  /* !< indoor machine auto Ventilation */

/* 再热除湿 */
#define INDOOR_MACH_REHEAT_DEHUMIDIFICATION_Pos         (8)                                                     /* !< indoor machine reheat dehumidification mode */
#define INDOOR_MACH_REHEAT_DEHUMIDIFICATION_Msk         (0x1ul << INDOOR_MACH_REHEAT_DEHUMIDIFICATION_Pos)      /* !< indoor machine reheat dehumidification mode */

/* 自动除湿 */
#define INDOOR_MACH_AUTO_DEHUMIDIFICATION_Pos           (9)                                                     /* !< indoor machine Automatic dehumidification mode */
#define INDOOR_MACH_AUTO_DEHUMIDIFICATION_Msk           (0x1ul << INDOOR_MACH_AUTO_DEHUMIDIFICATION_Pos)        /* !< indoor machine Automatic dehumidification mode */

/* 舒适睡眠 */
#define INDOOR_MACH_COMFORTABLE_SLEEP_Pos               (10)                                                    /* !< indoor machine confortable sleep mode */
#define INDOOR_MACH_COMFORTABLE_SLEEP_Msk               (0xful << INDOOR_MACH_COMFORTABLE_SLEEP_Pos)            /* !< indoor machine confortable sleep mode */

/* 干燥模式 */
#define INDOOR_MACH_DRING_MODE_Pos                      (11)                                                    /* !< indoor machine dring mode */
#define INDOOR_MACH_DRING_MODE_Msk                      (0x1ul << INDOOR_MACH_DRING_MODE_Pos)                   /* !< indoor machine dring mode */

/* 预备暖房 */
#define INDOOR_MACH_PREPARE_GREENHOUSE_Pos              (12)                                                    /* !< indoor machine prepare greenhouse mode */
#define INDOOR_MACH_PREPARE_GREENHOUSE_Msk              (0x1ul << INDOOR_MACH_PREPARE_GREENHOUSE_Pos)           /* !< indoor machine prepare greenhouse mode */

/* 浴室换气 */
#define INDOOR_MACH_BATHROOM_VENTILATION_Pos            (13)                                                    /* !< indoor machine Bathroom ventilation mode */
#define INDOOR_MACH_BATHROOM_VENTILATION_Msk            (0x1ul << INDOOR_MACH_BATHROOM_VENTILATION_Pos)         /* !< indoor machine Bathroom ventilation mode */

/* 湿度设定 */
#define INDOOR_MACH_HUMIDITY_SETTING_Pos                (14)                                                    /* !< indoor machine Humidity setting mode */
#define INDOOR_MACH_HUMIDITY_SETTING_Msk                (0x1ul << INDOOR_MACH_HUMIDITY_SETTING_Pos)             /* !< indoor machine Humidity setting mode */

/* 风向2 */
#define INDOOR_MACH_WIND_DIRECTION2_Pos                 (15)                                                    /* !< indoor machine Wind direction mode */
#define INDOOR_MACH_WIND_DIRECTION2_Msk                 (0x1ul << INDOOR_MACH_WIND_DIRECTION2_Pos)              /* !< indoor machine Wind direction mode */


/* -------------------------------------------------------------- */
/* 风向设置 寄存器32001,32007,....,32379 */
/* -------------------------------------------------------------- */

/* 运转/暂停状态 */
#define INDOOR_MACH_FAN_WORK_STATUS_Pos                 (0)                                                     /* !< indoor machine fan work status */
#define INDOOR_MACH_FAN_WORK_STATUS_Msk                 (0x1ul << INDOOR_MACH_FAN_WORK_STATUS_Pos)              /* !< indoor machine fan work status */

/* 强制停止状态 */
#define INDOOR_MACH_FORCED_STOP_STATE_Pos               (2)                                                     /* !< indoor machine forced stop state */
#define INDOOR_MACH_FORCED_STOP_STATE_Msk               (0x1ul << INDOOR_MACH_FORCED_STOP_STATE_Pos)            /* !< indoor machine forced stop state */

/* 方向(0-7) */
/* [10:8] */
/* --------------------------------------------------- */
/* |  值  |  0  |  1  |  2  |  3  |  4  | 5 | 6 | 7  | */
/* --------------------------------------------------- */
/* | 内容 | P0  | P1  | P2  | P3  | P4  | - | - | 摆动 |*/
/* --------------------------------------------------- */
/* P0:水平防线， P4:垂直方向 */
#define INDOOR_MACH_WIND_DIRECTION_Pos                  (8)                                                     /* !< indoor machine wind direction */
#define INDOOR_MACH_WIND_DIRECTION_Msk                  (0x7ul << INDOOR_MACH_WIND_DIRECTION_Pos)               /* !< indoor machine wind direction */

/* 风向自动 */
#define INDOOR_MACH_AUTO_WIND_DIRECTION_Pos             (11)                                                    /* !< indoor machine auto wind direction */
#define INDOOR_MACH_AUTO_WIND_DIRECTION_Msk             (0x1ul << INDOOR_MACH_AUTO_WIND_DIRECTION_Pos)          /* !< indoor machine auto wind direction */

/* 风量/换气量(1-7) 根据室内机性能不同，风量的风速设定叶也不一样 */
/* [14:12] */
/* 风量设定段数为0时 */
/* ---------------------------------------- */
/* |  值  | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | */
/* ---------------------------------------- */
/* | 内容  | - | - | - | - | - | - | - | - |*/
/* ---------------------------------------- */
/* 风量设定段数为1时 */
/* ---------------------------------------- */
/* |  值  | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | */
/* ---------------------------------------- */
/* | 内容  | - | - | - | - | - | H | - | - |*/
/* ---------------------------------------- */
/* 风量设定段数为2时 */
/* ------------------------------------------ */
/* |  值  |  0  | 1 | 2 | 3 | 4 | 5 | 6 | 7 | */
/* ------------------------------------------ */
/* | 内容 | 自动 | L | - | - | - | H | - | - | */
/* ------------------------------------------ */
/* 风量设定段数为3时 */
/* ------------------------------------------ */
/* |  值  |  0  | 1 | 2 | 3 | 4 | 5 | 6 | 7 | */
/* ------------------------------------------ */
/* | 内容 | 自动 | L | - | M | - | H | - | - | */
/* ------------------------------------------ */
/* 风量设定段数为2时 */
/* ------------------------------------------- */
/* |  值  |  0  | 1  | 2 | 3 | 4 | 5  | 6 | 7 | */
/* ------------------------------------------- */
/* | 内容 | 自动 | LL | L | M | H | HH | - | - | */
/* ------------------------------------------- */
#define INDOOR_MACH_AIR_VOLUME_Pos                      (12)                                                    /* !< indoor machine air volume*/
#define INDOOR_MACH_AIR_VOLUME_Msk                      (0x7ul << INDOOR_MACH_AIR_VOLUME_Pos)                   /* !< indoor machine air volume*/


/* -------------------------------------------------------------- */
/* 室内机运行状态 32002,32008,....,32380 */
/* -------------------------------------------------------------- */

/* 运行模式(0-15) */
/* [3:0] */
/* -------------------------------------------------------------- */
/* |  值  |  0  |  1  |  2   |  3  |    4   | 5～6 |  7  | 8～15 | */
/* -------------------------------------------------------------- */
/* | 内容 | 送风 | 暖房 | 冷房 | 自动 | 换气清洁 |  -  | 除湿 |   -  | */
/* -------------------------------------------------------------- */
#define INDOOR_MACH_RUNNING_MODE_Pos                    (0)                                                     /* !< indoor machine Running mode */
#define INDOOR_MACH_RUNNING_MODE_Msk                    (0xful << INDOOR_MACH_RUNNING_MODE_Pos)                 /* !< indoor machine Running mode */

/* 过滤信号状态(0或者1-15) */
/* 0:OFF，1-15:ON */
/* [7:4] */
#define INDOOR_MACH_FILTERED_SIGNAL_STATUS_Pos          (4)                                                     /* !< indoor machine Filtered signal status */
#define INDOOR_MACH_FILTERED_SIGNAL_STATUS_Msk          (0xful << INDOOR_MACH_FILTERED_SIGNAL_STATUS_Pos)       /* !< indoor machine Filtered signal status */

/* 运转模式(0-15) */
/* [11:8] */
/* --------------------------------- */
/* |  值  |  0  |  1  |  2  | 3～15 | */
/* --------------------------------- */
/* | 内容 | 送风 | 暖房 | 冷房 |  -  |  */
/* --------------------------------- */
#define INDOOR_MACH_OPERATION_MODE_Pos                  (8)                                                     /* !< indoor machine Operation mode */
#define INDOOR_MACH_OPERATION_MODE_Msk                  (0xful << INDOOR_MACH_OPERATION_MODE_Pos)               /* !< indoor machine Operation mode */

/* 冷/热选择权(0-2) */
/* 0:VRV空调系统的冷/暖选择权还没有决定，2:它可以改变VRV空调系统室内机的制冷/制热模式。 */
/* [7:6] */
/* ------------------------------ */
/* |     值     |  0  | 1 |  2  | */
/* ------------------------------ */
/* | 冷/热选择权 | 未定 | 无 | 有  | */
/* ------------------------------ */
#define INDOOR_MACH_COLD_OR_HOT_OPTION_Pos              (14)                                                    /* !< indoor machine Cold / hot option */
#define INDOOR_MACH_COLD_OR_HOT_OPTION_Msk              (0x3ul << INDOOR_MACH_COLD_OR_HOT_OPTION_Pos)           /* !< indoor machine Cold / hot option */


/* -------------------------------------------------------------- */
/* 设定温度 寄存器32003,32009,....,32381 */
/* -------------------------------------------------------------- */

/* 设定温度 (-127.9-127.9摄氏度)*/
/* 以0.1摄氏度为单位。此值乘以10以一个整数数值存储 */
#define INDOOR_MACH_SET_TEMPERATURE_Pos                 (0)                                                     /* !< indoor machine set temperature */
#define INDOOR_MACH_SET_TEMPERATURE_Msk                 (0xfffful << INDOOR_MACH_SET_TEMPERATURE_Pos)           /* !< indoor machine set temperature */


/* -------------------------------------------------------------- */
/* 异常状态 寄存器32004,32010,....,32382 */
/* -------------------------------------------------------------- */

/* 异常代码映射值1 (0-15) */
/* [3:0] */
/* --------------------------------------- */
/* | 值1 | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | */
/* --------------------------------------- */
/* | 代码 | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | */
/* --------------------------------------- */
/* -------------------------------------------- */
/* | 值1 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | */
/* -------------------------------------------- */
/* | 代码 | 8 | 9 | A  | H | C  | J  | E  | F  | */
/* -------------------------------------------- */
#define INDOOR_MACH_EXCEPTION_CODE_MAPPING1_Pos         (0)                                                     /* !< indoor machine exception code mapping1 */
#define INDOOR_MACH_EXCEPTION_CODE_MAPPING1_Msk         (0xful << INDOOR_MACH_EXCEPTION_CODE_MAPPING1_Pos)      /* !< indoor machine exception code mapping1 */

/* 异常代码映射值2 (0-31) */
/* [8:4] */
/* ---------------------------------------------------------------------------- */
/* | 值2 | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | */
/* ---------------------------------------------------------------------------- */
/* | 代码 | 0 | A | C | E | H | F | J | L | P | U | 9  | 8 |  7 |  6 |  5 |  4 | */
/* ---------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------- */
/* | 值2 | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 28 | 29 | 30 | 31 | */
/* -------------------------------------------------------------------------------------- */
/* | 代码 | 3 |  2 |  1 |  G |  K |  M |  N |  R |  T |  V |  W |  X |  Y |  Z |  * |     |*/
/* -------------------------------------------------------------------------------------- */
#define INDOOR_MACH_EXCEPTION_CODE_MAPPING2_Pos         (4)                                                     /* indoor machine exception code mapping2 */
#define INDOOR_MACH_EXCEPTION_CODE_MAPPING2_Msk         (0x1ful << INDOOR_MACH_EXCEPTION_CODE_MAPPING2_Pos)     /* indoor machine exception code mapping2 */

/* 故障 */
#define INDOOR_MACH_MALFUNCTION_Pos                     (9)                                                     /* !< indoor machine malfunction */
#define INDOOR_MACH_MALFUNCTION_Msk                     (0x1ul << INDOOR_MACH_MALFUNCTION_Pos)                  /* !< indoor machine malfunction */

/* 警告 */  
#define INDOOR_MACH_WARNING_Pos                         (10)                                                    /* !< indoor machine warning */
#define INDOOR_MACH_WARNING_Msk                         (0x1ul << INDOOR_MACH_WARNING_Pos)                      /* !< indoor machine warning */


/* -------------------------------------------------------------- */
/* 室内温度 寄存器32005,32011,....,32383 */
/* -------------------------------------------------------------- */

/* 室内温度 (-511.9-511.9摄氏度) */
/* [15:0] */
/* 以0.1摄氏度为单位。此值乘以10以一个整数数值存储 */
#define INDOOR_MACH_INDOOR_TEMPERATURE_Pos              (0)                                                     /* !< indoor machine Indoor temperature */
#define INDOOR_MACH_INDOOR_TEMPERATURE_Msk              (0xffff  << INDOOR_MACH_INDOOR_TEMPERATURE_Pos)         /* !< indoor machine Indoor temperature */


/* -------------------------------------------------------------- */
/* 室内机温度传感器 寄存器32006,32012,....,32384 */
/* -------------------------------------------------------------- */

/* 室内机温度传感器异常 */
#define INDOOR_MACH_TEMPERATURE_SENSOR_ABNORMAL_Pos     (8)                                                     /* !< indoor machine temperature sensot Abnormal */
#define INDOOR_MACH_TEMPERATURE_SENSOR_ABNORMAL_Msk     (0x1ul << INDOOR_MACH_TEMPERATURE_SENSOR_ABNORMAL_Pos)  /* !< indoor machine temperature sensot Abnormal */\

/* 室内机温度传感器数据接收 */
#define INDOOR_MACH_TEMPERATURE_SENSOR_RECEIVE_DATA_Pos (15)                                                        /* !< indoor machine temperature sensot Receive data */
#define INDOOR_MACH_TEMPERATURE_SENSOR_RECEIVE_DATA_Msk (0x1ul << INDOOR_MACH_TEMPERATURE_SENSOR_RECEIVE_DATA_Pos)  /* !< indoor machine temperature sensot Receive data */


/* -------------------------------------------------------------- */
/* 换气模式 寄存器32804,32808,....,33056 */
/* -------------------------------------------------------------- */

/* 换气模式(0-3) */
/* [7:6] */
/* -------------------------------------------- */
/* |  值  |   0   |    1   |    2    |    3   | */
/* -------------------------------------------- */
/* | 模式 |  NON  | 自动换气 | 全热换气 | 普通换气 | */
/* -------------------------------------------- */
#define INDOOR_MACH_VENTILATION_MODE_Pos                (6)                                                         /* !< indoor machine Ventilation mode */
#define INDOOR_MACH_VENTILATION_MODE_Msk                (0x3ul << INDOOR_MACH_VENTILATION_MODE_Pos)                 /* !< indoor machine Ventilation mode */


/* -------------------------------------------------------------- */
/* 风向2设定 寄存器34001,34003,....,34127 */
/* -------------------------------------------------------------- */

/* 方向2设定 (0-7) */
/* [10:8] */
/* --------------------------------------------------- */
/* |  值  |  0  |  1  |  2  |  3  |  4  | 5 | 6 | 7  | */
/* --------------------------------------------------- */
/* | 内容 | P0  | P1  | P2  | P3  | P4  | - | - | 摆动 |*/
/* --------------------------------------------------- */
/* P0:水平防线， P4:垂直方向 */
#define INDOOR_MACH_WIND_DIRECTION2_SET_Pos             (8)                                                     /* !< indoor machine wind direction2 */
#define INDOOR_MACH_WIND_DIRECTION2_SET_Msk             (0x7ul << INDOOR_MACH_WIND_DIRECTION2_SET_Pos)          /* !< indoor machine wind direction2 */


/* -------------------------------------------------------------- */
/* 运行模式设定 寄存器34002,34004,....,341281 */
/* -------------------------------------------------------------- */

/* 设定运转模式信息 (1-15) */
/* [3:0] */
/* -------------------------------------------------------------- */
/* |  值  |  0  |  1  |  2   |  3  |    4   | 5～6 |  7  | 8～15 | */
/* -------------------------------------------------------------- */
/* | 模式 | 送风 | 暖房 | 冷房 | 自动 | 换气清洁 |  -  | 除湿 |   -  | */
/* -------------------------------------------------------------- */
#define INDOOR_MACH_SET_RUNNING_MODE_INFO_Pos           (0)                                                     /* !< indoor machine set running mode */
#define INDOOR_MACH_SET_RUNNING_MODE_INFO_Msk           (0xful << INDOOR_MACH_SET_RUNNING_MODE_INFO_Pos)        /* !< indoor machine set running mode */

/* 设定运转模式详细信息 (0-7) */
/* [6:4] */
/* ------------------------------------------------------------------ */
/* |  值  |  0  |    1    |    2    |    3    |  4  |    5    | 6～7 | */
/* ------------------------------------------------------------------ */
/* | 模式 | 无效 | 再热除湿  | 自动除湿 | 贴心睡眠 | 干燥 | 预备暖房 |  -  | */
/* ------------------------------------------------------------------ */
#define INDOOR_MACH_SET_RUNNING_MODE_DETAILS_Pos        (4)                                                     /* !< indoor machine set running mode Details */
#define INDOOR_MACH_SET_RUNNING_MODE_DETAILS_Msk        (0x7ul << INDOOR_MACH_SET_RUNNING_MODE_DETAILS_Pos)     /* !< indoor machine set running mode Details */

/* 外机运转模式状态信息 (0-15) */
/* [11:8] */
/* --------------------------------- */
/* |  值  |  0  |  1  |  2  | 3～15 | */
/* --------------------------------- */
/* | 内容 | 送风 | 暖房 | 冷房 |  -  |  */
/* --------------------------------- */
#define OUTDOOR_MACH_OPERATION_MODE_INFO_Pos            (8)                                                     /* !< outdoor machine operation mode */
#define OUTDOOR_MACH_OPERATION_MODE_INFO_Msk            (0xful << OUTDOOR_MACH_OPERATION_MODE_INFO_Pos)         /* !< outdoor machine operation mode */

/* 设定湿度状态信息 (0-3) */
/* [13:12] */
/* ------------------------ */
/* |  值  | 0 | 1 | 2 | 3 | */
/* ------------------------ */
/* | 内容 | L | M | H | - |  */
/* ------------------------ */    
#define INDOOR_MACH_SET_HUMIDITY_INFO_Pos               (12)                                                    /* !< indoor machine set humidity */
#define INDOOR_MACH_SET_HUMIDITY_INFO_Msk               (0x7ul << INDOOR_MACH_SET_HUMIDITY_INFO_Pos)            /* !< indoor machine set humidity */

/* 浴室换气状态信息 (0-3) */
/* [15:14] */
/* ---------------------------- */
/* |  值  |  0  | 1  | 2  | 3 | */
/* ---------------------------- */
/* | 内容 | 停止 | 弱 | 强 | - |  */
/* ---------------------------- */ 
#define INDOOR_MACH_BATHROOM_VENTILATION_INFO_Pos       (14)                                                    /* !< indoor machine Bathroom ventilation */
#define INDOOR_MACH_BATHROOM_VENTILATION_INFO_Msk       (0x7ul << INDOOR_MACH_BATHROOM_VENTILATION_INFO_Pos)    /* !< indoor machine Bathroom ventilation */

/* ********************************************************************************************************************************** */
/* ----------------------------------------------------------------end--------------------------------------------------------------- */
/* ********************************************************************************************************************************** */




/* ********************************************************************************************************************************** */
/* --------------------------------------------------保持寄存器详情(holding register Details)------------------------------------------ */
/* ********************************************************************************************************************************** */

/* -------------------------------------------------------------- */
/* 适配器初始化状态 寄存器40001 */
/* 此寄存器不需要特别设置位默认即可 */
/* *此寄存器的值存储在EEPROM里，因此当适配器掉电时此值不会被丢失。 */
/* *当这个寄存器的值改变时，重启适配器电源后新的值才能被应用。 */
/* -------------------------------------------------------------- */

/* DIII的地址范围 1-00 ～ 4-15 */
/* [3:0] */
#define ADAPTOR_INIT_STATUS_DIII_ADDRESS_RANGE_Pos      (0)                                                     /* !< adaptor init status DIII machine Address range */
#define ADAPTOR_INIT_STATUS_DIII_ADDRESS_RANGE_Msk      (0xful << ADAPTOR_INIT_STATUS_DIII_ADDRESS_RANGE_Pos)   /* !< adaptor init status DIII machine Address range */

/* DIII电源供给有无 */
#define ADAPTOR_INIT_STATUS_DIII_POWER_Pos              (8)                                                     /* !< adaptor init status DIII whether Power on */
#define ADAPTOR_INIT_STATUS_DIII_POWER_Msk              (0x1ul << ADAPTOR_INIT_STATUS_DIII_POWER_Pos)           /* !< adaptor init status DIII whether Power on */

/* Auto 地址开始禁止 */
/* 向许可要求发送禁止信息 */
#define ADAPTOR_INIT_STATUS_AUTO_ADDR_START_BAN_Pos     (9)                                                     /* !< adaptor init status auto Address Start Ban*/
#define ADAPTOR_INIT_STATUS_AUTO_ADDR_START_BAN_Msk     (0x1ul << ADAPTOR_INIT_STATUS_AUTO_ADDR_START_BAN_Pos)  /* !< adaptor init status auto Address Start Ban*/

/* DIII网络通信开始/停止标志 */
/* 初始化值为1 */
#define ADAPTOR_INIT_STATUS_DIII_MACH_NETWORK_FLAG_Pos  (15)                                                        /* !< adaptor init status network communication start or stop flag*/
#define ADAPTOR_INIT_STATUS_DIII_MACH_NETWORK_FLAG_Msk  (0x1ul << ADAPTOR_INIT_STATUS_DIII_MACH_NETWORK_FLAG_Pos)   /* !< adaptor init status network communication start or stop flag*/

/* --------------------------------------------------室内机控制------------------------------------------ */

/* -------------------------------------------------------------- */
/* 室内机风量控制 寄存器42001,42004,....,42190 */
/* -------------------------------------------------------------- */

/* 运转或者停止 */
#define INDOOR_MACH_CTRL_START_OR_STOP_Pos              (0)                                                     /* !< indoor machine control start or stop */
#define INDOOR_MACH_CTRL_START_OR_STOP_Msk              (0x1ul << INDOOR_MACH_CTRL_START_OR_STOP_Pos)           /* !< indoor machine control start or stop */

/* 风向风量设定运转模式 (0-15) */
/* [7:4] */
/* -------------------------------------------------------------------- */
/* |  值  |  0  |  1  |  2   |  3  |    4    | 5 |   6   |  7  | 8～15 | */
/* -------------------------------------------------------------------- */
/* | 内容 | 送风 | 暖房 | 冷房 | 自动 | 换气清洁 | - |  温调  | 除湿 |   -  | */
/* -------------------------------------------------------------------- */
#define INDOOR_MACH_CTRL_WIND_DIR_OR_VOL_SET_MODE_Pos   (4)                                                         /* !< indoor machine control wind direction or air volume set mode */
#define INDOOR_MACH_CTRL_WIND_DIR_OR_VOL_SET_MODE_Msk   (0xful << INDOOR_MACH_CTRL_WIND_DIR_OR_VOL_SET_MODE_Pos)    /* !< indoor machine control wind direction or air volume set mode */

/* 风向 (0-7) */
/* 控制室内机的风向位置 */
/* [10:8] */
/* --------------------------------------------------- */
/* |  值  |  0  |  1  |  2  |  3  |  4  | 5 | 6 | 7  | */
/* --------------------------------------------------- */
/* | 内容 | P0  | P1  | P2  | P3  | P4  | - | - | 摆动 |*/
/* --------------------------------------------------- */
/* P0:水平防线， P4:垂直方向 */
#define INDOOR_MACH_CTRL_WIND_DIR_SET_Pos               (8)                                                     /* !< indoor machine control wind direction */
#define INDOOR_MACH_CTRL_WIND_DIR_SET_Msk               (0x7ul << INDOOR_MACH_CTRL_WIND_DIR_SET_Pos)            /* !< indoor machine control wind direction */

/* 风向自动 */
#define INDOOR_MACH_CTRL_AUTO_WIND_Pos                  (11)                                                    /* !< indoor machine control auto wind */
#define INDOOR_MACH_CTRL_AUTO_WIND_Msk                  (0x1ul << INDOOR_MACH_CTRL_AUTO_WIND_Pos)               /* !< indoor machine control auto wind */

/* 风量设定/换气量设定 */
/* [14:12] */
/* 风量设定段数为0时 */
/* ---------------------------------------- */
/* |  值  |  0  | 1 | 2 | 3 | 4 | 5 | 6 | 7 | */
/* ---------------------------------------- */
/* | 内容  | *注 | - | - | - | - | - | - | - |*/
/* ---------------------------------------- */
/* 风量设定段数为1时 */
/* ---------------------------------------- */
/* |  值  | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | */
/* ---------------------------------------- */
/* | 内容  | - | - | - | - | - | H | - | - |*/
/* ---------------------------------------- */
/* 风量设定段数为2时 */
/* ------------------------------------------ */
/* |  值  |  0  | 1 | 2 | 3 | 4 | 5 | 6 | 7 | */
/* ------------------------------------------ */
/* | 内容 | 自动 | L | - | - | - | H | - | - | */
/* ------------------------------------------ */
/* 风量设定段数为3时 */
/* ------------------------------------------ */
/* |  值  |  0  | 1 | 2 | 3 | 4 | 5 | 6 | 7 | */
/* ------------------------------------------ */
/* | 内容 | 自动 | L | - | M | - | H | - | - | */
/* ------------------------------------------ */
/* 风量设定段数为2时 */
/* ------------------------------------------- */
/* |  值  |  0  | 1  | 2 | 3 | 4 | 5  | 6 | 7 | */
/* ------------------------------------------- */
/* | 内容 | 自动 | LL | L | M | H | HH | - | - | */
/* ------------------------------------------- */
/* [-]的值Don't care (不进行设定) */
/* *注：风量设定段数为0时，表明无设定风量、则值设定为0 */
#define INDOOR_MACH_CTRL_AIR_OR_VENTILATION_VOL_Pos     (12)                                                    /* !< indoor machine control air or ventilation volume */
#define INDOOR_MACH_CTRL_AIR_OR_VENTILATION_VOL_Msk     (0x7ul << INDOOR_MACH_CTRL_AIR_OR_VENTILATION_VOL_Pos)  /* !< indoor machine control air or ventilation volume */


/* -------------------------------------------------------------- */
/* 运行、运转设定 寄存器42002,42005,....,42191 */
/* -------------------------------------------------------------- */

/* 设定运转模式 (0-15) */
/* [3:0] */
/* -------------------------------------------------------------------- */
/* |  值  |  0  |  1  |  2   |  3  |    4    | 5 |   6   |  7  | 8～15 | */
/* -------------------------------------------------------------------- */
/* | 内容 | 送风 | 暖房 | 冷房 | 自动 | 换气清洁 | - |  温调  | 除湿 |   -  | */
/* -------------------------------------------------------------------- */
#define INDOOR_MACH_CTRL_SET_OPERATION_MODE_Pos         (0)                                                     /* indoor machine control set operation mode */
#define INDOOR_MACH_CTRL_SET_OPERATION_MODE_Msk         (0xful << INDOOR_MACH_CTRL_SET_OPERATION_MODE_Pos)      /* indoor machine control set operation mode */

/* 过滤信号复位 (0或者15) */
/* [7:4] */
/* 0:无，15:复位 */
/* 注意：本适配器无论是否有室内机过滤信号，HA系统都会如上所示将过滤信号重置从0写成1，然后将过滤信号重置发至室内机。 */
/* HA系统会监控32002寄存器所对应的过滤重置，仅在当过滤信号为1时，过滤信号重置中会写入1。 */
/* 另外，当过滤信号为0时，为了下次发送重置而要写入0。 */
#define INDOOR_MACH_CTRL_FILTER_SIGNAL_RESET_Pos        (4)                                                     /* !< indoor machine control Filter signal reset */
#define INDOOR_MACH_CTRL_FILTER_SIGNAL_RESET_Msk        (0xful << INDOOR_MACH_CTRL_FILTER_SIGNAL_RESET_Pos)     /* !< indoor machine control Filter signal reset */

/* 运行模式 (0-15) */
/* [8:11] */
/* 向室内机发送 */
/* 如果，本寄存器中设定一下数值，就会将Modbus的写入命令当做不当数据反馈例外响应。 */
/* *比如设定了【-】的值时。 */
/* --------------------------------- */
/* |  值  |  0  |  1  |  2  | 3～15 | */
/* --------------------------------- */
/* | 内容 | 送风 | 暖房 | 冷房 |  -  |  */
/* --------------------------------- */
#define INDOOR_MACH_CTRL_RUNNING_MODE_SET_Pos           (8)                                                     /* !< indoor machine control running mode set */
#define INDOOR_MACH_CTRL_RUNNING_MODE_SET_Msk           (0xful << INDOOR_MACH_CTRL_RUNNING_MODE_SET_Msk)        /* !< indoor machine control running mode set */


/* -------------------------------------------------------------- */
/* 温度的设定 寄存器42003,42006,....,42192 */
/* -------------------------------------------------------------- */

/* 设定温度 (-127.9-127.9) */
/* [15:0] */
#define INDOOR_MACH_CTRL_TEMPERATURE_SET_Pos            (0)                                                     /* !< indoor machine control temperature */
#define INDOOR_MACH_CTRL_TEMPERATURE_SET_Msk            (0xfffful << (INDOOR_MACH_CTRL_TEMPERATURE_SET_Pos))    /* !< indoor machine control temperature */


/* -------------------------------------------------------------- */
/* 设置换气模式 寄存器42404,42408,....,42656 */
/* -------------------------------------------------------------- */
/* 换气模式设置 (0-3) */
/* [7:6] */
/* -------------------------------------------- */
/* |  值  |   0   |    1   |    2    |    3   | */
/* -------------------------------------------- */
/* | 模式 |  NON  | 自动换气 | 全热换气 | 普通换气 | */
/* -------------------------------------------- */
#define INDOOR_MACH_CTRL_VENTILATION_MODE_Pos           (6)                                                 /* !< indoor machine control ventilation mode*/
#define INDOOR_MACH_CTRL_VENTILATION_MODE_Msk           (0x3ul << INDOOR_MACH_CTRL_VENTILATION_MODE_Pos)    /* !< indoor machine control ventilation mode*/


/* -------------------------------------------------------------- */
/* 设置方向2 寄存器44001,44003,....,44127 */
/* -------------------------------------------------------------- */

/* 方向2设定 (0-7) */
/* [10:8] */
/* --------------------------------------------------- */
/* |  值  |  0  |  1  |  2  |  3  |  4  | 5 | 6 | 7  | */
/* --------------------------------------------------- */
/* | 内容 | P0  | P1  | P2  | P3  | P4  | - | - | 摆动 |*/
/* --------------------------------------------------- */
#define INDOOR_MACH_CTRL_WIND_DIRECTION2_SET_Pos        (8)                                                 /* !< indoor machine control wind direction2 set*/
#define INDOOR_MACH_CTRL_WIND_DIRECTION2_SET_Msk        (0x7ul << INDOOR_MACH_CTRL_WIND_DIRECTION2_SET_Pos) /* !< indoor machine control wind direction2 set*/


/* -------------------------------------------------------------- */
/* 运转 运行模式 寄存器44002,44004,....,44128 */
/* -------------------------------------------------------------- */

/* 设定运转模式信息 (1-15) */
/* [3:0] */
/* -------------------------------------------------------------- */
/* |  值  |  0  |  1  |  2   |  3  |    4   | 5～6 |  7  | 8～15 | */
/* -------------------------------------------------------------- */
/* | 模式 | 送风 | 暖房 | 冷房 | 自动 | 换气清洁 |  -  | 除湿 |   -  | */
/* -------------------------------------------------------------- */
#define INDOOR_MACH_CTRL_SET_RUNNING_MODE_Pos               (0)                                                     /* !< indoor machine control set running mode */
#define INDOOR_MACH_CTRL_SET_RUNNING_MODE_Msk               (0xful << INDOOR_MACH_CTRL_SET_RUNNING_MODE_Pos)        /* !< indoor machine control set running mode */

/* 设定运转模式详细信息 (0-7) */
/* [6:4] */
/* ------------------------------------------------------------------ */
/* |  值  |  0  |    1    |    2    |    3    |  4  |    5    | 6～7 | */
/* ------------------------------------------------------------------ */
/* | 模式 | 无效 | 再热除湿  | 自动除湿 | 贴心睡眠 | 干燥 | 预备暖房 |  -  | */
/* ------------------------------------------------------------------ */
#define INDOOR_MACH_CTRL_SET_RUNNING_MODE_DETAILS_Pos       (4)                                                        /* !< indoor machine control set running mode Details */
#define INDOOR_MACH_CTRL_SET_RUNNING_MODE_DETAILS_Msk       (0x7ul << INDOOR_MACH_CTRL_SET_RUNNING_MODE_DETAILS_Pos)   /* !< indoor machine control set running mode Details */

/* 外机运转模式状态信息 (0-15) */
/* [11:8] */
/* --------------------------------- */
/* |  值  |  0  |  1  |  2  | 3～15 | */
/* --------------------------------- */
/* | 内容 | 送风 | 暖房 | 冷房 |  -  |  */
/* --------------------------------- */
#define OUTDOOR_MACH_CTRL_OPERATION_MODE_Pos                 (8)                                                        /* !< outdoor machine control operation mode */
#define OUTDOOR_MACH_CTRL_OPERATION_MODE_Msk                 (0xful << OUTDOOR_MACH_CTRL_OPERATION_MODE_Pos)            /* !< outdoor machine control operation mode */

/* 设定湿度状态信息 (0-3) */
/* [13:12] */
/* ------------------------ */
/* |  值  | 0 | 1 | 2 | 3 | */
/* ------------------------ */
/* | 内容 | L | M | H | - |  */
/* ------------------------ */    
#define INDOOR_MACH_CTRL_SET_HUMIDITY_Pos                    (12)                                                       /* !< indoor machine control set humidity */
#define INDOOR_MACH_CTRL_SET_HUMIDITY_Msk                    (0x7ul << INDOOR_MACH_CTRL_SET_HUMIDITY_Pos)               /* !< indoor machine control set humidity */

/* 浴室换气状态信息 (0-3) */
/* [15:14] */
/* ---------------------------- */
/* |  值  |  0  | 1  | 2  | 3 | */
/* ---------------------------- */
/* | 内容 | 停止 | 弱 | 强 | - |  */
/* ---------------------------- */ 
#define INDOOR_MACH_CTRL_BATHROOM_VENTILATION_Pos            (14)                                                       /* !< indoor machine control Bathroom ventilation */
#define INDOOR_MACH_CTRL_BATHROOM_VENTILATION_Msk            (0x7ul << INDOOR_MACH_CTRL_BATHROOM_VENTILATION_Pos)       /* !< indoor machine control Bathroom ventilation */

/* ********************************************************************************************************************************** */
/* ----------------------------------------------------------------end--------------------------------------------------------------- */
/* ********************************************************************************************************************************** */





#endif // !_ADAPTOR_REG_H