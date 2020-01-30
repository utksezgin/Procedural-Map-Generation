// TODO: REFACTORING


#include "Graph.h"
#include <stack>

Graph::Graph(uint32 roomCount) {
	for (uint32 i = 1; i <= roomCount; ++i) {
		adj.Add(i, TArray<uint32>());
	}
}

void Graph::addEdge(uint32 u, uint32 v) {
	if(adj[u].Num() == 0)
		++numberOfNodes;
	if (adj[v].Num() == 0)
		++numberOfNodes;
	adj[u].AddUnique(v);
	adj[v].AddUnique(u);
}

void Graph::printGraph() {
	FString layoutString = "";
	for(int i = 1; i <= adj.Num(); ++i) {
		if (adj[i].Num() != 0)
			layoutString.Append(FString::FromInt(i) + "-> ");
		for (auto roomNo : adj[i]) {
			layoutString.Append(FString::FromInt(roomNo) + ", ");
		}
		layoutString.Append("\n");
	}
	UE_LOG(LogTemp, Warning, TEXT("\nGraph:\n%s\nNode Count: %d"), *layoutString, numberOfNodes);
}


float Graph::calculateHeuristic(uint16 sourceX, uint16 sourceY, uint16 destX, uint16 destY) {
	return (destX - sourceX)* (destX - sourceX) + (destY - sourceY) * (destY - sourceY);
}

Graph Graph::tracePath(uint32 startRoom, uint32 endRoom, TMap<uint32, AIslandNode*> *pivotRooms) {

	TArray<uint32> path;
	AIslandNode *currRoom = (*pivotRooms)[endRoom];
	while (currRoom->getRoomNo() != startRoom) {
		path.Push(currRoom->getRoomNo());
		currRoom = (*pivotRooms)[currRoom->getParentRoomNo()];
	}
	path.Push(currRoom->getRoomNo());
	FString pathString { "" };
	Graph singlePathLayout(adj.Num());
	for (int i = path.Num()-1; i >= 0 ; --i) {
		pathString.Append(FString::FromInt(path[i]) + "->");
		if (i - 1 >= 0) {
			singlePathLayout.addEdge(path[i], path[i-1]);
		}
	}
	
	return singlePathLayout;
}

//Priority q?
Graph Graph::AStar(uint32 startRoom, uint32 endRoom, TMap<uint32, AIslandNode*> *pivotRooms, bool isModified) {
	TSet<AIslandNode*> openList;
	TSet<AIslandNode*> closedList;
	TArray<int> test;
	openList.Add((*pivotRooms)[startRoom]);
	while (openList.Num() != 0) {

		openList.Sort([](const AIslandNode& A, const AIslandNode& B) {
			return A > B; // sort by factor
		});
		AIslandNode * currentRoom;
		if(isModified){
			auto iterator = openList.begin();
			int range = openList.Num() < 4 ? 0 : FMath::FRandRange(0, 3);
			for (int i = 0; i < range; ++i){
				++iterator;
			}
			currentRoom = *iterator;
		}
		else
			currentRoom = *openList.begin();
		
		openList.Remove(currentRoom);

		float fNew;
		for (uint32 roomNo : adj[currentRoom->getRoomNo()]){
			AIslandNode* neighbor = (*pivotRooms)[roomNo];
			neighbor->setHeuristic(calculateHeuristic(neighbor->getLocationX(), neighbor->getLocationY(), (*pivotRooms)[endRoom]->getLocationX(), (*pivotRooms)[endRoom]->getLocationY()));
			fNew = neighbor->getCost() + currentRoom->getCostSoFar() + neighbor->getHeuristic();
			if (roomNo == endRoom){
				neighbor->setCostSoFar(neighbor->getCost() + currentRoom->getCostSoFar());
				neighbor->setFactor(fNew);
				neighbor->setParentRoomNo(currentRoom->getRoomNo());
				openList.Add(neighbor);
				return tracePath(startRoom, endRoom, pivotRooms);
				
			}
			if (!(openList.Find(neighbor) && neighbor->getFactor() < fNew)) {
				//Heuristic + self cost + cost until here.
				if (!(closedList.Find(neighbor) && neighbor->getFactor() < fNew)) {
					openList.Remove(neighbor);
					neighbor->setCostSoFar(neighbor->getCost() + currentRoom->getCostSoFar());
					neighbor->setFactor(fNew);
					neighbor->setParentRoomNo(currentRoom->getRoomNo());
					openList.Add(neighbor);
				}
			}
		}
		closedList.Add(currentRoom);
	}
	UE_LOG(LogTemp, Warning, TEXT("\n Destination could not be found"));
	return Graph();
}

TArray<uint32> Graph::breadthFirstFindPathToPath(uint32 startRoom, TMap<uint32, AIslandNode*>* pivotRooms){
	TArray<uint32> nodesToPath;
	TQueue<uint32> queue;
	TMap<uint32, bool> visited;

	for( auto roomNo : adj){
		visited.Add(roomNo.Key, false);
	}

	queue.Enqueue(startRoom);

	while( !queue.IsEmpty() ){
		uint32 roomNo;
		queue.Dequeue(roomNo);
		nodesToPath.AddUnique(roomNo);
		for( auto neighborRoomNo : adj[roomNo]){
			auto neighbor = (*pivotRooms)[neighborRoomNo];
			if( neighbor->isPointless() && !visited[neighborRoomNo] ){
				visited[neighborRoomNo] = true;
				nodesToPath.AddUnique(roomNo);
				queue.Enqueue(neighborRoomNo);
			}
		}
	}
	return nodesToPath;
}