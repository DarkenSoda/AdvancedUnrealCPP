#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthPickupCast.generated.h"

UCLASS()
class ADVANCEDUNREALCPP_API AHealthPickupCast : public AActor
{
	GENERATED_BODY()
	
public:	
	AHealthPickupCast();

protected:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere)
    class USphereComponent* Collision;

    UPROPERTY(EditAnywhere)
    float HealAmount = 25.f;

    UFUNCTION()
    void OnOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

};
