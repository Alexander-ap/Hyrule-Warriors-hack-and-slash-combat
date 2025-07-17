#pragma once
#include"button.h"
extern   bool is_game_started;
class StartGameButton : public Button
{
public:
    StartGameButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
        : Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {
    }
    ~StartGameButton() = default;

protected:
    void OnClick()
    {
        is_game_started = true;
        mciSendString(_T("play bgm repeat from 0"), NULL, 0, NULL);
    }
};
