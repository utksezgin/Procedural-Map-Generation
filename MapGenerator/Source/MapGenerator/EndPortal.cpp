// Fill out your copyright notice in the Description page of Project Settings.


#include "EndPortal.h"

// Sets default values
AEndPortal::AEndPortal()
{
	PrimaryActorTick.bCanEverTick = false; //Make it true once you add logic to it.
	sm = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	sm->SetMobility(EComponentMobility::Movable);
	RootComponent = sm;

}

// Called when the game starts or when spawned
void AEndPortal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEndPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

