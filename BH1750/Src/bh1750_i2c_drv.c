#include "bh1750_i2c_drv.h"
#include "i2c.h"


/**
 * @brief	向BH1750发送一条指令
 * @param	cmd —— BH1750工作模式指令（在BH1750_MODE中枚举定义）
 * @retval	成功返回HAL_OK
*/
uint8_t	BH1750_Send_Cmd(BH1750_MODE cmd)
{
	return HAL_I2C_Master_Transmit(&hi2c2, BH1750_ADDR_WRITE, (uint8_t*)&cmd, 1, 0xFFFF);
}

/**
 * @brief	从BH1750接收一次光强数据
 * @param	dat —— 存储光照强度的地址（两个字节数组）
 * @retval	成功 —— 返回HAL_OK
*/
uint8_t BH1750_Read_Dat(uint8_t* dat)
{
	return HAL_I2C_Master_Receive(&hi2c2, BH1750_ADDR_READ, dat, 2, 0xFFFF);
}


/**
 * @brief	将BH1750的两个字节数据转换为光照强度值（0-65535）
 * @param	dat  —— 存储光照强度的地址（两个字节数组）
 * @retval	成功 —— 返回光照强度值
*/
uint16_t BH1750_Dat_To_Lux(uint8_t* dat)
{
	uint16_t lux = 0;
	lux = dat[0];
	lux <<= 8;
	lux += dat[1];
	lux = (int)(lux / 1.2);
	
	return lux;
}
uint16_t BH1750_Get_LuxValue(uint8_t* dat)
{
	if(HAL_OK == BH1750_Send_Cmd(ONCE_H_MODE))
	{
		HAL_Delay(200);
	if(HAL_OK == BH1750_Read_Dat(dat))
	{
	return BH1750_Dat_To_Lux(dat);
	}else
{return 0;
}
	}

	else
	{
	return 0;
	}
}
