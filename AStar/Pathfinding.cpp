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
static const int directions = 4;
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
	if (y != 0) return (x + (y * mapWidth));
	return (x + y);
}

/*
* Recursive parent lookup
*/
void fillBuffer(AStarNode* asn, int* buffer){
	if (asn != NULL){
		if (asn != nStart){
			buffer[asn->getgValue() - 1] = asn->getID();
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

	currentBufferSize = -1;
	nStart = new AStarNode(nStartX, nStartY);
	nTarget = new AStarNode(nTargetX, nTargetY);
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
	priority_queue<AStarNode*, vector<AStarNode*>, CompareAStarNode> minPQ;
	unordered_set<pair<int, int>, SimpleHash> uniq;
	vector<vector<int>>  map(nMapWidth, vector<int>(nMapHeight));

	//Initial node
	minPQ.push(nStart);
	uniq.insert(make_pair(nStartX, nStartY));

	//Assign map values
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
			currentBufferSize = current->getgValue();
			pOutBuffer = new int[currentBufferSize];
			fillBuffer(current, pOutBuffer);
			minPQ.swap(priority_queue<AStarNode*, vector<AStarNode*>, CompareAStarNode>());
		}else{ //Succescors
			if (current->getgValue() + 1 <= nOutBufferSize){ //Buffer
				int xChild;
				int yChild;
				for (int i = 0; i < directions; i++){
					xChild = dirX[i] + current->getxPos();
					yChild = dirY[i] + current->getyPos();
					if (!(xChild < 0) && xChild < nMapWidth && !(yChild < 0) && yChild < nMapHeight){ //Bounds
						if (uniq.insert(make_pair(xChild, yChild)).second){ //Unique
							if (map[xChild][yChild] != 0){ //Unblocked
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
				}//Succescors
				minPQ.pop();
			}else{//Buffer
				minPQ.swap(priority_queue<AStarNode*, vector<AStarNode*>, CompareAStarNode>());
			}
		}
	}
	uniq.clear();
	map.clear();
	delete child;
	delete nTarget;
	delete nStart;
	//cout << "Buffer contains: " << endl;
	//for (int i = 0; i < currentBufferSize; i++){
	//	cout << pOutBuffer[i] << endl;
	//}
	return currentBufferSize;
};




int main(int argc, char **argv)
{
	unsigned char pMap[] = { 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1 };
	int pOutBuffer[12];
	cout << "Pathsize for first input: " <<  FindPath(0, 0, 1, 2, pMap, 4, 3, pOutBuffer, 12) << endl;

	unsigned char pMab[] = { 0, 0, 1, 0, 1, 1, 1, 0, 1 };
	int pOutBuffer2[7];
	cout << "Pathsize for second input: " << FindPath(2, 0, 0, 2, pMab, 3, 3, pOutBuffer2, 7) << endl;

	cin.get();
};