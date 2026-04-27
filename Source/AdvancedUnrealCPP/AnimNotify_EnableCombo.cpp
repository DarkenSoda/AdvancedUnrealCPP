#include "AnimNotify_EnableCombo.h"
#include "TopDownCharacter.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_EnableCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);

    if (MeshComp && MeshComp->GetOwner())
    {
        ATopDownCharacter* Character = Cast<ATopDownCharacter>(MeshComp->GetOwner());
        if (Character)
        {
            Character->EnableCombo();
        }
    }
}
