#pragma once

//----------------------------------------
//弾幕練習プログラム
//製作者：珠響無色
//
//2019.03.06　ver1.0　フリーゲーム夢現に投稿しました。
//DanmakuTraining
//written by Tamayura Musyoku.
//----------------------------------------

#include <algorithm>
#include <string>
#include <vector>
#include "DxLib.h"
#include "TimeKeeper.h"
#include "Choices.h"
#include "MusicPlayer.h"

#define PI 3.1415926535897932384626433832795

using namespace std;

//条件分岐に使う定数
//gamestep
enum {
	TITLE = 0,
	GAME
};
//gamestate
enum {
	READY = 0,
	PLAYING,
	POUSE
};

//gameConfig
enum {
	BNUM = 0,
	BVMAX,
	BSMAX,
	BSMIN,
	SPACE,
	PV,
	PSLOWV
};

//感覚調整
#define INTERVAL		5
#define KEYINTERVAL		100
#define TITLEINTERVAL	25

//ファイル関連
#define RESULTGRAPHPASS "source/Result.png"
#define TITLEGRAPHPASS0 "source/Title/0.png"
#define TITLEGRAPHPASS1 "source/Title/1.png"
#define TITLEGRAPHPASS2 "source/Title/2.png"
#define BULLETGRAPHPASS "bullet"
#define PLAYERGRAPHPASS "player"

#define PUSHSOUNDPASS		"source/sound/push.mp3"
#define ENTERSOUNDPASS		"source/sound/enter.mp3"
#define COLLISIONSOUNDPASS	"source/sound/collision.mp3"
#define ENDSOUNDPASS		"source/sound/end.mp3"
#define STARTSOUNDPASS		"source/sound/start.mp3"
#define BGMPASS				"bgm"


bool		 endflag	 = false,
			 enterflag	 = false,
			 firstHit	 = false,
			 shiftFlag	 = false,
			 keyFlag	 = false,
			 pressStart  = false,
			 startInit	 = false,
			 titleChange = false,
			 resultShow	 = true,
			 pScale		 = true,
			 startSound  = false;
char		 key[256],
			 gamestep	 = TITLE,
			 gamestate   = READY;
int			 winX		 = 960,
			 winY		 = 720,
			 pouseFonth	 = 0,
			 resultFonth = 0,
			 fontSize	 = 40,
			 fontthick	 = -1,
			 bulletCount = 0,
			 resultGh,
			 titleGh[3],
		     titleX  = int(winX/5.0f), titleY  = int(winY/6.0f),
			 pleaseX = int(winX/2.5f), pleaseY = int(winY/1.5f),
			 scoreX  = int(winX/5.0f), scoreY  = int(winY/1.5f),
			 timeX   = int(winX/2.0f), timeY   = int(winY/2.2f),
			 pushSh, enterSh,
			 collisionSh,
			 startSh,
			 resultSh,
			 *bgmSh,
			 bulletGNum		= 0,
			 nowBulletGNum	= 0,
			 *bulletGh,
			 playerGNum		= 0,
			 nowPlayerGNum	= 0,
			 *playerGh,
			 *playerGX, *playerGY;
float		 sin45;

unsigned int		white,
					black;
unsigned long int	titleInterval	= 0,
					keyTime			= 0,
					oldTime			= 0,
					surviveTime		= 0,
					grazeCount		= 0,
					score			= 0;

//キーボード判定に使う
struct keyStateFlag {
	bool up		= false,
		 down	= false,
		 left	= false,
		 right	= false,
		 enter	= false,
		 shift  = false;
} k;

struct gameConfig {
	int bulletNum		     = 10,
		bulletVMax			 = 100,
		bulletSizeMax		 = 24,
		bulletSizeMin		 = 6;
	float	bulletVRatio	 = 0.05f,
			bulletSizeRatio  = 2;
} con;

struct player {
	bool    hit = false;
	int		size = 16, verNum = 16;
	float	x = 0,	y = 0,
			v = 10.0,  slowV = 6.0;
	unsigned int circleC = 0, charC = 0;
	char    s = 'P';
} p;

typedef struct {
	bool  collisionFlag = false;
	const int verNum = 64;
	float size	= 16,
		  x		= 0,	y	= 0,
		  vX	= 0,	vY  = 0;
	unsigned int circleC = 0;
} Bullet;

string gameName = "DanmakuTraining";

TimeKeeper	t;
Bullet		*b;
ChoicesItem pouseItem[3],
			pouseCursor,
			configNameItem[PSLOWV+1],
			configNumItem[PSLOWV+1];
Choices		pouse,
			configName,
			configNumColumns;
MusicPlayer m;

void set();
void load();
//ゲーム中以外の入力をまとめるためにつくりました。
void checkKey();

void gameMode();
void startMenu();
void pouseMenu();
void resultMenu();

//処理部と描画部を分けています。
void player();
void playerShow();
void bullet();
void bulletShow();

void elasticCollision(float *v1, float *v2, float m1, float m2);

int ProcessLoop();

//指定したフォルダ内のファイルを巡回する。
//folderPathで対象のフォルダを指定。pathでファイルの一覧を受け取る。
//fileFilterで拡張子を限定する。
//拡張子は一つのみ
int FindDirectory ( std::string folderPath, std::vector<std::string> &path);
int FindDirectory ( std::string folderPath, std::vector<std::string> &path, const string fileFilter);
int FindDirectoryAll ( std::string folderPath, std::vector<std::string> &path);
int FindDirectoryAll ( std::string folderPath, std::vector<std::string> &path, const std::string fileFilter);