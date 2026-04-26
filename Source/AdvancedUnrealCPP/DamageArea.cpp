
#include "DamageArea.h"
#include "TopDownCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

// Sets default values
ADamageArea::ADamageArea()
{
	bReplicates = true;

	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	RootComponent = Collision;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);

	Collision->OnComponentBeginOverlap.AddDynamic(
		this,
		&ADamageArea::OnOverlap
	);
}

void ADamageArea::OnOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{

	if (!HasAuthority()) return;

	if (ATopDownCharacter* Character = Cast<ATopDownCharacter>(OtherActor))
	{
		Character->ApplyDamage(Damage);
		Multicast_PlayEffects();
	}
}

void ADamageArea::Multicast_PlayEffects_Implementation()
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}

	if (HitVFX && !vfxSpawned)
	{
		UNiagaraComponent* FX =
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				HitVFX,
				GetActorLocation()
			);

		if (FX)
		{
			FX->SetAutoDestroy(false);

			FTimerHandle Timer;

			GetWorld()->GetTimerManager().SetTimer(
				Timer,
				[FX, this]()
				{
					if (FX) {
						FX->DestroyComponent();
						vfxSpawned = false;
					}
				},
				2.0f,
				false);
		}
	}
}