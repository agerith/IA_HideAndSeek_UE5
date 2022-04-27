// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BTTask_DestructAI.generated.h"

/**
 * 
 */
UCLASS()
class IAHIDEANDSEEK_API UBTTask_DestructAI : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:

	// Life Span to set
	UPROPERTY(Category = Setup, EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "LifeSpan"))
	float m_LifeSpan = 1.0f;
	
};
