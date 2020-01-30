// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Corridor.generated.h"

UCLASS()
class MAPGENERATOR_API ACorridor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACorridor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* sm;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* materialCorridor;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
