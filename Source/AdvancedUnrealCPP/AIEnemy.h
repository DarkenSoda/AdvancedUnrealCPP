#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Damageable.h"
#include "AIEnemy.generated.h"

UCLASS()
class ADVANCEDUNREALCPP_API AAIEnemy : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:
	AAIEnemy();

protected:
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	virtual void TakeDamage(float DamageAmount) override;
};
