// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <IAHideAndSeek/AI/AICharacter.h>
#include "Engine/TargetPoint.h"
#include "BehaviorTree/BehaviorTree.h"
#include "IAHideAndSeekGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScoreIncrement, int, CurrentScore);

UCLASS(minimalapi)
class AIAHideAndSeekGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay();

public:
	AIAHideAndSeekGameMode();

	UPROPERTY(Category = Setup, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", DisplayName = "Score"))
	int m_Score;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, DisplayName = "Target Location Spawn AI")
	TArray<AActor*> m_TargetLocationSpawnAI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (DisplayName = "Subclass Of AI"))
	TSubclassOf<AICharacter> m_AIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (DisplayName = "Subclass Of AI"))
	UBehaviorTree* m_BTAsset;

	UPROPERTY(BlueprintAssignable)
	FScoreIncrement OnScoreIncrement;

	void SpawnAI();
	APawn* SpawnAIController(FVector SpawnLocation, FRotator SpawnRotation);

	UFUNCTION(BlueprintCallable)
	void OnDetroyIA();

	void AddScore();

};



