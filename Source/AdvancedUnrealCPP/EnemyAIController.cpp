#include "EnemyAIController.h"
#include "AIEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"

AEnemyAIController::AEnemyAIController()
{
    PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>("Perception");

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("SightConfig");

    SightConfig->SightRadius = 1500.f;
    SightConfig->LoseSightRadius = 1700.f;
    SightConfig->PeripheralVisionAngleDegrees = 60.f;

    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

    PerceptionComp->ConfigureSense(*SightConfig);
    PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());

    PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(
        this,
        &AEnemyAIController::OnTargetDetected
    );
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    AAIEnemy* Enemy = Cast<AAIEnemy>(InPawn);
    if (Enemy && Enemy->BehaviorTree)
    {
        RunBehaviorTree(Enemy->BehaviorTree);
    }
}

void AEnemyAIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
    if (!GetBlackboardComponent()) return;

    if (Stimulus.WasSuccessfullySensed())
    {
        GetBlackboardComponent()->SetValueAsObject(
            "TargetActor",
            Actor
        );
    }
    else
    {
        GetBlackboardComponent()->ClearValue("TargetActor");
    }
}