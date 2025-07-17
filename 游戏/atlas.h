#pragma once
#include<vector>  //���Զ�̬�����ڴ�,����ʹ��vector�������������������
#include <graphics.h>
#include"util.h"

//atlas ������animation֮��Ĺ����ʲ�
using namespace std;
class Atlas
{
public:
    // �����캯��������·��ģ�� + ��������������ͼƬ
    Atlas(LPCTSTR path, int num)
    {
        TCHAR path_file[256];  // �洢ƴ�Ӻ������·������ img/player_0.png��

        // ѭ�� num �Σ�����ÿ��ͼƬ
        for (size_t i = 0; i < num; i++)
        {
            // �ؼ����� _stprintf_s ƴ��·����%d �ᱻ i �滻��
            _stprintf_s(path_file, path, i);

            // 1. ��̬����ͼ�����
            IMAGE* frame = new IMAGE();
            // 2. ���� EasyX ����ͼƬ��path_file ������·����
            loadimage(frame, path_file);
            // 3. ����������ͳһ����
            frame_list.push_back(frame);
        }
    }

    // ���������������Զ��ͷ�ͼƬ�ڴ棨��ֹ�ڴ�й©��
    ~Atlas()
    {
        // �����ͷ����ж�̬������ IMAGE
        for (size_t i = 0; i < frame_list.size(); i++)
            delete frame_list[i];
    }

public:
    // �洢���м��ص�ͼƬ֡��ָ��������
    std::vector<IMAGE*> frame_list;
};