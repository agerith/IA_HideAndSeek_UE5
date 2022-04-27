// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AttackBase.h"
#include "AttackBallistic.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMoveBulletDelegateSignature, FVector, StartPosition, FVector, CurrentPosition);

UCLASS()
class IAHIDEANDSEEK_API AAttackBallistic : public AAttackBase
{
	GENERATED_BODY()

	AAttackBallistic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(Category = Setup, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", DisplayName = "CylinderMesh"))
	UStaticMeshComponent* m_CylinderMesh;

public:

	UPROPERTY(BlueprintAssignable)
	FMoveBulletDelegateSignature OnMoveBullet;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	FVector m_StartPosition;
	bool m_bAlreadyHitSomething = false;
	
};
