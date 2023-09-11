// Fill out your copyright notice in the Description page of Project Settings.


#include "Battery.h"

#include "Components/SphereComponent.h"

ABattery::ABattery()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PickUpArea = CreateDefaultSubobject<USphereComponent>(TEXT("PickUpArea"));
	PickUpArea->SetupAttachment(RootComponent);
}

void ABattery::BeginPlay()
{
	Super::BeginPlay();
	
}
