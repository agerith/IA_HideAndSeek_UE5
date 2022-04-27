// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/Generators/EnvQueryGenerator_ProjectedPoints.h"
#include "DataProviders/AIDataProvider.h"
#include "EnvQueryGenerator_OffsetGrid.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Points: Offset Grid"))
class IAHIDEANDSEEK_API UEnvQueryGenerator_OffsetGrid : public UEnvQueryGenerator_ProjectedPoints
{
	GENERATED_BODY()

	UEnvQueryGenerator_OffsetGrid();

	UPROPERTY(EditDefaultsOnly, Category = "Grid Parameters", DisplayName = "Offset Space")
	FAIDataProviderFloatValue  m_OffsetSpace;

	UPROPERTY(EditDefaultsOnly, Category = "Grid Parameters", DisplayName = "Grid Half Size")
	FAIDataProviderFloatValue  m_GridHalfSize;

	UPROPERTY(EditDefaultsOnly, Category = "Grid Parameters", DisplayName = "Space Between")
	FAIDataProviderFloatValue  m_SpaceBetween;

	UPROPERTY(EditDefaultsOnly, Category = Generator, DisplayName = "Generate Around")
	TSubclassOf<UEnvQueryContext> m_GenerateAround;


	virtual void GenerateItems(FEnvQueryInstance& QueryInstance) const override;


	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;
	
};
