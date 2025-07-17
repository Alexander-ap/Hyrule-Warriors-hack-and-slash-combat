#pragma once
using namespace std;
#include <graphics.h>
//���ڽ���˵��İ�ť
class Button
{
public:
    void ResetStatus() {
        status = Status::Idle;
    }
    // ��Ĭ�Ϲ��캯�����������հ�ť�����������ʼ��״̬/ͼƬ��
    Button(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
    {
        // 1. ��ʼ����ť����λ�� + ��С��
        region = rect;

        // 2. ��������״̬��ͼƬ
        loadimage(&img_idle, path_img_idle);       // ����״̬ͼƬ
        loadimage(&img_hovered, path_img_hovered); // ��ͣ״̬ͼƬ
        loadimage(&img_pushed, path_img_pushed);   // ����״̬ͼƬ
    }

    // Ĭ�������������Զ��ͷ���Դ����ͼƬ���ֶ��ͷ�����չ��
    ~Button() = default;

    void Draw()
    {
        // ���ݰ�ť��ǰ״̬��status���л������߼�
        switch (status)
        {
            // ����״̬������ img_idle ͼƬ�� region ����
        case Status::Idle:
            putimage(region.left, region.top, &img_idle);
            break;

            // ��ͣ״̬������ img_hovered ͼƬ�� region ����
        case Status::Hovered:
            putimage(region.left, region.top, &img_hovered);
            break;

            // ����״̬������ img_pushed ͼƬ�� region ����
        case Status::Pushed:
            putimage(region.left, region.top, &img_pushed);
            break;
        }
    }


    void ProcessEvent(const ExMessage& msg)
    {
        switch (msg.message)
        {
            // ����ƶ��¼�
        case WM_MOUSEMOVE:
            // ����״̬ + ����ڰ�ť�� �� �л�����ͣ
            if (status == Status::Idle && CheckCursorHit(msg.x, msg.y))
                status = Status::Hovered;
            // ��ͣ״̬ + ����뿪��ť �� �л�������
            else if (status == Status::Hovered && !CheckCursorHit(msg.x, msg.y))
                status = Status::Idle;
            break;

            // �����������¼�
        case WM_LBUTTONDOWN:
            // ����ڰ�ť�� �� �л�������״̬
            if (CheckCursorHit(msg.x, msg.y))
                status = Status::Pushed;
            break;

            // �������ɿ��¼�
        case WM_LBUTTONUP:
            // ����״̬���ɿ� �� ��������߼�
            if (status == Status::Pushed) {
                // �ؼ���ֻ���ɿ�ʱ������ڰ�ť�ڣ��Ŵ������
                if (CheckCursorHit(msg.x, msg.y)) {
                    OnClick(); // ������ʼ/�˳��߼�
                }
                // �����Ƿ񴥷����ɿ�������Ϊ����״̬
                status = Status::Idle;
            }
            break;

        default:
            break;
        }
    }

private:
    // ��״̬ö�١������ư�ť״̬Ϊ Idle/Hovered/Pushed������Ƿ�ֵ
    enum class Status
    {
        Idle = 0,   // ��ʼ״̬�����ã�
        Hovered,    // �����ͣ
        Pushed      // ��갴��
    };

protected:
    virtual void OnClick() = 0;


private:
    RECT region;          // ��ť�ڴ����еľ�������λ�� + ��С��
    IMAGE img_idle;       // ����״̬��ʾ��ͼƬ
    IMAGE img_hovered;    // ��ͣ״̬��ʾ��ͼƬ
    IMAGE img_pushed;     // ����״̬��ʾ��ͼƬ
    Status status = Status::Idle;  // ��ǰ״̬��Ĭ�����ã�

private:
    //������⵱ǰ���λ����û�г����ڰ�ť������
    bool CheckCursorHit(int x, int y)
    {
        return x >= region.left && x <= region.right && y >= region.top && y <= region.bottom;

    }
};