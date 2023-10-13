// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireActor.generated.h"

class USphereComponent;

UCLASS()
class RESIDENCEEVIL_API AFireActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* BurstAudioComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USphereComponent* FireActorBoundsComponent;

	UFUNCTION(BlueprintImplementableEvent)
	void SpreadFire();

	UFUNCTION(BlueprintCallable)
	void StartFire();

	void StartFireFromRoom(class ARoom* RoomPtr);

	UPROPERTY(EditAnywhere)
	bool bIsFirstToStartFire;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsSetOnFire = false;

	UPROPERTY(EditAnywhere)
	double FireLevel;

	UPROPERTY(EditAnywhere)
	double FireSpreadThreshold = 10;

	UPROPERTY(EditAnywhere)
	double FireDeathThreshold = 20;

	UPROPERTY(EditAnywhere)
	USoundBase* FireSound;

	ACharacter* PLayerCharacter;

	float DistanceToPlayer = 0.1f;

	ARoom* Room = nullptr;
	
};
