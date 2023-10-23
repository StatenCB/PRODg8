// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject.h"

#include "Components/SphereComponent.h"

// Sets default values
AInteractableObject::AInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractableArea = CreateDefaultSubobject<USphereComponent>(TEXT("InteractableArea"));
	InteractableArea->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableObject::LogPickUpTimeAndLocation()
{
	FDateTime PickupTime = FDateTime::UtcNow();
	FVector PickupLoc = GetActorLocation();
	UE_LOG(LogTemp, Display, TEXT("%s ms Player picked up battery, at location %f, %f, %f"), *PickupTime.ToString(), PickupLoc.X, PickupLoc.Y, PickupLoc.Z);
}

