#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Damageable.h"
#include "TopDownCharacter.h"

AWeapon::AWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>("CollisionBox");
	CollisionBox->SetupAttachment(Mesh);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::EnableWeapon()
{
	HitActors.Empty();
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeapon::DisableWeapon()
{
	HitActors.Empty();
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetOwner() && OtherActor != this && OtherActor != GetAttachParentActor())
	{
		// Don't hit the weapon's wielder
		if ((GetInstigator() && OtherActor == GetInstigator()) || OtherActor == GetOwner() || OtherActor == GetAttachParentActor())
		{
			return;
		}

		if (!HitActors.Contains(OtherActor))
		{
			HitActors.Add(OtherActor);

			AActor* WeaponOwner = GetOwner();
			if (!WeaponOwner) WeaponOwner = GetAttachParentActor();
			
			ATopDownCharacter* Char = Cast<ATopDownCharacter>(WeaponOwner ? WeaponOwner : GetInstigator());
			if (Char)
			{
				if (Char->IsLocallyControlled())
				{
					Char->Server_DealDamage(OtherActor, WeaponData.BaseDamage);
				}
				// If not locally controlled, we rely on the client who owns this to send the hit.
			}
			else if (HasAuthority())
			{
				// Fallback for AI or objects wielding the weapon on the server
				IDamageable* DamageableActor = Cast<IDamageable>(OtherActor);
				if (DamageableActor)
				{
					DamageableActor->TakeDamage(WeaponData.BaseDamage);
				}
			}
		}
	}
}