#include "sccb.h"
#include "ov7725.h"
#include "ov7725_reg.h"


uint8_t set_pixformat()
{
    uint8_t reg;
    uint8_t ret = rdSensorReg8_8(COM7, &reg);

	reg = COM7_SET_FMT(reg, COM7_FMT_YUV);
	ret |= wrSensorReg8_8(DSP_CTRL4, DSP_CTRL4_YUV_RGB);

    // Write back register
    return wrSensorReg8_8(COM7, reg) | ret;
}

uint8_t set_framesize()
{
    uint8_t reg;
    uint8_t ret=0;
    uint16_t w = 320;
    uint16_t h = 240;
    bool vflip;

    // Write MSBs
    ret |= wrSensorReg8_8(HOUTSIZE, w>>2);
    ret |= wrSensorReg8_8(VOUTSIZE, h>>1);

    // Write LSBs
    ret |= wrSensorReg8_8(EXHCH, ((w&0x3) | ((h&0x1) << 2)));

    // Sample VFLIP
    ret |= rdSensorReg8_8(COM3, &reg);
    vflip = reg & COM3_VFLIP;
    ret |= rdSensorReg8_8(HREF, &reg);
    ret |= wrSensorReg8_8(HREF, (reg & 0xBF) | (vflip ? 0x40 : 0x00));

	// Set QVGA Resolution
	ret = rdSensorReg8_8(COM7, &reg);
	reg = COM7_SET_RES(reg, COM7_RES_QVGA);
	ret |= wrSensorReg8_8(COM7, reg);

	// Set QVGA Window Size
	ret |= wrSensorReg8_8(HSTART, 0x3F);
	ret |= wrSensorReg8_8(HSIZE,  0x50);
	ret |= wrSensorReg8_8(VSTART, 0x03 - vflip);
	ret |= wrSensorReg8_8(VSIZE,  0x78);

	// Enable auto-scaling/zooming factors
	ret |= wrSensorReg8_8(DSPAUTO, 0xFF);

    return ret;
}
