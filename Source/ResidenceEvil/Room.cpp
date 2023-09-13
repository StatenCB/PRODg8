// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"

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
}

void ARoom::StartFire()
{
	bFireStarted = true;
	AudioComponent->Play();
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
	AudioComponent->SetSound(FireSound);
}

// Called every frame
void ARoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bFireStarted)
	{
		FireLevel += DeltaTime;
		if(!bFireSpread && FireLevel > FireSpreadThreshold)
		{
			SpreadFire();
		}
		if(!bKilledPlayer && bPlayerInRoom && FireLevel > FireSpreadThreshold)
		{
			bKilledPlayer = true;
			UGameplayStatics::PlaySound2D(this, GameOverSound);
			FTimerHandle TimerHandle;
			
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ARoom::ResetLevel, 2.0f); 
		}
	}
}

void ARoom::ResetLevel() const
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this,LevelToLoad);
}


