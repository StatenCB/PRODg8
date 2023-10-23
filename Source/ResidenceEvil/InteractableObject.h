// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObject.generated.h"

class ARoom;

UCLASS()
class RESIDENCEEVIL_API AInteractableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableObject();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USphereComponent* InteractableArea;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintImplementableEvent)
	void OnPickUpDestroy();

	UFUNCTION(BlueprintCallable)
	void LogPickUpTimeAndLocation();

	UPROPERTY(BlueprintReadWrite)
	ARoom* CurrentRoom = nullptr;

};
