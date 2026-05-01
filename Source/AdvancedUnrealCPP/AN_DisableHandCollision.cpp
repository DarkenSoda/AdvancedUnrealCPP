#include "AN_DisableHandCollision.h"
#include "AIEnemy.h"
#include "Components/SkeletalMeshComponent.h"

void UAN_DisableHandCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);

    if (MeshComp && MeshComp->GetOwner())
    {
        AAIEnemy* Enemy = Cast<AAIEnemy>(MeshComp->GetOwner());
        if (Enemy)
        {
            Enemy->DisableHandCollision();
        }
    }
}
