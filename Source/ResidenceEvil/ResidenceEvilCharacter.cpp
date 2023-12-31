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
#include "InteractableObject.h"
#include "ResidenceEvilGameMode.h"
#include "Room.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


//////////////////////////////////////////////////////////////////////////
// AResidenceEvilCharacter

AResidenceEvilCharacter::AResidenceEvilCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Only rotate on yaw axis
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
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
	CameraBoom->TargetArmLength = 20.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Dont rotate freely, follow behind player
	

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
	

	RightArmBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightArm"));
	RightArmBox->SetupAttachment(RootComponent);

	LeftArmBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftArmBox"));
	LeftArmBox->SetupAttachment(RootComponent);

	ForwardArmBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ForwardArmBox"));
	ForwardArmBox->SetupAttachment(RootComponent);
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AResidenceEvilCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Update the timer.
	if(!bBatteryCheckBlocked)
	{
		if (!bCanCheckForBatteries)
		{
			if (CanCheckBatteriesTimer >= CheckForBatteriesFeedbackCooldown)
			{
				bCanCheckForBatteries = true;
				OnCanCheckForBatteries();
				CanCheckBatteriesTimer = 0.0f;
			}
			CanCheckBatteriesTimer += DeltaSeconds;
		}
	}
	
	if (CurrentRoom)
	{
		if (CurrentRoom->PerformedBatteryCheck)
		{
			OnPerformedSuccessfulBatteryCheck();
		}
	}
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

	StopFeeling();
	AllowCheckInputBattery();
}


//////////////////////////////////////////////////////////////////////////
// Input

void AResidenceEvilCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

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

		//StopFeeling
		EnhancedInputComponent->BindAction(StopFeelingAction, ETriggerEvent::Triggered, this, &AResidenceEvilCharacter::StopFeeling);

		//Pick up
		EnhancedInputComponent->BindAction(PickUpObjectAction, ETriggerEvent::Triggered, this, &AResidenceEvilCharacter::PickUpObject);

		//Open Door
		EnhancedInputComponent->BindAction(OpenDoorAction, ETriggerEvent::Triggered, this, &AResidenceEvilCharacter::OpenDoor);

		//Get battery count
		EnhancedInputComponent->BindAction(CheckForBatteriesAction, ETriggerEvent::Triggered, this, &AResidenceEvilCharacter::TryCheckForBatteries);
	}
}

void AResidenceEvilCharacter::RemovePickUp()
{
	CurrentPickUp = nullptr;
}

void AResidenceEvilCharacter::OnPerformedSuccessfulBatteryCheck()
{
	bCanCheckForBatteries = false;
}

void AResidenceEvilCharacter::Move(const FInputActionValue& Value)
{
	
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// Get look direction
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		//AddMovementInput(RightDirection, MovementVector.X);

		// rotate to direction
		//AddControllerYawInput(LookAxisVector.X);
		
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
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
		
	}
}

void AResidenceEvilCharacter::FeelRight()
{
	LeftArmBox->SetCollisionProfileName("NoCollision");
	RightArmBox->SetCollisionProfileName("OverlapAllDynamic");
	ForwardArmBox->SetCollisionProfileName("NoCollision");
}

void AResidenceEvilCharacter::FeelLeft()
{
	LeftArmBox->SetCollisionProfileName("OverlapAllDynamic");
	RightArmBox->SetCollisionProfileName("NoCollision");
	ForwardArmBox->SetCollisionProfileName("NoCollision");
}

void AResidenceEvilCharacter::FeelForward()
{
	LeftArmBox->SetCollisionProfileName("NoCollision");
	RightArmBox->SetCollisionProfileName("NoCollision");
	ForwardArmBox->SetCollisionProfileName("OverlapAllDynamic");
}

void AResidenceEvilCharacter::PickUpObject()
{
	if (bCanPickUp)
	{
		NumberOfBatteries++;
		if (CurrentPickUp)
		{
			bCanPickUp = false;
			CurrentPickUp->OnPickUpDestroy();
			
		}
	}
}

void AResidenceEvilCharacter::StopFeeling()
{
	LeftArmBox->SetCollisionProfileName("NoCollision");
	RightArmBox->SetCollisionProfileName("NoCollision");
	ForwardArmBox->SetCollisionProfileName("NoCollision");
}

void AResidenceEvilCharacter::OpenDoor()
{
	
	if (bCanOpen)
	{
		if (CurrentDoor)
		{
			CurrentDoor->MovePlayer();
		}
	}
}

void AResidenceEvilCharacter::TryCheckForBatteries()
{
	if (bAllowedToCheckBattery)
	{
		GetWorldTimerManager().SetTimer(AllowBatteryInputHandle, this, &AResidenceEvilCharacter::PerformBatteryButtonPress, 0.1f, false, 0.1f);
	}
}


void AResidenceEvilCharacter::CheckForBatteries()
{
	if (CurrentRoom)
	{
		Cast<AResidenceEvilGameMode>(GetWorld()->GetAuthGameMode())->AddToLog(FDateTime::UtcNow().ToString() + " ms Player checked for batteries in " + CurrentRoom->RoomName);
		if(CurrentRoom->bIsGarage || bHasCarBattery)
		{
			CurrentRoom->OnPlayStatic();
		}
		else
		{
			CurrentRoom->OnCheckValidBatteries();
			CurrentRoom->OnCheckDoorLocations();
		}
	}
}

void AResidenceEvilCharacter::ResetInputBatteriesTimer()
{
	//UE_LOG(LogTemp, Warning, TEXT("Cant press button"));
	bAllowedToCheckBattery = false;
	GetWorldTimerManager().SetTimer(ResetTimerHandle, this, &AResidenceEvilCharacter::AllowCheckInputBattery, 0.1f, false, 1.f);
}

void AResidenceEvilCharacter::AllowCheckInputBattery()
{
	//UE_LOG(LogTemp, Warning, TEXT("Can press button"));
	bAllowedToCheckBattery = true;
}

void AResidenceEvilCharacter::PerformBatteryButtonPress()
{
	if(!bBatteryCheckBlocked)
	{
		if (bCanCheckForBatteries)
		{
			CheckForBatteries();
		}else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Button pressed after cooldown"));
			OnDeniedCheckForBatteries();
		}
		GetWorldTimerManager().SetTimer(DenyBatteryInputHandle, this, &AResidenceEvilCharacter::AllowCheckInputBattery, AllowBatteryInputDelay, false, AllowBatteryInputDelay);	
	}
}



