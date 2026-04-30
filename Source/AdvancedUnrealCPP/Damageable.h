#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

UINTERFACE(MinimalAPI)
class UDamageable : public UInterface
{
    GENERATED_BODY()
};

class ADVANCEDUNREALCPP_API IDamageable
{
    GENERATED_BODY()

public:
    virtual void TakeDamage(float DamageAmount) = 0;
};
