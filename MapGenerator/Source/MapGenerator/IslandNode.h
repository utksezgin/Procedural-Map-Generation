// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Island.h"
#include "IslandNode.generated.h"

UCLASS()
class MAPGENERATOR_API AIslandNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIslandNode();
	void InitRoom(uint16 h, uint16 w, uint32 no, bool val);

	uint32 getRoomNo() const { return roomNo; }
	uint32 getParentRoomNo() const { return parentRoomNo; }
	uint16 getHeight() const { return height; }
	uint16 getWidth() const { return width; }
	uint16 getLocationX() const { return locationX; }
	uint16 getLocationY() const { return locationY; }
	uint8 getDifficulty() const { return difficulty; }
	bool isActive() const { return bActive; }
	bool isPointless() const { return bIsPointless; }
	bool isPopulated() const { return bIsPopulated; }
	bool isPivot() const { return bPivot; }
	float getCost() const { return cost; }
	float getCostSoFar() const { return costSoFar; }
	float getHeuristic() const { return heuristic; }
	float getFactor() const { return factor; }
	uint32 getArea() const { return height * width; }
	AIsland * getIsland() const { return island; }

	void setRoomNo(uint32 no) { roomNo = no; }
	void setParentRoomNo(uint32 no) { parentRoomNo = no; }
	void setHeight(uint16 h) { height = h; }
	void setWidth(uint16 w) { width = w; }
	void setLocationX(uint16 x) { locationX = x; }
	void setLocationY(uint16 y) { locationY = y; }
	void setDifficulty(uint8 dif) { difficulty = dif; }
	void setActive(bool val) { bActive = val; }
	void setPointless(bool val) { bIsPointless = val; }
	void setPopulated(bool val) { bIsPointless = val; }
	void setPivot(bool val) { bPivot = val; }
	void setCost(float c) { cost = c; }
	void setCostSoFar(float f) { costSoFar = f; }
	void setHeuristic(float h) { heuristic = h; }
	void setFactor(float f) { factor = f; }
	void setIsland(AIsland * i) { island = i; }

	void calculateCost() { cost = height * height + width * width; }

	bool operator==(const AIslandNode & rhs) const { return roomNo == rhs.roomNo; }
	bool operator>(const AIslandNode & rhs) const { return factor > rhs.factor; }

	//friend uint32 GetTypeHash(AWall r);
	friend bool Matches(AIslandNode A, AIslandNode B);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	uint16 height { 1 };
	uint16 width { 1 };
	uint32 roomNo { 0 };
	uint32 parentRoomNo { 1 };
	uint8 difficulty { 0 };
	uint16 locationX { 0 };
	uint16 locationY { 0 };
	float heuristic { 0.f };
	float cost { 0.f };
	float costSoFar { 0.f };
	float factor { 0.f };
	bool bPivot { false };
	bool bActive { false };
	bool bIsPopulated { false };
	bool bIsPointless { true };

	UPROPERTY()
	AIsland * island;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
