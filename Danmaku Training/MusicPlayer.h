#pragma once

#include "DxLib.h"
#include <vector>
#include <string>

/*
BGM��A���Đ����邾���B
*/

class MusicPlayer {

	int musicNum = 0,
		nowMusic = 0,
		*musicSh;
	unsigned int interval = 0, count = 0;

public:
	MusicPlayer();
	bool Load(std::vector<std::string> &_str);
	bool Load(int _num, int *_sh);
	//�ȂƋȂ̊�
	bool SetInterval(unsigned int _n);
	bool Play();
	bool Start();
	bool Stop();
	~MusicPlayer();
};