#pragma once
using namespace std;
#include <graphics.h>
#include"player.h"

class Bullet
{
public:
    POINT position = { 0,0 };  // 刀片位置信息
    int angle = 0;             // 刀片旋转角度
public:
    bool hasHit = false;
    bool isDead = false; 

public:
    Bullet() = default;
    ~Bullet() = default;

    void Draw() const {
        // 刀片的多边形顶点（相对于中心）
        POINT blade[] = {
            {-20, 0},  // 左
            {-10, -5}, // 上左
            {10, -5},  // 上右
            {20, 0},   // 右
            {10, 5},   // 下右
            {-10, 5}   // 下左
        };
        int vertexCount = sizeof(blade) / sizeof(POINT);
        vector<POINT> transformedBlade(vertexCount);

        // 手动实现旋转和平移
        double rad = angle * 3.14159 / 180;
        double cosA = cos(rad);
        double sinA = sin(rad);
        for (int i = 0; i < vertexCount; i++) {
            // 先旋转
            int xRot = (int)(blade[i].x * cosA - blade[i].y * sinA);
            int yRot = (int)(blade[i].x * sinA + blade[i].y * cosA);
            // 再平移
            transformedBlade[i].x = xRot + position.x;
            transformedBlade[i].y = yRot + position.y;
        }

        setlinecolor(RGB(139, 69, 19)); // 棕色边框
        setfillcolor(RGB(210, 180, 140)); // 木色填充
        fillpolygon(transformedBlade.data(), vertexCount);
    }

    // 更新刀片角度（可选，让刀片旋转）
    void UpdateAngle() {
        angle = (angle + 5) % 360; // 每次旋转 5 度
    }

private:
    // 刀片尺寸（可调整）
    
    const int BLADE_LENGTH = 40; // 刀片长度
    const int BLADE_WIDTH = 10;  // 刀片宽度

};

// 修改 bullet.h 中的 UpdateBullets 函数
// 修改 bullet.h 中的 UpdateBullets 函数
inline void UpdateBullets(vector<Bullet>& bullet_list, const Player& player) {
    const double TANGENT_SPEED = 0.0055;
    const double FIXED_RADIUS = 100.0; // 固定半径值
    double radian_interval = 2 * 3.14159 / bullet_list.size();
    POINT player_position = player.GetPosition();

    for (size_t i = 0; i < bullet_list.size(); i++) {
        double radian = GetTickCount() * TANGENT_SPEED + radian_interval * i;
        bullet_list[i].position.x = player_position.x + player.FRAME_WIDTH / 2 +
            (int)(FIXED_RADIUS * sin(radian));
        bullet_list[i].position.y = player_position.y + player.FRAME_HEIGHT / 2 +
            (int)(FIXED_RADIUS * cos(radian));
        bullet_list[i].UpdateAngle();
    }

    // 完全移除半径波动相关的重置逻辑
}