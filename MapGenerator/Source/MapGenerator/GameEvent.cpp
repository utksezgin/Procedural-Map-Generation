// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEvent.h"

// Sets default values
AGameEvent::AGameEvent()
{
	PrimaryActorTick.bCanEverTick = false; //Make it true once you add logic to it.
	sm = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	sm->SetMobility(EComponentMobility::Movable);
	RootComponent = sm;
}

// Called when the game starts or when spawned
void AGameEvent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameEvent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameEvent::eventComplete(){
	if(reward){
		FTransform transform = FTransform(FRotator(0, 0, 0), FVector(0, 0, 1), FVector(1, 1, 1));
		transform.SetLocation(this->GetActorLocation());
		this->SetActorHiddenInGame(true);
		this->SetActorEnableCollision(false);
		GetWorld()->SpawnActor<AReward>(reward, transform);
	}
}
