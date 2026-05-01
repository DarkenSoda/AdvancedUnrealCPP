// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EnemyDeath.h"
#include "AIController.h"
#include "AIEnemy.h"

UBTT_EnemyDeath::UBTT_EnemyDeath()
{
	NodeName = "Enemy Death";
}

EBTNodeResult::Type UBTT_EnemyDeath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Succeeded;
}

