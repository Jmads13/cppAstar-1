#include <queue>
#include <unordered_set>
#include <iostream>

class ANode{
private:
	ANode* parent; //Discovere of this node
	int id; //Identifier in char map
	int x; //x position in map
	int y; //y position in map
	int f; //Combined cost
	int g; //Cost of getting to goal from start
	int h; //Cost of getting to goal from current ndoe
	const int cost = 1; //Cost of moving a step
public:
	ANode(const int x, const int y){
		this->x = x;
		this->y = y;
	}

	int getID() const { return id; }
	void setID(int id){ this->id = id; }
	int getxPos() const { return x; }
	void setxPos(int x){ this->x = x; }
	int getyPos() const { return y; }
	void setyPos(int y){ this->y = y; }
	int getfValue() const { return f; }
	int getCost() const { return cost; }
	int getgValue() const { return g; }
	int gethValue() const { return h; }
	void setfValue(int f) { this->f = f; }
	void setgValue(int g) { this->g = g; }
	void sethValue(int h) { this->h = h; }
	ANode* getParent() { return parent; }
	void setParent(ANode* parent) { this->parent = parent; }
};

ANode* nStart;
ANode* nTarget;
ANode* current;
ANode* child;

static const int directions = 4;
static int dirX[directions] = { 1, 0, -1, 0 };
static int dirY[directions] = { 0, 1, 0, -1 };

/*
* Reverse PQ comparison
*/
struct CompareAStarNode : public std::binary_function<ANode*, ANode*, bool>{
	bool operator()(const ANode* lhs, const ANode* rhs) const
	{
		return lhs->getfValue() > rhs->getfValue();
	}
};

/*
* Hash to ensure unique nodes
*/
struct SimpleHash {
	size_t operator()(const std::pair<int, int>& p) const {
		return p.first ^ p.second;
	}
};

/*
* Manhattan heuristic
*/
int manhattenDistance(ANode* start, ANode* target){
	return (abs(start->getxPos() - target->getxPos()) + abs(start->getyPos() - target->getyPos()));
}

/*
* '1 dimensional' ID
*/
int calculateID(int x, int y, int mapWidth){
	if (y != 0) return (x + (y * mapWidth));
	return (x + y);
}

/*
* Recursive parent lookup
*/
void fillBuffer(ANode* asn, int* buffer){
	if (asn != NULL){
		if (asn != nStart){
			*(buffer + asn->getgValue() - 1) = asn->getID();
		}
		fillBuffer(asn->getParent(), buffer);
	}
}

/*
* Simplified A* with manhattan heuristic
*/
int FindPath(const int nStartX, const int nStartY,
	const int nTargetX, const int nTargetY,
	const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
	int* pOutBuffer, const int nOutBufferSize){

	int pathLength = -1; //No path found
	nStart = new ANode(nStartX, nStartY);
	nTarget = new ANode(nTargetX, nTargetY);
	nStart->setgValue(0);
	nStart->setfValue(0);
	nStart->sethValue(manhattenDistance(nStart, nTarget));
	nStart->setParent(NULL);

	/*
	* Collections
	*
	* minPQ	: Minimum Priority Queue for sorting fvalues
	* uniq	: unique Set maintained to ensure uniqueness in PQ
	* map	: 2D Map with 'blocked' values from pMap
	*
	*/
	std::priority_queue<ANode*, std::vector<ANode*>, CompareAStarNode> minPQ;
	std::unordered_set<std::pair<int, int>, SimpleHash> uniq;
	std::vector<std::vector<int>>  map(nMapWidth, std::vector<int>(nMapHeight));

	//Initial node
	minPQ.push(nStart);
	uniq.insert(std::make_pair(nStartX, nStartY));

	//Assign map values - could be done at discovery of node
	int counter = 0;
	for (int y = 0; y<nMapHeight; y++){
		for (int x = 0; x<nMapWidth; x++){
			map[x][y] = (int)pMap[counter++];
		}
	}

	while (!minPQ.empty())
	{
		current = minPQ.top(); //Current node for evaluation

		if (current->getxPos() == nTargetX && current->getyPos() == nTargetY){
			pathLength = current->getgValue();
			fillBuffer(current, pOutBuffer);
			minPQ = std::priority_queue<ANode*, std::vector<ANode*>, CompareAStarNode>();
		}else{
			if (current->getgValue() + 1 <= nOutBufferSize){
				int xChild;
				int yChild;
				for (int i = 0; i < directions; i++){
					xChild = dirX[i] + current->getxPos();
					yChild = dirY[i] + current->getyPos();
					if (!(xChild < 0) && xChild < nMapWidth && !(yChild < 0) && yChild < nMapHeight){
						if (uniq.insert(std::make_pair(xChild, yChild)).second){
							if (map[xChild][yChild] != 0){ 
								child = new ANode(xChild, yChild);
								child->setID(calculateID(xChild, yChild, nMapWidth));
								child->setgValue(current->getgValue() + current->getCost());
								child->sethValue(manhattenDistance(child, nTarget));
								child->setfValue(child->getgValue() + child->gethValue());
								child->setParent(current);
								minPQ.push(child);
							} //Unique
						} //Unblocked
					} //Bounds
				} //Succescors
				minPQ.pop();
			}else{ //Buffer
				minPQ = std::priority_queue<ANode*, std::vector<ANode*>, CompareAStarNode>();
			}
		}
	}

	//for (int i = 0; i < pathLength; i++){
	//	cout << pOutBuffer[i] << endl;
	//}

	uniq.clear();
	map.clear();
	delete child;
	delete nTarget;
	delete nStart;
	return pathLength;
};

//int main(int argc, char **argv)
//{
//	unsigned char pMap[] = { 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1 };
//	int pOutBuffer[12];
//	std::cout << "Pathsize for first input: " << FindPath(0, 0, 1, 2, pMap, 4, 3, pOutBuffer, 12) << std::endl;
//
//
//
//	int pOutBuffer2[7];
//	unsigned char pMab[] = { 0, 0, 1, 0, 1, 1, 1, 0, 1 };
//	std::cout << "Pathsize for second input: " << FindPath(2, 0, 0, 2, pMab, 3, 3, pOutBuffer2, 7) << std::endl;
//
//
//	unsigned char pMak[] = { 1, 0, 1, 1, 1, 0, 1, 0, 1, 1,
//		1, 0, 1, 0, 1, 0, 1, 1, 1, 1,
//		1, 0, 1, 0, 1, 0, 1, 0, 1, 1,
//		1, 1, 1, 0, 1, 1, 1, 0, 1, 1 };
//	int pOutBuffer3[100];
//	std::cout << "Pathsize for third input: " << FindPath(0, 0, 9, 3, pMak, 10, 4, pOutBuffer3, 100) << std::endl;
//
//	std::cin.get();
//};