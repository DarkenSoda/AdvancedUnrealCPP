#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIEnemy.generated.h"

UCLASS()
class ADVANCEDUNREALCPP_API AAIEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AAIEnemy();

protected:
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree* BehaviorTree;

};
