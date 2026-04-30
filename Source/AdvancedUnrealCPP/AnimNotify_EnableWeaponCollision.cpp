#include "AnimNotify_EnableWeaponCollision.h"
#include "TopDownCharacter.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_EnableWeaponCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);

    if (MeshComp && MeshComp->GetOwner())
    {
        ATopDownCharacter* Character = Cast<ATopDownCharacter>(MeshComp->GetOwner());
        if (Character)
        {
            Character->EnableWeaponCollision();
        }
    }
}
