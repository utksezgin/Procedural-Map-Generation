// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Reward.generated.h"

UCLASS()
class MAPGENERATOR_API AReward : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReward();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent * sm;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
