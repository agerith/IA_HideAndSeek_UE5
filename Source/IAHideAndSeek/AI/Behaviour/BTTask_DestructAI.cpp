// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DestructAI.h"
#include "AIController.h"
#include <IAHideAndSeek/Toolbox/Toolbox.h>
#include <IAHideAndSeek/AI/AICharacter.h>
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"

UBTTask_DestructAI::UBTTask_DestructAI(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Destruct AI";
	bCreateNodeInstance = false;
}

EBTNodeResult::Type UBTTask_DestructAI::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (OwnerComp.GetAIOwner()->GetPawn() != nullptr)
	{
		OwnerComp.GetAIOwner()->GetPawn()->SetLifeSpan(m_LifeSpan);
		const APawn* Pawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
		if (Pawn)
		{
			AICharacter* AiCharacter = Cast<AICharacter>(OwnerComp.GetAIOwner()->GetPawn());
			if (AiCharacter)
			{
				AiCharacter->DestructAI();
			}
		}
		UToolbox::PrintLog("[AI] -> " + OwnerComp.GetAIOwner()->GetPawn()->GetName() + " marked for Destruction. (UBTTask_DestructAI::ExecuteTask)", false, true, ELog);
		return EBTNodeResult::InProgress;
	}

	UToolbox::PrintLog("[AI] -> BTTask called with invalidate Owner. (UBTTask_DestructAI::ExecuteTask)", true, true, EError, FLinearColor::Red);
	return EBTNodeResult::Failed;
}