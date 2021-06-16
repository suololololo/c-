#pragma once
#ifndef GAMEINDEX_H
#define GAMEINDEX_H
#include"Draw.h"
class GameSetting;
class MakeQuestion;


class GameIndex:public Draw {
public:
	GameIndex();
	~GameIndex();
	void DrawBoard();
	void Click();
	void DrawMusicButton();
	void PlayMusic(bool on);
	int GetMode();
	void SetGameSetting(GameSetting* gSet);
private:
	bool mode;
	bool musicIsOn;
	GameSetting* gameSetting;
	MakeQuestion* makeQuestion;
	bool PressMusicButton(int x, int y);
	bool CloseMusic(int x);
	int DrawQuestion();
};

#endif // !GAMEINDEX_H