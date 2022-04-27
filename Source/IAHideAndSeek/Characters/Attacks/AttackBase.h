// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <IAHideAndSeek/Characters/Attacks/AttackDataAsset.h>
#include "Particles/ParticleSystemComponent.h"
#include "AttackBase.generated.h"

class UArrowComponent;

UCLASS()
class IAHIDEANDSEEK_API AAttackBase : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AAttackBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE UAttackDataAsset* GetDataAsset() { return m_AttackDataAsset; }
	FORCEINLINE void SetDataAsset(UAttackDataAsset* attackDataAsset) { m_AttackDataAsset = attackDataAsset; }
	FORCEINLINE float GetSpeed() { return m_Speed; }
	FORCEINLINE void SetSpeed(float Speed) { m_Speed = Speed; }
	FORCEINLINE void SetRadius(float Radius) { m_Radius = Radius; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetDistanceForFrame(float FrameTime)
	{
		return (FrameTime == 0.0f ? 0.0f : m_Speed * FrameTime);
	}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector GetHitImpactNormal() { return HitImpactNormal; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector GetHitImpactLocation() { return HitImpactLocation; }

protected:

	UPROPERTY(Category = Setup, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", DisplayName = "ArrowComponent"))
	UArrowComponent* m_ArrowComponent;

	UPROPERTY(Category = Setup, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", DisplayName = "AttackDataAsset"), meta = (ExposeOnSpawn = "true"))
	UAttackDataAsset* m_AttackDataAsset;

	UPROPERTY(Category = Setup, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", DisplayName = "PSComponent"))
	UParticleSystemComponent* m_ParticleSystemComponent;

	TArray<TEnumAsByte<EObjectTypeQuery>> m_ObjectsTypeForCollisionDamage;

	TArray<TEnumAsByte<EObjectTypeQuery>> m_ObjectsTypeForCollisionNoDamage;

	FVector HitImpactNormal = FVector(0.0f);

	FVector HitImpactLocation = FVector(0.0f);

	float m_Speed = 1.0f;

	float m_Radius = 1.0f;

};
