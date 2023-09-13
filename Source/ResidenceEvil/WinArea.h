// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WinArea.generated.h"

class UBoxComponent;
UCLASS()
class RESIDENCEEVIL_API AWinArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWinArea();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void WinGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* TriggerArea;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	

	

	

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
 
};


