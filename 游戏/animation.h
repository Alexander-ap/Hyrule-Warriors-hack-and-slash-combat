#pragma once
#include<vector>  //可以动态分配内存,所以使用vector容器来替代常见的数组
#include <graphics.h>
#include"util.h"
extern const int player_width;   // 声明：告诉编译器“这个变量在别处定义”
extern const int player_height;
extern const int shadow_width;
extern const int window_width;
extern const int window_height;
extern bool is_move_up;
extern bool is_move_down;
extern bool is_move_left;
extern bool is_move_right;


//剪影的图片数组
IMAGE img_player_left_sketch[6];
using namespace std;


//动画类，用于封装加载图片功能，
class Animation
{
public:
	//利用构造函数充当加载函数的功能
	// 构造函数：增加 isFlipped 参数，控制是否水平反转
	Animation(LPCTSTR path, int num, int interval, bool isFlipped = false)
		: interval_ms(interval), isFlipped(isFlipped) {

		TCHAR path_file[256];
		for (size_t i = 0; i < num; i++) {
			_stprintf_s(path_file, path, i);

			// 先加载原始帧
			IMAGE* frame = new IMAGE();
			loadimage(frame, path_file);

			// 如果需要反转，手动逐像素水平翻转
			if (isFlipped) {
				FlipHorizontally(frame); // 调用自定义翻转函数
			}

			frame_list.push_back(frame);
		}
	}

	~Animation()
	{
		for (size_t i = 0; i<frame_list.size(); i++)  //因为前面使用了new，则要在析构函数中进行内存管理
			delete frame_list[i];
	}


	//定义play函数来作为动画播放的函数
	bool play(int x, int y, int delta, bool flipH = false) {
		timer += delta;
		bool finished = false;

		if (timer >= interval_ms) {
			idx_frame++;
			timer = 0;
			if (idx_frame >= frame_list.size()) {
				idx_frame = 0;
				finished = true;
			}
		}

		IMAGE* frame = frame_list[idx_frame];

		if (flipH) {
			// 创建一个临时翻转帧（可缓存优化）
			static IMAGE temp;
			temp = *frame;
			FlipHorizontally(&temp);
			putimage_alpha(x, y, &temp);
		}
		else {
			putimage_alpha(x, y, frame);
		}

		return finished;
	}

	
	// 手动逐像素水平翻转函数
	void FlipHorizontally(IMAGE* img) {
		int width = img->getwidth();
		int height = img->getheight();

		// 获取图像缓冲区
		DWORD* colorBuffer = GetImageBuffer(img);

		// 逐行翻转像素
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width / 2; x++) {
				// 交换 (x, y) 和 (width - x - 1, y) 的像素
				int leftIdx = y * width + x;
				int rightIdx = y * width + (width - x - 1);
				swap(colorBuffer[leftIdx], colorBuffer[rightIdx]);
			}
		}
	}


private:
	vector<IMAGE*> frame_list;//vector<IMAGE*>，这是一种结构类型，相当于IMAGE*[]数组,有点是可以动态分配内存
	//动画图片存储容器
	int interval_ms = 0; //帧间隔
	int timer = 0; //动画计时器
	int idx_frame = 0;  //动画帧索引
	bool isFlipped = false;    // 是否水平反转








};