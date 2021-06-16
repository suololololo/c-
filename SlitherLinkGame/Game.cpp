#include"GameBoard.h"
#include"GameIndex.h"
#include"LoadQuestion.h"
#include"GameSetting.h"
#include<graphics.h>
#include<iostream>
#include<time.h>
#define QUESTIONPATH "data.txt"
int main() {

	char username[30] = " ";
	cout << "请输入游戏者的名字";
	cin >> username;
	initgraph(640, 640);
	GameIndex* gameIndex = new GameIndex();
	GameSetting* gameSetting = new GameSetting(gameIndex);
	gameIndex->SetGameSetting(gameSetting);
	gameIndex->PlayMusic(true);
	gameIndex->DrawBoard();
	int i = 0;
	LoadQuestion* loadQuestion = new LoadQuestion();
	LoadQuestion::Question list = loadQuestion->Load(QUESTIONPATH, i);
	while (true) {
		while (list.difficulty != gameSetting->GetDifficulty()) {
			i++;
			list = loadQuestion->Load(QUESTIONPATH, i);
		}
		GameBoard* gameBoard = new GameBoard(list);
		gameBoard->DrawBoard();
		time_t start = time(NULL);
		gameBoard->Click();
		if (gameBoard->GetFinish()) {
			i++;
			if (gameIndex->GetMode()) {
				gameBoard->DrawFinish(gameIndex->GetMode(), gameIndex);
			}
			else {
				vector<LoadQuestion::Tuple> tuples = loadQuestion->HallofFame(username, gameBoard->Countdown(start));
				gameBoard->DrawFinish(gameIndex->GetMode(), gameIndex);
			}
			list = loadQuestion->Load(QUESTIONPATH, i);
		}
	}

	return 1;
}