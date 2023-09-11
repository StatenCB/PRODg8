// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"

#include "Components/BoxComponent.h"


// Sets default values
ARoom::ARoom()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RoomBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("RoomBounds"));
	RootComponent = RoomBounds;
}

void ARoom::StartFire()
{
	bFireStarted = true;
}

void ARoom::SpreadFire()
{
	for(ARoom* Room : NeighboringRooms)
	{
		Room->StartFire();
	}
}

// Called when the game starts or when spawned
void ARoom::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

