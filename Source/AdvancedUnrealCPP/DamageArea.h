#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "DamageArea.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class USoundBase;
class UParticleSystem;
class UPrimitiveComponent;

UCLASS()
class ADVANCEDUNREALCPP_API ADamageArea : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADamageArea();

protected:

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Collision;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	float Damage = 20.f;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraSystem* HitVFX;
	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* HitSound;

	UFUNCTION()
	void OnOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayEffects();

private:
	bool vfxSpawned = false;
};
