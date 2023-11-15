// Copyright Epic Games, Inc. All Rights Reserved.

#include "ResidenceEvilGameMode.h"

#include "ResidenceEvilCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Kismet/KismetStringLibrary.h"


AResidenceEvilGameMode::AResidenceEvilGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AResidenceEvilGameMode::BeginPlay()
{
	Super::BeginPlay();

	FilePath = FPaths::ConvertRelativePathToFull(FPaths::GameAgnosticSavedDir() + TEXT("/LoggedData.txt"));
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(FilePath));

	FString SessionHead = "New Session Started " + FDateTime::UtcNow().ToString();
	AddToLog(SessionHead);
}

void AResidenceEvilGameMode::AddToLog(FString TextToLog)
{
	FFileHelper::SaveStringToFile(TextToLog + "\n", *FilePath ,FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
}

void AResidenceEvilGameMode::WinGameLog()
{
	AddToLog(FDateTime::UtcNow().ToString() + " ms Player won game!");
}
