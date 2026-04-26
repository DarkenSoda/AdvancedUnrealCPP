#include "FindPatrolLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UFindPatrolLocation::UFindPatrolLocation()
{
    NodeName = "Find Patrol Location";
}

EBTNodeResult::Type UFindPatrolLocation::ExecuteTask(
    UBehaviorTreeComponent& OwnerComp,
    uint8* NodeMemory)
{
    APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();

    if (!Pawn) return EBTNodeResult::Failed;

    FVector Origin = Pawn->GetActorLocation();

    FNavLocation Location;

    UNavigationSystemV1* NavSys =
        UNavigationSystemV1::GetCurrent(GetWorld());

    if (NavSys && NavSys->GetRandomReachablePointInRadius(
        Origin,
        PatrolRadius,
        Location))
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(
            "PatrolLocation",
            Location.Location
        );

        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}