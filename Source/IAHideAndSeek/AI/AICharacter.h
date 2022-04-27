// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <IAHideAndSeek/ActorComponents/LifeComponent.h>
#include <IAHideAndSeek/IAHideAndSeek.h>
#include "AIDataAsset.h"
#include "AICharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAIDestroyed);

class UAnimMontage;

UCLASS()
class IAHIDEANDSEEK_API AICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AICharacter();

	// Actor Components
	UPROPERTY(Category = ActorComponents, EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "LifeComponent"))
	ULifeComponent* m_LifeComponent;

	// Delegate broadcast for the destruction of the AI
	UPROPERTY(BlueprintAssignable)
	FAIDestroyed OnDestruct;

protected:

	UPROPERTY(Category = Setup, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", DisplayName = "DataAsset"))
	UAIDataAsset* m_DataAsset;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* m_HitAnimation;

	bool m_bAlreadyDestruct = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Apply damage to this actor.
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Called when the OnDeath of the LifeComponent is trigger
	UFUNCTION()
	virtual void DestructAI();

};
