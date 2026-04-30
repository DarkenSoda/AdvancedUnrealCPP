#include "EnemyAIController.h"
#include "AIEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "TopDownCharacter.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FindClosestPlayer();
}

void AEnemyAIController::FindClosestPlayer()
{
	if (!GetBlackboardComponent() || !GetPawn()) return;

	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATopDownCharacter::StaticClass(), Players);

	AActor* ClosestPlayer = nullptr;
	float MinDistance = MAX_flt;

	for (AActor* Player : Players)
	{
		float Distance = FVector::Dist(GetPawn()->GetActorLocation(), Player->GetActorLocation());
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			ClosestPlayer = Player;
		}
	}

	if (ClosestPlayer)
	{
		GetBlackboardComponent()->SetValueAsObject("TargetActor", ClosestPlayer);
	}
	else
	{
		GetBlackboardComponent()->ClearValue("TargetActor");
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AAIEnemy* Enemy = Cast<AAIEnemy>(InPawn);
	if (Enemy && Enemy->BehaviorTree)
	{
		RunBehaviorTree(Enemy->BehaviorTree);
	}
}