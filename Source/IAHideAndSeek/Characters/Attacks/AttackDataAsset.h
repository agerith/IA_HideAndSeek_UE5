// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AttackDataAsset.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class IAHIDEANDSEEK_API UAttackDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	// Objects Channel for Collision Damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup, meta = (DisplayName = "ObjectsTypeForCollisionDamage"))
	TArray<TEnumAsByte<EObjectTypeQuery>> m_ObjectsTypeForCollisionDamage;

	// Objects Channel for Collision No Damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup, meta = (DisplayName = "ObjectsTypeForCollisionNoDamage"))
	TArray<TEnumAsByte<EObjectTypeQuery>> m_ObjectsTypeForCollisionNoDamage;

	// Particle System spawn on hit ObjectsTypeForCollisionNoDamage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup, meta = (DisplayName = "PS HitObjectNoDamage"))
	class UParticleSystem* m_PSHitObjectNoDamage;

	// Particle System spawn on hit ObjectsTypeForCollisionDamage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup, meta = (DisplayName = "PS HitObjectDamage"))
	class UParticleSystem* m_PSHitObjectDamage;

	// LifeTime of the ATK
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup, meta = (DisplayName = "LifeTime", ClampMin = "0.0"))
	float m_LifeTime = 1.0f;

	// Speed of the ATK in m/s
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup, meta = (DisplayName = "Speed", ClampMin = "0.0"))
	float m_Speed = 1.0f;

	// Radius of the ATK
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup, meta = (DisplayName = "Radius", ClampMin = "0.0"))
	float m_Radius = 1.0f;

	// Damage of the ATK
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup, meta = (DisplayName = "Damage", ClampMin = "0.0"))
	float m_Damage = 10.0f;

};
