// Fill out your copyright notice in the Description page of Project Settings.


#include "IslandNode.h"

// Sets default values
AIslandNode::AIslandNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AIslandNode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIslandNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool Matches(AIslandNode A, AIslandNode B) {
	return A == B;
}

