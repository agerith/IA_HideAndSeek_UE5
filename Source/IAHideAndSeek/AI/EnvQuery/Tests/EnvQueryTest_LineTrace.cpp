// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryTest_LineTrace.h"
#include "UObject/Package.h"
#include "CollisionQueryParams.h"
#include "WorldCollision.h"
#include "Engine/World.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"

#define LOCTEXT_NAMESPACE "EnvQueryGenerator"

UEnvQueryTest_LineTrace::UEnvQueryTest_LineTrace(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Cost = EEnvTestCost::High;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
	SetWorkOnFloatValues(false);

	m_Context = UEnvQueryContext_Querier::StaticClass();
	m_TraceData.SetGeometryOnly();
}

void UEnvQueryTest_LineTrace::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* DataOwner = QueryInstance.Owner.Get();
	BoolValue.BindData(DataOwner, QueryInstance.QueryID);
	m_TraceFromContext.BindData(DataOwner, QueryInstance.QueryID);
	m_ItemHeightOffset.BindData(DataOwner, QueryInstance.QueryID);
	m_ContextHeightOffset.BindData(DataOwner, QueryInstance.QueryID);

	bool bWantsHit = BoolValue.GetValue();
	bool bTraceToItem = m_TraceFromContext.GetValue();
	float ItemZ = m_ItemHeightOffset.GetValue();
	float ContextZ = m_ContextHeightOffset.GetValue();

	TArray<FVector> ContextLocations;
	if (!QueryInstance.PrepareContext(m_Context, ContextLocations))
	{
		return;
	}

	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(EnvQueryTrace), m_TraceData.bTraceComplex);

	TArray<AActor*> IgnoredActors;
	if (QueryInstance.PrepareContext(m_Context, IgnoredActors))
	{
		TraceParams.AddIgnoredActors(IgnoredActors);
	}

	ECollisionChannel TraceCollisionChannel = UEngineTypes::ConvertToCollisionChannel(m_TraceData.TraceChannel);
	FVector TraceExtent(m_TraceData.ExtentX, m_TraceData.ExtentY, m_TraceData.ExtentZ);
	FRunTraceSignature TraceFunc;
	if (m_TraceData.TraceShape != EEnvTraceShape::Line)
	{
		return;
	}
	else
	{
		TraceFunc.BindUObject(const_cast<UEnvQueryTest_LineTrace*>(this), bTraceToItem ? &UEnvQueryTest_LineTrace::RunLineTraceTo : &UEnvQueryTest_LineTrace::RunLineTraceFrom);
	}

	for (int32 ContextIndex = 0; ContextIndex < ContextLocations.Num(); ContextIndex++)
	{
		ContextLocations[ContextIndex].Z += ContextZ;
	}

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex()) + FVector(0, 0, ItemZ);
		AActor* ItemActor = GetItemActor(QueryInstance, It.GetIndex());

		for (int32 ContextIndex = 0; ContextIndex < ContextLocations.Num(); ContextIndex++)
		{
			const bool bHit = TraceFunc.Execute(ItemLocation, ContextLocations[ContextIndex], ItemActor, QueryInstance.World, TraceCollisionChannel, TraceParams, TraceExtent);
			It.SetScore(TestPurpose, FilterType, bHit, bWantsHit);
		}
	}
}

void UEnvQueryTest_LineTrace::PostLoad()
{
	Super::PostLoad();
	m_TraceData.OnPostLoad();
}

FText UEnvQueryTest_LineTrace::GetDescriptionTitle() const
{
	UEnum* ChannelEnum = StaticEnum<ETraceTypeQuery>();
	FString ChannelDesc = ChannelEnum->GetDisplayNameTextByValue(m_TraceData.TraceChannel).ToString();

	FString DirectionDesc = m_TraceFromContext.IsDynamic() ?
		FString::Printf(TEXT("%s, direction: %s"), *UEnvQueryTypes::DescribeContext(m_Context).ToString(), *m_TraceFromContext.ToString()) :
		FString::Printf(TEXT("%s %s"), m_TraceFromContext.DefaultValue ? TEXT("from") : TEXT("to"), *UEnvQueryTypes::DescribeContext(m_Context).ToString());

	return FText::FromString(FString::Printf(TEXT("%s: %s on %s"),
		*Super::GetDescriptionTitle().ToString(), *DirectionDesc, *ChannelDesc));
}

FText UEnvQueryTest_LineTrace::GetDescriptionDetails() const
{
	return FText::Format(FText::FromString("{0}\n{1}"),
		m_TraceData.ToText(FEnvTraceData::Detailed), DescribeBoolTestParams("hit"));
}

bool UEnvQueryTest_LineTrace::RunLineTraceTo(const FVector& ItemPos, const FVector& ContextPos, AActor* ItemActor, UWorld* World, enum ECollisionChannel Channel, const FCollisionQueryParams& Params, const FVector& Extent)
{
	FCollisionQueryParams TraceParams(Params);
	TraceParams.AddIgnoredActor(ItemActor);

	const bool bHit = World->LineTraceTestByChannel(ContextPos, ItemPos, Channel, TraceParams);
	return bHit;
}

bool UEnvQueryTest_LineTrace::RunLineTraceFrom(const FVector& ItemPos, const FVector& ContextPos, AActor* ItemActor, UWorld* World, enum ECollisionChannel Channel, const FCollisionQueryParams& Params, const FVector& Extent)
{
	FCollisionQueryParams TraceParams(Params);
	TraceParams.AddIgnoredActor(ItemActor);

	const bool bHit = World->LineTraceTestByChannel(ItemPos, ContextPos, Channel, TraceParams);
	return bHit;
}

#undef LOCTEXT_NAMESPACE
