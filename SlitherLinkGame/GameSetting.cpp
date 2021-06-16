#include"GameSetting.h"
#include"GameIndex.h"
#include<graphics.h>
#include<stdio.h>
#include <iostream>
#include<string>
using namespace std;
GameSetting::GameSetting(GameIndex* gIndex) {
	difficulty = 1;
	challengeTime = 100;
	gameIndex = gIndex;
	musicIsOn = true;
}

void GameSetting::Click() {
	MOUSEMSG m;
	bool isBreak = false;
	while (!isBreak) {
		m = GetMouseMsg();
		switch (m.uMsg) {
		case WM_LBUTTONDOWN:
			// 更新难度
			if (m.x >= 340 && m.x <= 430 && m.y >= 240 && m.y <= 290) {
				UpdateDifficulty();
				return DrawBoard();
			}
			// 更新音乐
			if (m.x >= 340 && m.x <= 430 && m.y >= 310 && m.y <= 360) {
				PlayMusic(!musicIsOn);
				return DrawBoard();
			}
			if (m.x >= 340 && m.x <= 430 && m.y >= 380 && m.y <= 430) {
				SetChallengeTime();
			}
			//  返回主菜单
			if (m.x >= 300 && m.x <= 410 && m.y >= 450 && m.y <= 500) {
				isBreak = true;
			}
			break;
		}
	}
	gameIndex->DrawBoard();
}

void GameSetting::DrawBoard() {
	cleardevice();
	char difficultyText[] = "游戏难度";
	char time[] = "挑战时间";
	char music[] = "背景音乐";
	char returnToMain[] = "返回主菜单";
	outtextxy(250, 265, difficultyText);
	outtextxy(250, 325, music);
	outtextxy(250, 395, time);
	// 难度框
	fillroundrect(340, 240, 430, 290, 10, 10);
	//音乐框
	fillroundrect(340, 310, 430, 360, 10, 10);
	//时间框
	fillroundrect(340, 380, 430, 430, 10, 10);
	// 返回主菜单
	fillroundrect(300, 450, 410, 500, 10, 10);
	outtextxy(310, 475, returnToMain);

	if (difficulty == 1) {
		char difficultyPriot[] = "简单";
		outtextxy(365, 265, difficultyPriot);
	}
	else if (difficulty == 2) {
		char difficultyPriot[] = "中等";
		outtextxy(365, 265, difficultyPriot);
	}
	else {
		char difficultyPriot[] = "困难";
		outtextxy(365, 265, difficultyPriot);
	}
	if (musicIsOn) {
		outtextxy(365, 335, "开");
	}
	else {
		outtextxy(365, 335, "关");
	}
	string temp = to_string(challengeTime) + "秒";
	//char*  t = temp;
	char a[10];
	strcpy_s(a, temp.c_str());
	outtextxy(365, 405, a);
	Click();
}


void GameSetting::UpdateDifficulty() {
	if (difficulty < 3) {
		difficulty++;
	}
	else {
		difficulty = 1;
	}
}

void GameSetting::SetChallengeTime() {
	//检测键盘动作
	char t[10];
	InputBox(t, 10, "输入挑战时间");
	int ti = atoi(t);
	challengeTime = ti;
	cleardevice();
	char difficultyText[] = "游戏难度";
	char time[] = "挑战时间";
	char music[] = "背景音乐";
	char returnToMain[] = "返回主菜单";
	outtextxy(250, 265, difficultyText);
	outtextxy(250, 325, music);
	outtextxy(250, 395, time);
	// 难度框
	fillroundrect(340, 240, 430, 290, 10, 10);
	//音乐框
	fillroundrect(340, 310, 430, 360, 10, 10);
	//时间框
	fillroundrect(340, 380, 430, 430, 10, 10);
	// 返回主菜单
	fillroundrect(300, 450, 410, 500, 10, 10);
	outtextxy(310, 475, returnToMain);

	if (difficulty == 1) {
		char difficultyPriot[] = "简单";
		outtextxy(365, 265, difficultyPriot);
	}
	else if (difficulty == 2) {
		char difficultyPriot[] = "中等";
		outtextxy(365, 265, difficultyPriot);
	}
	else {
		char difficultyPriot[] = "困难";
		outtextxy(365, 265, difficultyPriot);
	}
	if (musicIsOn) {
		outtextxy(365, 335, "开");
	}
	else {
		outtextxy(365, 335, "关");
	}
	string temp = to_string(challengeTime) + "秒";
	//char*  t = temp;
	char a[10];
	strcpy_s(a, temp.c_str());
	outtextxy(365, 405, a);

}


void GameSetting::PlayMusic(bool on) {

	if (on && !musicIsOn) {
		mciSendString("open music2.mp3", 0, 0, 0);
		mciSendString("play music2.mp3", 0, 0, 0);
		musicIsOn = true;
	}
	else {
		mciSendString("pause music2.mp3", 0, 0, 0);
		musicIsOn = false;
	}
}

int GameSetting::GetDifficulty() {
	return difficulty;
}
int GameSetting::GetChallengeTime() {
	return challengeTime;
}