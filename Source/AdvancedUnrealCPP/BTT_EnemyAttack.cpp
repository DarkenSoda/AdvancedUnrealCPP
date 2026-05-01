// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EnemyAttack.h"
#include "AIController.h"
#include "AIEnemy.h"

UBTT_EnemyAttack::UBTT_EnemyAttack()
{
	NodeName = "Enemy Attack";
}

EBTNodeResult::Type UBTT_EnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		AAIEnemy* Enemy = Cast<AAIEnemy>(AIController->GetPawn());
		if (Enemy && AttackMontage)
		{
			Enemy->Multicast_PlayAttackMontage(AttackMontage);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}

