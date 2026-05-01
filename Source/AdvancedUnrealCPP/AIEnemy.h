#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Damageable.h"
#include "AIEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDiedSignature, AAIEnemy*, DeadEnemy);

UCLASS()
class ADVANCEDUNREALCPP_API AAIEnemy : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:
	AAIEnemy();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEnemyDiedSignature OnEnemyDiedDelegate;

protected:
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	class UBoxComponent* RightHandCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	class UBoxComponent* LeftHandCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackDamage = 20.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange = 200.0f;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EnableHandCollision();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void DisableHandCollision();

	UFUNCTION()
	void OnHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	UPROPERTY(ReplicatedUsing = OnRep_IsDead, VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	bool IsDead = false;

	UFUNCTION()
	void OnRep_IsDead();

	virtual void TakeDamage(float DamageAmount) override;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayAttackMontage(UAnimMontage* Montage);

protected:
	UPROPERTY()
	TArray<AActor*> HitActors;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
