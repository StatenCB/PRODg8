// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableDoor.generated.h"

UCLASS()
class RESIDENCEEVIL_API AInteractableDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableDoor();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UBoxComponent* InteractableArea;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* MeshComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintImplementableEvent)
    void MovePlayer();

};
