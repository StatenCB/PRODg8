// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "Battery.generated.h"

/**
 * 
 */
UCLASS()
class RESIDENCEEVIL_API ABattery : public AInteractableObject
{
	GENERATED_BODY()

public:

	ABattery();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* PickUpArea;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
