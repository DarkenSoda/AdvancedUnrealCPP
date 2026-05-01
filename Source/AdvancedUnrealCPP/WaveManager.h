#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveManager.generated.h"

UCLASS()
class ADVANCEDUNREALCPP_API AWaveManager : public AActor
{
    GENERATED_BODY()

public:	
    AWaveManager();

protected:
    virtual void BeginPlay() override;

public:	
    UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Wave System")
    int32 CurrentWave;

    UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Wave System")
    int32 EnemiesAlive;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave System")
    int32 lastWave = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave System")
    TSubclassOf<class AAIEnemy> EnemyClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave System")
    TArray<AActor*> SpawnPoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Pickups")
    TSubclassOf<class AHealthPickupInterface> HealthPickupClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Pickups")
    float HealthSpawnInterval = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Pickups")
    float HealthSpawnRadius = 1500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave System")
    int32 BaseEnemiesPerWave = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave System")
    float WaveMultiplier = 1.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave System")
    float MinSpawnDelay = 0.2f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave System")
    float MaxSpawnDelay = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave System")
    float TimeBetweenWaves = 5.0f;

    UFUNCTION(BlueprintCallable, Category = "Wave System")
    void OnEnemyDied(class AAIEnemy* DeadEnemy);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameWonSignature);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnGameWonSignature OnGameWonDelegate;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    int32 EnemiesToSpawnThisWave;

    FTimerHandle SpawnTimerHandle;
    FTimerHandle WaveDelayTimerHandle;
    FTimerHandle HealthSpawnTimerHandle;

    void StartNextWave();
    void SpawnNextEnemy();
    void SpawnHealthPickup();
};
