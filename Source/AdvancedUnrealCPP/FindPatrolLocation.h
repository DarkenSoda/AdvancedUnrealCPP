#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FindPatrolLocation.generated.h"

UCLASS()
class ADVANCEDUNREALCPP_API UFindPatrolLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
    UFindPatrolLocation();

    virtual EBTNodeResult::Type ExecuteTask(
        UBehaviorTreeComponent& OwnerComp,
        uint8* NodeMemory) override;

    UPROPERTY(EditAnywhere)
    float PatrolRadius = 1000.f;
};
