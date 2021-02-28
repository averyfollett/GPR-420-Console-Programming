// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FPSBombActor.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class AFPSProjectile;
class USoundBase;
class UAnimSequence;
class AFPSBombActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerWarningDelegate, FVector, PlayerLocation);

UCLASS()
class AFPSCharacter final : public ACharacter
{
	GENERATED_BODY()

protected:

	/** Pawn mesh: 1st person view  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh")
	USkeletalMeshComponent* Mesh1PComponent;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* GunMeshComponent;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;

public:
	AFPSCharacter();

	UFUNCTION()
	virtual void Tick(float DeltaSeconds) override;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<AFPSProjectile> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
    TSubclassOf<AFPSProjectile> SpecialProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category="Bombs")
	TSubclassOf<AFPSBombActor> BombClass;
	
	/** Sound to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	UAnimSequence* FireAnimation;

	UPROPERTY(BlueprintAssignable)
	FPlayerWarningDelegate PlayerWarningDelegate;

protected:
	
	/** Fires a projectile. */
	void Fire();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	bool bSpecialActive = false;

	bool bSpecialFireAvailable = true;

	void SetSpecialActive();

	void SetSpecialInactive();

	void SetSpecialAvailable();

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	FTimerHandle SpecialFireTimerHandle;

	float SpecialFireTime = 3.0f;
	float SpecialFireSize = 1.0f;

	//Handles Restarting a level
	void MyRestart();

	//Delgate restart function
	UFUNCTION()
	void DelegateRestart(float currentTime);

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1PComponent; }

	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return CameraComponent; }

};

