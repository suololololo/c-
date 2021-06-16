#include"MakeQuestion.h"
#include"Block.h"
#include<graphics.h>
#include<vector>
#include"LoadQuestion.h"
#define DISTANCE 50


using namespace std;
MakeQuestion::MakeQuestion(int s) {
	size = s;
	for (int i = 0; i < size; i++) {
		vector<Block*> temp;
		for (int j = 0; j < size; j++) {
			Block* block = new Block();
			temp.push_back(block);
		}
		blockList.push_back(temp);
	}
}


void MakeQuestion::DrawBoard() {
	cleardevice();
	int x = 150;
	int y = 150;;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			fillroundrect(x + j * DISTANCE, y + i * DISTANCE, x + (j + 1) * DISTANCE, y + (i + 1) * DISTANCE, 10, 10);
			int temp = blockList[i][j]->GetNum();
			if (temp != -1) {
				char num[5];
				sprintf_s(num, "%d", temp);
				outtextxy(x + j * DISTANCE + 10, y + i * DISTANCE+10, num);
			}

		}
		
	}

	fillroundrect(500, 500, 550, 550, 10, 10);
	outtextxy(510, 510, "È·¶¨");
}

void MakeQuestion::Click() {
	int x = 150;
	int y = 150;
	bool isBegin = false;
	MOUSEMSG m;
	while (!isBegin) {
		m = GetMouseMsg();

		switch (m.uMsg) {
		case WM_LBUTTONDOWN:
			int rTemp = (m.x - x) / DISTANCE;
			int cTemp = (m.y - y) / DISTANCE;
			if (rTemp >= 0 && rTemp < size && cTemp >= 0 && cTemp < size) {
				blockList[cTemp][rTemp]->SetNum();
				DrawBoard();
			}
			if (m.x >= 500 && m.x <= 550 && m.y >= 500 && m.y <= 550) {
				isBegin = true;
			}
			break;
		}
	}
	Save();
}

void MakeQuestion::Save() {
	LoadQuestion* loadQuestion = new LoadQuestion();
	LoadQuestion::Question ques;
	if (size > 7) {
		ques.difficulty = 3;
	}
	else if (size < 5) {
		ques.difficulty = 1;
	}
	else {
		ques.difficulty = 2;
	}
	ques.size = size;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			ques.field[i][j] = blockList[i][j]->GetNum();
		}
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			blockList[i][j]->~Block();
		}
	}
	blockList.clear();
	loadQuestion->Save("data.txt", ques); 
}