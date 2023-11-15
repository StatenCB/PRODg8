// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ResidenceEvilGameMode.generated.h"

UCLASS(minimalapi)
class AResidenceEvilGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AResidenceEvilGameMode();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void AddToLog(FString TextToLog);

	UFUNCTION(BlueprintCallable)
	void WinGameLog();

private:
	FString FilePath;
};



