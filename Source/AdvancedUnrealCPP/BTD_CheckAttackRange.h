// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTD_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDUNREALCPP_API UBTD_CheckAttackRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTD_CheckAttackRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, Category = "AI")
	struct FBlackboardKeySelector AttackRange;

	UPROPERTY(EditAnywhere, Category = "AI")
	struct FBlackboardKeySelector TargetKey;
};
