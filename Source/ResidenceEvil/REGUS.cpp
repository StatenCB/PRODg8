// Fill out your copyright notice in the Description page of Project Settings.


#include "REGUS.h"

UREGUS::UREGUS(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	FireTimerMultiplier = 1.f;
}

UREGUS* UREGUS::GetREGUS()
{
	return Cast<UREGUS>(UGameUserSettings::GetGameUserSettings());
}
