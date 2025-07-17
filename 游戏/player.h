#pragma once
#include"animation.h"
#include"button.h"
using namespace std;
extern const int PLAYER_SPEED;//�ٶ�
extern POINT player_pos ;//���ڼ�¼���������λ�ã����Ǹ��������͵ı���
extern Animation anim_left_player;
extern Animation anim_right_player;
extern IMAGE img_shadow;
extern bool is_move_up;
extern bool is_move_down;
extern bool is_move_left;
extern  bool is_move_right;
//��Ϊ�н�ɫ�͵���,����Ҫ����һ��player����
class Player
{
public:
    const int FRAME_WIDTH = 80;   // ��ҿ��
    const int FRAME_HEIGHT = 80;  // ��Ҹ߶�
public:
    Player()    //���ù��캯�������ؽ�ɫ�Ķ�������ӰͼƬ
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

    void ProcessEvent(const ExMessage& msg) //������Ҳ�����Ϣ
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

    void Move()  //���ƽ�ɫ�ƶ�
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

        //Ϊ��ʹ��ɫ���ߵ�������
        if (player_pos.x < 0) player_pos.x = 0;
        if (player_pos.y < 0) player_pos.y = 0;
        if (player_pos.x + player_width > window_width) player_pos.x = window_width - player_width;
        if (player_pos.y + player_height > window_height) player_pos.y = window_height - player_height;

      
    }

    void Draw(int delta)   //�������
    {
        int pos_shadow_y = player_pos.y + player_height - 8;
        int pos_shadow_x = player_pos.x + (player_width / 2 - shadow_width / 2); //����Ӱ����

        putimage_alpha(pos_shadow_x, pos_shadow_y, &img_shadow);  //�ڻ�����Ҷ���ǰ�Ȼ�����ӰͼƬ����
        int dir_x = is_move_right - is_move_left;
        static bool facing_left = false;  //�����жϷ���
        if (dir_x < 0)
            facing_left = true;
        else if (dir_x > 0)
            facing_left = false;
        //�������0���򱣳�facing_left��ֵ���伴��


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
    const int SHADOW_WIDTH = 32;  // ��Ӱ���

private:
    IMAGE img_shadow;
    Animation* anim_left;
    Animation* anim_right;
};

//����DrawPayerΪ��ʹ��ɫ�����ƶ�ʱ���Ըı䷽��
void DrawPlay(int delta, int dir_x)  //y��ΪҪ�������ҷ�ת�����Ի�Ҫ��dir_x����ʾ��ǰ�����x���ϵ��ƶ�����
{
    int pos_shadow_y = player_pos.y + player_height - 8;
    int pos_shadow_x = player_pos.x + (player_width / 2 - shadow_width / 2); //����Ӱ����

    putimage_alpha(pos_shadow_x, pos_shadow_y, &img_shadow);  //�ڻ�����Ҷ���ǰ�Ȼ�����ӰͼƬ����

    static bool facing_left = false;  //�����жϷ���
    if (dir_x < 0)
        facing_left = true;
    else if (dir_x > 0)
        facing_left = false;
    //�������0���򱣳�facing_left��ֵ���伴��


    if (facing_left)
        anim_left_player.play(player_pos.x, player_pos.y, delta);
    else
        anim_right_player.play(player_pos.x, player_pos.y, delta);
}

