#include "AIEnemy.h"
#include "EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

AAIEnemy::AAIEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// smooth rotation when moving
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void AAIEnemy::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void AAIEnemy::TakeDamage(float DamageAmount)
{
	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0.0f)
	{
		Destroy(); // For now, just destroy on death

		// play death action in behavior tree
	}
}

