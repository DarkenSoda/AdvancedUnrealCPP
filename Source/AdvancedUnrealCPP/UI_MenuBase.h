#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_MenuBase.generated.h"

/**
 * Base C++ class for Main Menu, Pause Menu, and Game End screens.
 */
UCLASS()
class ADVANCEDUNREALCPP_API UUI_MenuBase : public UUserWidget
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "Menu")
    void QuitGame();

    UFUNCTION(BlueprintCallable, Category = "Menu")
    void StartNewGame(FName LevelName);

    UFUNCTION(BlueprintCallable, Category = "Menu")
    void ReturnToMainMenu(FName MainMenuLevelName);

    UFUNCTION(BlueprintCallable, Category = "Menu")
    void ResumeGame();
};
