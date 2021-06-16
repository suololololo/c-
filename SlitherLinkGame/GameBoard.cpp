#include"GameBoard.h"
#include"GameIndex.h"
//#include"GameIndex.h"
#include<graphics.h>
#include<stdio.h>
#include <iostream>
#include<time.h>
#include<thread>
#include<sstream>
#define SIZE 10
#define DISTANCE 50
using namespace std;
int GameBoard::challegeTime = 100;
bool GameBoard::stop = false;
GameBoard::GameBoard(LoadQuestion::Question que) {
	GameBoard::stop = false;
	size = que.size;
	ques = que;
	numMap = new int* [size];
	connectMap = new int* [size + 1];
	checkMap = new int* [size + 1];
	for (int i = 0; i < size; i++) {
		numMap[i] = new int[size];
	}
	for (int i = 0; i < size + 1; i++) {
		connectMap[i] = new int[size + 1];
		checkMap[i] = new int[size + 1];
	}
	GameInit();

}
void GameBoard::GameInit() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			numMap[i][j] = ques.field[i][j];
		}
	}
	for (int i = 0; i < size + 1; i++) {
		for (int j = 0; j < size + 1; j++) {
			connectMap[i][j] = 0;
			checkMap[i][j] = 0;
		}
	}
}
void GameBoard::DrawBoard() {
	cleardevice();
	for (int i = 0; i < size; i++) {
		//int y = starty + i * DISTANCE;
		for (int j = 0; j < size; j++) {
			int temp = numMap[i][j];
			if (temp == -1) {
				continue;
			}
			char num[5];
			sprintf_s(num, "%d", temp);
			int midx = (startx + j * DISTANCE + startx + (j + 1) * DISTANCE) / 2;
			int midy = (starty + i * DISTANCE + starty + (i + 1) * DISTANCE) / 2;
			outtextxy(midx, midy, num);
		}

	}
	setfillcolor(GREEN);
	for (int i = 0; i < size + 1; i++) {
		for (int j = 0; j < size + 1; j++) {
			setfillcolor(GREEN);
			fillroundrect(startx + j * DISTANCE, starty + i * DISTANCE,
				startx + j * DISTANCE + SIZE, starty + i * DISTANCE + SIZE, 5, 5);
			int state = connectMap[i][j];

			if ((state & 0x1) != 0) {
				line(startx + j * DISTANCE + SIZE / 2, starty + i * DISTANCE + SIZE / 2,
					startx + (j + 1) * DISTANCE + SIZE / 2, starty + i * DISTANCE + SIZE / 2);
			}
			if ((state & 0x100) != 0) {
				line(startx + j * DISTANCE + SIZE / 2, starty + i * DISTANCE + SIZE / 2,
					startx + j * DISTANCE + SIZE / 2, starty + (i + 1) * DISTANCE + SIZE / 2);
			}
			//画叉
			setfillcolor(RED);
			if ((state & 0xff) == 0x10) {
				line(startx + j * DISTANCE + DISTANCE * 0.3, starty + i * DISTANCE + DISTANCE * 0.3,
					startx + j * DISTANCE + DISTANCE * 0.6, starty + i * DISTANCE + DISTANCE * 0.6 + SIZE);
				line(startx + j * DISTANCE + DISTANCE * 0.3, starty + i * DISTANCE + DISTANCE * 0.3 + SIZE,
					startx + j * DISTANCE + DISTANCE * 0.6, starty + i * DISTANCE + DISTANCE * 0.6);
			}
			if ((state & 0xff00) == 0x1000) {
				line(startx + j * DISTANCE + DISTANCE * 0.3, starty + i * DISTANCE + DISTANCE * 0.3,
					startx + j * DISTANCE + DISTANCE * 0.6, starty + i * DISTANCE + DISTANCE * 0.6 + SIZE);
				line(startx + j * DISTANCE + DISTANCE * 0.3, starty + i * DISTANCE + DISTANCE * 0.3 + SIZE,
					startx + j * DISTANCE + DISTANCE * 0.6, starty + i * DISTANCE + DISTANCE * 0.6);
			}

		}
	}

	setfillcolor(BLACK);
	fillroundrect(380, 500, 450, 530, 10, 10);
	fillroundrect(380, 550, 450, 580, 10, 10);
	char reset[] = "reset";
	char finish[] = "finish";
	outtextxy(400, 510, reset);
	outtextxy(400, 560, finish);
	DrawMusicButton();
}

bool GameBoard::CheckFit() {
	CreateCheckMap();
	// 检查数字周围线的数量是否正确
	bool success = true;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (numMap[i][j] >= 0) {
				if (numMap[i][j] != checkMap[i][j])
					return false;
			}
		}
	}
	Point* head = new Point;
	int i;
	int j;
	Direction direction;
	int** cloneMap = Clone(connectMap);
	for (i = 0; i < size + 1; i++) {
		for (j = 0; j < size + 1; j++) {
			if (connectMap[i][j] != 0) {
				break;
			}
		}
		if (connectMap[i][j] != 0) {
			break;
		}
	}

	if ((connectMap[i][j] & 0x1) == 0) {
		success = false;
	}
	else {
		head->row = i;
		head->column = j;
		head->next = nullptr;
		direction = Right;
		cloneMap[i][j] &= 0xff00;
		j++;
		while ((i != head->row) || (j != head->column)) {
			Point* temp = new Point;
			temp->row = i;
			temp->column = j;
			temp->next = nullptr;
			if (find(head, i, j)) {
				success = false;
				break;
			}
			insert(head, temp);
			int tempState = cloneMap[i][j];
			if (direction == Right) {
				if ((tempState & 0x1) != 0) {
					cloneMap[i][j] &= 0xff00;
					direction = Right;
					j++;
					continue;
				}
				else if ((tempState & 0x100) != 0) {
					cloneMap[i][j] &= 0xff;
					direction = Down;
					i++;
					continue;
				}
				else if (i > 0 && (cloneMap[i - 1][j] & 0x100) != 0) {
					direction = Up;
					i--;
					cloneMap[i][j] &= 0xff;
					continue;
				}
				else {
					success = false;
					break;
				}
			}
			else if (direction == Down) {
				if ((tempState & 0x1) != 0) {
					cloneMap[i][j] &= 0xff00;
					direction = Right;
					j++;
					continue;
				}
				else if ((tempState & 0x100) != 0) {
					cloneMap[i][j] &= 0xff;
					direction = Down;
					i++;
					continue;
				}

				else if (j > 0 && (cloneMap[i][j - 1] & 0x1) != 0) {
					direction = Left;
					j--;
					cloneMap[i][j] &= 0xff00;
					continue;
				}
				else {
					success = false;
					break;
				}
			}
			else if (direction == Left) {
				if ((tempState & 0x100) != 0) {
					cloneMap[i][j] &= 0xff;
					direction = Down;
					i++;
					continue;
				}
				else if (j > 0 && (cloneMap[i][j - 1] & 0x1) != 0) {
					direction = Left;
					j--;
					cloneMap[i][j] &= 0xff00;
					continue;
				}
				else if (i > 0 && (cloneMap[i - 1][j] & 0x100) != 0) {
					direction = Up;
					i--;
					cloneMap[i][j] &= 0xff;
					continue;
				}
				else {
					success = false;
					break;
				}
			}

			else if (direction == Up) {
				if ((tempState & 0x1) != 0) {
					cloneMap[i][j] &= 0xff00;
					direction = Right;
					j++;
					continue;
				}
				else if (j > 0 && (cloneMap[i][j - 1] & 1) != 0) {
					direction = Left;
					j--;
					cloneMap[i][j] &= 0xff00;
					continue;
				}
				else if (i > 0 && (cloneMap[i - 1][j] & 0x100) != 0) {
					direction = Up;
					i--;
					cloneMap[i][j] &= 0xff;
					continue;
				}
				else {
					success = false;
					break;
				}
			}
		}
	}

	if (success) {
		if ((i != head->row) && (j != head->column)) {
			success = false;
		}
		else {
			for (int r = 0; r < size + 1; r++) {
				if (!success) {
					break;
				}
				for (int c = 0; c < size + 1; c++) {
					if (cloneMap[r][c] != 0) {
						success = false;
						break;
					}
				}
			}
		}
	}
	return success;

}

int** GameBoard::Clone(int** connectMap) {
	int** result = new int* [size + 1];
	for (int i = 0; i < size + 1; i++) {
		result[i] = new int[size + 1];
		for (int j = 0; j < size + 1; j++) {
			result[i][j] = connectMap[i][j];
		}
	}

	return result;
}


void GameBoard::CreateCheckMap() {
	for (int i = 0; i < size + 1; i++) {
		for (int j = 0; j < size + 1; j++) {
			checkMap[i][j] = 0;
		}
	}
	for (int i = 0; i < size + 1; i++) {
		for (int j = 0; j < size + 1; j++) {
			int state = connectMap[i][j];
			if ((state & 0x1) != 0) {
				if (i < size) {
					checkMap[i][j] += 1;
				}
				if (i > 0) {
					checkMap[i - 1][j] += 1;
				}
			}
			if ((state & 0x100) != 0) {
				if (j < size) {
					checkMap[i][j] += 1;
				}
				if (j > 0) {
					checkMap[i][j - 1] += 1;
				}
			}
		}
	}
}

void GameBoard::DrawMusicButton() {
	char closeMusic[] = { "关闭音乐" };
	char openMusic[] = { "打开音乐" };
	fillroundrect(460, 10, 530, 40, 10, 10);
	fillroundrect(550, 10, 620, 40, 10, 10);
	outtextxy(470, 20, openMusic);
	outtextxy(560, 20, closeMusic);
}


void GameBoard::Click() {
	MOUSEMSG m;
	time_t start = time(NULL);
	thread p(&GameBoard::DrawTime, start);
	p.detach();
	while (!finished) {

		m = GetMouseMsg();
		int row = CaculateRow(m.y);
		int column = CaculateColumn(m.x);
		int direction = CaculateDirection(m.x, m.y);
		if (direction == Down) {
			row++;
			direction = Up;
		}
		else if (direction == Right) {
			column++;
			direction = Left;
		}
		int currState = connectMap[row][column];
		switch (m.uMsg)
		{

		case WM_LBUTTONDOWN:
			if (finish(m.x, m.y)) {
				finished = CheckFit();
				if (finished) {
					GameBoard::stop = true;
				}
				else {
					char fail[] = "有错误的地方";
					outtextxy(500, 500, fail);
				}
				break;
			}
			else if (Reset(m.x, m.y)) {
				GameInit();
				DrawBoard();
				break;
			}
			if (PressMusicButton(m.x, m.y)) {
				PlayMusic(!CloseMusic(m.x));
			}


			if (direction == Up) {
				if ((currState & 0x01) == 0) {
					SetConnect(row, column, Up, Connect);
				}
				else {
					SetConnect(row, column, Up, NoChange);
				}
			}
			else {
				if ((currState & 0x0100) == 0) {
					SetConnect(row, column, Left, Connect);
				}
				else {
					SetConnect(row, column, Left, NoChange);
				}
			}
			DrawBoard();

			break;
		case WM_RBUTTONDOWN:
			if (direction == Up || direction == Down) {
				if ((currState & 0x10) == 0) {
					SetConnect(row, column, (Direction)direction, Disconnect);
				}
				else {
					SetConnect(row, column, (Direction)direction, NoChange);
				}
			}
			else {
				if ((currState & 0x1000) == 0) {
					SetConnect(row, column, (Direction)direction, Disconnect);
				}
				else {
					SetConnect(row, column, (Direction)direction, NoChange);
				}
			}
			DrawBoard();
			break;
		}
	}

}


int GameBoard::CaculateRow(int y) {
	if (y >= starty + size * DISTANCE) {

		y = starty + size * DISTANCE - 1;
	}
	if (y <= starty) {

		y = starty + 1;
	}
	int row = (y - starty) / DISTANCE;
	return row;
}
int GameBoard::CaculateColumn(int x) {
	if (x >= startx + size * DISTANCE) {
		x = startx + size * DISTANCE - 1;
	}
	if (x <= startx) {
		x = startx + 1;
	}
	int column = (x - startx) / DISTANCE;
	return column;
}
int GameBoard::CaculateDirection(int x, int y) {
	if (y >= starty + size * DISTANCE) {
		y = starty + size * DISTANCE - 1;
	}
	if (y <= starty) {
		y = starty + 1;
	}
	if (x >= startx + size * DISTANCE) {
		x = startx + size * DISTANCE - 1;
	}
	if (x <= startx) {
		x = startx + 1;
	}
	int tempx = (x - startx) % DISTANCE;
	int tempy = (y - starty) % DISTANCE;
	if (tempy <= 16) {
		return Up;
	}
	if (tempy >= 32) {
		return Down;
	}
	if (tempx <= 25) {
		return Left;
	}
	else {
		return Right;
	}
}

bool GameBoard::find(Point* head, int i, int j) {
	bool success = false;
	Point* curr = head;
	while (curr->next != nullptr) {
		if (curr->next->row == i && curr->next->column == j) {
			success = true;
			break;
		}
		curr = curr->next;
	}
	return success;
}

void GameBoard::insert(Point* head, Point* insert) {
	Point* curr = head;
	while (curr->next != nullptr) {
		curr = curr->next;
	}
	curr->next = insert;
}

void GameBoard::SetConnect(int row, int column, Direction direction, ConnectState state) {
	if (row > size || column > size) {
		return;
	}
	int currState = connectMap[row][column];
	int temp;
	if (state == 0) {
		temp = 0x00;
	}
	else if (state == 1) {
		temp = 0x01;
	}
	else {
		temp = 0x10;
	}
	//对列进行操作
	if (direction == Left) {
		connectMap[row][column] = (currState & 0xff) | (temp << 8);
	}
	else {
		connectMap[row][column] = (currState & 0xff00) | (temp);
	}

}
void GameBoard::DrawTime(time_t start)
{
	while (!stop) {
		time_t end = time(NULL);
		double time = difftime(end, start);
		int drawTime = (int)time;

		char text[5];
		strcpy_s(text, to_string(drawTime).c_str());

		outtextxy(280, 20, text);
		if (drawTime > challegeTime) {
			return;
		}

	}

}

bool GameBoard::Reset(int x, int y) {

	if (x >= 380 && x <= 450 && y >= 500 && y <= 530) {
		return true;
	}
	else {
		return false;
	}

}

bool GameBoard::PressMusicButton(int x, int y) {
	if (x >= 460 && x <= 620 && y >= 10 && y <= 40) {
		return true;
	}
	return false;
}


void GameBoard::PlayMusic(bool on) {

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

bool GameBoard::CloseMusic(int x) {
	if (x >= 550) {
		return true;
	}
	else {
		return false;
	}
}

bool GameBoard::finish(int x, int y) {

	if (x >= 380 && x <= 450 && y >= 550 && y <= 580) {
		return true;
	}
	else {
		return false;
	}
}

bool GameBoard::GetFinish() {
	return finished;
}
void GameBoard::DrawFinish(bool mode, GameIndex* gameIndex) {
	cleardevice();
	//闯关模式
	if (mode) {
		//显示答题正确 然后问是否下一题
		char text[] = "恭喜你！答对了";
		char next[] = "下一题";
		char returnButton[] = "返回主菜单";
		outtextxy(200, 100, text);
		DrawButton(280, 310, next);
		DrawButton(280, 360, returnButton);
		while (true) {
			MOUSEMSG m;
			m = GetMouseMsg();
			switch (m.uMsg)
			{

			case WM_LBUTTONDOWN:
				if (m.x >= 280 && m.x <= 370 && m.y >= 310 && m.y <= 360) {
					return;
				}
				if (m.x >= 280 && m.x <= 370 && m.y > 360 && m.y <= 410) {
					return gameIndex->DrawBoard();
				}
			default:
				break;
			}
		}

	}
	else {
		HallofFame(gameIndex);
	}


}

void GameBoard::DrawButton(int x, int y, char text[]) {
	//fillroundrect(280, 310, 370, 360, 10, 10);
	fillroundrect(x, y, x + 90, y + 50, 10, 10);
	outtextxy(x + 10, y + 10, text);
}

void GameBoard::HallofFame(GameIndex* gameIndex) {
	cleardevice();
	vector<LoadQuestion::Tuple> tuples = LoadQuestion::HallofFame();
	for (int i = 0; i < tuples.size(); i++) {
		string id = to_string(tuples[i].id);
		string time = to_string(tuples[i].time);
		string result;
		result.append(id);
		result.append(tuples[i].username);
		result.append(time);
		string temp = "秒";
		result.append(temp);
		char username[30] = { '\0' };
		strcpy_s(username, result.c_str());
		outtextxy(startx, starty + i * DISTANCE, username);
	}

	char next[] = "下一题";
	char returnButton[] = "返回主菜单";
	DrawButton(500, 530, next);
	DrawButton(500, 580, returnButton);
	MOUSEMSG m;
	while (true) {
		m = GetMouseMsg();
		switch (m.uMsg) {
		case WM_LBUTTONDOWN:
			if (m.x >= 500 && m.x <= 590 && m.y >= 530 && m.y <= 580) {
				return;
			}
			if (m.x >= 500 && m.x <= 590 && m.y > 580 && m.y <= 630) {
				return gameIndex->DrawBoard();
			}
		}
	}

}

double GameBoard::Countdown(time_t start) {
	time_t end = time(NULL);
	return difftime(end, start);
}

void GameBoard::HallofFame(vector<LoadQuestion::Tuple> tuples) {
	cleardevice();
	for (int i = 0; i < tuples.size(); i++) {

		string temp = tuples[i].username;
		string time;
		stringstream ss;
		ss << tuples[i].time;
		ss >> time;
		temp.append(time);
		char username[20];
		for (int j = 0; j < temp.length(); j++) {
			username[j] = temp[j];
		}
		outtextxy(startx, starty + i * DISTANCE, username);
	}

}