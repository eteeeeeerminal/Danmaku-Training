#include "Choices.h"

ChoicesItem::ChoicesItem() {
}

bool ChoicesItem::setString(int _fh, string _str) {
	fonth    = _fh;
	str      = _str;
	char name[256];
	int  size, thick;
	GetFontStateToHandle(name, &size, &thick, fonth);
	halfHeight = size / 2;
	halfWidth  = GetDrawFormatStringWidthToHandle(fonth, str.c_str()) / 2;
	return true;
}

bool ChoicesItem::setString(int _fh, string _str, unsigned int _color) {
	fonth    = _fh;
	str      = _str;
	color    = _color;
	char name[256];
	int  size, thick;
	GetFontStateToHandle(name, &size, &thick, fonth);
	halfHeight = size / 2;
	halfWidth  = GetDrawFormatStringWidthToHandle(fonth, str.c_str()) / 2;
	return true;
}

bool ChoicesItem::setGraph(int _gh) {
	gh		 = _gh;
	GetGraphSize(gh, &halfWidth, &halfHeight);
	halfHeight /= 2;
	halfWidth  /= 2;
	return true;
}

bool ChoicesItem::setFont(int _fh) {
	fonth = _fh;
	return true;
}


bool ChoicesItem::setNumMode(float _initNum, float _updownRatio, float _max, float _min) {
	currentNum  = _initNum;
	updownRatio = _updownRatio;
	max			= _max;
	min			= _min;
	numMode		= true;
	return true;
}

bool ChoicesItem::setNumMode(float _initNum, float _updownRatio, float _max, float _min, unsigned int _color) {
	currentNum  = _initNum;
	updownRatio = _updownRatio;
	max			= _max;
	min			= _min;
	color		= _color;
	numMode		= true;
	return true;
}

bool ChoicesItem::setNumMode(float _initNum, float _updownRatio, float _max, float _min, unsigned int _color, bool _floatFrag) {
	currentNum  = _initNum;
	updownRatio = _updownRatio;
	max			= _max;
	min			= _min;
	color		= _color;
	numMode		= true;
	floatFlag   = _floatFrag;
	return true;
}

bool ChoicesItem::setPos(float _x, float _y) {
	x = _x;
	y = _y;
	return true;
}

bool ChoicesItem::upNum() {
	currentNum += updownRatio;
	if (currentNum > max) currentNum = max;
	return true;
}

bool ChoicesItem::downNum() {
	currentNum -= updownRatio;
	if (currentNum < min) currentNum = min;
	return true;
}

float ChoicesItem::getNum() {
	return currentNum;
}

int  ChoicesItem::getWidth() {
	return halfWidth * 2;
}

int  ChoicesItem::getHeight() {
	return halfHeight * 2;
}

float ChoicesItem::getX() {
	return x;
}

float ChoicesItem::getY() {
	return y;
}

bool ChoicesItem::show() {
	if (numMode) {
		if (floatFlag) {
			setString(fonth, to_string(currentNum));
		}else {
			setString(fonth, to_string((int)currentNum));
		}
	}
	if (gh != 0) {
		DrawGraph((int)x - halfWidth, (int)y - halfHeight, gh, TRUE);
		return true;
	}
	if (str != "") {
		DrawStringToHandle((int)x - halfWidth, (int)y - halfHeight, str.c_str(), color, fonth);
		return true;
	}
	return false;
}

Choices::Choices() {
}

bool Choices::setCursorItem(ChoicesItem *_Items) {
	cursorItem = _Items;
	return true;
}

bool Choices::setItem(int _itemNum, ChoicesItem *_Items) {
	items   = _Items;
	itemNum = _itemNum;
	return true;
}

bool Choices::setPos(float _x, float _y) {
	x = _x;
	y = _y;
	return true;
}

bool Choices::setBackGh(int _gh) {
	backItem.setGraph(_gh);
	backItem.setPos(0, 0);
	return true;
}

bool Choices::setBasePos(char mode) {
	basePos = mode;
	return true;
}

bool Choices::upItem() {
	if (cursorNum != 0) cursorNum--;
	return true;
}

bool Choices::downItem() {
	if (cursorNum < itemNum-1) cursorNum++;
	return true;
}

bool Choices::setCurrentItem(int _num) {
	cursorNum = _num;
	return true;
}

int  Choices::currentItem() {
	return cursorNum;
}

bool Choices::show() {
	
	float _x = x, _y = y;

	backItem.setPos(x, y);
	backItem.show();
	for (int i = 0; i < itemNum; i++) {
		_y += items[i].getHeight()/2.0f;
		switch (basePos) {
		case LEFTED:
			items[i].setPos(_x + items[i].getWidth()/2.0f, _y);
			break;

		case CENTERED:
			items[i].setPos(_x, _y);
			break;

		case RIGHTED:
			items[i].setPos(_x - items[i].getWidth()/2.0f, _y);
			break;
		}
		items[i].show();
		_y += items[i].getHeight()/2.0f;
	}
	if (cursorItem) {
		cursorItem->setPos(items[cursorNum].getX() - items[cursorNum].getWidth()/2.0f - cursorItem->getWidth()/2.0f, items[cursorNum].getY());
		cursorItem->show();
	}
	return true;
}

Choices::~Choices() {
}