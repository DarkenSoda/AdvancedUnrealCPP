// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthPickupDelegate.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealPickup, float, Amount);

UCLASS()
class ADVANCEDUNREALCPP_API AHealthPickupDelegate : public AActor
{
	GENERATED_BODY()
	
public:	
	AHealthPickupDelegate();

	UPROPERTY(BlueprintAssignable)
	FOnHealPickup OnPickedUp;

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
