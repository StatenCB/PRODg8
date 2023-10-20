// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "InteractableDoor.h"
#include "ResidenceEvilCharacter.generated.h"


class UBoxComponent;
class AInteractableObject;
class ARoom;

UCLASS(config=Game)
class AResidenceEvilCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FeelRightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FeelLeftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FeelForwardAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PickUpObjectAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* StopFeelingAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* OpenDoorAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CheckForBatteriesAction;


public:
	AResidenceEvilCharacter();

	virtual void Tick(float DeltaSeconds) override;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Arm")
	UBoxComponent* RightArmBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Arm")
	UBoxComponent* LeftArmBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Arm")
	UBoxComponent* ForwardArmBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NumberOfBatteries = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bHasStartedInitialFire = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasCarBattery;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Pickup")
	bool bCanPickUp = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanOpen = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AInteractableDoor* CurrentDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AInteractableObject* CurrentPickUp;

	UFUNCTION(BlueprintCallable)
	void RemovePickUp();

	UFUNCTION()
	void TryCheckForBatteries();

	UFUNCTION(BlueprintImplementableEvent)
	void OnCanCheckForBatteries();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeniedCheckForBatteries();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CheckForBatteriesFeedbackCooldown = 10.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanCheckForBatteries = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bAllowedToCheckBattery = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bBatteryCheckBlocked = true;

	FTimerHandle ResetTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ResetTimerDelay = 2.f;

	

	FTimerHandle AllowBatteryInputHandle;
	FTimerHandle DenyBatteryInputHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AllowBatteryInputDelay = 2.f;


	UFUNCTION()
	void OnPerformedSuccessfulBatteryCheck();
	
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void MoveArm(const FInputActionValue& Value);

	UFUNCTION()
	void FeelRight();

	UFUNCTION()
	void FeelLeft();

	UFUNCTION()
	void FeelForward();

	UFUNCTION()
	void PickUpObject();

	UFUNCTION()
	void StopFeeling();

	UFUNCTION()
	void OpenDoor();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arm")
    float FeelRange;

	UFUNCTION(BlueprintImplementableEvent)
	void FeltSomething(FHitResult Result);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ARoom* CurrentRoom = nullptr;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	void CheckForBatteries();

	float CanCheckBatteriesTimer = 0.f;

	void ResetInputBatteriesTimer();

	void AllowCheckInputBattery();

	void PerformBatteryButtonPress();

	

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	
};

