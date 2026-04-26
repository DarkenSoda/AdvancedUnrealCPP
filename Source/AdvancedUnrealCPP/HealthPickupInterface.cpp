#include "HealthPickupInterface.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HealInterface.h"

AHealthPickupInterface::AHealthPickupInterface()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	//SetReplicateMovement(true); // not needed

	Collision = CreateDefaultSubobject<USphereComponent>("Collision");
	SetRootComponent(Collision);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AHealthPickupInterface::OnOverlap);
}

void AHealthPickupInterface::OnOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->Implements<UHealInterface>())
	{
		if (!HasAuthority()) return;

		IHealInterface::Execute_Heal(OtherActor, HealAmount);
		Multicast_PlayEffects();
		Destroy();
	}
}

void AHealthPickupInterface::Multicast_PlayEffects_Implementation()
{
	if (HealFX)
	{
		UNiagaraComponent* FX =
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				HealFX,
				GetActorLocation()
			);

		if (FX)
		{
			FX->SetAutoDestroy(false);

			FTimerHandle Timer;

			GetWorld()->GetTimerManager().SetTimer(
				Timer,
				[FX]()
				{
					if (FX)
						FX->DestroyComponent();
				},
				2.0f,
				false);
		}
	}

	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			PickupSound,
			GetActorLocation()
		);
	}
}