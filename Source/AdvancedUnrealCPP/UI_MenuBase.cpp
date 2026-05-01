#include "UI_MenuBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UUI_MenuBase::QuitGame()
{
    if (APlayerController* PC = GetOwningPlayer())
    {
        UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
    }
}

void UUI_MenuBase::StartNewGame(FName LevelName)
{
    UGameplayStatics::OpenLevel(this, LevelName);
}

void UUI_MenuBase::ReturnToMainMenu(FName MainMenuLevelName)
{
    UGameplayStatics::OpenLevel(this, MainMenuLevelName);
}

void UUI_MenuBase::ResumeGame()
{
    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->SetPause(false);
        PC->SetShowMouseCursor(false);
        PC->SetInputMode(FInputModeGameOnly());
    }
    RemoveFromParent();
}
