// Fill out your copyright notice in the Description page of Project Settings.


#include "FireActor.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AFireActor::AFireActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FireActorBoundsComponent = CreateDefaultSubobject<USphereComponent>(TEXT("FireBounds"));
	RootComponent = FireActorBoundsComponent;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(FireActorBoundsComponent);
}

// Called when the game starts or when spawned
void AFireActor::BeginPlay()
{
	Super::BeginPlay();

	AudioComponent->Stop();
}

// Called every frame
void AFireActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSetOnFire)
	{
		FireLevel += DeltaTime;
		AudioComponent->VolumeMultiplier = 0.2 + (FireLevel/FireDeathThreshold) * 2;
	}
	
}

void AFireActor::StartFire()
{
	SpreadFire();  
}

