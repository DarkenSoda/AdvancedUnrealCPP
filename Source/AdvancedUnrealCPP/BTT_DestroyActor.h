// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_DestroyActor.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDUNREALCPP_API UBTT_DestroyActor : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_DestroyActor();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
