#pragma once

#include "DxLib.h"
#include <string>

using namespace std;

/*
ポーズメニュー等を作るためのクラスです。
ChoicesItem　で画像、文字、数字を一緒に扱えるようにして、
Choices　で選択肢にします。
*/
//左揃え　中央揃え　右揃え
enum {
	LEFTED = 0,
	CENTERED,
	RIGHTED
};

class ChoicesItem {

	bool  numMode   = false,
		  floatFlag = false;
	unsigned int color = GetColor(0, 0, 0);
	int   halfWidth = 0, halfHeight = 0,
		  fonth  = 0,
		  gh     = 0;
	float x      = 0, y = 0,
		  currentNum  = 0,
		  updownRatio = 0,
		  max	= 0,
		  min	= 0;
	string str   = "";

public:
	ChoicesItem();
	bool  setString(int _fh, string _str);
	bool  setString(int _fh, string _str, unsigned int _color);
	bool  setGraph(int _gh);
	bool  setFont(int _fh);
	bool  setNumMode(float _initNum, float _updownRatio, float _max, float _min);
	bool  setNumMode(float _initNum, float _updownRatio, float _max, float _min, unsigned int _color);
	bool  setNumMode(float _initNum, float _updownRatio, float _max, float _min, unsigned int _color, bool _floatFrag);
	bool  setPos(float _x, float _y);
	bool  upNum();
	bool  downNum();
	float getNum();
	int   getWidth();
	int   getHeight();
	float getX();
	float getY();
	bool  show();
};

class Choices {

	char cursorNum = 0,
		 basePos   = CENTERED;
	int  itemNum    = 0,
		 halfWidth  = 0, halfHeight = 0;
	float x = 0, y = 0;
	ChoicesItem *cursorItem = 0,
				*items = 0,
				backItem;

public:
	Choices();
	bool  setCursorItem(ChoicesItem *_Items);
	bool  setItem(int _itemNum, ChoicesItem *_Items);
	bool  setPos(float _x, float _y);
	bool  setBackGh(int _gh);
	bool  setBasePos(char mode);
	bool  upItem();
	bool  downItem();
	bool  setCurrentItem(int _num);
	int   currentItem();
	bool  show();
	~Choices();
};