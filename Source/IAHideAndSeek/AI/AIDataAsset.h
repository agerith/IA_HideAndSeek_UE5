// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "AIDataAsset.generated.h"

class AAttackBase;

/**
 * 
 */
UCLASS(Blueprintable)
class IAHIDEANDSEEK_API UAIDataAsset : public UDataAsset
{
	GENERATED_BODY()
	UAIDataAsset();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1 - Setup", meta = (DisplayName = "Name AI"))
	FString m_Name;

	// Life start
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1 - Setup", meta = (DisplayName = "Life", ClampMin = "0.0"))
	float m_Life = 30.0f;

	// Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1 - Setup", meta = (DisplayName = "Speed", ClampMin = "0.0"))
	float m_Speed = 1.0f;
	
};
