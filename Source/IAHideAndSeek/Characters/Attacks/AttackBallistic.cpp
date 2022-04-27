// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackBallistic.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include <IAHideAndSeek/AI/AICharacter.h>

AAttackBallistic::AAttackBallistic()
{
	m_CylinderMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("CylinderMesh"));
	if (m_CylinderMesh != nullptr)
	{
		m_CylinderMesh->SetupAttachment(RootComponent);
		m_CylinderMesh->SetRelativeTransform(FTransform(FRotator(90.0f, 0.0f, 0.0f), FVector(0.0f), FVector(1.0f)));
		m_CylinderMesh->SetEnableGravity(false);
		m_CylinderMesh->SetGenerateOverlapEvents(false);
		m_CylinderMesh->SetCollisionProfileName("NoCollision");
		//m_CylinderMesh->SetHiddenInGame(true);
	}
}

void AAttackBallistic::BeginPlay()
{
	Super::BeginPlay();

	m_StartPosition = GetActorLocation();

	if (m_AttackDataAsset)
	{
		FVector NewScale = FVector(m_AttackDataAsset->m_Radius / 90, m_AttackDataAsset->m_Radius / 90, m_AttackDataAsset->m_Speed / 100);
		m_CylinderMesh->SetRelativeScale3D(NewScale);
		FVector NewLocation = FVector(m_AttackDataAsset->m_Speed / 100, 0, 0);
		m_CylinderMesh->SetRelativeLocation(NewLocation);
	}
}

void AAttackBallistic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_bAlreadyHitSomething && m_ArrowComponent != nullptr)
	{
		const FVector StartPos = m_ArrowComponent->GetComponentLocation();
		const FVector EndPos = m_ArrowComponent->GetComponentLocation() + m_ArrowComponent->GetForwardVector() * GetDistanceForFrame(DeltaTime) * 100.0f;
		const TArray<AActor*> ActorToIgnore;
		FHitResult HitObjectDamage;
		FHitResult HitObjectNoDamage;
		bool HitSomethingDamage = UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(), StartPos, EndPos, m_Radius, 0.0f, m_ObjectsTypeForCollisionDamage, true, ActorToIgnore, EDrawDebugTrace::None, HitObjectDamage, true);
		bool HitSomethingNoDamage = UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(), StartPos, EndPos, m_Radius, 0.0f, m_ObjectsTypeForCollisionNoDamage, true, ActorToIgnore, EDrawDebugTrace::None, HitObjectNoDamage, true);
		if (HitSomethingDamage)
		{
			AActor* HitActor = HitObjectDamage.GetActor();
			if (HitActor != nullptr && m_AttackDataAsset != nullptr)
			{
				HitImpactNormal = HitObjectDamage.ImpactNormal;
				HitImpactLocation = HitObjectDamage.Location;

				AICharacter* AICharacterHit = Cast<AICharacter>(HitActor);
				if (AICharacterHit != nullptr)
				{
					UGameplayStatics::SpawnEmitterAtLocation(this, m_AttackDataAsset->m_PSHitObjectDamage, HitImpactLocation, UKismetMathLibrary::MakeRotFromX(HitImpactNormal), FVector(2.0f));
					UGameplayStatics::ApplyDamage(HitActor, m_AttackDataAsset->m_Damage, nullptr, this, nullptr);
				}
			}
			SetLifeSpan(0.01f);
			m_bAlreadyHitSomething = true;
		}
		else if (HitSomethingNoDamage)
		{
			if (m_AttackDataAsset->m_PSHitObjectNoDamage)
			{
				UGameplayStatics::SpawnEmitterAtLocation(this, m_AttackDataAsset->m_PSHitObjectNoDamage, HitObjectNoDamage.ImpactPoint, UKismetMathLibrary::MakeRotFromZ(HitObjectNoDamage.ImpactNormal), FVector(2.0f));
			}
			SetLifeSpan(0.01f);
			m_bAlreadyHitSomething = true;
		}
		else
		{
			AddActorWorldOffset(m_ArrowComponent->GetForwardVector() * GetDistanceForFrame(DeltaTime) * 100.0f);
			OnMoveBullet.Broadcast(m_StartPosition, GetActorLocation());
		}
	}
}