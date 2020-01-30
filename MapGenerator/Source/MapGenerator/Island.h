// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstance.h"
#include "Island.generated.h"

UENUM()
enum class EMaterialEnum : uint8 {
	mat_Start UMETA(DisplayName = "Start"),
	mat_End 	UMETA(DisplayName = "End"),
	mat_Islands	UMETA(DisplayName = "Islands"),
};

UCLASS()
class MAPGENERATOR_API AIsland : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AIsland();
	void setScale(FVector scale, float scaleMultiplier) { sm->SetWorldScale3D(scale * 0.5f); }
	void setNotPointlessMat() { if (matNotPointlessArea) sm->SetMaterial(0, matNotPointlessArea); }
	void switchMaterial(EMaterialEnum mat);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* sm;	
	UPROPERTY(EditAnywhere)
	UMaterialInterface* materialStart;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* materialEnd;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* materialIslands;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* matNotPointlessArea;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
