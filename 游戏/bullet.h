#pragma once
using namespace std;
#include <graphics.h>
#include"player.h"

class Bullet
{
public:
    POINT position = { 0,0 };  // ��Ƭλ����Ϣ
    int angle = 0;             // ��Ƭ��ת�Ƕ�
public:
    bool hasHit = false;
    bool isDead = false; 

public:
    Bullet() = default;
    ~Bullet() = default;

    void Draw() const {
        // ��Ƭ�Ķ���ζ��㣨��������ģ�
        POINT blade[] = {
            {-20, 0},  // ��
            {-10, -5}, // ����
            {10, -5},  // ����
            {20, 0},   // ��
            {10, 5},   // ����
            {-10, 5}   // ����
        };
        int vertexCount = sizeof(blade) / sizeof(POINT);
        vector<POINT> transformedBlade(vertexCount);

        // �ֶ�ʵ����ת��ƽ��
        double rad = angle * 3.14159 / 180;
        double cosA = cos(rad);
        double sinA = sin(rad);
        for (int i = 0; i < vertexCount; i++) {
            // ����ת
            int xRot = (int)(blade[i].x * cosA - blade[i].y * sinA);
            int yRot = (int)(blade[i].x * sinA + blade[i].y * cosA);
            // ��ƽ��
            transformedBlade[i].x = xRot + position.x;
            transformedBlade[i].y = yRot + position.y;
        }

        setlinecolor(RGB(139, 69, 19)); // ��ɫ�߿�
        setfillcolor(RGB(210, 180, 140)); // ľɫ���
        fillpolygon(transformedBlade.data(), vertexCount);
    }

    // ���µ�Ƭ�Ƕȣ���ѡ���õ�Ƭ��ת��
    void UpdateAngle() {
        angle = (angle + 5) % 360; // ÿ����ת 5 ��
    }

private:
    // ��Ƭ�ߴ磨�ɵ�����
    
    const int BLADE_LENGTH = 40; // ��Ƭ����
    const int BLADE_WIDTH = 10;  // ��Ƭ���

};

// �޸� bullet.h �е� UpdateBullets ����
// �޸� bullet.h �е� UpdateBullets ����
inline void UpdateBullets(vector<Bullet>& bullet_list, const Player& player) {
    const double TANGENT_SPEED = 0.0055;
    const double FIXED_RADIUS = 100.0; // �̶��뾶ֵ
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

    // ��ȫ�Ƴ��뾶������ص������߼�
}