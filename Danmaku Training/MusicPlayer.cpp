#include "MusicPlayer.h"

MusicPlayer::MusicPlayer(){
}

bool MusicPlayer::Load(std::vector<std::string> &_str) {
	musicNum = (int)_str.size();
	musicSh	 = new int[musicNum];
	for (int i=0; i < _str.size(); i++) {
		musicSh[i] = LoadSoundMem(_str[i].c_str());
	}
	return true;
}

bool MusicPlayer::Load(int _num, int *_sh) {
	if (musicSh) {
		musicSh = _sh;
		musicNum = _num;
	} else {
		return false;
	}

	return true;
}

bool MusicPlayer::SetInterval(unsigned int _n) {
	interval = _n;
	return true;
}

bool MusicPlayer::Play() {
	if (musicSh) {
		if (count > interval) {
			nowMusic = GetRand(musicNum-1);
			PlaySoundMem(musicSh[nowMusic], DX_PLAYTYPE_BACK, TRUE);
			count = 0;
		}
		if (CheckSoundMem(musicSh[nowMusic]) == 0) {
			count++;
		}
		return true;

	}else {
		return false;
	}
}

bool MusicPlayer::Start() {
	if (musicSh) {
		if (CheckSoundMem(musicSh[nowMusic]) == 0)
			PlaySoundMem(musicSh[nowMusic], DX_PLAYTYPE_BACK, FALSE);
		return true;
	}else {
		return false;
	}
}

bool MusicPlayer::Stop() {
	if (musicSh) {
		StopSoundMem(musicSh[nowMusic]);
		return true;
	}else {
		return false;
	}
}

MusicPlayer::~MusicPlayer() {
	if (musicSh) delete[] musicSh;
}