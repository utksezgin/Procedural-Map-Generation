// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Reward.h"
#include "Components/StaticMeshComponent.h"
#include "GameEvent.generated.h"

UCLASS()
class MAPGENERATOR_API AGameEvent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameEvent();
	void setReward(TSubclassOf<class AReward> r){ reward = r; }
	UFUNCTION(BlueprintCallable, Category = "Complete")
	void eventComplete();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent * sm;
	TSubclassOf<class AReward> reward;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
