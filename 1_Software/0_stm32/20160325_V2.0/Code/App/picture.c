/**
*************************************************************************************************
* @file    picture.c
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.21
* @brief   OLED显示应用层用到的图片、字体等的取模数组
*************************************************************************************************
@verbatim 

- 版本号       日期      创建人        描述 
  V1.0      2016.03.21   Ge Yue    OLED显示应用层用到的图片、字体等的取模数组; 

- 版本号       日期      修改人        描述 
@endverbatim 
*/

/**
* @defgroup WIDGET Widget: APP Group
* @{
*/

/************************************6*8的点阵************************************/
/** @brief LED图标 */
const unsigned char gImage_LED[512] = { 
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X80,0XC0,0XE0,0XE0,0XF0,0XF0,0XF8,0XF8,0XF8,0XF8,0XF8,
0XF8,0XF8,0XF8,0XF8,0XF8,0XF0,0XF0,0XE0,0XE0,0XC0,0X80,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0XF8,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X03,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC1,0X1F,0XFB,0XFB,
0XFF,0XFB,0XFB,0X3F,0XC1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X7F,0X07,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X03,0X07,0X1F,0X7F,0XFF,0XFF,0XFF,0XFC,0XC1,0X1F,
0XFF,0X3F,0XC3,0XF8,0XFF,0XFF,0XFF,0X7F,0X1F,0X07,0X01,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X03,0XF7,0XF7,0XF7,0XF6,
0XF1,0XF4,0XF7,0XF7,0XE7,0X07,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0B,0X0B,0X2B,0X2B,
0X2B,0X2B,0X2B,0X0B,0X0B,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X08,0XF8,0XF8,0X08,0X00,0X00,0X00,0X00,0X08,0XF8,0XF8,0X88,
0X88,0X18,0X00,0X08,0XF8,0XF8,0X08,0X08,0X18,0XF0,0XF0,0XC0,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X08,0X0F,0X0F,0X08,0X08,0X08,0X0C,0X00,0X08,0X0F,0X0F,0X08,
0X08,0X08,0X00,0X08,0X0F,0X0F,0X08,0X08,0X0C,0X07,0X07,0X01,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};

/** @brief 设置图标 */
const unsigned char gImage_SETTING[512] = { 
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XF8,0XF8,0XF8,0XF8,0XF8,0XF8,0XF8,
0XF8,0XF8,0XF8,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0XC0,0XF0,0XFC,0XFC,0XFC,
0XFC,0XF8,0XF8,0XF8,0XF8,0X78,0X3C,0X9C,0XDE,0XCF,0XEF,0XEF,0XE7,0XE7,0XE7,0XE7,
0XEF,0XEF,0XCF,0XDE,0X9C,0X38,0X78,0XF8,0XF8,0XF8,0XF8,0XFC,0XFC,0XFC,0XFC,0XF0,
0XC0,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0X07,0X07,0X0F,0X0F,0XFF,
0XFF,0XFF,0X0F,0X03,0XF8,0XFE,0XFF,0XFF,0XFF,0X1F,0X0F,0X07,0X03,0X03,0X03,0X03,
0X07,0X0F,0X1F,0XFF,0XFF,0XFF,0XFE,0XF8,0X03,0X0F,0XFF,0XFF,0XFF,0X0F,0X0F,0X07,
0X07,0X03,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X70,0XF8,0XF8,0XFC,0XFC,0XFF,
0XFF,0XFF,0XFC,0XF0,0XC7,0X9F,0X3F,0X7F,0XFF,0XFE,0XFC,0XF8,0XF0,0XF0,0XF0,0XF0,
0XF8,0XFC,0XFE,0XFF,0X7F,0X3F,0X9F,0XC7,0XF0,0XFC,0XFF,0XFF,0XFF,0XFC,0XFC,0XF8,
0XF8,0X70,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0X0F,0X0F,0X0F,
0X0F,0X07,0X07,0X07,0X07,0X07,0X07,0X0E,0X1E,0XFC,0XFD,0XFD,0XF9,0XF9,0XF9,0XF9,
0XFD,0XFD,0XFC,0X1E,0X0E,0X07,0X07,0X07,0X07,0X07,0X07,0X0F,0X0F,0X0F,0X0F,0X03,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X07,0X07,0X07,0X07,0X07,0X07,0X07,
0X07,0X07,0X07,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X1C,0X32,0X32,0X62,0X64,0XCE,0X00,0X02,0XFE,0XFE,0X22,
0X72,0X06,0X00,0X06,0X02,0X02,0XFE,0XFE,0X02,0X06,0X00,0X06,0X02,0X02,0XFE,0XFE,
0X02,0X06,0X00,0X02,0XFE,0XFE,0X02,0X00,0XFE,0X0E,0X1C,0X38,0X70,0XE2,0XFE,0X02,
0X00,0XFC,0XFC,0X02,0X02,0X42,0XC4,0XCE,0X40,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X01,0X02,0X02,0X02,0X02,0X01,0X00,0X02,0X03,0X03,0X02,
0X02,0X03,0X00,0X00,0X00,0X02,0X03,0X03,0X02,0X00,0X00,0X00,0X00,0X02,0X03,0X03,
0X02,0X00,0X00,0X02,0X03,0X03,0X02,0X00,0X03,0X02,0X00,0X00,0X00,0X01,0X03,0X00,
0X00,0X01,0X01,0X02,0X02,0X02,0X03,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};

/** @brief 信息图标 */
const unsigned char gImage_INFO[512] = { 
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,
0X80,0XC0,0XE0,0XF0,0X70,0X70,0X38,0X38,0X1C,0X1C,0X1C,0X9C,0X9E,0X8E,0X8E,0X8E,
0X8E,0X9E,0X9C,0X1C,0X1C,0X3C,0X38,0X38,0X70,0X70,0XE0,0XE0,0XC0,0X80,0X80,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XC0,0XF0,0XF8,0X7C,0X1F,0X0F,
0X07,0XC3,0XE1,0XF0,0XF8,0XFC,0XFE,0XFE,0XFE,0XFF,0XFF,0XFF,0X9F,0X0F,0X07,0X07,
0X0F,0X0F,0XFF,0XFF,0XFF,0XFE,0XFE,0XFE,0XFC,0XF8,0XF0,0XE1,0XC3,0X87,0X0F,0X3F,
0X7C,0XF8,0XF0,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XE0,0XFF,0XFF,0X3F,0X01,0X00,0XE0,0XFC,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC3,0X03,0X02,0X02,
0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0XF0,
0X00,0X03,0XFF,0XFF,0XFE,0XE0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XFF,0XFF,0XFC,0XC0,0X00,0X07,0X3F,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,
0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X7F,0X0F,
0X00,0XC0,0XFF,0XFF,0X7F,0X07,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0X0F,0X1F,0X7E,0XF8,0XF0,
0XE0,0XC3,0X87,0X0F,0X1F,0X3F,0X3F,0X7F,0X7F,0X7F,0XFF,0XFF,0XFF,0XF0,0XF0,0XF0,
0XF0,0XFF,0XFF,0XFF,0XFF,0X7F,0X7F,0X3F,0X3F,0X1F,0X8F,0X87,0XC3,0XE1,0XF8,0XFC,
0X3F,0X1F,0X07,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,
0X03,0X03,0X07,0X0F,0X0F,0X1E,0X1C,0X3C,0X3C,0X38,0X38,0X78,0X78,0X79,0X79,0X79,
0X79,0X78,0X78,0X38,0X38,0X3C,0X3C,0X1E,0X1E,0X0F,0X07,0X07,0X03,0X01,0X01,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X10,0XF0,0XF0,0X10,0X10,0XF0,0X70,0XF0,0XC0,0X80,0X10,0XF0,0X10,0X10,0XF0,
0XF0,0X10,0X90,0X10,0X10,0X80,0XE0,0XE0,0X30,0X10,0X10,0X30,0XE0,0XE0,0X80,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X10,0X1F,0X1F,0X10,0X10,0X1F,0X10,0X00,0X01,0X07,0X0F,0X1F,0X00,0X10,0X1F,
0X1F,0X11,0X03,0X00,0X00,0X01,0X0F,0X0F,0X18,0X10,0X10,0X18,0X0F,0X0F,0X03,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};

/** @brief LED禁止图标 */
const unsigned char gImage_LED_DISABLE[512] = { 
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0XC0,0XC0,0XE0,0XF0,0X78,0X38,0X3C,
0X1C,0X1C,0X0E,0X8E,0X8E,0XC7,0XC7,0XC7,0XC7,0XC7,0XC7,0XC7,0XC7,0XC7,0X8E,0X0E,
0X0E,0X1C,0X1C,0X3C,0X38,0X78,0XF0,0XE0,0XC0,0XC0,0X80,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0XE0,0XF8,0XFC,0X3F,0X0F,0X1F,0X3F,0X7F,0XFE,0XFC,0XF8,0XF0,
0XFC,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFC,0XF8,0XF0,0X00,0X00,0X00,0X01,0X03,0X07,0X0F,0X3E,0XFC,0XF8,0XE0,0X80,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0XF8,0XFF,0XFF,0X07,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X0F,
0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XFF,0XF7,0XF7,0XC3,0XFF,0XFF,0XFF,
0XFF,0XFF,0X7F,0X0F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X07,0XFF,0XFF,
0XF8,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X0F,0X7F,0XFF,0XF0,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X01,0X07,0X1F,0X7F,0XFF,0XFF,0XFF,0X03,0X7F,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF1,0XE0,0XC0,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0XF0,0XFF,0XFF,
0X0F,0X00,0XC0,0X40,0X70,0X70,0X70,0X70,0X70,0X70,0X40,0XC0,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X03,0X0F,0X1F,0X3E,0XF8,0XF0,0XE0,0XC0,0X80,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X3D,0XBD,0XBD,0XBD,0XBC,0XBD,0XBD,0XBD,0X3D,0X00,0X01,
0X03,0X07,0X0F,0X0F,0X1F,0X3F,0XFF,0XFE,0XFC,0XF8,0XF8,0X7E,0X1F,0X0F,0X03,0X00,
0X00,0X00,0XFF,0X00,0X00,0X00,0XFE,0XFE,0X00,0X00,0X00,0XFF,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X01,0X03,0X07,0X0F,0X0E,0X1E,
0X1C,0X3C,0X38,0X38,0X38,0X70,0X70,0X72,0X72,0X72,0X72,0X72,0X70,0X70,0X38,0X38,
0X38,0X3C,0X1C,0X1E,0X0E,0X0F,0X07,0X03,0X03,0X01,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0XFF,0X80,0X80,0X80,0X99,0X99,0X80,0X80,0X80,0XFF,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X08,0XF8,0XF8,0X08,0X00,0X00,0X00,0X00,0X08,0XF8,0XF8,0X88,
0XC8,0X18,0X00,0X08,0XF8,0XF8,0X08,0X08,0X18,0XF0,0XF0,0XC0,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X08,0X0F,0X0F,0X08,0X08,0X08,0X0C,0X00,0X08,0X0F,0X0F,0X08,
0X09,0X0C,0X00,0X08,0X0F,0X0F,0X08,0X08,0X0C,0X07,0X07,0X01,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};

/** @brief 时间显示超大字体 */
const unsigned char fontTimeNum[][28]=
{

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0x40,0x40,0x40,0xC0,0xC0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0xC0,0xF0,0xF8,0xFE,0xFF,0x3F,0x07,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x3F,0xFF,0xFE,0xF8,0xF0,0x80,0x00,0x00},
{0x00,0x00,0xFC,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFC,0x00},
{0x00,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xFF,0xFF,0xFF,0xFF,0x1F,0x00},
{0x00,0x00,0x00,0x00,0x07,0x0F,0x3F,0x7F,0x7E,0xF0,0xE0,0x80,0x80,0x00,0x00,0x00,0x80,0xC0,0xE0,0xF0,0x7E,0x7F,0x3F,0x0F,0x07,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"0",0*/
/* (28 X 48 , BatangChe, 加粗 )*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x06,0x06,0x06,0x06,0x06,0x07,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xFF,0xFF,0xFF,0xFF,0xFF,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00},/*"1",1*/
/* (28 X 48 , BatangChe, 加粗 )*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0xC0,0xC0,0x40,0x40,0x40,0x40,0x40,0xC0,0xC0,0xC0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0xF8,0xFC,0xFE,0xFF,0xFF,0xC3,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x07,0xFF,0xFF,0xFE,0xFC,0xF0,0x00,0x00},
{0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0xF8,0xFE,0x7F,0x3F,0x1F,0x0F,0x03,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0x70,0x38,0x1C,0x1E,0x0F,0x07,0x03,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0xC0,0xE0,0xF0,0xDC,0xCE,0xC7,0xC3,0xC1,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xE0,0xF0,0xF8,0xFC,0x7F,0x0F,0x01,0x00},
{0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00},/*"2",2*/
/* (28 X 48 , BatangChe, 加粗 )*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0x40,0x40,0x40,0x40,0x40,0xC0,0xC0,0xC0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x38,0x7E,0x7F,0x7F,0x7F,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x07,0xFF,0xFF,0xFF,0xFC,0xF8,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x40,0x40,0x60,0xE0,0xE0,0xB0,0x98,0x9E,0x0F,0x0F,0x07,0x03,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x03,0x0F,0xFF,0xFE,0xFC,0xF8,0xF0,0x00},
{0x00,0x00,0x1E,0x3F,0x7F,0x7F,0xFF,0xFE,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0xF0,0xFC,0x7F,0x3F,0x1F,0x0F,0x03,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"3",3*/
/* (28 X 48 , BatangChe, 加粗 )*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xF0,0x78,0x1C,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0x78,0x3C,0x0F,0x07,0x03,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x60,0x78,0x7C,0x5E,0x4F,0x43,0x41,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0xFF,0xFF,0xFF,0xFF,0xFF,0x40,0x40,0x40,0x40,0x40,0x40},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x80,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00},/*"4",4*/
/* (28 X 48 , BatangChe, 加粗 )*/

{0x00,0x00,0x00,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0xC0,0xFF,0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0xFF,0xFF,0xEF,0x60,0x30,0x10,0x18,0x18,0x08,0x08,0x08,0x08,0x18,0x18,0x38,0x70,0xF0,0xE0,0xE0,0xC0,0x80,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFE,0xF8,0x00},
{0x00,0x00,0x00,0x0E,0x3F,0x7F,0x7F,0xFF,0xFE,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0xF0,0xFC,0x7F,0x3F,0x1F,0x0F,0x01,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"5",5*/
/* (28 X 48 , BatangChe, 加粗 )*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0x40,0x40,0x40,0x40,0xC0,0xC0,0xC0,0xC0,0x80,0x80,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0xC0,0xE0,0xF8,0xFC,0x7E,0x0F,0x07,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x0F,0x1F,0x1F,0x1F,0x0F,0x0E,0x00,0x00},
{0x00,0x00,0xF0,0xFF,0xFF,0xFF,0xFF,0x87,0xC0,0x60,0x20,0x30,0x10,0x18,0x08,0x08,0x08,0x18,0x18,0x38,0x78,0xF0,0xF0,0xE0,0xC0,0x00,0x00,0x00},
{0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFC,0x00},
{0x00,0x00,0x00,0x03,0x0F,0x1F,0x3F,0x7F,0xF0,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xF0,0x7C,0x7F,0x3F,0x1F,0x0F,0x01,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"6",6*/
/* (28 X 48 , BatangChe, 加粗 )*/

{0x00,0x00,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x00},
{0x00,0x00,0xF0,0xFE,0x3F,0x0F,0x07,0x03,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x81,0xE1,0xF9,0x7D,0x3F,0x0F,0x03,0x01,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xE0,0xF8,0xFC,0x7F,0x1F,0x07,0x01,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xE0,0xF8,0xFE,0xFF,0x7F,0x07,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"7",7*/
/* (28 X 48 , BatangChe, 加粗 )*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0xC0,0x40,0x40,0x40,0x40,0xC0,0xC0,0xC0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0xF0,0xFC,0xFE,0x07,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x07,0xFF,0xFF,0xFF,0xFE,0xF8,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x01,0x07,0x0F,0x1F,0x3C,0x38,0xF0,0xF0,0xE0,0xE0,0xC0,0xE0,0xE0,0xF0,0xF8,0xBE,0x3F,0x1F,0x0F,0x07,0x01,0x00,0x00,0x00},
{0x00,0x00,0xC0,0xF0,0xF8,0xFC,0xFE,0x1E,0x07,0x03,0x03,0x01,0x00,0x00,0x01,0x01,0x03,0x03,0x03,0x07,0x07,0x0F,0x3E,0xFE,0xFC,0xF8,0xE0,0x00},
{0x00,0x00,0x07,0x1F,0x3F,0x7F,0x7F,0xF0,0xE0,0xC0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xE0,0x78,0x3F,0x3F,0x1F,0x07,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"8",8*/
/* (28 X 48 , BatangChe, 加粗 )*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0x40,0x40,0x40,0x40,0x40,0xC0,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0xC0,0xF0,0xFC,0xFE,0xFF,0x1F,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x3F,0xFE,0xFC,0xF8,0xE0,0x00,0x00},
{0x00,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0xE0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xF0,0xFF,0xFF,0xFF,0xFF,0xFF,0x00},
{0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x07,0x0F,0x0F,0x0E,0x0E,0x0C,0x0C,0x0C,0x0E,0x0E,0x07,0x07,0x03,0x01,0xE0,0xFF,0xFF,0xFF,0x7F,0x07,0x00},
{0x00,0x00,0x00,0x38,0x7C,0xFC,0xFC,0xF8,0xF8,0xC0,0x80,0x00,0x00,0x00,0x00,0x80,0xC0,0xC0,0xF0,0x78,0x3F,0x1F,0x0F,0x07,0x01,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"9",9*/
/* (28 X 48 , BatangChe, 加粗 )*/
};

/** @brief 时间显示用的冒号 */
const unsigned char fontTimeColon[][16]=
{
	
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0xE0,0xF0,0xF0,0xF0,0xF0,0xF0,0xE0,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x07,0x0F,0x0F,0x0F,0x0F,0x0F,0x07,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*":",10*/
/* (28 X 48 , BatangChe, 加粗 )*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",11*/
/* (28 X 48 , BatangChe, 加粗 )*/

};

/** @brief 电量显示图标 */
const unsigned char fontBattery[][10] = 
{

{0x00,0xFC,0x04,0x06,0x02,0x02,0x06,0x04,0xFC,0x00},
{0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x00},/*"0",4*/
/* (6 X 8 )*/
{0x00,0xFC,0x04,0x06,0x02,0x02,0x06,0x04,0xFC,0x00},
{0x00,0x7F,0x70,0x70,0x70,0x70,0x70,0x70,0x7F,0x00},/*"1/6",3*/
/* (6 X 8 )*/
{0x00,0xFC,0x04,0x06,0x02,0x02,0x06,0x04,0xFC,0x00},
{0x00,0x7F,0x7C,0x7C,0x7C,0x7C,0x7C,0x7C,0x7F,0x00},/*"2/6",0*/
/* (6 X 8 )*/
{0x00,0xFC,0x04,0x06,0x02,0x02,0x06,0x04,0xFC,0x00},
{0x00,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x00},/*"3/6",2*/
/* (6 X 8 )*/
{0x00,0xFC,0xC4,0xC6,0xC2,0xC2,0xC6,0xC4,0xFC,0x00},
{0x00,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x00},/*"4/6",1*/
/* (6 X 8 )*/
{0x00,0xFC,0xF4,0xF6,0xF2,0xF2,0xF6,0xF4,0xFC,0x00},
{0x00,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x00},/*"5/6",0*/
/* (6 X 8 )*/
{0x00,0xFC,0xFC,0xFE,0xFE,0xFE,0xFE,0xFC,0xFC,0x00},
{0x00,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x00},/*"1",0*/
/* (6 X 8 )*/
};

/** @brief 蓝牙图标 */
const unsigned char fontBLE[][10] = 
{
/* (6 X 8 )*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"disable",0*/
/* (6 X 8 )*/
{0x10,0x20,0x40,0xFF,0x82,0x44,0x28,0x10,0x00,0x00},
{0x08,0x04,0x02,0x7F,0x20,0x11,0x0A,0x04,0x00,0x00},/*"enable",1*/
};

/** @brief 时钟图标 */
const unsigned char fontDelay[][15] = 
{
/* (6 X 8 )*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"disable",0*/
/* (6 X 8 )*/
{0xC0,0x70,0x18,0x0C,0x24,0x46,0x82,0x02,0x02,0x06,0x04,0x0C,0x18,0x70,0xC0},
{0x07,0x1C,0x30,0x60,0x40,0xC0,0x80,0x81,0x81,0xC1,0x41,0x61,0x31,0x1C,0x07},/*"enable",1*/
};

/** @brief 勾选框图标 */
const unsigned char fontEnable[][8] =
{
{0x00,0x7F,0x41,0x41,0x41,0x41,0x41,0x7F},/*"未选中",0*/
/* (8 X 8 )*/
{0x00,0x7F,0x51,0x61,0x51,0x49,0x45,0x7F},/*"选中",0*/
/* (8 X 8 )*/
};

/** @brief 进度条图标 */
const unsigned char fontSlide[][100] = 
{ 
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 
};

/**  @} */
