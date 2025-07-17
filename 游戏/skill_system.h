#pragma once
#include <vector>
#include "Enemy.h"
#include <graphics.h>
#include <string>  
#include"enemy_boss.h"
using namespace std; // 或直接用 std::wstring  
extern int killCount;

// 技能状态变量（外部声明，在主文件中定义）
extern int skill_counter;       // 技能计数器（累计击杀数）
extern bool can_use_skill;      // 技能是否就绪
extern bool is_skill_active;    // 技能是否正在生效
extern int score;
// 技能配置常量（可自定义）
const int SKILL_REQUIRE_KILLS = 50;  // 释放技能需要的击杀数
const TCHAR* SKILL_READY_SOUND = _T("mus/skill_ready.wav");  // 技能就绪音效
const TCHAR* SKILL_USE_SOUND = _T("mus/skill_use.wav");      // 技能释放音效

// 初始化技能系统
void InitSkillSystem();

// 更新技能计数（每击杀1个敌人调用一次）
void UpdateSkillCounter();

// 释放技能（清屏所有敌人）
void UseSkill(std::vector<Enemy*>& enemy_list);

// 绘制技能状态提示（在游戏界面显示）
void DrawSkillStatus();

// 在 skill_system.h 中继续添加
inline void InitSkillSystem() {
    skill_counter = 0;
    can_use_skill = false;
    is_skill_active = false;
    // 预加载技能音效（确保文件存在）
   // 替换原加载音效的两行
    wstring readySoundPath = SKILL_READY_SOUND;
    mciSendString((_T("open ") + readySoundPath + _T(" alias skill_ready_sound")).c_str(), NULL, 0, NULL);

    wstring useSoundPath = SKILL_USE_SOUND;
    mciSendString((_T("open ") + useSoundPath + _T(" alias skill_use_sound")).c_str(), NULL, 0, NULL);
}

inline void UpdateSkillCounter() {
    if (is_skill_active) return;

    skill_counter++;
    if (skill_counter >= SKILL_REQUIRE_KILLS) {
        can_use_skill = true;
        skill_counter %= SKILL_REQUIRE_KILLS;  // ✅ 保留溢出
        mciSendString(_T("play skill_ready_sound from 0"), NULL, 0, NULL);
    }
}

// 释放技能（清屏所有敌人）
inline void UseSkill(std::vector<Enemy*>& enemy_list) {
    if (!can_use_skill || is_skill_active) return;

    can_use_skill = false;
    is_skill_active = true;
    skill_counter %= SKILL_REQUIRE_KILLS;  // ✅ 保留溢出击杀数

    mciSendString(_T("play skill_use_sound from 0"), NULL, 0, NULL);

    // 清屏特效
    setfillcolor(RGB(255, 255, 255));
    solidrectangle(0, 0, window_width, window_height);
    FlushBatchDraw();
    Sleep(100);

    // ✅ 遍历敌人，区分 Boss 和普通敌人
    for (Enemy* enemy : enemy_list) {
        Boss* boss = dynamic_cast<Boss*>(enemy);
            score += 1;
            killCount++;  // 如果你用 killCount 触发 Boss
        delete enemy;
    }
    enemy_list.clear();

    Sleep(200);
    is_skill_active = false;
}

inline void DrawSkillStatus() {
    // 设置字体：这里用 “微软雅黑”，字号 24，粗体（可自行调整）
    LOGFONT logFont = { 0 };
    _tcscpy_s(logFont.lfFaceName, LF_FACESIZE, _T("微软雅黑"));
    logFont.lfHeight = 24;
    logFont.lfWeight = 700; // 700 对应粗体，400 是常规
    settextstyle(&logFont);

    setbkmode(TRANSPARENT);  // 透明背景，避免遮挡地图
    settextcolor(RGB(255, 255, 255)); // 文字默认白色

    // 先绘制分数（单独调整位置，避免重合）
    TCHAR scoreText[50];
    _stprintf_s(scoreText, _T("当前玩家的分数：%d"), score);
    outtextxy(20, 20, scoreText); // 分数固定显示在上方

    // 绘制技能条背景（灰色底）
    solidrectangle(20, 50, 320, 70); // x1,y1（左、上）, x2,y2（右、下），可调整长度

    // 根据充能进度绘制技能条前景（绿色进度条）
    int skillBarWidth = (skill_counter * 1.0f / SKILL_REQUIRE_KILLS) * 300; // 300 是背景条长度
    setfillcolor(RGB(30, 144, 255));  // 亮蓝色;
    solidrectangle(20, 50, 20 + skillBarWidth, 70);

    // 绘制技能条文字说明（进度数值 + 提示）
    TCHAR skillText[50];
    _stprintf_s(skillText, _T("技能充能：%d/%d"), skill_counter, SKILL_REQUIRE_KILLS);
    outtextxy(20, 80, skillText);

    // 技能就绪时，额外显示提示（可加个闪烁或颜色变化，这里简单改红色文字）
    if (can_use_skill) {
        settextcolor(RGB(255, 0, 0));
        outtextxy(20, 110, _T("技能就绪！按空格键释放"));
    }
}