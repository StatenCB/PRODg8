﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"

#include "FireActor.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ARoom::ARoom()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RoomBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("RoomBounds"));
	RootComponent = RoomBounds;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RoomBounds);

	GuidingSoundAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("GuidingSoundAudioComponent"));
	GuidingSoundAudioComponent->SetupAttachment(RoomBounds);
}

void ARoom::StartFire()
{
	bFireStarted = true;
	//AudioComponent->Play();
}

void ARoom::StartGuidingSound()
{
	OnStartingGuidingSoundEvent();
}

void ARoom::SpreadFire()
{
	bFireSpread = true;
	for(ARoom* Room : NeighboringRooms)
	{
		Room->StartFire();
	}
}

// Called when the game starts or when spawned
void ARoom::BeginPlay()
{
	Super::BeginPlay();
	//AudioComponent->SetSound(FireSound);

	GetWorldTimerManager().SetTimer(GatherOverlappingFireActorsHandle, this, &ARoom::GatherOverlappingFireActors, 0.1f, false);
	
}

// Called every frame
void ARoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bFireStarted)
	{
		
		for (AFireActor* FireActor : FireActors)
		{
			if (FireActor->bIsFirstToStartFire && !FireActor->bIsSetOnFire)
			{
				FDateTime StartTime = FDateTime::UtcNow();
				UE_LOG(LogTemp, Display, TEXT("%s ms start"), *StartTime.ToString())
				FireActor->StartFireFromRoom(this);
			}
		}
		FireLevel += DeltaTime;
		//AudioComponent->VolumeMultiplier = 0.2 + (FireLevel/FireDeathThreshold) * 2;
		if(!bFireSpread && FireLevel > FireSpreadThreshold)
		{
			SpreadFire();
		}
		
		if(!bKilledPlayer && bPlayerInRoom && FireLevel > FireSpreadThreshold)
		{
			bKilledPlayer = true;
			UGameplayStatics::PlaySound2D(this, GameOverSound);
			FTimerHandle TimerHandle;
			
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ARoom::ResetLevel, TimeUntilRestart); 
		}
	}
}


void ARoom::ResetLevel() const
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this,LevelToLoad);
}

void ARoom::GatherOverlappingFireActors()
{
	// populate array with all overlapping fire actors
	TArray<AActor*> OverlappingFireActors;
	RoomBounds->GetOverlappingActors(OverlappingFireActors, AFireActor::StaticClass());
	for(AActor* OverlappingActor : OverlappingFireActors)
	{
		AFireActor* FireActor = Cast<AFireActor>(OverlappingActor);
		if(ensure( FireActor != nullptr)) FireActors.Add(FireActor);
	}
	UE_LOG(LogTemp, Warning, TEXT("Added %i fireactors"), FireActors.Num());
}


