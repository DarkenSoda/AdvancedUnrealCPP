// Copyright Epic Games, Inc. All Rights Reserved.

#include "AdvancedUnrealCPPGameMode.h"

AAdvancedUnrealCPPGameMode::AAdvancedUnrealCPPGameMode()
{
	// stub
    bUseSeamlessTravel = true;
}


void AAdvancedUnrealCPPGameMode::TravelToLevel(const FString& MapPath)
{
    UWorld* World = GetWorld();
    if (World)
    {
        World->ServerTravel(MapPath, false);
    }
}