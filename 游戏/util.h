#pragma once
#include <graphics.h>
using namespace std;
#pragma comment (lib,"MSIMG32.LIB") //这个代码必不可少


//重构了putimage函数，以构造透明图片类
inline void putimage_alpha(int x, int y, IMAGE* img)
{
    int w = img->getwidth();
    int h = img->getheight();
    AlphaBlend(GetImageHDC(NULL), x, y, w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}