#pragma once
#ifndef MAKEQUESTION_H
#define MAKEQUESTION_H
#include"Draw.h"
#include<vector>
class Block;
using namespace std;
class MakeQuestion :public Draw {
public:
	MakeQuestion(int s);
	void DrawBoard();
	void Click();
private:
	int size;
	vector<vector<Block*>> blockList;
	void Save();

};

#endif // !MAKEQUESTION_H
