// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class AFPSProjectile : public AActor
{
	GENERATED_BODY()

public:
	void SetSpecialExplosionSize(const float Size)
	{
		SpecialExplosionSize = Size;
	}

	float GetSpecialExplosionSize() const
	{
		return SpecialExplosionSize;
	}

protected:
	/** Sphere collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Projectile")
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY()
	float SpecialExplosionSize;

public:
	AFPSProjectile();

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};

