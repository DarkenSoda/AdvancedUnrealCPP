#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HealInterface.generated.h"

UINTERFACE(MinimalAPI)
class UHealInterface : public UInterface
{
	GENERATED_BODY()
};

class ADVANCEDUNREALCPP_API IHealInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
	void Heal(float Amount);
};
