// Copyright Epic Games, Inc. All Rights Reserved.

#include "ResidenceEvilCharacter.h"

#include "ArmActor.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


//////////////////////////////////////////////////////////////////////////
// AResidenceEvilCharacter

AResidenceEvilCharacter::AResidenceEvilCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AResidenceEvilCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	/*
	Arm = GetWorld()->SpawnActor<AArmActor>(ArmClass);
	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::KeepWorld, true);
	Arm->AttachToComponent(GetMesh(), AttachmentTransformRules, TEXT("spine_04"));
	Arm->SetActorRelativeRotation(FRotator(0, 0, 90));
	Arm->SetActorScale3D(FVector(0.25, 0.25, 1));
	*/
}

//////////////////////////////////////////////////////////////////////////
// Input

void AResidenceEvilCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AResidenceEvilCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AResidenceEvilCharacter::MoveArm);

		//Arm movement
		EnhancedInputComponent->BindAction(FeelRightAction, ETriggerEvent::Triggered, this, &AResidenceEvilCharacter::FeelRight);

		//Arm movement
		EnhancedInputComponent->BindAction(FeelLeftAction, ETriggerEvent::Triggered, this, &AResidenceEvilCharacter::FeelLeft);

		//Arm movement
		EnhancedInputComponent->BindAction(FeelForwardAction, ETriggerEvent::Triggered, this, &AResidenceEvilCharacter::FeelForward);

	}

}

void AResidenceEvilCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		//AddMovementInput(RightDirection, MovementVector.X);

		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AResidenceEvilCharacter::MoveArm(const FInputActionValue& Value)
{
	
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	//FRotator NewRotator = LookAxisVector.Rp;

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		//Arm->SetActorRelativeRotation(NewRotator);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AResidenceEvilCharacter::FeelRight()
{
	FHitResult HitResult;
	FVector Origin, Extent;
	GetActorBounds(true, Origin, Extent);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Visibility));

	TArray<AActor*> IgnoredActors = TArray<AActor*>();
	IgnoredActors.Add(this);

	bool bHit;

	bHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorRightVector() * 200.f, ObjectTypes, true, IgnoredActors, EDrawDebugTrace::ForOneFrame, HitResult, true, FLinearColor::Red, FLinearColor::Blue, 0.f);
}

void AResidenceEvilCharacter::FeelLeft()
{
	FHitResult HitResult;
	FVector Origin, Extent;
	GetActorBounds(true, Origin, Extent);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Visibility));

	TArray<AActor*> IgnoredActors = TArray<AActor*>();
	IgnoredActors.Add(this);

	bool bHit;

	bHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), GetActorLocation(), GetActorLocation() - GetActorRightVector()* 200.f, ObjectTypes, true, IgnoredActors, EDrawDebugTrace::ForOneFrame, HitResult, true, FLinearColor::Red, FLinearColor::Blue, 0.f);
}

void AResidenceEvilCharacter::FeelForward()
{
	FHitResult HitResult;
	FVector Origin, Extent;
	GetActorBounds(true, Origin, Extent);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Visibility));

	TArray<AActor*> IgnoredActors = TArray<AActor*>();
	IgnoredActors.Add(this);

	bool bHit;

	bHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 200.f, ObjectTypes, true, IgnoredActors, EDrawDebugTrace::ForOneFrame, HitResult, true, FLinearColor::Red, FLinearColor::Blue, 0.f);
}




