// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Reward.h"
#include "GameFramework/Actor.h"
#include "Monster.generated.h"

UCLASS(BlueprintType)
class MAPGENERATOR_API AMonster : public AActor{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *sm;
	UPROPERTY(EditAnywhere)
	float health;
	UPROPERTY(EditAnywhere)
	bool canDrop;
	UPROPERTY(EditAnywhere)
	AReward *loot;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
