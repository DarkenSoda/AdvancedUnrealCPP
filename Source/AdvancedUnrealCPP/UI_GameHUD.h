#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_GameHUD.generated.h"

class AWaveManager;

/**
 * 
 */
UCLASS()
class ADVANCEDUNREALCPP_API UUI_GameHUD : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUD")
    AWaveManager* GetWaveManager() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUD")
    int32 GetCurrentWave() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUD")
    int32 GetEnemiesAlive() const;

private:
    UPROPERTY()
    mutable AWaveManager* CachedWaveManager;
};
