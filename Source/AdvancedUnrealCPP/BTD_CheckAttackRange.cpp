// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CheckAttackRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"

UBTD_CheckAttackRange::UBTD_CheckAttackRange()
{
	NodeName = "Check Attack Range";
}

bool UBTD_CheckAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (TargetActor && AIController && AIController->GetPawn())
	{
		float Distance = FVector::Dist(TargetActor->GetActorLocation(), AIController->GetPawn()->GetActorLocation());
		return Distance <= OwnerComp.GetBlackboardComponent()->GetValueAsFloat(AttackRange.SelectedKeyName);
	}

	return false;
}

