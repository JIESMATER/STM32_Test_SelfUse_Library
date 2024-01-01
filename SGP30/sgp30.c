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
#include "sgp30.h"

sgp30_data_t sgp30_data;

/**
 * @brief	��SGP30����һ��ָ��(16bit)
 * @param	cmd SGP30ָ��
 * @retval	�ɹ�����HAL_OK
*/
static uint8_t sgp30_send_cmd(sgp30_cmd_t cmd)
{
    uint8_t cmd_buffer[2];
    cmd_buffer[0] = cmd >> 8;
    cmd_buffer[1] = cmd;
    return HAL_I2C_Master_Transmit(&SGP30_I2C_Handle_Name, SGP30_ADDR_WRITE, (uint8_t*) cmd_buffer, 2, 0xFFFF);
}

/**
 * @brief	��λSGP30
 * @param	none
 * @retval	�ɹ�����HAL_OK
*/
static int sgp30_soft_reset(void)
{
    uint8_t cmd = 0x06;
    return HAL_I2C_Master_Transmit(&SGP30_I2C_Handle_Name, 0x00, &cmd, 1, 0xFFFF);
}

/**
 * @brief	��ʼ��SGP30������������ģʽ
 * @param	none
 * @retval	�ɹ�����0��ʧ�ܷ���-1
*/
int sgp30_init(void)
{
    int status;
    
    status = sgp30_soft_reset();
    if (status != HAL_OK) {
        return -1;
    }
    
    HAL_Delay(100);
    
    status = sgp30_send_cmd(INIT_AIR_QUALITY);
    
    HAL_Delay(100);
    
    return status == 0 ? 0 : -1;
}

/**
 * @brief	��ʼ��SGP30������������ģʽ
 * @param	none
 * @retval	�ɹ�����HAL_OK
*/
static int sgp30_start(void)
{
    return sgp30_send_cmd(MEASURE_AIR_QUALITY);
}

#define CRC8_POLYNOMIAL 0x31

static uint8_t CheckCrc8(uint8_t* const message, uint8_t initial_value)
{
    uint8_t  remainder;	    //����
    uint8_t  i = 0, j = 0;  //ѭ������

    /* ��ʼ�� */
    remainder = initial_value;

    for(j = 0; j < 2;j++)
    {
        remainder ^= message[j];

        /* �����λ��ʼ���μ���  */
        for (i = 0; i < 8; i++)
        {
            if (remainder & 0x80)
            {
                remainder = (remainder << 1)^CRC8_POLYNOMIAL;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }
    }

    /* ���ؼ����CRC�� */
    return remainder;
}

/**
 * @brief	��ȡһ�ο�����������
 * @param	none
 * @retval	�ɹ�����0��ʧ�ܷ���-1
*/
int sgp30_read(void)
{
    int status;
    uint8_t recv_buffer[6]={0};
    
    /* �������� */
    status = sgp30_start();
    if (status != 0) {
        printf("sgp30 start fail\r\n");
        return -1;
    }
    
    HAL_Delay(100);
    
    /* ��ȡ�������� */
    status = HAL_I2C_Master_Receive(&SGP30_I2C_Handle_Name, SGP30_ADDR_READ, (uint8_t*)recv_buffer, 6, 0xFFFF);
    if (status != HAL_OK) {
        printf("I2C Master Receive fail\r\n");
        return -1;
    }
    
    /* У����յĲ������� */
    if (CheckCrc8(&recv_buffer[0], 0xFF) != recv_buffer[2]) {
        printf("co2 recv data crc check fail\r\n");
        return -1;
    }
    if (CheckCrc8(&recv_buffer[3], 0xFF) != recv_buffer[5]) {
        printf("tvoc recv data crc check fail\r\n");
        return -1;
    }
    
    
    /* ת���������� */
    sgp30_data.co2  = recv_buffer[0] << 8 | recv_buffer[1];
    sgp30_data.tvoc = recv_buffer[3] << 8 | recv_buffer[4];
    
    return 0;
}
