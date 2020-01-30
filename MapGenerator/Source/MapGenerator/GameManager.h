// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RandomMapGenerator.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"

UCLASS()
class MAPGENERATOR_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();
	~AGameManager();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
