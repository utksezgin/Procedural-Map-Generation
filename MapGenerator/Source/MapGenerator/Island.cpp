// Fill out your copyright notice in the Description page of Project Settings.


#include "Island.h"

// Sets default values
AIsland::AIsland()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	sm = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	sm->SetMobility(EComponentMobility::Movable);
	RootComponent = sm;

}

// Called when the game starts or when spawned
void AIsland::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AIsland::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AIsland::switchMaterial(EMaterialEnum mat){
	switch(mat)
	{
		case EMaterialEnum::mat_Start:
			if(materialStart)
				sm->SetMaterial(0, materialStart);
			break;
		case EMaterialEnum::mat_End:
			if (materialStart)
				sm->SetMaterial(0, materialEnd);
			break;
		case EMaterialEnum::mat_Islands:
			if (materialStart)
				sm->SetMaterial(0, materialIslands);
			break;
	}
}

