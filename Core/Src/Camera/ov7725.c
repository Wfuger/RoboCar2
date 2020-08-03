#include "sccb.h"
#include "cambus.h"
#include "ov7725.h"
#include "ov7725_reg.h"

uint8_t set_pixformat()
{
    uint8_t reg;
    uint8_t ret = sccb_read(COM7, &reg);

	reg = COM7_SET_FMT(reg, COM7_FMT_YUV);
	ret |= sccb_write(DSP_CTRL4, DSP_CTRL4_YUV_RGB);

    // Write back register
    return sccb_write(COM7, reg) | ret;
}

uint8_t set_framesize()
{
    uint8_t reg;
    uint8_t ret=0;
    uint16_t w = 320;
    uint16_t h = 240;
    bool vflip;

    // Write MSBs
    ret |= sccb_write(HOUTSIZE, w>>2);
    ret |= sccb_write(VOUTSIZE, h>>1);

    // Write LSBs
    ret |= sccb_write(EXHCH, ((w&0x3) | ((h&0x1) << 2)));

    // Sample VFLIP
    ret |= sccb_read(COM3, &reg);
    vflip = reg & COM3_VFLIP;
    ret |= sccb_read(HREF, &reg);
    ret |= sccb_write(HREF, (reg & 0xBF) | (vflip ? 0x40 : 0x00));

	// Set QVGA Resolution
	ret = sccb_read(COM7, &reg);
	reg = COM7_SET_RES(reg, COM7_RES_QVGA);
	ret |= sccb_write(COM7, reg);

	// Set QVGA Window Size
	ret |= sccb_write(HSTART, 0x3F);
	ret |= sccb_write(HSIZE,  0x50);
	ret |= sccb_write(VSTART, 0x03 - vflip);
	ret |= sccb_write(VSIZE,  0x78);

	// Enable auto-scaling/zooming factors
	ret |= sccb_write(DSPAUTO, 0xFF);

    return ret;
}


//uint8_t set_pixformat(I2C_HandleTypeDef *i2c)
//{
//    uint8_t reg;
//    uint8_t ret = cambus_readb(i2c, COM7, &reg);
//
//	reg = COM7_SET_FMT(reg, COM7_FMT_YUV);
//	ret |= cambus_writeb(i2c, DSP_CTRL4, DSP_CTRL4_YUV_RGB);
//
//    // Write back register
//    return cambus_writeb(i2c, COM7, reg) | ret;
//}
//
//uint8_t set_framesize(I2C_HandleTypeDef *i2c)
//{
//    uint8_t reg;
//    uint8_t ret=0;
//    uint16_t w = 320;
//    uint16_t h = 240;
//    bool vflip;
//
//    // Write MSBs
//    ret |= cambus_writeb(i2c, HOUTSIZE, w>>2);
//    ret |= cambus_writeb(i2c, VOUTSIZE, h>>1);
//
//    // Write LSBs
//    ret |= cambus_writeb(i2c, EXHCH, ((w&0x3) | ((h&0x1) << 2)));
//
//    // Sample VFLIP
//    ret |= cambus_readb(i2c, COM3, &reg);
//    vflip = reg & COM3_VFLIP;
//    ret |= cambus_readb(i2c, HREF, &reg);
//    ret |= cambus_writeb(i2c, HREF, (reg & 0xBF) | (vflip ? 0x40 : 0x00));
//
//	// Set QVGA Resolution
//	ret = cambus_readb(i2c, COM7, &reg);
//	reg = COM7_SET_RES(reg, COM7_RES_QVGA);
//	ret |= cambus_writeb(i2c, COM7, reg);
//
//	// Set QVGA Window Size
//	ret |= cambus_writeb(i2c, HSTART, 0x3F);
//	ret |= cambus_writeb(i2c, HSIZE,  0x50);
//	ret |= cambus_writeb(i2c, VSTART, 0x03 - vflip);
//	ret |= cambus_writeb(i2c, VSIZE,  0x78);
//
//	// Enable auto-scaling/zooming factors
//	ret |= cambus_writeb(i2c, DSPAUTO, 0xFF);
//
//    return ret;
//}
