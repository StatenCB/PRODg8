// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableDoor.h"

#include "Components/BoxComponent.h"

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

