// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Templates/SubclassOf.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_Distance3D.generated.h"

/**
 * 
 */
UCLASS()
class IAHIDEANDSEEK_API UEnvQueryTest_Distance3D : public UEnvQueryTest
{
	GENERATED_UCLASS_BODY()

	/** context */
	UPROPERTY(EditDefaultsOnly, Category = Distance)
	TSubclassOf<UEnvQueryContext> m_DistanceTo;

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;
	
};
