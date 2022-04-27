// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryTest_Distance3D.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"

#define ENVQUERYTEST_DISTANCE_NAN_DETECTION 1

namespace
{
	FORCEINLINE float CalcDistance3D(const FVector& PosA, const FVector& PosB)
	{
		return (PosB - PosA).Size();
	}

	FORCEINLINE void CheckItemLocationForNaN(const FVector& ItemLocation, UObject* QueryOwner, int32 Index)
	{
#if ENVQUERYTEST_DISTANCE_NAN_DETECTION
		ensureMsgf(!ItemLocation.ContainsNaN(), TEXT("EnvQueryTest_Distance NaN in ItemLocation with owner %s. X=%f,Y=%f,Z=%f. Index:%d"), *GetPathNameSafe(QueryOwner), ItemLocation.X, ItemLocation.Y, ItemLocation.Z, Index);
#endif
	}

	FORCEINLINE void CheckContextLocationForNaN(const FVector& ContextLocation, UObject* QueryOwner, int32 Index)
	{
#if ENVQUERYTEST_DISTANCE_NAN_DETECTION
		ensureMsgf(!ContextLocation.ContainsNaN(), TEXT("EnvQueryTest_Distance NaN in ContextLocations with owner %s. X=%f,Y=%f,Z=%f. Index:%d"), *GetPathNameSafe(QueryOwner), ContextLocation.X, ContextLocation.Y, ContextLocation.Z, Index);
#endif
	}
}

UEnvQueryTest_Distance3D::UEnvQueryTest_Distance3D(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	m_DistanceTo = UEnvQueryContext_Querier::StaticClass();
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
}

void UEnvQueryTest_Distance3D::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();
	if (QueryOwner == nullptr)
	{
		return;
	}

	FloatValueMin.BindData(QueryOwner, QueryInstance.QueryID);
	float MinThresholdValue = FloatValueMin.GetValue();

	FloatValueMax.BindData(QueryOwner, QueryInstance.QueryID);
	float MaxThresholdValue = FloatValueMax.GetValue();

	// don't support context Item here, it doesn't make any sense
	TArray<FVector> ContextLocations;
	if (!QueryInstance.PrepareContext(m_DistanceTo, ContextLocations))
	{
		return;
	}

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());
		CheckItemLocationForNaN(ItemLocation, QueryOwner, It.GetIndex());
		for (int32 ContextIndex = 0; ContextIndex < ContextLocations.Num(); ContextIndex++)
		{
			CheckContextLocationForNaN(ContextLocations[ContextIndex], QueryOwner, ContextIndex);
			const float Distance = CalcDistance3D(ItemLocation, ContextLocations[ContextIndex]);
			It.SetScore(TestPurpose, FilterType, Distance, MinThresholdValue, MaxThresholdValue);
		}
	}
}

FText UEnvQueryTest_Distance3D::GetDescriptionTitle() const
{
	return FText::FromString(FString::Printf(TEXT("%s: to %s"),
		*Super::GetDescriptionTitle().ToString(),
		*UEnvQueryTypes::DescribeContext(m_DistanceTo).ToString()));
}

FText UEnvQueryTest_Distance3D::GetDescriptionDetails() const
{
	return DescribeFloatTestParams();
}
