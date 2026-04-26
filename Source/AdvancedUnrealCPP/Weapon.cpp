#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"

AWeapon::AWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(Root);

	// set overlap in editor
}

void AWeapon::Equip()
{
    EquipState = EEquipState::Equipped;
}

void AWeapon::Unequip()
{
    EquipState = EEquipState::Unequipped;
}