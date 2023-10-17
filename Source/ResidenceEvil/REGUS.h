// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "REGUS.generated.h"

/**
 * 
 */
UCLASS()
class RESIDENCEEVIL_API UREGUS : public UGameUserSettings
{
	GENERATED_UCLASS_BODY()
public:

	UFUNCTION(BlueprintCallable)
	static UREGUS* GetREGUS();
	
	UPROPERTY(Config, BlueprintReadOnly)
	float FireTimerMultiplier;
};
