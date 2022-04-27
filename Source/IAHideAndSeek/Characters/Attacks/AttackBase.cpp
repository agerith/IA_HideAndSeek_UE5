// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackBase.h"

#include "Components/ArrowComponent.h"
#include <IAHideAndSeek/Characters/Attacks/AttackDataAsset.h>

// Sets default values
AAttackBase::AAttackBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_ArrowComponent = CreateDefaultSubobject<UArrowComponent>(FName("ArrowComp"));
	if (m_ArrowComponent != nullptr)
	{
		SetRootComponent(m_ArrowComponent);
	}

	m_ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(FName("PS_Component"));
	if (m_ParticleSystemComponent)
	{
		m_ParticleSystemComponent->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void AAttackBase::BeginPlay()
{
	Super::BeginPlay();

	if (m_AttackDataAsset != nullptr)
	{
		SetLifeSpan(m_AttackDataAsset->m_LifeTime);
		m_Speed = m_AttackDataAsset->m_Speed;
		m_Radius = m_AttackDataAsset->m_Radius;
		m_ObjectsTypeForCollisionDamage = m_AttackDataAsset->m_ObjectsTypeForCollisionDamage;
		m_ObjectsTypeForCollisionNoDamage = m_AttackDataAsset->m_ObjectsTypeForCollisionNoDamage;
	}
	
}

// Called every frame
void AAttackBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	 
}

