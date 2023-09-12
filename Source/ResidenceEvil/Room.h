// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.generated.h"

class UBoxComponent;

UCLASS()
class RESIDENCEEVIL_API ARoom : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	double FireLevel;

	UPROPERTY(EditAnywhere)
	double FireSpreadThreshold = 100;

	UPROPERTY(EditAnywhere)
	double FireDeathThreshold = 200;

	UPROPERTY(EditAnywhere)
	UBoxComponent* RoomBounds;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARoom> RoomClass;
	
	UPROPERTY(EditAnywhere)
	TArray<ARoom*> NeighboringRooms;

	UPROPERTY(EditAnywhere)
	bool bFireStarted = false;

	UFUNCTION(BlueprintCallable)
	void StartFire();

private:
	void SpreadFire();

	bool bFireSpread = false;
};
