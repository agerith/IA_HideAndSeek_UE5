// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "AIControllerNPC.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
AICharacter::AICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// LifeComponent
	m_LifeComponent = CreateDefaultSubobject<ULifeComponent>(FName("LifeComp"));
	if (m_LifeComponent)
	{
		AddOwnedComponent(m_LifeComponent);
		m_LifeComponent->OnDeath.AddUniqueDynamic(this, &AICharacter::DestructAI);
	}
	else
	{
		UToolbox::PrintLog("[AI] -> The Life Component hasn't been created. (AICharacter::AICharacter)", true, true, EWarning, FLinearColor::Yellow);
	}
}

// Called when the game starts or when spawned
void AICharacter::BeginPlay()
{
	Super::BeginPlay();

	if (m_LifeComponent != nullptr && m_DataAsset != nullptr)
	{
		m_LifeComponent->SetLifeMax(m_DataAsset->m_Life);
		m_LifeComponent->SetLifeCurrent(m_LifeComponent->GetLifeMax());
	}
	else
	{
		UToolbox::PrintLog("[AI] -> The Life Component of " + this->GetName() + " hasn't been initialized. (AICharacter::BeginPlay)", true, true, EWarning, FLinearColor::Yellow);
	}

	if (GetCharacterMovement() != nullptr && m_DataAsset != nullptr)
	{
		GetCharacterMovement()->MaxWalkSpeed = m_DataAsset->m_Speed;
	}
	else
	{
		UToolbox::PrintLog("[AI] -> The Character Movement Component of " + this->GetName() + " hasn't been initialized. (AICharacter::BeginPlay)", true, true, EWarning, FLinearColor::Yellow);
	}
}

// Called every frame
void AICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AICharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (m_LifeComponent != nullptr)
	{
		if (!m_LifeComponent->GetAlreadyDead())
		{
			m_LifeComponent->ApplyDamageToLife(Damage, DamageCauser);
			UToolbox::PrintLog("[AI] -> " + this->GetName() + " take " + FString::SanitizeFloat(Damage) + " Damage. (AICharacter::TakeDamage)", false, true, EVerbose);

			// try and play a firing animation if specified
			if (m_HitAnimation != nullptr)
			{
				// Get the animation object for the arms mesh
				UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
				if (AnimInstance != nullptr)
				{
					AnimInstance->Montage_Play(m_HitAnimation, 1.f);
				}
			}
		}
		else
		{
			UToolbox::PrintLog("[AI] -> " + this->GetName() + " is already dead. (AICharacter::TakeDamage)", false, true, EVeryVerbose);
		}
	}
	else
	{
		UToolbox::PrintLog("[AI] -> The Life Component of " + this->GetName() + " hasn't been initialized. (AICharacter::TakeDamage)", true, true, EWarning, FLinearColor::Yellow);
	}

	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

void AICharacter::DestructAI()
{
	if (!m_bAlreadyDestruct)
	{
		m_bAlreadyDestruct = true;

		if (Cast<AAIControllerNPC>(GetController()) != nullptr && Cast<AAIControllerNPC>(GetController())->GetBlackboardComponent() != nullptr)
		{
			Cast<AAIControllerNPC>(GetController())->GetBlackboardComponent()->SetValueAsBool("MarkedForDestruction", true);
		}
		else
		{
			UToolbox::PrintLog("[AI] -> The Controller or the Blackboard of " + this->GetName() + " are invalids. (AICharacter::DestructAI)", true, true, EWarning, FLinearColor::Yellow);
		}

		OnDestruct.Broadcast();

		/*if (m_AudioEvent_Destroy != nullptr)
		{
			UAkGameplayStatics::PostEventAtLocation(m_AudioEvent_Destroy, GetRootComponent()->GetComponentLocation(), GetRootComponent()->GetComponentRotation(), "", GetWorld());
		}
		else
		{
			UToolbox::PrintLog("[AI] -> The AudioEvent for the Destruction of " + this->GetName() + " hasn't been initialized. (AAICharacterBase::BeginPlay)", true, true, EWarning, FLinearColor::Yellow);
		}*/
	}
}
