﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.generated.h"

class AFireActor;
class USoundCue;
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
	double FireSpreadThreshold = 10;

	UPROPERTY(EditAnywhere)
	double FireDeathThreshold = 20;

	UPROPERTY(EditAnywhere)
	UBoxComponent* RoomBounds;

	UPROPERTY(EditAnywhere)
	UAudioComponent* AudioComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* GuidingSoundAudioComponent;

	UPROPERTY(EditAnywhere)
	TArray<AFireActor*> FireActors;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARoom> RoomClass;
	
	UPROPERTY(EditAnywhere)
	TArray<ARoom*> NeighboringRooms;

	UPROPERTY(EditAnywhere)
	bool bFireStarted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPlayerInRoom = false;

	//UPROPERTY(EditAnywhere)
	//USoundBase* FireSound;

	UPROPERTY(EditAnywhere)
	USoundBase* GameOverSound;

	UFUNCTION(BlueprintCallable)
	void StartFire();

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> LevelToLoad;

	void StartGuidingSound();

	UFUNCTION(BlueprintImplementableEvent)
	void OnStartingGuidingSoundEvent();

private:
	void SpreadFire();

	UPROPERTY(EditAnywhere)
	float TimeUntilRestart;
	
	void ResetLevel() const;
	bool bFireSpread = false;
	bool bKilledPlayer = false;

	FTimerHandle  GatherOverlappingFireActorsHandle;
	
	void GatherOverlappingFireActors();
};
