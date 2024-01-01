/**********************************************************
 *@file    sgp30.h
 *@brief
 *         ����STM32 HAL ���SGP30 ��ȩ����������
 *
 *@author  mculover666<2412828003@qq.com>
 *@date    2020/07/23
 *@note
 *         1. ����Ĭ��ʹ��Ӳ��i2c 1
 *         2. �������Ҫ��ӡ������Ϣ������ȥ��printf���
**********************************************************/

#ifndef _SGP30_H_
#define _SGP30_H_

#include "stm32l4xx.h"
#include <stdio.h>

#define SGP30_ADDR          0x58
#define	SGP30_ADDR_WRITE	SGP30_ADDR<<1       //0xb0
#define	SGP30_ADDR_READ		(SGP30_ADDR<<1)+1   //0xb1

/* SGP30�������ӵ�Ӳ��I2C�ӿ� */
#define SGP30_I2C_Handle_Name   hi2c1

typedef struct sgp30_data_st {
    uint16_t co2;
    uint16_t tvoc;
}sgp30_data_t;

typedef enum sgp30_cmd_en {
    /* ��ʼ�������������� */
    INIT_AIR_QUALITY = 0x2003,
    
    /* ��ʼ������������ */
    MEASURE_AIR_QUALITY = 0x2008
    
} sgp30_cmd_t;

extern I2C_HandleTypeDef SGP30_I2C_Handle_Name;
extern sgp30_data_t      sgp30_data;

int sgp30_init(void);
int sgp30_read(void);


#endif /* _SGP30_H_ */
