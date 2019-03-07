#pragma once

#include "DxLib.h"

//fps��ۂ��߂̃N���X�B

class TimeKeeper {

	int		startTime, fcount;
	float	fps;
	unsigned long int	allCount;
	static const int	N	 = 60;
	static const int	FPS	 = 60;

public:
	TimeKeeper();
	float record();
	unsigned long int getCount();
	void  wait();
	void init();
};
