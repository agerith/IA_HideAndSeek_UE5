// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryContext_Player.h"
#include "GameFramework/Actor.h"
#include "AITypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

void UEnvQueryContext_Player::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	AActor* ResultingActor = Cast<AActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//AActor* ResultingActor = Cast<AActor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, ResultingActor);
}

