#pragma once
#pragma once
#include"animation.h"
#include"bullet.h"
using namespace std;
extern const int PLAYER_SPEED ;//速度
extern POINT player_pos;//用于记录玩家所处的位置，这是个坐标类型的变量
extern Animation anim_left_player;
extern Animation anim_right_player;
//因为有角色和敌人,所以要定义一个player基类
class Enemy
{
public:
    virtual void Hurt() { alive = false; }  // ✅ 一行搞定
    Enemy()
    {
        loadimage(&img_shadow, _T("img/shadow_enemy.png"));
        anim_left = new Animation(_T("img/enemy_left_%d.png"), 6, 45);
        anim_right = new Animation(_T("img/enemy_right_%d.png"), 6, 45);

        //使用枚举来表示敌人生成的边界
        enum class SpawnEdge
        {
            Up = 0,
            Down,
            Left,
            Right
        };

        SpawnEdge edge = (SpawnEdge)(rand() % 4);   //将敌人放置在地图外边界处的随机位置
        //使用随机数取模可以实现随机地图边界的效果
        switch (edge)
        {
        case SpawnEdge::Up:
            position.x = rand() % window_width;
            position.y = -FRAME_HEIGHT;
            break;
        case SpawnEdge::Down:
            position.x = rand() % window_width;
            position.y = window_height;
            break;
        case SpawnEdge::Left:
            position.x = -FRAME_WIDTH;
            position.y = rand() % window_height;
            break;
        case SpawnEdge::Right:
            position.x = window_width;
            position.y = rand() % window_height;
            break;
        default:
            break;
        }


    }
   
    virtual  ~Enemy()
    {
        delete anim_left;
        delete anim_right;
    }

    //检测是否与玩家的子弹或者玩家相碰
    virtual bool CheckBulletCollision(const Bullet& bullet)
    {
        // 将子弹等效为点，判断点是否在敌人矩形内
        bool is_overlap_x = bullet.position.x >= position.x && bullet.position.x <= position.x + FRAME_WIDTH;
        bool is_overlap_y = bullet.position.y >= position.y && bullet.position.y <= position.y + FRAME_HEIGHT;
        return is_overlap_x && is_overlap_y;
    }

    virtual bool CheckPlayerCollision(const Player& player)
    {
        // 获取玩家的位置
        const POINT& playerPos = player.GetPosition();
        // 敌人用于碰撞检测的判定点（这里取敌人矩形中心，也可根据需求调整）
        POINT check_position = { position.x + FRAME_WIDTH / 2, position.y + FRAME_HEIGHT / 2 };

        // 判断敌人的判定点是否在玩家的矩形范围内
        // 玩家矩形范围：以玩家位置为左上角，宽高可根据实际 Player 类中的定义，这里假设和敌人类似有对应宽度和高度常量，比如 PLAYER_FRAME_WIDTH、PLAYER_FRAME_HEIGHT
        // 若 Player 类中没有直接暴露宽高，也可在 Player 类中增加获取宽高的接口，如 const int& GetFrameWidth() const { return FRAME_WIDTH; }  ，这里为示例先假设已有对应常量
        bool isOverlapX = check_position.x >= playerPos.x && check_position.x <= playerPos.x + PLAYER_FRAME_WIDTH;
        bool isOverlapY = check_position.y >= playerPos.y && check_position.y <= playerPos.y + PLAYER_FRAME_HEIGHT;

        return isOverlapX && isOverlapY;
    }


    //敌人通过追踪玩家移动而移动，则也要玩家的参数
 // 敌人通过追踪玩家移动而移动，则也要玩家的参数
    virtual void Move(const Player& player) {
        const POINT& player_position = player.GetPosition();
        int dir_x = player_position.x - position.x;
        int dir_y = player_position.y - position.y;
        double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);

        if (len_dir != 0) {
            double normalized_x = dir_x / len_dir;
            double normalized_y = dir_y / len_dir;
            position.x += (int)(SPEED * normalized_x);
            position.y += (int)(SPEED * normalized_y);

            const int TURN_THRESHOLD = 10;
            if (abs(dir_x) > TURN_THRESHOLD) {
                facing_left = (dir_x < 0);
            }
        }
    }

    virtual  void Draw(int delta)
    {
        int pos_shadow_x = position.x + (FRAME_WIDTH / 2 - SHADOW_WIDTH / 2);
        int pos_shadow_y = position.y + FRAME_HEIGHT - 35;
        putimage_alpha(pos_shadow_x, pos_shadow_y, &img_shadow);

        if (facing_left)
            anim_left->play(position.x, position.y, delta);
        else
            anim_right->play(position.x, position.y, delta);
    }

   
    bool CheckAlive()    //是否存货
    {
        return alive;
    }


private:
    const int SPEED = 2;
    const int FRAME_WIDTH = 80;    // 敌人宽度
    const int FRAME_HEIGHT = 80;   // 敌人高度
    const int SHADOW_WIDTH = 48;   // 阴影宽度
    const int PLAYER_FRAME_WIDTH = 80;  //玩家宽度
    const int PLAYER_FRAME_HEIGHT = 80;  //玩家高度

public:
    IMAGE img_shadow;  //阴影
    Animation* anim_left;   // 向左动画
    Animation* anim_right;   // 向右动画
    POINT position = { 0, 0 };  //敌人位置
    bool facing_left = false;  //判断敌人向左还是向右
    bool alive = true; // 用于判断敌人是否死亡
};

inline void TryGenerateEnemy(vector<Enemy*>& enemy_list, int& current_score) {
    // 基础间隔，得分越高，间隔越小（最低为 10，避免过于密集）
    int base_interval = 100;
    // 每得 10 分，间隔减 5（可根据需求调整衰减速率）
    int interval = max(100 - current_score / 15 * 5, 30);

    static int counter = 0;
    if ((++counter) % interval == 0) {
        enemy_list.push_back(new Enemy());
    }
}