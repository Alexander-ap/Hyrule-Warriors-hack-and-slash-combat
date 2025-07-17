#pragma once
#include"animation.h"
#include"button.h"
using namespace std;
extern const int PLAYER_SPEED;//速度
extern POINT player_pos ;//用于记录玩家所处的位置，这是个坐标类型的变量
extern Animation anim_left_player;
extern Animation anim_right_player;
extern IMAGE img_shadow;
extern bool is_move_up;
extern bool is_move_down;
extern bool is_move_left;
extern  bool is_move_right;
//因为有角色和敌人,所以要定义一个player基类
class Player
{
public:
    const int FRAME_WIDTH = 80;   // 玩家宽度
    const int FRAME_HEIGHT = 80;  // 玩家高度
public:
    Player()    //利用构造函数，加载角色的动画和阴影图片
    {
        loadimage(&img_shadow, _T("img/shadow_player.png"));
        anim_left = new Animation(_T("img/player_left_%d.png"), 6, 45);
        anim_right = new Animation(_T("img/player_right_%d.png"), 6, 45);
    }

    ~Player()
    {
        delete anim_left;
        delete anim_right;
    }

    void ProcessEvent(const ExMessage& msg) //处理玩家操作消息
    {
        switch (msg.message)
        {
        case WM_KEYDOWN:
            switch (msg.vkcode)
            {
            case VK_UP:
                is_move_up = true;
                break;

            case VK_DOWN:
                is_move_down = true;
                break;

            case VK_LEFT:
                is_move_left = true;
                break;

            case VK_RIGHT:
                is_move_right = true;
                break;
            }
            break;

        case WM_KEYUP:
            switch (msg.vkcode)
            {
            case VK_UP:
                is_move_up = false;
                break;

            case VK_DOWN:
                is_move_down = false;
                break;

            case VK_LEFT:
                is_move_left = false;
                break;

            case VK_RIGHT:
                is_move_right = false;
                break;
            }
            break;
       }
    }

    void Move()  //控制角色移动
    {
        int dir_x = is_move_right - is_move_left;
        int dir_y = is_move_down - is_move_up;
        double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);
        if (len_dir != 0)
        {
            double normalized_x = dir_x / len_dir;
            double normalized_y = dir_y / len_dir;
            player_pos.x += (int)(PLAYER_SPEED * normalized_x);
            player_pos.y += (int)(PLAYER_SPEED * normalized_y);
        }

        //为了使角色不走到窗口外
        if (player_pos.x < 0) player_pos.x = 0;
        if (player_pos.y < 0) player_pos.y = 0;
        if (player_pos.x + player_width > window_width) player_pos.x = window_width - player_width;
        if (player_pos.y + player_height > window_height) player_pos.y = window_height - player_height;

      
    }

    void Draw(int delta)   //绘制玩家
    {
        int pos_shadow_y = player_pos.y + player_height - 8;
        int pos_shadow_x = player_pos.x + (player_width / 2 - shadow_width / 2); //让阴影据中

        putimage_alpha(pos_shadow_x, pos_shadow_y, &img_shadow);  //在绘制玩家动画前先绘制阴影图片即可
        int dir_x = is_move_right - is_move_left;
        static bool facing_left = false;  //用来判断方向
        if (dir_x < 0)
            facing_left = true;
        else if (dir_x > 0)
            facing_left = false;
        //如果等于0，则保持facing_left的值不变即可


        if (facing_left)
            anim_left_player.play(player_pos.x, player_pos.y, delta);
        else
            anim_right_player.play(player_pos.x, player_pos.y, delta);
    }

    const POINT& GetPosition() const
    {
        return player_pos;
    }

private:
    const int SPEED = 3;
    const int SHADOW_WIDTH = 32;  // 阴影宽度

private:
    IMAGE img_shadow;
    Animation* anim_left;
    Animation* anim_right;
};

//定义DrawPayer为了使角色左右移动时可以改变方向
void DrawPlay(int delta, int dir_x)  //y因为要考虑左右翻转，所以还要有dir_x来表示当前玩家在x轴上的移动方向
{
    int pos_shadow_y = player_pos.y + player_height - 8;
    int pos_shadow_x = player_pos.x + (player_width / 2 - shadow_width / 2); //让阴影据中

    putimage_alpha(pos_shadow_x, pos_shadow_y, &img_shadow);  //在绘制玩家动画前先绘制阴影图片即可

    static bool facing_left = false;  //用来判断方向
    if (dir_x < 0)
        facing_left = true;
    else if (dir_x > 0)
        facing_left = false;
    //如果等于0，则保持facing_left的值不变即可


    if (facing_left)
        anim_left_player.play(player_pos.x, player_pos.y, delta);
    else
        anim_right_player.play(player_pos.x, player_pos.y, delta);
}

