#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "HealthPickupInterface.generated.h"

UCLASS()
class ADVANCEDUNREALCPP_API AHealthPickupInterface : public AActor
{
	GENERATED_BODY()

public:
	AHealthPickupInterface();

	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraSystem* HealFX;
	UPROPERTY(EditAnywhere, Category = "Effects")
	class USoundBase* PickupSound;

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* Collision;

	UPROPERTY(EditAnywhere)
	float HealAmount = 25.f;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayEffects();

	UFUNCTION()
	void OnOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

};
