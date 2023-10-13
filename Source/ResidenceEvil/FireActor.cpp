// Fill out your copyright notice in the Description page of Project Settings.


#include "FireActor.h"

#include "Room.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFireActor::AFireActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FireActorBoundsComponent = CreateDefaultSubobject<USphereComponent>(TEXT("FireBounds"));
	RootComponent = FireActorBoundsComponent;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(FireActorBoundsComponent);

	BurstAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BurstAudioComponent"));
	BurstAudioComponent->SetupAttachment(FireActorBoundsComponent);
}

// Called when the game starts or when spawned
void AFireActor::BeginPlay()
{
	Super::BeginPlay();

	AudioComponent->Stop();
	BurstAudioComponent->Stop();

	PLayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ensure (PLayerCharacter != nullptr);
}

// Called every frame
void AFireActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSetOnFire)
	{
		DistanceToPlayer = FVector::Distance(GetActorLocation(), PLayerCharacter->GetActorLocation());

		//AudioComponent->VolumeMultiplier = 0.1 + (DistanceToPlayer / FireSpreadThreshold) * 0.3f;
		if (bIsFirstToStartFire) GEngine->AddOnScreenDebugMessage(-1, 0.20f, FColor::Red, FString::Printf(TEXT("Volume multiuplier %f"), AudioComponent->VolumeMultiplier));


		// Ensure that the VolumeMultiplier is clamped between 0.0 and 1.0
		//AudioComponent->VolumeMultiplier = FMath::Clamp(AudioComponent->VolumeMultiplier, 0.0f, 1.0f);
		//AudioComponent->SetVolumeMultiplier( 1 + ((DistanceToPlayer / 80.f)  * 0.3f));
		//AudioComponent->VolumeMultiplier = FMath::Clamp(AudioComponent->VolumeMultiplier, 0.2f, 50.0f);
		/*
		FireLevel += DeltaTime;
		AudioComponent->VolumeMultiplier = 0.1 + (FireLevel/FireDeathThreshold) * 1.5;
		if (bIsFirstToStartFire) GEngine->AddOnScreenDebugMessage(-1, 0.20f, FColor::Red, FString::Printf(TEXT("Volume multiuplier %f"), AudioComponent->VolumeMultiplier));
		*/
	}
}

void AFireActor::StartFireFromRoom(ARoom* RoomPtr)
{
	Room = RoomPtr;
	ensure (Room != nullptr);
	Room->StartGuidingSound();
	StartFire();
}

void AFireActor::StartFire()
{
	bIsSetOnFire = true;
	SpreadFire();
}

