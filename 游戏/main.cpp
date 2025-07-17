#include <graphics.h>
#include<string>
#include<vector>  //可以动态分配内存,所以使用vector容器来替代常见的数组
#include"util.h"
#include"animation.h"
#include"player.h"
#include"bullet.h"
#include"Enemy.h"
#include"button.h"
#include"start_button.h"
#include"quit_button.h"
#include"skill_system.h"
#include"enemy_boss.h"

//存储当前动画帧索引
using namespace std;
enum AudioState { MENU_MUSIC, GAME_MUSIC, HIGH_SCORE_MUSIC } current_audio;//音频状态变量
//extern bool bossGenerated;
const int player_width = 80;  //玩家宽度
const int player_height = 80; //玩家高度
const int shadow_width = 32;  //阴影宽度
const int window_width = 1280; //窗口宽度
const int window_height = 720; //窗口高度
const int PLAYER_SPEED = 3;//速度
const int PLAYER_ANIM_NUM = 6;  //图片数

const int BUTTON_WIDTH = 192;
const int BUTTON_HEIGHT = 75;

int score = 0;  //用来记录玩家得分
int idx_current_anim = 0;  //索引
int skill_counter = 0;    // 技能计数器
int killCount = 0;

IMAGE img_background;//图片类型的变量
IMAGE img_player_left[PLAYER_ANIM_NUM];//定义图片数组，用来连续播放以渲染动画
IMAGE img_player_right[PLAYER_ANIM_NUM];
IMAGE img_shadow;
IMAGE img_menu;

POINT player_pos = { 500,500 };//用于记录玩家所处的位置，这是个坐标类型的变量


//构造两个Animation类变量，以简洁的方式来实现动画
Animation anim_left_player(_T("img/player_left_%d.png"), 6, 45);
Animation anim_right_player(_T("img/player_right_%d.png"), 6, 45);


//定义四个布尔变量，来标识玩家是否向对应方向移动
bool is_move_up = false;
bool is_move_down = false;
bool is_move_left = false;
bool is_move_right = false;
bool is_game_started = false;
bool running = true;
bool can_use_skill = false; // 是否可以使用技能
bool is_skill_active = false; // 技能是否正在生效
//为了使角色不跑出窗口外
 
//定义按钮所在的矩形
RECT region_btn_start_game,region_btn_quit_game;

#pragma comment (lib,"MSIMG32.LIB") //这个代码必不可少
#pragma comment (lib,"Winmm.lib") 


//用来循环加载图片的
void loadAnimation()
{

    for (size_t i = 0; i < PLAYER_ANIM_NUM; i++)
    {
        wstring path = L"img/player_left_" + to_wstring(i) + L".png";
        loadimage(&img_player_left[i], path.c_str());
    }


    for (size_t i = 0; i < PLAYER_ANIM_NUM; i++)
    {
        wstring path = L"img/player_right_" + to_wstring(i) + L".png";
        loadimage(&img_player_right[i], path.c_str());
    }

}

//添加此函数用于正确关闭指定音频
// 修改为：只停止音频，不关闭（保留资源，方便再次播放）
void StopMusic(LPCTSTR alias) {
    TCHAR cmd[256];
    _stprintf_s(cmd, _T("stop %s"), alias); // 仅暂停，不释放资源
    mciSendString(cmd, NULL, 0, NULL);
}

void CloseMusic(LPCTSTR alias) {
    TCHAR cmd[256];
    _stprintf_s(cmd, _T("stop %s"), alias);
    mciSendString(cmd, NULL, 0, NULL);
    _stprintf_s(cmd, _T("close %s"), alias);
    mciSendString(cmd, NULL, 0, NULL);
}



//动画帧的总数量
int main()
{
    initgraph(1280, 720, 1);
    //这里本质是加载音频
    mciSendString(_T("open mus/bgmm.mp3 alias menu_music"), NULL, 0, NULL);    // 菜单音乐（原bgmm）
    mciSendString(_T("open mus/bgm.mp3 alias game_music"), NULL, 0, NULL);    // 游戏音乐（原bgm）
    mciSendString(_T("open mus/bgmh.mp3 alias high_music"), NULL, 0, NULL);    // 高分音乐（原bgmh）
    mciSendString(_T("open mus/hit.wav alias hit_sound"), NULL, 0, NULL);      // 击中音效（原hit）
    mciSendString(_T("open \"mus/hit.wav\" alias hit_sound"), NULL, 0, NULL);

    // 2. 初始播放菜单音乐
    mciSendString(_T("play menu_music repeat"), NULL, 0, NULL);
    AudioState current_audio = MENU_MUSIC;  // 记录当前音频状态


    Player player;  //玩家对象全局只有一个，所以定义在主循环的外部
    ExMessage msg;
    vector<Enemy*> enemy_list;

    vector<Bullet> bullet_list(3);  //初始化vector的子弹容器，用于存储子弹
   
    //初始化技能系统
    InitSkillSystem();



   // 开始按钮区域（水平居中 → 用 window_width）
    region_btn_start_game.left = (window_width - BUTTON_WIDTH) / 2;
    region_btn_start_game.right = region_btn_start_game.left + BUTTON_WIDTH;
    region_btn_start_game.top = 430;
    region_btn_start_game.bottom = region_btn_start_game.top + BUTTON_HEIGHT;

    // 退出按钮区域（水平居中 → 用 window_width，原代码正确，可保留）
    region_btn_quit_game.left = (window_width - BUTTON_WIDTH) / 2;
    region_btn_quit_game.right = region_btn_quit_game.left + BUTTON_WIDTH;
    region_btn_quit_game.top = 550;
    region_btn_quit_game.bottom = region_btn_quit_game.top + BUTTON_HEIGHT;
    
    // 创建开始按钮对象（传入区域 + 三种状态图片路径）
    StartGameButton btn_start_game = StartGameButton(region_btn_start_game,
        _T("img/ui_start_idle.png"),
        _T("img/ui_start_hovered.png"),
        _T("img/ui_start_pushed.png"));

    // 创建退出按钮对象（传入区域 + 三种状态图片路径）
    QuitGameButton btn_quit_game = QuitGameButton(region_btn_quit_game,
        _T("img/ui_quit_idle.png"),
        _T("img/ui_quit_hovered.png"),
        _T("img/ui_quit_pushed.png"));

    //加载图片
    loadimage(&img_menu, _T("img/menu.png"));
    loadimage(&img_background, _T("img/background.png"));
    loadimage(&img_shadow, _T("img/shadow_player.png"));
    //记得加载动画图片到主函数
    loadAnimation();
    BeginBatchDraw();
    while (running)
    {
        DWORD start_time = GetTickCount();


        //这是事件处理部分
        while (peekmessage(&msg))
        {

            // 在主循环开始处
            for (Bullet& bullet : bullet_list) {
                bullet.hasHit = false;
            }

            //要判断是执行游戏内逻辑还是主菜单逻辑
            if (is_game_started)
            {
                player.ProcessEvent(msg);
                if (msg.message == WM_KEYDOWN && msg.vkcode == VK_SPACE) {
                    UseSkill(enemy_list);  // 调用技能释放
                }
            }
            else
            {
                btn_start_game.ProcessEvent(msg);
                btn_quit_game.ProcessEvent(msg);
            }
        }
        if (is_game_started)
        {

            if (current_audio != GAME_MUSIC && current_audio != HIGH_SCORE_MUSIC) {
                StopMusic(_T("menu_music")); // 关闭菜单音乐
                mciSendString(_T("play game_music repeat"), NULL, 0, NULL);
                current_audio = GAME_MUSIC;
            }


            if (is_game_started && !is_skill_active) { // 技能生效时不生成敌人
                TryGenerateEnemy(enemy_list, score);  //传递score引用
            }

            // 生成 Boss：每击杀 30 个普通敌人生成一个 
            TryGenerateBoss(enemy_list, killCount,player);  // 新增调用，需要定义 killCount 变量并维护其值

            player.Move();
            UpdateBullets(bullet_list, player);
            for (Enemy* enemy : enemy_list) //遍历vector中每个敌人，依次调用move方法
                enemy->Move(player);

            // 检测子弹和敌人碰撞，并立即移除死亡敌人
            for (size_t i = 0; i < enemy_list.size();) {
                Enemy* enemy = enemy_list[i];
                bool hit = false;
                bool isBoss = dynamic_cast<Boss*>(enemy) != nullptr;

                for (Bullet& bullet : bullet_list) {
                    if (!bullet.hasHit && enemy->CheckBulletCollision(bullet)) {
                        bullet.hasHit = true;
                        hit = true;

                        if (!isBoss) break;
                        break;
                    }
                }

                if (hit) {
                    mciSendString(_T("play hit_sound from 0"), NULL, 0, NULL);
                    enemy->Hurt(); // ✅ 现在会调用 Boss::Hurt()

                    if (!enemy->CheckAlive()) {
     
                            score++;
                           killCount++;
                        UpdateSkillCounter();
                    }
                }

                if (!enemy->CheckAlive()) {
                    delete enemy;
                    enemy_list.erase(enemy_list.begin() + i);
                }
                else {
                    ++i;
                }
            }
            // 检查玩家是否被敌人碰撞
            bool playerHit = false;
            for (Enemy* enemy : enemy_list) {
                if (enemy->CheckPlayerCollision(player)) {
                    playerHit = true;
                    break;
                }
            }

            if (playerHit) {
                // 1. 先停止游戏逻辑
                is_game_started = false;

                // 2. 显示消息框（阻塞）
                static TCHAR text[256];
                _stprintf_s(text, _T("最终得分是：%d ！"), score);
                MessageBox(GetHWnd(), text, _T("游戏结束"), MB_OK);

                // 3. 清空敌人列表（在弹窗后统一处理）
                for (Enemy* e : enemy_list) delete e;
                enemy_list.clear();

                // 4. 重置游戏状态
                score = 0;
                killCount = 0;
                player_pos = { 500, 500 };
                is_move_up = false;
                is_move_down = false;
                is_move_left = false;
                is_move_right = false;

                btn_start_game.ResetStatus();
                btn_quit_game.ResetStatus();
                InitSkillSystem();
                ResetBossGeneration();
                ResetBossHitFlag();
            }
            
        }
        else
        {
            // 返回菜单：切换到菜单音乐（仅第一次返回时）
            if (current_audio != MENU_MUSIC) {
                StopMusic(_T("game_music"));   // 关闭游戏音乐
                StopMusic(_T("high_music"));  // 关闭高分音乐
                mciSendString(_T("play menu_music repeat"), NULL, 0, NULL);
                current_audio = MENU_MUSIC;
            }
        }


        cleardevice();
        if (is_game_started)
        {
            putimage(-70,-70, &img_background);
            player.Draw(1000 / 144);
            for (Enemy* enemy : enemy_list)
                enemy->Draw(1000 / 144);   //调用渲染也要依次遍历vector中每个敌人
            //player_pos.x, player_pos.y,当改变量改变时角色也就跟着动

            for (const Bullet& bullet : bullet_list)   //使子弹还有不断波动的径向速度，会围绕角色进行时远时近的圆周运动
                bullet.Draw();

            if (score >= 50)
            {
                if (current_audio != HIGH_SCORE_MUSIC) {
                    StopMusic(_T("game_music")); // 关闭游戏音乐
                    mciSendString(_T("play high_music repeat"), NULL, 0, NULL);
                    current_audio = HIGH_SCORE_MUSIC;
                }
            }
            DrawSkillStatus();
        }
        else
        {
            putimage(-600,-200 , &img_menu);
            btn_start_game.Draw();
            btn_quit_game.Draw();

        }

        FlushBatchDraw();
        DWORD end_time = GetTickCount();
        DWORD delta_time = end_time - start_time;

        if (delta_time < 1000 / 144)
        {
            Sleep(1000 / 144 - delta_time);
        }
    }

    EndBatchDraw();

    CloseMusic(_T("menu_music"));
    CloseMusic(_T("game_music"));
    CloseMusic(_T("high_music"));
    CloseMusic(_T("hit_sound"));
    return 0;
}