#pragma once
using namespace std;
#include <graphics.h>
//用于界面菜单的按钮
class Button
{
public:
    void ResetStatus() {
        status = Status::Idle;
    }
    // 【默认构造函数】：创建空按钮对象（需后续初始化状态/图片）
    Button(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
    {
        // 1. 初始化按钮区域（位置 + 大小）
        region = rect;

        // 2. 加载三种状态的图片
        loadimage(&img_idle, path_img_idle);       // 闲置状态图片
        loadimage(&img_hovered, path_img_hovered); // 悬停状态图片
        loadimage(&img_pushed, path_img_pushed);   // 按下状态图片
    }

    // 默认析构函数（自动释放资源，若图片需手动释放需扩展）
    ~Button() = default;

    void Draw()
    {
        // 根据按钮当前状态（status）切换绘制逻辑
        switch (status)
        {
            // 闲置状态：绘制 img_idle 图片到 region 区域
        case Status::Idle:
            putimage(region.left, region.top, &img_idle);
            break;

            // 悬停状态：绘制 img_hovered 图片到 region 区域
        case Status::Hovered:
            putimage(region.left, region.top, &img_hovered);
            break;

            // 按下状态：绘制 img_pushed 图片到 region 区域
        case Status::Pushed:
            putimage(region.left, region.top, &img_pushed);
            break;
        }
    }


    void ProcessEvent(const ExMessage& msg)
    {
        switch (msg.message)
        {
            // 鼠标移动事件
        case WM_MOUSEMOVE:
            // 闲置状态 + 鼠标在按钮内 → 切换到悬停
            if (status == Status::Idle && CheckCursorHit(msg.x, msg.y))
                status = Status::Hovered;
            // 悬停状态 + 鼠标离开按钮 → 切换到闲置
            else if (status == Status::Hovered && !CheckCursorHit(msg.x, msg.y))
                status = Status::Idle;
            break;

            // 鼠标左键按下事件
        case WM_LBUTTONDOWN:
            // 鼠标在按钮内 → 切换到按下状态
            if (CheckCursorHit(msg.x, msg.y))
                status = Status::Pushed;
            break;

            // 鼠标左键松开事件
        case WM_LBUTTONUP:
            // 按下状态下松开 → 触发点击逻辑
            if (status == Status::Pushed) {
                // 关键：只有松开时鼠标仍在按钮内，才触发点击
                if (CheckCursorHit(msg.x, msg.y)) {
                    OnClick(); // 触发开始/退出逻辑
                }
                // 无论是否触发，松开后重置为闲置状态
                status = Status::Idle;
            }
            break;

        default:
            break;
        }
    }

private:
    // 【状态枚举】：限制按钮状态为 Idle/Hovered/Pushed，避免非法值
    enum class Status
    {
        Idle = 0,   // 初始状态（闲置）
        Hovered,    // 鼠标悬停
        Pushed      // 鼠标按下
    };

protected:
    virtual void OnClick() = 0;


private:
    RECT region;          // 按钮在窗口中的矩形区域（位置 + 大小）
    IMAGE img_idle;       // 闲置状态显示的图片
    IMAGE img_hovered;    // 悬停状态显示的图片
    IMAGE img_pushed;     // 按下状态显示的图片
    Status status = Status::Idle;  // 当前状态（默认闲置）

private:
    //用来检测当前鼠标位置有没有出现在按钮区域内
    bool CheckCursorHit(int x, int y)
    {
        return x >= region.left && x <= region.right && y >= region.top && y <= region.bottom;

    }
};