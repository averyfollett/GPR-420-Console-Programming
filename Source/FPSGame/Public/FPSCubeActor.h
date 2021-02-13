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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
    UStaticMeshComponent* MeshComp;

	UPROPERTY()
	UStaticMesh* CubeMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AFPSMiniCubeActor> MiniCubeActor;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
