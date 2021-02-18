// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCubeActor.h"

#include "FPSMiniCubeActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSCubeActor::AFPSCubeActor()
{
	PrimaryActorTick.bCanEverTick = true;

	CubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;

	//Set up a mesh for this object that responds to physics and reacts to hits
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetStaticMesh(CubeMesh);
	MeshComp->SetSimulatePhysics(true);
	MeshComp->OnComponentHit.AddDynamic(this, &AFPSCubeActor::OnHit);
    RootComponent = MeshComp;
}

// Called when the game starts or when spawned
void AFPSCubeActor::BeginPlay()
{
	Super::BeginPlay();

}

void AFPSCubeActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	//If this is hit by a special attack bullet
	if (OtherActor->ActorHasTag("Special"))
	{
		//Get objects within a given radius of the impacted cube and destroy them
		TArray<FOverlapResult> OutOverlaps;
	
		FCollisionObjectQueryParams QueryParams;
		QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		QueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	
		FCollisionShape CollShape;
		CollShape.SetSphere(500.0f); 
	
		GetWorld()->OverlapMultiByObjectType(OutOverlaps, GetActorLocation(), FQuat::Identity, QueryParams, CollShape);

		//Destroy nearby objects
		for (FOverlapResult Result : OutOverlaps)
		{
			AActor* Overlap = Result.GetActor();

			Overlap->Destroy();
		}
	}
	else
	{
		const FVector Location = GetActorLocation();
		const FRotator Rotation = GetActorRotation();
		const FActorSpawnParameters SpawnParameters;

		// GetWorld()->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
		GetWorld()->SpawnActor<AFPSMiniCubeActor>(MiniCubeClass, Location + FVector(1.0f, 0.0f, 0.0f), Rotation, SpawnParameters);
		GetWorld()->SpawnActor<AFPSMiniCubeActor>(MiniCubeClass, Location + FVector(-1.0f, 0.0f, 0.0f), Rotation, SpawnParameters);
		GetWorld()->SpawnActor<AFPSMiniCubeActor>(MiniCubeClass, Location + FVector(0.0f, 1.0f, 0.0f), Rotation, SpawnParameters);
		GetWorld()->SpawnActor<AFPSMiniCubeActor>(MiniCubeClass, Location + FVector(0.0f, -1.0f, 0.0f), Rotation, SpawnParameters);
	}
	
	Destroy();
}

// Called every frame
void AFPSCubeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

