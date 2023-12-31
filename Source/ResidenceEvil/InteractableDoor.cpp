// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableDoor.h"

#include "Components/BoxComponent.h"
#include "ResidenceEvilGameMode.h"

// Sets default values
AInteractableDoor::AInteractableDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);

	InteractableArea = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractableArea"));
	InteractableArea->SetupAttachment(MeshComponent);

}

// Called when the game starts or when spawned
void AInteractableDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableDoor::LogDoor()
{
	FString DoorName = GetName();
	UE_LOG(LogTemp, Display, TEXT("Player Opened door %s"), *DoorName);
	FString DoorLogEntry = "" + FDateTime::UtcNow().ToString() + " ms Player opened door " + DoorName;
	Cast<AResidenceEvilGameMode>(GetWorld()->GetAuthGameMode())->AddToLog(DoorLogEntry);
}


