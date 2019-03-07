#include "Main.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	//準備
	set();
	if (DxLib_Init() == -1) return -1;
	SetDrawScreen(DX_SCREEN_BACK);
	load();

	//MainLoop
	while (ProcessLoop() == 0 && !endflag) {

		t.record();
		gameMode();
		ScreenFlip();
		t.wait();
	}

	//終了処理
	if (b)			delete[] b;
	if (bgmSh)		delete[] bgmSh;
	if (bulletGh)	delete[] bulletGh;
	if (playerGh)	delete[] playerGh;
	if (playerGX)	delete[] playerGX;
	if (playerGY)	delete[] playerGY;
	DxLib_End();
	return 0;
}

void set() {
	ChangeWindowMode(TRUE);
	SetMainWindowText(gameName.c_str());
	SetGraphMode(winX, winY, 32);
	SetWindowSizeChangeEnableFlag(TRUE);
}

void load() {

	white		= GetColor(255, 255, 255);
	black		= GetColor(0, 0, 0);
	sin45		= sinf((float)PI/4.0f);

	//読み込み↓
	resultGh	= LoadGraph(RESULTGRAPHPASS);
	titleGh[0]	= LoadGraph(TITLEGRAPHPASS0);
	titleGh[1]	= LoadGraph(TITLEGRAPHPASS1);
	titleGh[2]	= LoadGraph(TITLEGRAPHPASS2);
	
	vector <string> bulletGraphPass;
	FindDirectoryAll( BULLETGRAPHPASS, bulletGraphPass, "\\*.jpg");
	FindDirectoryAll( BULLETGRAPHPASS, bulletGraphPass, "\\*.png");
	bulletGNum	= (int)bulletGraphPass.size();
	bulletGh	= new int[bulletGNum];
	for (int i=0; i < bulletGNum; i++) {
		bulletGh[i] = LoadGraph(bulletGraphPass[i].c_str());
	}
	nowBulletGNum = bulletGNum;

	vector <string> playerGraphPass;
	FindDirectoryAll( PLAYERGRAPHPASS, playerGraphPass, "\\*.jpg");
	FindDirectoryAll( PLAYERGRAPHPASS, playerGraphPass, "\\*.png");
	playerGNum	= (int)playerGraphPass.size();
	playerGh	= new int[playerGNum];
	playerGX	= new int[playerGNum];
	playerGY	= new int[playerGNum];
	for (int i=0; i < playerGNum; i++) {
		playerGh[i] = LoadGraph(playerGraphPass[i].c_str());
		GetGraphSize(playerGh[i], &playerGX[i], &playerGY[i]);
	}
	nowPlayerGNum = playerGNum;
	
	pushSh		= LoadSoundMem(PUSHSOUNDPASS);
	enterSh		= LoadSoundMem(ENTERSOUNDPASS);
	collisionSh	= LoadSoundMem(COLLISIONSOUNDPASS);
	resultSh	= LoadSoundMem(ENDSOUNDPASS);
	startSh		= LoadSoundMem(STARTSOUNDPASS);
	
	vector <string> bgmSoundPass;
	FindDirectoryAll( BGMPASS, bgmSoundPass, "\\*.mp3");
	FindDirectoryAll( BGMPASS, bgmSoundPass, "\\*.wav");
	m.Load(bgmSoundPass);
	//↑ここまで
	
	//player つくる
	p.circleC	= black;
	p.charC		= white;
	SetFontSize(p.size*2);

	//メニュー系　生成
	pouseFonth  = CreateFontToHandle(NULL, fontSize, fontthick, DX_FONTTYPE_NORMAL);
	resultFonth = CreateFontToHandle(NULL, int(fontSize*3.0f), fontthick, DX_FONTTYPE_NORMAL);
	pouseCursor.setString(pouseFonth, "->");
	pouseItem[0].setString(pouseFonth, "戻る");
	pouseItem[1].setString(pouseFonth, "タイトルへ");
	pouseItem[2].setString(pouseFonth, "終わる");
	pouse.setCursorItem(&pouseCursor);
	pouse.setItem((int)size(pouseItem), pouseItem);
	pouse.setPos(winX/2.0f, winY/2.6f);

	configNameItem[BNUM].setString(pouseFonth, "弾の数");
	configNameItem[BVMAX].setString(pouseFonth, "速さ");
	configNameItem[BSMAX].setString(pouseFonth, "最大サイズ");
	configNameItem[BSMIN].setString(pouseFonth, "最小サイズ");
	configNameItem[SPACE].setString(pouseFonth, "　　　　　");
	configNameItem[PV].setString(pouseFonth, "自機の速さ");
	configNameItem[PSLOWV].setString(pouseFonth, "自機の速さ2");
	configName.setCursorItem(&pouseCursor);
	configName.setItem((int)size(configNameItem), configNameItem);
	configName.setBasePos(LEFTED);
	configName.setPos(winX/4.0f, winY/4.0f);

	configNumItem[BNUM].setNumMode( (float)con.bulletNum,	  1.0f, 999.0f, 1.0f);
	configNumItem[BNUM].setFont(pouseFonth);
	configNumItem[BVMAX].setNumMode((float)con.bulletVMax,	  1.0f, 999.0f, 1.0f);
	configNumItem[BVMAX].setFont(pouseFonth);
	configNumItem[BSMAX].setNumMode((float)con.bulletSizeMax, 1.0f, 999.0f, 1.0f);
	configNumItem[BSMAX].setFont(pouseFonth);
	configNumItem[BSMIN].setNumMode((float)con.bulletSizeMin, 1.0f, 999.0f, 1.0f);
	configNumItem[BSMIN].setFont(pouseFonth);
	configNumItem[SPACE].setString(pouseFonth, "　　　");
	configNumItem[PV].setNumMode(p.v, 1.0f, 999.0f, 1.0f);
	configNumItem[PV].setFont(pouseFonth);
	configNumItem[PSLOWV].setNumMode(p.slowV, 1.0f, 999.0f, 1.0f);
	configNumItem[PSLOWV].setFont(pouseFonth);
	configNumColumns.setItem((int)size(configNumItem), configNumItem);
	configNumColumns.setBasePos(LEFTED);
	configNumColumns.setPos(winX/1.8f, winY/4.0f);
}

void checkKey() {

	GetHitKeyStateAll(key);
	//ポーズメニューに入るだけ
	if (key[KEY_INPUT_ESCAPE] == 1) {
		m.Stop();
		gamestate = POUSE;
		PlaySoundMem(pushSh, DX_PLAYTYPE_BACK, TRUE);
	}
	//enterは長押し防止
	if (key[KEY_INPUT_RETURN] == 1) {
		k.enter	  = false;
		if (!enterflag) k.enter = true;
		enterflag = true;
	}else {
		k.enter	  = false;
		enterflag = false;
	}

	//KEYINTERVAL ごとに取得
	if (!keyFlag) {
		if (key[KEY_INPUT_UP] == 1 || key[KEY_INPUT_W] == 1) {
			k.up	= true;
			keyTime = GetNowCount();
			keyFlag = true;

		} else if (key[KEY_INPUT_DOWN] == 1 || key[KEY_INPUT_S] == 1)  {
			k.down	= true;
			keyTime = GetNowCount();
			keyFlag = true;

		} else if (key[KEY_INPUT_RIGHT] == 1 || key[KEY_INPUT_D] == 1) {
			k.right = true;
			keyTime = GetNowCount();
			keyFlag = true;

		} else if (key[KEY_INPUT_LEFT] == 1 || key[KEY_INPUT_A] == 1)  {
			k.left  = true;
			keyTime = GetNowCount();
			keyFlag = true;

		}else if (key[KEY_INPUT_LSHIFT] == 1 || key[KEY_INPUT_RSHIFT] == 1) {
			k.shift = true;
			keyTime = GetNowCount();
			keyFlag = true;

		}
	}else {
		k.enter = false;
		k.up	= false;
		k.down	= false;
		k.right = false;
		k.left  = false;
		k.shift = false;
	}
	if (GetNowCount() - keyTime >= KEYINTERVAL) keyFlag = false;
}

void gameMode() {

	//背景
	DrawBox(0, 0, winX+1, winY+1, white, TRUE);

	checkKey();

	switch (gamestep) {

	case TITLE:
		startMenu();
		break;

	case GAME:

		if (gamestate == PLAYING) {
			player();
			bullet();
			if (!firstHit) {
				m.Play();
				surviveTime++;
			}
		}
		bulletShow();

		if (!p.hit) {
			playerShow();

		}else if(!firstHit) {
			m.Stop();
			PlaySoundMem(resultSh, DX_PLAYTYPE_BACK, TRUE);
			//スコア計算。長い。特に考えてない。
			score	  = unsigned long ((surviveTime + grazeCount) * (p.v+p.slowV + con.bulletVMax * (con.bulletSizeMax-con.bulletSizeMin+1.0) * 0.008f + con.bulletSizeMin * con.bulletNum * 0.2f));
			firstHit  = true;
		}
		if (firstHit) {
			resultMenu();
		}

		//弾の準備
		if (gamestate == READY) {
			if (t.getCount() - oldTime >= INTERVAL && bulletCount < con.bulletNum) {
				oldTime = t.getCount();
				//自機近くに出るのを防ぐ。
				do {
					b[bulletCount].x = (float)GetRand(winX);
					b[bulletCount].y = (float)GetRand(winY);
				} while ( (b[bulletCount].x-p.x)*(b[bulletCount].x-p.x) + (b[bulletCount].y-p.y)*(b[bulletCount].y-p.y) 
							< (b[bulletCount].size+p.size+con.bulletVMax)*(b[bulletCount].size+p.size+con.bulletVMax));

				b[bulletCount].vX	= GetRand(con.bulletVMax) * con.bulletVRatio;
				b[bulletCount].vY	= GetRand(con.bulletVMax) * con.bulletVRatio;
				b[bulletCount].size = (GetRand(con.bulletSizeMax-con.bulletSizeMin) + con.bulletSizeMin) * con.bulletSizeRatio;
				b[bulletCount].circleC = GetColor(	int(255.0f/2.0f + 255.0f/2.0f * (b[bulletCount].vX   / ((float)con.bulletVMax * con.bulletVRatio) )),
													int(255.0f/2.0f + 255.0f/2.0f * (b[bulletCount].vY   / ((float)con.bulletVMax * con.bulletVRatio) )),
													int(255.0f * (b[bulletCount].size / ((float)con.bulletSizeMax * con.bulletSizeRatio) )));
				bulletCount++;
			}else if (bulletCount == con.bulletNum) {
				//準備後、スタート音を鳴らす。
				//鳴り終わった瞬間スタート。
				if (!startSound) {
					PlaySoundMem(startSh, DX_PLAYTYPE_BACK, TRUE);
					startSound = true;
				}else if (CheckSoundMem(startSh) == 0) {
					gamestate = PLAYING;
				}
			}
		}
		if (gamestate == POUSE) pouseMenu();
		break;
	}
}

void startMenu() {

	//スタート画面で弾を動かす。
	if (!startInit) {
		p.x = winX/2.0f;
		p.y = winY/2.0f;
		b	= new Bullet[con.bulletNum];
		for (bulletCount = 0; bulletCount < con.bulletNum; bulletCount++) {
			b[bulletCount].x	= (float)GetRand(winX);
			b[bulletCount].y	= (float)GetRand(winY);
			b[bulletCount].vX	= GetRand(con.bulletVMax) * con.bulletVRatio;
			b[bulletCount].vY	= GetRand(con.bulletVMax) * con.bulletVRatio;
			b[bulletCount].size = (GetRand(con.bulletSizeMax-con.bulletSizeMin) + con.bulletSizeMin) * con.bulletSizeRatio;
			b[bulletCount].circleC = GetColor(	int(255.0f/2.0f + 255.0f/2.0f * (b[bulletCount].vX   / ((float)con.bulletVMax * con.bulletVRatio) )),
												int(255.0f/2.0f + 255.0f/2.0f * (b[bulletCount].vY   / ((float)con.bulletVMax * con.bulletVRatio) )),
												int(255.0f * (b[bulletCount].size / ((float)con.bulletSizeMax * con.bulletSizeRatio) )));
		}
		startInit = true;
	}

	bullet();
	bulletShow();
	//player()は無し。
	playerShow();

	//スタート画面
	if (!pressStart) {
		if (titleInterval <= TITLEINTERVAL)			DrawGraph(titleX, titleY, titleGh[0], TRUE);
		else if (titleInterval <= TITLEINTERVAL*2)	DrawGraph(titleX, titleY, titleGh[1], TRUE);
		else if (titleInterval <= TITLEINTERVAL*3)	DrawGraph(titleX, titleY, titleGh[2], TRUE);
		else										titleInterval = 0;
		titleInterval++;
		DrawStringToHandle(pleaseX, pleaseY, "press ENTER", black, pouseFonth);
		if (k.left) {
			PlaySoundMem(pushSh, DX_PLAYTYPE_BACK, TRUE);
			nowBulletGNum--;
			if (nowBulletGNum < 0) nowBulletGNum = bulletGNum;
		}else if (k.right) {
			PlaySoundMem(pushSh, DX_PLAYTYPE_BACK, TRUE);
			nowBulletGNum++;
			if (nowBulletGNum > bulletGNum) nowBulletGNum = 0;
		}else if (k.up) {		
			PlaySoundMem(pushSh, DX_PLAYTYPE_BACK, TRUE);
			nowPlayerGNum++;
			if (nowPlayerGNum > playerGNum) nowPlayerGNum = 0;
		}else if (k.down) {
			PlaySoundMem(pushSh, DX_PLAYTYPE_BACK, TRUE);
			nowPlayerGNum--;
			if (nowPlayerGNum < 0) nowPlayerGNum = playerGNum;
		}else if (k.shift) {
			PlaySoundMem(pushSh, DX_PLAYTYPE_BACK, TRUE);
			pScale =! pScale;
		}
	}

	//ゲームスタート
	if (k.enter) {
		PlaySoundMem(enterSh, DX_PLAYTYPE_BACK, TRUE);

		if (pressStart) {
			//リセット
			pressStart = false;
			configName.setCurrentItem(0);
			configNumColumns.setCurrentItem(0);

			//最初の処理
			con.bulletNum		= (int)configNumItem[BNUM].getNum();
			con.bulletVMax		= (int)configNumItem[BVMAX].getNum();
			con.bulletSizeMax	= (int)configNumItem[BSMAX].getNum();
			con.bulletSizeMin	= (int)configNumItem[BSMIN].getNum();
			p.v					= configNumItem[PV].getNum();
			p.slowV				= configNumItem[PSLOWV].getNum();
			b			= new Bullet[con.bulletNum];
			bulletCount = 0;
			surviveTime = 0;
			p.x			= winX/2.0f;
			p.y			= winY/2.0f;
			p.hit		= false;
			firstHit	= false;
			resultShow  = true;
			startSound  = false;
			gamestep	= GAME;
			gamestate	= READY;
		}
		pressStart = true;
	}

	if (pressStart) {
		if (k.up) {
			configName.upItem();
			configNumColumns.upItem();
			PlaySoundMem(pushSh, DX_PLAYTYPE_BACK, TRUE);

		}else if (k.down) {
			configName.downItem();
			configNumColumns.downItem();
			PlaySoundMem(pushSh, DX_PLAYTYPE_BACK, TRUE);
			
		}else if (k.right) {
			configNumItem[configNumColumns.currentItem()].upNum();
			PlaySoundMem(pushSh, DX_PLAYTYPE_BACK, TRUE);
			
		}else if (k.left) {
			configNumItem[configNumColumns.currentItem()].downNum();
			PlaySoundMem(pushSh, DX_PLAYTYPE_BACK, TRUE);
			
		}
		configName.show();
		configNumColumns.show();
	}
}

void pouseMenu() {

	if (k.up) {
		pouse.upItem();
		PlaySoundMem(pushSh, DX_PLAYTYPE_BACK, TRUE);
	}else if (k.down) {
		pouse.downItem();
		PlaySoundMem(pushSh, DX_PLAYTYPE_BACK, TRUE);
	}

	if (k.enter) {
		PlaySoundMem(enterSh, DX_PLAYTYPE_BACK, TRUE);

		switch (pouse.currentItem()) {
		//戻る
		case 0:
			m.Start();
			gamestate = PLAYING;
			break;
		//タイトルへ
		case 1:
			if (b) delete[] b;
			gamestep   = TITLE;
			pressStart = false;
			startInit  = false;
			break;
		//終わる
		case 2:
			endflag = true;
			break;
		}
	}
	pouse.show();
}

void resultMenu() {
	if (k.shift) resultShow =! resultShow;
	if (resultShow) {
		DrawGraph(0,0,resultGh, TRUE);
		DrawFormatStringToHandle(scoreX, scoreY, black, resultFonth, "%d", score);
		DrawFormatStringToHandle(timeX, timeY, black, pouseFonth, "生存時間%3.1f秒", (float)surviveTime/60.f);
	}
}

void player() {

	float x = 0, y = 0;

	//入力処理
	if (key[KEY_INPUT_LEFT]  == 1 || key[KEY_INPUT_A] == 1) x--;
	if (key[KEY_INPUT_RIGHT] == 1 || key[KEY_INPUT_D] == 1) x++;
	if (key[KEY_INPUT_UP]    == 1 || key[KEY_INPUT_W] == 1) y--;
	if (key[KEY_INPUT_DOWN]  == 1 || key[KEY_INPUT_S] == 1) y++;

	//斜め移動
	if ( x*y != 0) {
		x *= sin45;
		y *= sin45;
	}
	//移動速度判定
	if(key[KEY_INPUT_LSHIFT] == 1 || key[KEY_INPUT_RSHIFT] == 1){
		x *= p.slowV;
		y *= p.slowV;
		shiftFlag = true;
	}else {
		x *= p.v;
		y *= p.v;
		shiftFlag = false;
	}
	p.x += x;
	p.y += y;

	//壁
	if (p.x < 0) {
		p.x = 0;
	}else if (p.x > winX) {
		p.x = (float)winX;
	}
	if (p.y < 0) {
		p.y = 0;
	}else if (p.y > winY) {
		p.y = (float)winY;
	}
}

void playerShow() {
	//画像とデフォルトの切り替え
	if (nowPlayerGNum == playerGNum) {
		DrawCircleAA(p.x, p.y, (float)p.size, p.verNum, p.circleC, TRUE);
		DrawString((int)p.x-p.size/2, (int)p.y-p.size, &p.s, p.charC);
	}else {
		if (pScale) {
			DrawExtendGraph(int(p.x-p.size), int(p.y-p.size), int(p.x+p.size), int(p.y+p.size), playerGh[nowPlayerGNum], TRUE);
		}else {
			DrawGraph(int(p.x-playerGX[nowPlayerGNum]/2.0f), int(p.y-playerGY[nowPlayerGNum]/2.0f), playerGh[nowPlayerGNum], TRUE);
		}
	}
}

void bullet() {

	int   i	   = 0,	j	 = 0,
		  disX = 0,	disY = 0;
	float distance = 0, size2 = 0;

	for (i = 0; i < con.bulletNum; i++) {

		b[i].x +=b[i].vX;
		b[i].y +=b[i].vY;
		
		//弾同士の跳ね返り
		for (j = i+1; j < con.bulletNum; j++) {

			if ( (abs(b[i].x - b[j].x) < b[i].size + b[j].size) && (abs(b[i].y - b[j].y) < b[i].size + b[j].size)){

				distance = (b[i].x - b[j].x)*(b[i].x - b[j].x) + (b[i].y - b[j].y)*(b[i].y - b[j].y);
				size2    = (b[i].size + b[j].size)*(b[i].size + b[j].size);
				if (distance - size2 < 0) {
					elasticCollision( &b[i].vX, &b[j].vX, b[i].size, b[j].size);
					elasticCollision( &b[i].vY, &b[j].vY, b[i].size, b[j].size);
					b[i].x += ((b[i].x - b[j].x > 0) ? 1.0f : -1.0f) * (sqrtf(size2/distance) + 1.0f);
					b[i].y += ((b[i].y - b[j].y > 0) ? 1.0f : -1.0f) * (sqrtf(size2/distance) + 1.0f);
				}
			}
		}
		
		//壁との跳ね返り
		if (b[i].x - b[i].size < 0) {

			if (b[i].vX < 0) b[i].vX *= -1;

		}else if (b[i].x + b[i].size > winX) {

			if (b[i].vX > 0) b[i].vX *= -1;
		}
		if (b[i].y - b[i].size <= 0) {

			if (b[i].vY < 0) b[i].vY *= -1;

		}else if (b[i].y + b[i].size > winY) {

			if (b[i].vY > 0) b[i].vY *= -1;
		}

		b[i].circleC = GetColor(int(255.0f/2.0f + 255.0f/2.0f * (b[i].vX   / ((float)con.bulletVMax * con.bulletVRatio) )),
								int(255.0f/2.0f + 255.0f/2.0f * (b[i].vY   / ((float)con.bulletVMax * con.bulletVRatio) )),
								int(255.0f * (b[i].size / ((float)con.bulletSizeMax * con.bulletSizeRatio) )));

		//自機との当たり判定
		if ( (abs(b[i].x - p.x) < (b[i].size + p.size)) && ( abs(b[i].y - p.y) < (b[i].size + p.size))) {
			grazeCount++;
			if ((b[i].x - p.x)*(b[i].x - p.x) + (b[i].y - p.y)*(b[i].y - p.y) < (b[i].size + p.size)*(b[i].size + p.size)) {
				p.hit = true;
			}
		}
	}
}

void bulletShow() {
	for (int i = 0; i < con.bulletNum; i++) {
		if (nowBulletGNum == bulletGNum) {
			DrawCircleAA(b[i].x, b[i].y, b[i].size, b[i].verNum, b[i].circleC, TRUE);
		}else {
			DrawExtendGraph(int(b[i].x-b[i].size), int(b[i].y-b[i].size), int(b[i].x+b[i].size+1), int(b[i].y+b[i].size+1), bulletGh[nowBulletGNum], TRUE);
		}
	}
}

//衝突後の速度を求めて更新します。
void elasticCollision(float *v1, float *v2, float m1, float m2) {
	float	v1Old = *v1,
			v2Old = *v2;

	*v1 = (v1Old*m1 - v1Old*m2 + 2*v2Old*m2) / (m1+m2);
	*v2 = (v2Old*m2 - v2Old*m1 + 2*v1Old*m1) / (m1+m2);
}

//dxlibの必要なやつ
int ProcessLoop() {
	if (ProcessMessage() != 0 || ClearDrawScreen() != 0) return -1;
	return 0;
}

int FindDirectory ( std::string folderPath, std::vector<std::string> &path) {

	FindDirectory (folderPath, path, "\\*.*");

	return 0;
}

int FindDirectory ( std::string folderPath, std::vector<std::string> &path, const std::string fileFilter) {
	WIN32_FIND_DATA tFindFileData;

	folderPath += fileFilter;

	// 最初に一致するファイルを取得
	HANDLE hFile = FindFirstFile( folderPath.c_str(), &tFindFileData);
	if (INVALID_HANDLE_VALUE == hFile) return -1;

	folderPath = folderPath.erase( folderPath.size()-fileFilter.size(), folderPath.size()-1);
	
	do { 
		std::string wpFileName = tFindFileData.cFileName;

		//L"."とL".."はスキップ
		if ('.' == wpFileName[0])
			if ( ('\0' == wpFileName[1]) || ('.' == wpFileName[1] && '\0' == wpFileName[2]) )
				continue;

		std::string fullPath = folderPath + "\\" + wpFileName;
		path.push_back(fullPath);

	} while ( FindNextFile(hFile, &tFindFileData) );
	
	FindClose(hFile);

	return 0;
}

int FindDirectoryAll ( std::string folderPath, std::vector<std::string> &path) {

	FindDirectoryAll (folderPath, path, "\\*.*");

	return 0;
}

int FindDirectoryAll ( std::string folderPath, std::vector<std::string> &path, const std::string fileFilter) {
	WIN32_FIND_DATA tFindFileData;

	folderPath += fileFilter;

	// 最初に一致するファイルを取得
	HANDLE hFile = FindFirstFile( folderPath.c_str(), &tFindFileData);
	if (INVALID_HANDLE_VALUE == hFile) return -1;

	folderPath = folderPath.erase( folderPath.size()-fileFilter.size(), folderPath.size()-1);

	do { 
		std::string wpFileName = tFindFileData.cFileName;

		if (tFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

			//L"."とL".."はスキップ
			if ('.' == wpFileName[0])
				if ( ('\0' == wpFileName[1]) || ('.' == wpFileName[1] && '\0' == wpFileName[2]) )
					continue;

			std::string fullPath = folderPath + "\\" + wpFileName;
			path.push_back(fullPath);

			FindDirectoryAll ( fullPath, path, fileFilter);

		}else {

			std::string fullPath = folderPath + "\\" + wpFileName;
			path.push_back(fullPath);
		}

	} while ( FindNextFile(hFile, &tFindFileData) );

	FindClose(hFile);

	return 0;
}