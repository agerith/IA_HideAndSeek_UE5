// Fill out your copyright notice in the Description page of Project Settings.

#include "LifeComponent.h"
#include <IAHideAndSeek/Toolbox/Toolbox.h>

// Sets default values for this component's properties
ULifeComponent::ULifeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}


// Called when the game starts
void ULifeComponent::BeginPlay()
{
	Super::BeginPlay();
	SetLifeCurrent(m_LifeMax);

	// If Max Life <= 0, call the Death() with a Warning
	if (m_LifeMax <= 0.0f && GetOwner() != nullptr)
	{
		UToolbox::PrintLog("[ActorComponent] -> LifeComponent initialize with 0 LifeMax -> Death is called for " + GetOwner()->GetName() + ". (ULifeComponent::BeginPlay)", true, true, EWarning, FLinearColor::Yellow);
		Death();
	}
}

bool ULifeComponent::ApplyDamageToLife(float Damage, AActor* Causer)
{
	bool returnValue = false;
	if (!m_bAlreadyDead)
	{
		m_LifeCurrent -= Damage;
		OnHit.Broadcast();
		if (m_LifeCurrent <= 0.0f)
		{
			Death();
		}
		returnValue = true;
	}
	return returnValue;
}

void ULifeComponent::ApplyLifeRegeneration(float Regeneration)
{
	if (!m_bAlreadyDead)
	{
		m_LifeCurrent += Regeneration;
		if (m_LifeCurrent > m_LifeMax)
		{
			m_LifeCurrent = m_LifeMax;
		}
		OnHit.Broadcast();
	}
}

void ULifeComponent::Death()
{
	if (!m_bAlreadyDead && GetOwner() != nullptr)
	{
		m_bAlreadyDead = true;
		OnDeath.Broadcast();
		OnDeathOwner.Broadcast(Cast<AActor>(GetOwner()));
	}
}

