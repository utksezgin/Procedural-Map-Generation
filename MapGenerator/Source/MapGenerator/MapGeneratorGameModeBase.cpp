// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGeneratorGameModeBase.h"
#include "ConstructorHelpers.h"

AMapGeneratorGameModeBase::AMapGeneratorGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
