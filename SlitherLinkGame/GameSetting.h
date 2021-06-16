#pragma once
#ifndef GAMESETTING_H
#define GAMESETTING_H
#include"Draw.h"
#include"GameIndex.h"


class GameSetting :public Draw {
public:
	GameSetting(GameIndex* gIndex);
	void DrawBoard();
	void Click();
	int GetDifficulty();
	int GetChallengeTime();
private:
	int difficulty;
	int challengeTime;
	bool musicIsOn;
	GameIndex* gameIndex;
	void UpdateDifficulty();
	void SetChallengeTime();
	void PlayMusic(bool on);
};


#endif // !GAMESETTING_H

