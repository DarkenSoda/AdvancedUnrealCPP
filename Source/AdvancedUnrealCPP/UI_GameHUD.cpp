#include "UI_GameHUD.h"
#include "WaveManager.h"
#include "Kismet/GameplayStatics.h"

AWaveManager* UUI_GameHUD::GetWaveManager() const
{
    if (!CachedWaveManager)
    {
        CachedWaveManager = Cast<AWaveManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWaveManager::StaticClass()));
    }
    return CachedWaveManager;
}

int32 UUI_GameHUD::GetCurrentWave() const
{
    if (AWaveManager* WM = GetWaveManager())
    {
        return WM->CurrentWave;
    }
    return 0;
}

int32 UUI_GameHUD::GetEnemiesAlive() const
{
    if (AWaveManager* WM = GetWaveManager())
    {
        return WM->EnemiesAlive;
    }
    return 0;
}
