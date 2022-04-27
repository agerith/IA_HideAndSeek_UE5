// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AIAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class IAHIDEANDSEEK_API UAIAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, DisplayName = "Is Couch")
	bool m_bIsCouch = false;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, DisplayName = "Is Running")
	bool m_bIsRunning = false;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, DisplayName = "Speed")
	float m_Speed = false;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, DisplayName = "Is Dead")
	bool m_bIsDead = false;
	
};
