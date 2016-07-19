#include <queue>
#include <unordered_set>
#include <iostream>
#include "AStarNode.h"

using namespace std;

AStarNode* nStart;
AStarNode* nTarget;
AStarNode* current;
AStarNode* child;

int currentBufferSize;
const int directions = 4;
static int dirX[directions] = { 1, 0, -1, 0 };
static int dirY[directions] = { 0, 1, 0, -1 };

/*
* Reverse PQ comparison
*/
struct CompareAStarNode : public binary_function<AStarNode*, AStarNode*, bool>{
	bool operator()(const AStarNode* lhs, const AStarNode* rhs) const
	{
		return lhs->getfValue() > rhs->getfValue();
	}
};

/*
* Hash to ensure unique nodes
*/
struct SimpleHash {
	size_t operator()(const pair<int, int>& p) const {
		return p.first ^ p.second;
	}
};

/*
* Manhattan heuristic
*/
int manhattenDistance(AStarNode* start, AStarNode* target){
	return (abs(start->getxPos() - target->getxPos()) + abs(start->getyPos() - target->getyPos()));
}

/*
* '1 dimensional' ID
*/
int calculateID(int x, int y, int mapWidth){
	if (y != 0) {
		return (x + (y * mapWidth));
	}
	else {
		return (x + y);
	}
}

/*
* Recursive parent lookup
*/
void fillBuffer(AStarNode* asn, int* buffer){
	if (asn != NULL){
		if (asn != nStart){
			buffer[asn->getgValue() - 1] = asn->getID(); //Possible due to cost always = 1
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

	currentBufferSize = 0;
	nStart = new AStarNode(nStartX, nStartY);
	nTarget = new AStarNode(nTargetX, nTargetY);
	nStart->setgValue(0);
	nStart->setfValue(0);
	nStart->sethValue(manhattenDistance(nStart, nTarget));
	nStart->setParent(NULL);

	/*
	* Collections
	*
	* minPQ			: Minimum Priority Queue for sorting fvalues
	* uniqueOpen	: unique Set maintained to ensure uniqueness in PQ
	* map			: 2D Map with 'blocked' values from pMap
	*
	*/
	priority_queue<AStarNode*, vector<AStarNode*>, CompareAStarNode> minPQ;
	unordered_set<pair<int, int>, SimpleHash> uniqueOpen;
	vector<vector<int>>  map(nMapWidth, vector<int>(nMapHeight));

	//initial node
	minPQ.push(nStart);
	uniqueOpen.insert(make_pair(nStartX, nStartY));

	//assign map values
	int counter = 0;
	for (int y = 0; y<nMapHeight; y++){
		for (int x = 0; x<nMapWidth; x++){
			map[x][y] = (int)pMap[counter++];
		}
	}

	while (!minPQ.empty())
	{
		current = minPQ.top();//Current node for evaluation

		if (current->getxPos() == nTargetX && current->getyPos() == nTargetY){
			currentBufferSize = current->getgValue(); //Possible due to cost always = 1
			pOutBuffer = new int[currentBufferSize];
			fillBuffer(current, pOutBuffer);
			return currentBufferSize;
		}

		//generate succescor
		int xChild;
		int yChild;
		for (int i = 0; i < directions; i++)
		{
			if (current->getgValue() + 1 <= nOutBufferSize){										//Buffer limit
				xChild = dirX[i] + current->getxPos();
				yChild = dirY[i] + current->getyPos();
				if (!(xChild < 0) && xChild < nMapWidth && !(yChild < 0) && yChild < nMapHeight){	//Within bounds
					if (uniqueOpen.insert(make_pair(xChild, yChild)).second){						//Unique
						if (map[xChild][yChild] != 0){ 												//blocked
							child = new AStarNode(xChild, yChild);
							child->setID(calculateID(xChild, yChild, nMapWidth));
							child->setgValue(current->getgValue() + current->getCost());
							child->sethValue(manhattenDistance(child, nTarget));
							child->setfValue(child->getgValue() + child->gethValue());
							child->setParent(current);
							minPQ.push(child);
						}
					}
				}
			}
		}
		minPQ.pop();
	}
	minPQ = priority_queue<AStarNode*, vector<AStarNode*>, CompareAStarNode>();
	uniqueOpen.clear();
	delete child;
	delete nTarget;
	delete nStart;
	return -1;
};




int main(int argc, char **argv)
{
	unsigned char pMap[] = { 1, 1, 1, 1,
							0, 1, 0, 1,
							0, 1, 1, 1 };
	int pOutBuffer[12];
	cout << FindPath(0, 0, 1, 2, pMap, 4, 3, pOutBuffer, 12) << endl;

	unsigned char pMab[] = { 0, 0, 1,
							0, 1, 1,
							1, 0, 1 };
	int pOutBuffer2[7];
	cout << FindPath(2, 0, 0, 2, pMab, 3, 3, pOutBuffer2, 7) << endl;

	unsigned char pMap3[]={ 1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
							1, 0, 1, 0, 0, 0, 0, 0, 1, 1,
							1, 0, 1, 1, 1, 1, 1, 0, 1, 1,
							1, 0, 0, 0, 0, 0, 1, 0, 1, 1,
							1, 1, 1, 1, 1, 1, 1, 0, 1, 1 };
	int pOutBuffer3[5];

	cout << FindPath(0, 0, 9, 3, pMap3, 10, 5, pOutBuffer3, 100) << endl;


	cin.get();
};