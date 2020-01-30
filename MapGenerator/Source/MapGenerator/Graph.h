// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IslandNode.h"

//Consider removing this class and move them into map generator.
class MAPGENERATOR_API Graph
{
	TMap<int, TArray<uint32>> adj;

	public:
		Graph(){}
		Graph(uint32 roomCount);
		~Graph(){}

		void addEdge(uint32 u, uint32 v);
		void printGraph();
		Graph AStar(uint32 startRoom, uint32 endRoom, TMap<uint32, AIslandNode*> *pivotRooms, bool isModified);
		float calculateHeuristic(uint16 sourceX, uint16 sourceY, uint16 destX, uint16 destY);
		Graph tracePath(uint32 startRoom, uint32 endRoom, TMap<uint32, AIslandNode*> *pivotRooms);
		TArray<uint32> breadthFirstFindPathToPath(uint32 startRoom, TMap<uint32, AIslandNode*>* pivotRooms);
	
		TArray<uint32> operator[](int index){ return adj[index]; }

		bool contains(int roomNo){ return adj[roomNo].Num() != 0; }
		int size(){ return numberOfNodes; }

	private:
		int numberOfNodes { 0 };
};
