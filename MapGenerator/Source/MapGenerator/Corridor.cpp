// Fill out your copyright notice in the Description page of Project Settings.


#include "Corridor.h"

// Sets default values
ACorridor::ACorridor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	sm = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	sm->SetMobility(EComponentMobility::Movable);
	RootComponent = sm;
	if(materialCorridor)
		sm->SetMaterial(0, materialCorridor);
}

// Called when the game starts or when spawned
void ACorridor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACorridor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

