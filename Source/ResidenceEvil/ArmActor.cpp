// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmActor.h"

// Sets default values
AArmActor::AArmActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArmMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmMesh"));
	ArmMesh->SetupAttachment(RootComponent);
	ArmMesh->SetWorldScale3D(FVector(0.25, 0.25, 5));
	ArmMesh->SetRelativeScale3D(FVector(0.25, 0.25, 5));
	//ArmMesh->AttachToComponent(Mesh1P ,FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("spline_01")));

	//ArmMesh->SetMobility(EComponentMobility::Movable);

}

// Called when the game starts or when spawned
void AArmActor::BeginPlay()
{
	Super::BeginPlay();

	
	
}

// Called every frame
void AArmActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

