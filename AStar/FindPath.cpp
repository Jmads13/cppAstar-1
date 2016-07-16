#include <queue>
#include <unordered_set>
#include <iostream>
#include "AStarNode.h"

using namespace std;


AStarNode* nStart;
AStarNode* nTarget;
AStarNode* current;
AStarNode* child;
const int directions = 4;
static int dirX[directions] = { 1, 0, -1, 0 };
static int dirY[directions] = { 0, 1, 0, -1 };


/*
*
* Reverse PQ comparison
*
*/
struct CompareAStarNode : public binary_function<AStarNode*, AStarNode*, bool>{
	bool operator()(const AStarNode* lhs, const AStarNode* rhs) const
	{
		return lhs->getfValue() > rhs->getfValue();
	}
};

/*
*
* Hash to ensure unique nodes
*
*/
struct SimpleHash {
	size_t operator()(const pair<int, int>& p) const {
		return p.first ^ p.second;
	}
};

/*
*
* Manhattan heuristic
*
*/
int ManhattenDistance(AStarNode* start, AStarNode* target){
	return (abs(start->getxPos() - target->getxPos()) + abs(start->getyPos() - target->getyPos()));
}

/*
*
* Recursive exploration of parents in node
*
*/
void travel(AStarNode* asn){
	cout << asn->getxPos() << ", " << asn->getyPos() << endl;
	if (asn == nTarget){
		cout << "travelled" << endl;
	}else{
		travel(asn->getParent());
	}
}

/*
*
* herp derp
*
*/
int FindShortestPath(const int nStartX, const int nStartY, const int nTargetX, const int nTargetY, const unsigned char* pMap, const int nMapWidth, const int nMapHeight, int* pOutBuffer, const int nOutBufferSize){
	nStart = new AStarNode(nStartX, nStartY);
	nTarget = new AStarNode(nTargetX, nTargetY);
	nStart->setgValue(0);
	nStart->setfValue(0);
	nStart->sethValue(ManhattenDistance(nStart, nTarget));
	nStart->setParent(nTarget);

	/*
	* Collections
	*
	* minPQ			: Minimum Priority Queue for sorting fvalues
	* uniqueOpen	: unique Set maintained to ensure uniqueness in PQ
	* map			: 2D Map with blocked values from pMap
	*
	*/
	priority_queue<AStarNode*, vector<AStarNode*>, CompareAStarNode> minPQ;
	unordered_set<pair<int, int>, SimpleHash> uniqueOpen;
	vector<vector<int>>  map(nMapWidth, vector<int>(nMapHeight));
	
	//initial node
	minPQ.push(nStart);
	uniqueOpen.insert(make_pair(nStartX, nStartY));
	
	//assign tile values
	int counter = 0;
	for (int y = 0; y<nMapHeight; y++){
		for (int x = 0; x<nMapWidth; x++){
			map[x][y] = (int)pMap[counter++];
		}
	}

	while (!minPQ.empty())
	{
		current = minPQ.top();	//Current node for evaluation
		if (current->getxPos() == nTargetX && current->getyPos() == nTargetY){
			travel(current);	//Recursive parent node traveling
			return 0;
		}

		int xChild;
		int yChild;
		for (int i = 0; i < directions; i++)
		{
			//generate succescor
			xChild = dirX[i] + current->getxPos();
			yChild = dirY[i] + current->getyPos();

			if (!(xChild < 0) && xChild < nMapWidth && !(yChild < 0) && yChild < nMapHeight){	//Within bounds
				if (uniqueOpen.insert(make_pair(xChild, yChild)).second)						//Unique
					if (map[xChild][yChild] != 0){												//blocked
						{
							child = new AStarNode(xChild, yChild);
							child->setgValue(current->getgValue() + current->getCost());
							child->sethValue(ManhattenDistance(child, nTarget));
							child->setfValue(child->getgValue() + child->gethValue());
							child->setParent(current);
							minPQ.push(child);
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
	return 0;
};




int main(int argc, char **argv)
{

	unsigned char pMap[] = {	1, 1, 1, 1, 
								0, 1, 0, 1, 
								0, 1, 1, 1		};
	int pOutBuffer[12];
	FindShortestPath(0, 0, 1, 2, pMap, 4, 3, pOutBuffer, 12);


	unsigned char pMap2[] = {	0, 0, 1,
								0, 1, 1,
								1, 0, 1		};

	int pOutBuffer2[7];
	FindShortestPath(2, 0, 0 ,2, pMap2, 3, 3, pOutBuffer2, 7);


	cin.get();

};