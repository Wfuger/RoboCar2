#include "sccb.h"
#include "cambus.h"
#include "ov7725.h"
#include "ov7725_reg.h"


uint8_t set_pixformat(I2C_HandleTypeDef *i2c)
{
    uint8_t reg;
    uint8_t ret = cambus_readb(i2c, COM7, &reg);

	reg = COM7_SET_FMT(reg, COM7_FMT_YUV);
	ret |= cambus_writeb(i2c, DSP_CTRL4, DSP_CTRL4_YUV_RGB);

    // Write back register
    return cambus_writeb(i2c, COM7, reg) | ret;
}

uint8_t set_framesize(I2C_HandleTypeDef *i2c)
{
    uint8_t reg;
    uint8_t ret=0;
    uint16_t w = 320;
    uint16_t h = 240;
    bool vflip;

    // Write MSBs
    ret |= cambus_writeb(i2c, HOUTSIZE, w>>2);
    ret |= cambus_writeb(i2c, VOUTSIZE, h>>1);

    // Write LSBs
    ret |= cambus_writeb(i2c, EXHCH, ((w&0x3) | ((h&0x1) << 2)));

    // Sample VFLIP
    ret |= cambus_readb(i2c, COM3, &reg);
    vflip = reg & COM3_VFLIP;
    ret |= cambus_readb(i2c, HREF, &reg);
    ret |= cambus_writeb(i2c, HREF, (reg & 0xBF) | (vflip ? 0x40 : 0x00));

	// Set QVGA Resolution
	ret = cambus_readb(i2c, COM7, &reg);
	reg = COM7_SET_RES(reg, COM7_RES_QVGA);
	ret |= cambus_writeb(i2c, COM7, reg);

	// Set QVGA Window Size
	ret |= cambus_writeb(i2c, HSTART, 0x3F);
	ret |= cambus_writeb(i2c, HSIZE,  0x50);
	ret |= cambus_writeb(i2c, VSTART, 0x03 - vflip);
	ret |= cambus_writeb(i2c, VSIZE,  0x78);

	// Enable auto-scaling/zooming factors
	ret |= cambus_writeb(i2c, DSPAUTO, 0xFF);

    return ret;
}
