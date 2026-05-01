#include "EnemyAnimInstance.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIEnemy.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    Pawn = TryGetPawnOwner();
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (Pawn)
    {
        // Get velocity and calculate speed in the XY plane
        FVector Velocity = Pawn->GetVelocity();
        Velocity.Z = 0.0f; // Disregard Z-axis to get true horizontal ground speed
        Speed = Velocity.Size();
    }
}
