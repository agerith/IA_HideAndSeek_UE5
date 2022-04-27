// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "GameFramework/DamageType.h"
#include "LifeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHitDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLifeDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLifeWithOwnerDelegateSignature, AActor*, OwnerWhoDied);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IAHIDEANDSEEK_API ULifeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULifeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called to apply damage to the Actor
	UFUNCTION(BlueprintCallable)
	virtual	bool ApplyDamageToLife(float Damage, AActor* Causer);

	// Called to apply damage to the Actor
	UFUNCTION(BlueprintCallable)
	virtual	void ApplyLifeRegeneration(float Regeneration);

	// Called when Current Life <= 0
	UFUNCTION(BlueprintCallable)
	void Death();

	// Delegate broadcast when the Owner receive a damage
	UPROPERTY(BlueprintAssignable)
	FOnHitDelegateSignature OnHit;

	// Delegate broadcast for the dead of the Owner
	UPROPERTY(BlueprintAssignable)
	FLifeDelegateSignature OnDeath;

	// Delegate broadcast for the dead of the Owner with the Owner
	UPROPERTY(BlueprintAssignable)
	FLifeWithOwnerDelegateSignature OnDeathOwner;

	FORCEINLINE void SetLifeMax(float Life) { m_LifeMax = Life; }
	FORCEINLINE float GetLifeMax() { return m_LifeMax; }
	FORCEINLINE void SetLifeCurrent(float Life) { m_LifeCurrent = Life; }
	FORCEINLINE float GetLifeCurrent() { return m_LifeCurrent; }
	FORCEINLINE void SetAlreadyDead(bool AlreadyDead) { m_bAlreadyDead = AlreadyDead; }
	FORCEINLINE bool GetAlreadyDead() { return m_bAlreadyDead; }

protected:
	// Life Max of the Actor
	UPROPERTY(Category = Setup, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = "0.0", DisplayName = "LifeMax"))
	float m_LifeMax = 100.0f;
	// Current Life of the Actor
	UPROPERTY(Category = Runtime, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", DisplayName = "LifeCurrent"))
	float m_LifeCurrent = 100.0f;
	// Check if it's already dead
	UPROPERTY()
	bool m_bAlreadyDead = false;
};
