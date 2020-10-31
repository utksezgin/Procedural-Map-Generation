// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EndPortal.h"
#include "Graph.h"
#include "Island.h"
#include "Reward.h"
#include "Monster.h"
#include "Corridor.h"
#include "GameEvent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "IslandNode.h"
#include "RandomMapGenerator.generated.h"

USTRUCT()
struct FNestedRewardArray {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class AReward>> arr;
};

USTRUCT()
struct FNestedEventArray {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<class AGameEvent>> arr;
};

USTRUCT()
struct FNestedMonsterArray {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<class AMonster>> arr;
};

USTRUCT(BlueprintType)
struct FParameters {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MapGenerate)
	int rectangleHeight			{ 20 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MapGenerate)
	int rectangleWidth			{ 20 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MapGenerate)
	uint8  maxIslandHeight		{ 4 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MapGenerate)
	uint8  maxIslandWidth		{ 4 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MapGenerate)
	int maxDeadEndCount			{ 2 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MapGenerate)
	int pointlessAreaThreshold	{ 1 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MapGenerate)
	float mergeChance			{ 0.3f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MapGenerate)
	float sidePathMerge			{ 0.25f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MapGenerate)
	float  islandGap			{ 3000.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MapGenerate)
	float  islandScale			{ 20.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MapGenerate)
	float  eventProcChance		{ 0.25f };
};

UCLASS()
class MAPGENERATOR_API ARandomMapGenerator : public AActor
{
	GENERATED_BODY()
	
	public:	
		ARandomMapGenerator();

		UFUNCTION(BlueprintCallable)
		float generateMap();
		UFUNCTION(BlueprintCallable)
		FParameters getParameters() const{ return generatorParameters; }
		UFUNCTION(BlueprintCallable)
		void initParameters(const FParameters& parameters) { generatorParameters = parameters; }
		UFUNCTION(BlueprintCallable)
		int getLayoutArea();


	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

	private:

		void initLayout();
		int createInitialLayout();
		//Merge two above functions
	
		void mergeRooms();
		bool CheckOverlap(AIslandNode* newRoom, uint16 height, uint16 width);
		void expandLayout();
		AIsland* spawnIsland(FTransform transform) const;
		void printLayout();
		void solvePointlessAreas();
		void insertLayout();
		void spawnEndPortal();
		void spawnCorridor(const FVector& islandLocation1, const FVector& islandLocation2) const;
		void spawnEvent(int eventNo, int rewardQuality, const FVector& islandLocation);
		void spawnReward(int rewardTier, const FVector& islandLocation);
		void populateLayout();
		void spawnMonsters(int monsterCount, const FVector& islandLocation);
	

		uint8  minRectangleHeight	 { 1 };
		uint8  minRectangleWidth	 { 1 };
		uint32 roomCount			 { 0 };
		uint32 pointlessRoomCount	 { 0 };
		uint32 startRoomNo			 { 0 };
		uint32 endRoomNo			 { 0 };
		float  mergeOfDeadEndChance { 0.1f }; //Chance of merged island connecting with already existing island.
		float  startEndOffset		 { 0.2f };

		UPROPERTY(EditAnywhere, Category = Parameters)
		FParameters generatorParameters;

		UPROPERTY(EditAnywhere, Category= Spawn)
		TSubclassOf<class AIsland> island;
		UPROPERTY(EditAnywhere, Category = Spawn)
		TSubclassOf<class AEndPortal> endPortal;
		UPROPERTY(EditAnywhere, Category = Spawn)
		TSubclassOf<class ACorridor> corridor;
		UPROPERTY(EditAnywhere)
		ACharacter* player;
		UPROPERTY(EditAnywhere, Category = Population)
		TArray<FNestedRewardArray> rewardList;
		UPROPERTY(EditAnywhere, Category = Population)
		TArray<FNestedMonsterArray> monsterList;
		UPROPERTY(EditAnywhere, Category = Population)
		TArray<TSubclassOf<class AGameEvent>> eventList;
	
		UPROPERTY()
		TMap<uint32, AIslandNode*> pivotRooms;
		//Idk why did I do all that (TArray.GetData() + index) conversion from vector.at()
		TArray<TArray<AIslandNode*>> initialLayout;
		Graph layout;
		Graph finalLayout;

};
