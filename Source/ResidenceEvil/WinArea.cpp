// Fill out your copyright notice in the Description page of Project Settings.


#include "WinArea.h"

#include "ResidenceEvilCharacter.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AWinArea::AWinArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerArea = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerArea"));
	RootComponent = TriggerArea;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	TriggerArea->OnComponentBeginOverlap.AddUniqueDynamic(this, &AWinArea::OverlapBegin);

}

// Called when the game starts or when spawned
void AWinArea::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWinArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AWinArea::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AResidenceEvilCharacter* Character = Cast<AResidenceEvilCharacter>(OtherActor);
	if(Character && Character->bHasCarBattery)
	{
		WinGame();
		FDateTime EndTime = FDateTime::UtcNow();
		UE_LOG(LogTemp, Display, TEXT("%s ms end"), *EndTime.ToString())
	}
}




