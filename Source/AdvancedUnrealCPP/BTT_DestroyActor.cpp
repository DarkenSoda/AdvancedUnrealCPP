// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_DestroyActor.h"
#include "AIController.h"
#include "GameFramework/Actor.h"

UBTT_DestroyActor::UBTT_DestroyActor()
{
	NodeName = "Destroy Actor";
}

EBTNodeResult::Type UBTT_DestroyActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		if (APawn* ControlledPawn = AIController->GetPawn())
		{
			ControlledPawn->Destroy();
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}

