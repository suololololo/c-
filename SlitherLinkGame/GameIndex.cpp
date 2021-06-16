#include"GameIndex.h"
#include"GameSetting.h"
#include<graphics.h>
#include"MakeQuestion.h"
#pragma comment(lib, "winmm.lib")
GameIndex::GameIndex() {
	mode = true;
	musicIsOn = false;
}
void GameIndex::DrawBoard() {
	cleardevice();
	setfillcolor(BLACK);
	fillroundrect(280, 240, 370, 290, 10, 10);
	fillroundrect(280, 310, 370, 360, 10, 10);
	fillroundrect(280, 380, 370, 430, 10, 10);
	fillroundrect(280, 450, 370, 500, 10, 10);
	char reset[] = "闯关模式";
	char finish[] = "记时模式";
	char setting[] = "游戏设置";
	char que[] = "玩家出题";
	outtextxy(305, 265, reset);
	outtextxy(305, 325, finish);
	outtextxy(300, 395, setting);
	outtextxy(300, 465, que);
	DrawMusicButton();
	Click();
}

void GameIndex::DrawMusicButton() {
	char closeMusic[] = { "关闭音乐" };
	char openMusic[] = { "打开音乐" };
	fillroundrect(460, 10, 530, 40, 10, 10);
	fillroundrect(550, 10, 620, 40, 10, 10);
	outtextxy(470, 20, openMusic);
	outtextxy(560, 20, closeMusic);
}

void GameIndex::Click() {
	MOUSEMSG m;
	bool isBegin = false;
	while (!isBegin) {
		m = GetMouseMsg();
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:
			if (PressMusicButton(m.x, m.y)) {
				PlayMusic(!CloseMusic(m.x));
			}
			if (m.x >= 280 && m.x <= 370 && m.y >= 380 && m.y <= 430) {
				return gameSetting->DrawBoard();
			}
			else if (m.x >= 280 && m.x <= 370 && m.y >= 240 && m.y <= 290) {
				mode = true;
				return;

			}
			else if (m.x >= 280 && m.x <= 370 && m.y >= 310 && m.y <= 360) {
				mode = false;
				return;
			}
			else if (m.x >= 280 && m.x <= 370 && m.y >= 450 && m.y <= 500) {
				 makeQuestion = new MakeQuestion(DrawQuestion());
				 isBegin = true;
			}
			break;

		}
	}
	makeQuestion->DrawBoard();
	makeQuestion->Click();
}

bool GameIndex::PressMusicButton(int x, int y) {
	if (x >= 460 && x <= 620 && y >= 10 && y <= 40) {
		return true;
	}
	return false;
}


void GameIndex::PlayMusic(bool on) {

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

bool GameIndex::CloseMusic(int x) {
	if (x >= 550) {
		return true;
	}
	else {
		return false;
	}
}

int GameIndex::GetMode() {
	return mode;
}

void GameIndex::SetGameSetting(GameSetting* gSet) {
	gameSetting = gSet;
}

int GameIndex::DrawQuestion() {
	cleardevice();
	int size = 4;

	char time[] = "挑战时间";
	outtextxy(250, 265, "题目大小");
	// size框
	fillroundrect(340, 240, 430, 290, 10, 10);
	fillroundrect(340, 310, 430, 360, 10, 10);
	outtextxy(250, 265, "题目大小");
	MOUSEMSG m;
	bool isBegin = false;
	while (!isBegin) {
		m = GetMouseMsg();
		switch (m.uMsg) {
		case WM_LBUTTONDOWN:
			if (m.x >= 340 && m.x <= 430 && m.y >= 240 && m.y <= 290) {
				char t[10];
				InputBox(t, 10, "输入题目大小");
				int ti = atoi(t);
				if (ti >= 1 && ti <= 9) {
					size = ti;
				}
			}
			if (m.x >= 340 && m.x <= 430 && m.y >= 310 && m.y <= 360) {
				isBegin = true;
			}

			break;
		}
	}
	return size;
}