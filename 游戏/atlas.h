#pragma once
#include<vector>  //可以动态分配内存,所以使用vector容器来替代常见的数组
#include <graphics.h>
#include"util.h"

//atlas 是其与animation之间的公共资产
using namespace std;
class Atlas
{
public:
    // 【构造函数】：按路径模板 + 数量，批量加载图片
    Atlas(LPCTSTR path, int num)
    {
        TCHAR path_file[256];  // 存储拼接后的完整路径（如 img/player_0.png）

        // 循环 num 次，加载每张图片
        for (size_t i = 0; i < num; i++)
        {
            // 关键：用 _stprintf_s 拼接路径（%d 会被 i 替换）
            _stprintf_s(path_file, path, i);

            // 1. 动态创建图像对象
            IMAGE* frame = new IMAGE();
            // 2. 调用 EasyX 加载图片（path_file 是完整路径）
            loadimage(frame, path_file);
            // 3. 存入容器，统一管理
            frame_list.push_back(frame);
        }
    }

    // 【析构函数】：自动释放图片内存（防止内存泄漏）
    ~Atlas()
    {
        // 遍历释放所有动态创建的 IMAGE
        for (size_t i = 0; i < frame_list.size(); i++)
            delete frame_list[i];
    }

public:
    // 存储所有加载的图片帧（指针容器）
    std::vector<IMAGE*> frame_list;
};