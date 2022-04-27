// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FirstPersonAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class IAHIDEANDSEEK_API UFirstPersonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, DisplayName = "Is Moving")
	bool m_IsMoving = false;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, DisplayName = "Is In Air")
	bool m_IsInAir = false;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, DisplayName = "Location Left Hand")
	FVector m_LocationLeftHand;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, DisplayName = "Joint Target Location")
	FVector m_JointTargetLocation = FVector(150.0f, -130.0f, -130.0f);
};
