// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryGenerator_OffsetGrid.h"
#include "AI/Navigation/NavigationTypes.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"

#define LOCTEXT_NAMESPACE "EnvQueryGenerator"

UEnvQueryGenerator_OffsetGrid::UEnvQueryGenerator_OffsetGrid()
{
	m_GenerateAround = UEnvQueryContext_Querier::StaticClass();

	m_GridHalfSize.DefaultValue = 3000.0f;
	m_SpaceBetween.DefaultValue = 100.0f;
	m_OffsetSpace.DefaultValue = 200.0f;
}

void UEnvQueryGenerator_OffsetGrid::GenerateItems(FEnvQueryInstance& QueryInstance) const
{
	// Data Binding 
	UObject* BindOwner = QueryInstance.Owner.Get();
	m_GridHalfSize.BindData(BindOwner, QueryInstance.QueryID);
	m_SpaceBetween.BindData(BindOwner, QueryInstance.QueryID);
	m_OffsetSpace.BindData(BindOwner, QueryInstance.QueryID);

	float RadiusValue = m_GridHalfSize.GetValue();
	float DensityValue = m_SpaceBetween.GetValue();
	float OffsetValue = m_OffsetSpace.GetValue();

	// Get number of items per row and calculate the indexes ranges for the hole
	const int32 ItemsCount = FPlatformMath::TruncToInt((RadiusValue * 2.0 / DensityValue) + 1);
	const int32 ItemsCountHalf = ItemsCount / 2;
	const int32 LeftRangeIndex = ItemsCountHalf - FPlatformMath::TruncToInt(OffsetValue / DensityValue) - 1;
	const int32 RightRangeIndex = ItemsCountHalf + FPlatformMath::TruncToInt(OffsetValue / DensityValue) + 1;
	const int32 OffsetItemsCount = FPlatformMath::TruncToInt((ItemsCount * 2.0 / DensityValue) + 1);

	// Get locations for each context (we might have more that one context)
	TArray<FVector> ContextLocations;
	QueryInstance.PrepareContext(m_GenerateAround, ContextLocations);

	// Reserve the needed memory space of items for each context.
	// the total items count is calculated subtracting the items located into the hole from the total list of items. 
	TArray<FNavLocation> GridPoints;
	GridPoints.Reserve(((ItemsCount * ItemsCount) - (OffsetItemsCount * OffsetItemsCount)) * ContextLocations.Num());

	// Calculate position of each item
	for (int32 ContextIndex = 0; ContextIndex < ContextLocations.Num(); ContextIndex++) {
		for (int32 IndexX = 0; IndexX < ItemsCount; ++IndexX)
		{
			for (int32 IndexY = 0; IndexY < ItemsCount; ++IndexY)
			{
				// it the item is inside the hole ranges, just skip it.
				if ((IndexY > LeftRangeIndex && IndexY < RightRangeIndex) && (IndexX > LeftRangeIndex && IndexX < RightRangeIndex)) {
					continue;
				}
				// starting from the context location, define the location of the current item 
				// and add it to the gridPoints array.
				else {
					const FNavLocation TestPoint = FNavLocation(ContextLocations[ContextIndex] - FVector(DensityValue * (IndexX - ItemsCountHalf), DensityValue * (IndexY - ItemsCountHalf), 0));
					GridPoints.Add(TestPoint);
				}
			}
		}
	}

	// Project all the points, remove those outside the current navmesh and store the result.
	ProjectAndFilterNavPoints(GridPoints, QueryInstance);
	StoreNavPoints(GridPoints, QueryInstance);
}

FText UEnvQueryGenerator_OffsetGrid::GetDescriptionTitle() const
{
	return FText::Format(LOCTEXT("OffsetGridDescriptionGenerateAroundContext", "{0}: generate around {1}"),
		Super::GetDescriptionTitle(), UEnvQueryTypes::DescribeContext(m_GenerateAround));
};

FText UEnvQueryGenerator_OffsetGrid::GetDescriptionDetails() const
{
	FText Desc = FText::Format(LOCTEXT("GridOffseDescription", "radius: {0}, space between: {1}, offset:{2}"),
				 FText::FromString(m_GridHalfSize.ToString()), FText::FromString(m_SpaceBetween.ToString()), FText::FromString(m_OffsetSpace.ToString()));

	FText ProjDesc = ProjectionData.ToText(FEnvTraceData::Brief);
	if (!ProjDesc.IsEmpty())
	{
		FFormatNamedArguments ProjArgs;
		ProjArgs.Add(TEXT("Description"), Desc);
		ProjArgs.Add(TEXT("ProjectionDescription"), ProjDesc);
		Desc = FText::Format(LOCTEXT("OffsetGridDescriptionWithProjection", "{Description}, {ProjectionDescription}"), ProjArgs);
	}

	return Desc;
}

#undef LOCTEXT_NAMESPACE