// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSCharacter.h"

#include <string>

#include "FPSProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimSequence.h"

AFPSCharacter::AFPSCharacter()
{
	// Create a CameraComponent	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(0, 0, BaseEyeHeight)); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1PComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	Mesh1PComponent->SetupAttachment(CameraComponent);
	Mesh1PComponent->CastShadow = false;
	Mesh1PComponent->SetRelativeRotation(FRotator(2.0f, -15.0f, 5.0f));
	Mesh1PComponent->SetRelativeLocation(FVector(0, 0, -160.0f));

	// Create a gun mesh component
	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	GunMeshComponent->CastShadow = false;
	GunMeshComponent->SetupAttachment(Mesh1PComponent, "GripPoint");
}

void AFPSCharacter::Tick(float DeltaSeconds)
{
	if (bSpecialActive)
	{
		SpecialFireSize += DeltaSeconds;
		UE_LOG(LogTemp, Log, TEXT("Size: %f"), SpecialFireSize);
	}
}


void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::Jump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);
	PlayerInputComponent->BindAction("MyRestart", IE_Pressed, this, &AFPSCharacter::MyRestart);

	PlayerInputComponent->BindAction("ActivateProjectile", IE_Pressed, this, &AFPSCharacter::SetSpecialActive);
	PlayerInputComponent->BindAction("ActivateProjectile", IE_Released, this, &AFPSCharacter::SetSpecialInactive);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void AFPSCharacter::MyRestart() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Restart game! in 5 seconds"));
	FTimerDelegate TimerDel;

	FTimerHandle TimerHandle;
	float Time = GetWorld()->TimeSeconds;

	TimerDel.BindUFunction(this, FName("DelegateRestart"), Time);
	

	GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, 5.0f, false);

}

void AFPSCharacter::DelegateRestart(float currentTime) {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Time: %f"), currentTime));
	GetWorld()->GetFirstPlayerController()->RestartLevel();
}

void AFPSCharacter::Fire()
{
	// try and fire a projectile
	if (!bSpecialActive && ProjectileClass)
	{
		// Grabs location from the mesh that must have a socket called "Muzzle" in his skeleton
		const FVector MuzzleLocation = GunMeshComponent->GetSocketLocation("Muzzle");
		// Use controller rotation which is our view direction in first person
		const FRotator MuzzleRotation = GetControlRotation();

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// spawn the projectile at the muzzle
		GetWorld()->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
	}
	else if (bSpecialActive && SpecialProjectileClass)
	{
		// Grabs location from the mesh that must have a socket called "Muzzle" in his skeleton
		const FVector MuzzleLocation = GunMeshComponent->GetSocketLocation("Muzzle");
		// Use controller rotation which is our view direction in first person
		const FRotator MuzzleRotation = GetControlRotation();

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// spawn the projectile at the muzzle
		AFPSProjectile * Projectile = GetWorld()->SpawnActor<AFPSProjectile>(SpecialProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
		Projectile->SetSpecialExplosionSize(SpecialFireSize);

		SetSpecialInactive();
	}

	// try and play the sound if specified
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1PComponent->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->PlaySlotAnimationAsDynamicMontage(FireAnimation, "Arms", 0.0f);
		}
	}
}


void AFPSCharacter::MoveForward(const float Val)
{
	if (Val != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Val);
	}
}


void AFPSCharacter::MoveRight(const float Val)
{
	if (Val != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Val);
	}
}

void AFPSCharacter::Jump()
{
	ACharacter::Jump();
	PlayerWarningDelegate.Broadcast(GetActorLocation());
}

void AFPSCharacter::SetSpecialActive()
{
	if (bSpecialFireAvailable)
	{
		bSpecialActive = true;
		
		UE_LOG(LogTemp, Warning, TEXT("Special active and timer set"));
	}
}

void AFPSCharacter::SetSpecialInactive()
{
	//Set special attacks off. Called after effectr is used
	bSpecialActive = false;
	bSpecialFireAvailable = false;
	SpecialFireSize = 1.0f;
	GetWorld()->GetTimerManager().SetTimer(SpecialFireTimerHandle, this, &AFPSCharacter::SetSpecialAvailable, SpecialFireTime, false);

	UE_LOG(LogTemp, Warning, TEXT("Special not available"));
}

void AFPSCharacter::SetSpecialAvailable()
{
	bSpecialFireAvailable = true;
	GetWorld()->GetTimerManager().ClearTimer(SpecialFireTimerHandle);
	UE_LOG(LogTemp, Warning, TEXT("Special available and timer reset"));
}
