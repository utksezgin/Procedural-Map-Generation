// Fill out your copyright notice in the Description page of Project Settings.


#include "Reward.h"

// Sets default values
AReward::AReward()
{
	PrimaryActorTick.bCanEverTick = false; //Make it true once you add logic to it.
	sm = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	sm->SetMobility(EComponentMobility::Movable);
	RootComponent = sm;

}

// Called when the game starts or when spawned
void AReward::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AReward::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

