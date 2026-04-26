#include "HealthPickupCast.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include <TopDownCharacter.h>

AHealthPickupCast::AHealthPickupCast()
{
	PrimaryActorTick.bCanEverTick = false;

    Collision = CreateDefaultSubobject<USphereComponent>("Collision");
    SetRootComponent(Collision);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    Mesh->SetupAttachment(RootComponent);

    Collision->OnComponentBeginOverlap.AddDynamic(this, &AHealthPickupCast::OnOverlap);
}

void AHealthPickupCast::OnOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    ATopDownCharacter* Player = Cast<ATopDownCharacter>(OtherActor);

    if (Player)
    {
        Player->MyHeal(HealAmount);
        Destroy();
    }
}