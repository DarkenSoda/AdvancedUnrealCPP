#include "WaveManager.h"
#include "AIEnemy.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"
#include "NavigationSystem.h"
#include "HealthPickupInterface.h"

// Sets default values
AWaveManager::AWaveManager()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;
    bAlwaysRelevant = true; // Make sure clients always receive the wave counts easily

    CurrentWave = 0;
    EnemiesAlive = 0;
    EnemiesToSpawnThisWave = 0;
}

// Called when the game starts or when spawned
void AWaveManager::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        CurrentWave = 0;
        EnemiesAlive = 0;

        // Initial start (small delay to let clients load)
        GetWorld()->GetTimerManager().SetTimer(WaveDelayTimerHandle, this, &AWaveManager::StartNextWave, TimeBetweenWaves, false);
    	
    	if (HealthPickupClass)
    	{
    		GetWorld()->GetTimerManager().SetTimer(HealthSpawnTimerHandle, this, &AWaveManager::SpawnHealthPickup, HealthSpawnInterval, true);
    	}
    }
}

void AWaveManager::StartNextWave()
{
    CurrentWave++;
    // Calculate new total number of enemies to spawn. Base number scales up with wave multiplier.
    EnemiesToSpawnThisWave = FMath::RoundToInt(BaseEnemiesPerWave * FMath::Pow(WaveMultiplier, CurrentWave - 1));

    EnemiesAlive += EnemiesToSpawnThisWave;

    // Start spawning enemies!
    SpawnNextEnemy();
}

void AWaveManager::SpawnNextEnemy()
{
    if (EnemiesToSpawnThisWave <= 0) return;

    if (EnemyClass && SpawnPoints.Num() > 0)
    {
        // Pick random spawn point
        int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
        AActor* SpawnPoint = SpawnPoints[RandomIndex];

        if (SpawnPoint)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

            AAIEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AAIEnemy>(EnemyClass, SpawnPoint->GetActorLocation(), SpawnPoint->GetActorRotation(), SpawnParams);

            if (SpawnedEnemy)
            {
                // Subscribe to death event smoothly!
                SpawnedEnemy->OnEnemyDiedDelegate.AddDynamic(this, &AWaveManager::OnEnemyDied);
                EnemiesToSpawnThisWave--;
            }
        }
    }

    // Schedule the next enemy spawn if there are still enemies queuing up.
    if (EnemiesToSpawnThisWave > 0)
    {
        float NextSpawnDelay = FMath::RandRange(MinSpawnDelay, MaxSpawnDelay);
        GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AWaveManager::SpawnNextEnemy, NextSpawnDelay, false);
    }
}

void AWaveManager::OnEnemyDied(AAIEnemy* DeadEnemy)
{
    if (HasAuthority())
    {
    	if (DeadEnemy)
    	{
    		DeadEnemy->OnEnemyDiedDelegate.RemoveDynamic(this, &AWaveManager::OnEnemyDied);
    	}

        EnemiesAlive--;

        // If no one is alive, and we aren't currently waiting to spawn any more enemies
        if (EnemiesAlive <= 0 && EnemiesToSpawnThisWave <= 0)
        {
            if (CurrentWave >= lastWave)
            {
                // We won the game!
                OnGameWonDelegate.Broadcast();
            }
            else
            {
                // Give the players a breathing window, then start the next wave seamlessly!
                GetWorld()->GetTimerManager().SetTimer(WaveDelayTimerHandle, this, &AWaveManager::StartNextWave, TimeBetweenWaves, false);
            }
        }
    }
}

void AWaveManager::SpawnHealthPickup()
{
	if (HealthPickupClass)
	{
		UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
		if (NavSys)
		{
			FNavLocation RandomLocation;
			// Use the WaveManager's location as the center for the random point search
			if (NavSys->GetRandomReachablePointInRadius(GetActorLocation(), HealthSpawnRadius, RandomLocation))
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				
				// Spawn health pickup slightly above ground
				FVector SpawnLoc = RandomLocation.Location + FVector(0.0f, 0.0f, 50.0f);
				GetWorld()->SpawnActor<AHealthPickupInterface>(HealthPickupClass, SpawnLoc, FRotator::ZeroRotator, SpawnParams);
			}
		}
	}
}

void AWaveManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AWaveManager, CurrentWave);
    DOREPLIFETIME(AWaveManager, EnemiesAlive);
}
