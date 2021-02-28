// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParticleDefinitions.h"
#include "FPSMiniCubeActor.generated.h"

UCLASS()
class FPSGAME_API AFPSMiniCubeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSMiniCubeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY()
	UStaticMesh* CubeMesh;

	//Effect to be spawned when this object is destroyed
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* Explosion;

	//When hit destroy this object and spawn explosion
	UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
    void PlayerCloseWarning(FVector PlayerLocation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
