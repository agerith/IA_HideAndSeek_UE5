// Copyright Epic Games, Inc. All Rights Reserved.

#include "IAHideAndSeekCharacter.h"
#include <IAHideAndSeek/IAHideAndSeekProjectile.h>
#include <IAHideAndSeek/Characters/FirstPersonAnimInstance.h>
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AIAHideAndSeekCharacter

AIAHideAndSeekCharacter::AIAHideAndSeekCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	m_BaseTurnRate = 45.f;
	m_BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	m_FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	m_FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	m_FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	m_FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	m_Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	m_Mesh1P->SetOnlyOwnerSee(true);
	m_Mesh1P->SetupAttachment(m_FirstPersonCameraComponent);
	m_Mesh1P->bCastDynamicShadow = false;
	m_Mesh1P->CastShadow = false;
	m_Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	m_Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	m_FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	m_FP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	m_FP_Gun->bCastDynamicShadow = false;
	m_FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	m_FP_Gun->SetupAttachment(RootComponent);

	m_FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	m_FP_MuzzleLocation->SetupAttachment(m_FP_Gun);
	m_FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	m_GunOffset = FVector(100.0f, 0.0f, 10.0f);
}

void AIAHideAndSeekCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	m_FP_Gun->AttachToComponent(m_Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	m_Mesh1P->SetHiddenInGame(false, true);
}

void AIAHideAndSeekCharacter::Tick(float DeltaTime)
{
	// Call the base class 
	Super::Tick(DeltaTime);

	UFirstPersonAnimInstance* FP_AnimInstance = Cast<UFirstPersonAnimInstance>(m_Mesh1P->GetAnimInstance());
	if (FP_AnimInstance && m_FP_Gun)
	{
		FP_AnimInstance->m_LocationLeftHand = m_FP_Gun->GetSocketLocation("b_gun_pumpSocket");
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AIAHideAndSeekCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AIAHideAndSeekCharacter::OnFire);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AIAHideAndSeekCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AIAHideAndSeekCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AIAHideAndSeekCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AIAHideAndSeekCharacter::LookUpAtRate);
}

void AIAHideAndSeekCharacter::OnFire()
{
	// try and fire a attack
	if (m_AttackClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			const FRotator SpawnRotation = GetControlRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((m_FP_MuzzleLocation != nullptr) ? m_FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(m_GunOffset);

			FHitResult OutHit;
			FVector StartTrace = m_FirstPersonCameraComponent != nullptr ? m_FirstPersonCameraComponent->GetComponentLocation() : GetActorLocation();
			FVector EndTrace = StartTrace + (m_FP_MuzzleLocation->GetForwardVector() * 50000);
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(this);

			GetWorld()->LineTraceSingleByChannel(OutHit, StartTrace, EndTrace, ECC_Visibility, CollisionParams);
			FVector OutLocationTrace = OutHit.bBlockingHit == true ? OutHit.Location : EndTrace;

			FRotator RotationBullet =  UKismetMathLibrary::MakeRotFromX(OutLocationTrace - StartTrace);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// spawn the projectile at the muzzle
			World->SpawnActor<AAttackBase>(m_AttackClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}

	// try and play the sound if specified
	if (m_FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, m_FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (m_FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = m_Mesh1P->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(m_FireAnimation, 1.f);
		}
	}
}

void AIAHideAndSeekCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AIAHideAndSeekCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AIAHideAndSeekCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * m_BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AIAHideAndSeekCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * m_BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}