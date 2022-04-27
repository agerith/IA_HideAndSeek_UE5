// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "IAHideAndSeekHUD.generated.h"

UCLASS()
class AIAHideAndSeekHUD : public AHUD
{
	GENERATED_BODY()

public:
	AIAHideAndSeekHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* m_CrosshairTex;

};

