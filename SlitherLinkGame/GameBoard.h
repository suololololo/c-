#pragma once
#ifndef GAMEBOARD_H
#define GAMEBOARD_H
#include"Draw.h"
#include"LoadQuestion.h"
class GameIndex;

class GameBoard:public Draw {
public :
	GameBoard(LoadQuestion::Question ques);
	~GameBoard();
	void DrawBoard();
	void Click();
	bool CheckFit();
	bool GetFinish();
	void DrawFinish(bool mode, GameIndex* gameIndex);
	double Countdown(time_t start);
private:
	bool musicIsOn = true;
	struct Point {
		int row;
		int column;
		Point* next;
	};
	enum Direction {
		Up = 0,
		Left = 1,
		Down = 2,
		Right = 3,
	};
	enum ConnectState {
		Connect = 1,
		Disconnect = 2,
		NoChange = 0,
	};
	bool finished = false;
	int size;
	int** connectMap;
	int** numMap;
	int** checkMap;
	int startx = 100;
	int starty = 100;
	LoadQuestion::Question ques;
	void CreateCheckMap();
	void DrawMusicButton();
	int** Clone(int** connectMap);
	int CaculateRow(int y);
	int CaculateColumn(int x);
	int CaculateDirection(int x, int y);
	bool find(Point* head, int i, int j);
	void insert(Point* head, Point* insert);
	void SetConnect(int row, int column, Direction direction, ConnectState state);
	static void DrawTime(time_t start);
	static bool stop;
	int static challegeTime;
	bool Reset(int x, int y);
	void GameInit();
	bool PressMusicButton(int x, int y);
	void PlayMusic(bool on);
	bool CloseMusic(int x);
	bool finish(int x, int y);
	void DrawButton(int x, int y, char text[]);
	void HallofFame(GameIndex* gameIndex);
	void HallofFame(vector<LoadQuestion::Tuple> tuples);
};


#endif // !GAMEBOARD_H
