// Copyright Epic Games, Inc. All Rights Reserved.

#include "IAHideAndSeekGameMode.h"
#include <IAHideAndSeek/UI/IAHideAndSeekHUD.h>
#include <IAHideAndSeek/Characters/IAHideAndSeekCharacter.h>
#include <IAHideAndSeek/AI/AIControllerNPC.h>
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include <IAHideAndSeek/Toolbox/Toolbox.h>

void AIAHideAndSeekGameMode::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), m_TargetLocationSpawnAI);

	SpawnAI();
}

AIAHideAndSeekGameMode::AIAHideAndSeekGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AIAHideAndSeekHUD::StaticClass();
}

void AIAHideAndSeekGameMode::SpawnAI()
{
	UWorld* const World = GetWorld();
	if (World != nullptr && m_TargetLocationSpawnAI.Num() > 0)
	{
		UToolbox::ShuffleArray(m_TargetLocationSpawnAI);

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const FVector SpawnLocation = m_TargetLocationSpawnAI[0]->GetActorLocation();
		const FRotator SpawnRotation = m_TargetLocationSpawnAI[0]->GetActorRotation();

		AICharacter* AiCharacter = Cast<AICharacter>(SpawnAIController(SpawnLocation, SpawnRotation));
		if (AiCharacter)
		{
			AiCharacter->OnDestruct.AddUniqueDynamic(this, &AIAHideAndSeekGameMode::OnDetroyIA);
		}
	}
}

APawn* AIAHideAndSeekGameMode::SpawnAIController(FVector SpawnLocation, FRotator SpawnRotation)
{
	APawn* NewPawn = nullptr;

	UWorld* const World = GetWorld();
	if (World && m_AIClass)
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.Owner = GetOwner();
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		NewPawn = World->SpawnActor<APawn>(m_AIClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

		if (NewPawn != nullptr)
		{
			if (NewPawn->Controller == nullptr)
			{
				NewPawn->SpawnDefaultController();
			}
		}

		if (m_BTAsset != nullptr)
		{
			AAIController* AIController = Cast<AAIController>(NewPawn->Controller);

			if (AIController != nullptr)
			{
				AIController->RunBehaviorTree(m_BTAsset);
			}
		}
	}

	return NewPawn;
}

void AIAHideAndSeekGameMode::OnDetroyIA()
{
	AddScore();
	SpawnAI();
}

void AIAHideAndSeekGameMode::AddScore()
{
	m_Score++;
	OnScoreIncrement.Broadcast(m_Score);
}


