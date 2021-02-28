// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FPSMiniCubeActor.h"
#include "GameFramework/Actor.h"
#include "FPSCubeActor.generated.h"

UCLASS()
class FPSGAME_API AFPSCubeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSCubeActor();

	void SetColor(UMaterialInterface * color);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
    UStaticMeshComponent* MeshComp;

	UPROPERTY()
	UStaticMesh* CubeMesh;

	UPROPERTY()
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AFPSMiniCubeActor> MiniCubeActor;

	//The smaller cube spawned when this dies is set in the inspector
	UPROPERTY(EditDefaultsOnly, Category = "Mini Cube")
	TSubclassOf<AFPSMiniCubeActor> MiniCubeClass;

	

	//Handle an impact. Responds by destroying this object and spawning four mini cubes
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//Effect to be spawned when object destroyed by special attack
	UPROPERTY(EditDefaultsOnly)
		UParticleSystem* Explosion;

	UFUNCTION()
	void PlayerCloseWarning(FVector PlayerLocation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};