// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_EnemyDeath.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDUNREALCPP_API UBTT_EnemyDeath : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_EnemyDeath();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
