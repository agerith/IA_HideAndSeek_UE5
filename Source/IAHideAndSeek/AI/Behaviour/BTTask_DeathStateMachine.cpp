// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DeathStateMachine.h"
#include "AIController.h"
#include <IAHideAndSeek/Toolbox/Toolbox.h>
#include <IAHideAndSeek/AI/AICharacter.h>
#include <IAHideAndSeek/AI/AIAnimInstance.h>
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"

UBTTask_DeathStateMachine::UBTTask_DeathStateMachine(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Death State Machine";
	bCreateNodeInstance = false;
}

EBTNodeResult::Type UBTTask_DeathStateMachine::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (OwnerComp.GetAIOwner()->GetPawn() != nullptr)
	{
		const APawn* Pawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
		if (Pawn)
		{
			const AICharacter* AiCharacter = Cast<AICharacter>(OwnerComp.GetAIOwner()->GetPawn());
			if (AiCharacter)
			{
				if (AiCharacter->GetMesh())
				{
					UAIAnimInstance* AI_AnimInstance = Cast<UAIAnimInstance>(AiCharacter->GetMesh()->GetAnimInstance());
					if (AI_AnimInstance)
					{
						AI_AnimInstance->m_bIsDead = true;
						UToolbox::PrintLog("[AI] -> " + OwnerComp.GetAIOwner()->GetPawn()->GetName() + " marked for Destruction. (UBTTask_DestructAI::ExecuteTask)", false, true, ELog);
						return EBTNodeResult::Succeeded;
					}
				}
			}
		}
	}

	UToolbox::PrintLog("[AI] -> BTTask called with invalidate Owner. (UBTTask_DestructAI::ExecuteTask)", true, true, EError, FLinearColor::Red);
	return EBTNodeResult::Failed;
}

