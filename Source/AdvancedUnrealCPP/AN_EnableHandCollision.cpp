#include "AN_EnableHandCollision.h"
#include "AIEnemy.h"
#include "Components/SkeletalMeshComponent.h"

void UAN_EnableHandCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);

    if (MeshComp && MeshComp->GetOwner())
    {
        AAIEnemy* Enemy = Cast<AAIEnemy>(MeshComp->GetOwner());
        if (Enemy)
        {
            Enemy->EnableHandCollision();
        }
    }
}
