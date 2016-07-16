#ifndef AStarNode_H
#define AStarNode_H

class AStarNode{
private:
	AStarNode* parent; //Discovere of this node
	int x; //x position in map
	int y; //y position in map
	int f; //Combined cost
	int g; //Cost of getting to goal from start
	int h; //Cost of getting to goal from current ndoe
	const int cost = 1; //Cost of moving a step

public:
	AStarNode(const int x, const int y);
	int getxPos() const;
	int getyPos() const;
	void setxPos(int x);
	void setyPos(int y);
	int getfValue() const;
	int gethValue() const;
	int getgValue() const;
	int getCost() const;
	void setfValue(int f);
	void setgValue(int g);
	void sethValue(int h);

	AStarNode* getParent();
	void setParent(AStarNode* parent);
	virtual ~AStarNode();
};

#endif

