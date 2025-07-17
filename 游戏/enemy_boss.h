#pragma once
#include "animation.h"
#include <graphics.h>
#include <vector>
#include <iostream>
using namespace std;
extern int lastBossKillCount; // 声明
extern bool boss_hit_this_frame;
int lastBossKillCount = 0; // 定义
bool boss_hit_this_frame = false; // 定义
//bool isAttacking = false;  // 新增：表示 Boss 是否处于攻击状态


class Boss : public Enemy {
public:
    bool wasHitThisFrame = false;
    Boss(const Player& player) {
        srand(static_cast<unsigned>(time(nullptr)));

        // 确保 Boss 不会刷在玩家脸上
        const int SAFE_DISTANCE = 150;  // 至少离玩家 150 像素
        while (true) {
            fx = static_cast<float>(rand() % 1000);
            fy = static_cast<float>(rand() % 600);

            int dx = static_cast<int>(fx) - player.GetPosition().x;
            int dy = static_cast<int>(fy) - player.GetPosition().y;
            int distSq = dx * dx + dy * dy;

            if (distSq >= SAFE_DISTANCE * SAFE_DISTANCE) break;
        }

        // 初始化动画、血量等
        anim = new Animation(_T("img/boss_left_%d.png"), 3, 45);
        attackAnim = new Animation(_T("img/nut_attack_ex_%d.png"), 3, 45);
        health = 4;
        facing_left = true;
        alive = true;
    }

    ~Boss() override {
        delete anim;
        delete attackAnim;
    }

    // 受击逻辑
    void Hurt() override {
        if (health > 0) {
            health--;
            mciSendString(_T("play hit_sound from 0"), NULL, 0, NULL);
            isAttacking = true;  // 新增：被击中后进入攻击状态
            if (health <= 0) {
                alive = false;
            }
        }
    }

    virtual void Move(const Player& player) override {
        const POINT& p = player.GetPosition();
        float dx = static_cast<float>(p.x - fx);
        float dy = static_cast<float>(p.y - fy);
        float len = sqrt(dx * dx + dy * dy);

        if (len > 5.f) {
            fx += SPEED * dx / len;
            fy += SPEED * dy / len;

            position.x = static_cast<int>(fx + 0.5f); // 四舍五入
            position.y = static_cast<int>(fy + 0.5f);

            facing_left = (dx < 0.f);
        }
    }


    // 绘制Boss及血条
    void Draw(int delta) {
        if (alive) {
            if (isAttacking) {
                if (facing_left) {
                    attackAnim->play(position.x, position.y, delta,true);
                }
                else {
                    attackAnim->play(position.x, position.y, delta);
                }
            }
            else {
                if (facing_left) {
                    anim->play(position.x, position.y, delta,true);
                }
                else {
                    anim->play(position.x, position.y, delta);
                }
            }
            DrawHealthBar();
        }
        wasHitThisFrame = false; // 每帧结束重置
    }
    // 检查是否存活
    bool CheckAlive() const { return alive; }

    // 获取剩余血量
    int GetHealth() const { return health; }


    // 设置位置
    void SetPosition(int x, int y) { position.x = x; position.y = y; }
    POINT GetPosition() const { return position; }

    // 设置朝向
    void SetFacingLeft(bool left) { facing_left = left; }

    // 保留 Enemy 里的：

    bool CheckPlayerCollision(const Player& player) override {
        if (!alive) return false; // ✅ 死亡后不再碰撞

        const POINT& playerPos = player.GetPosition();
        POINT check_position = { position.x + FRAME_WIDTH / 2, position.y + FRAME_HEIGHT / 2 };

        bool isOverlapX = check_position.x >= playerPos.x &&
            check_position.x <= playerPos.x + 80;
        bool isOverlapY = check_position.y >= playerPos.y &&
            check_position.y <= playerPos.y + 80;

        return isOverlapX && isOverlapY;
    }

    bool CheckBulletCollision(const Bullet& bullet) override {
        if (wasHitThisFrame) return false;

        bool hit = bullet.position.x >= position.x &&
            bullet.position.x <= position.x + FRAME_WIDTH &&
            bullet.position.y >= position.y &&
            bullet.position.y <= position.y + FRAME_HEIGHT;

        if (hit) {
            wasHitThisFrame = true;
        }
        return hit;
    }

private:
    void DrawHealthBar() {
        int barX = position.x + 10;
        int barY = position.y - 20;
        int barWidth = 60;
        int barHeight = 8;
        static constexpr int MAX_HEALTH = 4;
        setfillcolor(RGB(100, 100, 100));
        solidrectangle(barX, barY, barX + barWidth, barY + barHeight);

        int healthWidth = (health * 1.0f / MAX_HEALTH) * barWidth;
        setfillcolor(RGB(255, 0, 0));
        solidrectangle(barX, barY, barX + healthWidth, barY + barHeight);
    }

    int health = 4;
    int scoreValue = 10;
    bool alive = true;
    bool facing_left = true;
    bool isAttacking = false;
    POINT position = { 0, 0 };
    const int SPEED = 1;
    float fx = 0.f;
    float fy = 0.f;
    static constexpr int FRAME_WIDTH = 96;   // 与普通敌人同宽
    static constexpr int FRAME_HEIGHT = 96;   // 与普通敌人同高
    Animation* anim = nullptr;
    Animation* attackAnim = nullptr;
};

inline void TryGenerateBoss(std::vector<Enemy*>& enemy_list,
    int& killCount,
    const Player& player) {
    // ❌ 删除这一行：static int lastBossKillCount = 0;
    if (killCount - lastBossKillCount >= 6) {
        enemy_list.push_back(new Boss(player));
        lastBossKillCount = killCount;
    }
}

inline void ResetBossGeneration() {
    lastBossKillCount = 0;
}


// 在 enemy_boss.h 中新增   //防止多个Boss时只有最后一个受击
inline void ResetBossHitFlag() {
    boss_hit_this_frame = false;
}