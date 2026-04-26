#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include <Perception/AISenseConfig_Sight.h>
#include "EnemyAIController.generated.h"

UCLASS()
class ADVANCEDUNREALCPP_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	class UBlackboardComponent* BlackboardComp;
	class UBehaviorTreeComponent* BehaviorTreeComp;

	UPROPERTY(VisibleAnywhere)
	UAIPerceptionComponent* PerceptionComp;

	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);
};
