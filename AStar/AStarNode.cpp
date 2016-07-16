#include "AStarNode.h"
#include <string>
#include <iostream>
using namespace std;

AStarNode::AStarNode(const int x, const int y){
	this->x = x;
	this->y = y;
}

int AStarNode::getxPos() const { return x; }
void AStarNode::setxPos(int x){ this->x = x; }
int AStarNode::getyPos() const { return y; }
void AStarNode::setyPos(int y){ this->y = y; }
int AStarNode::getfValue() const { return f; }
int AStarNode::getCost() const { return cost; }
int AStarNode::getgValue() const { return g; }
int AStarNode::gethValue() const { return h; }
void AStarNode::setfValue(int f) { this->f = f; }
void AStarNode::setgValue(int g) { this->g = g; }
void AStarNode::sethValue(int h) { this->h = h; }
AStarNode* AStarNode::getParent() { return parent; }
void AStarNode::setParent(AStarNode* parent) { this->parent = parent; }


AStarNode::~AStarNode()
{
	cout << "Destroyed : (" << x << "," << y << ")" << endl;
}



