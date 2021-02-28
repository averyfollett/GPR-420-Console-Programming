// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSMiniCubeActor.h"


#include "FPSCharacter.h"
#include "FPSProjectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSMiniCubeActor::AFPSMiniCubeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetStaticMesh(CubeMesh);
	MeshComp->SetSimulatePhysics(true);
	MeshComp->OnComponentHit.AddDynamic(this, &AFPSMiniCubeActor::OnHit);
	RootComponent = MeshComp;
}

// Called when the game starts or when spawned
void AFPSMiniCubeActor::BeginPlay()
{
	Super::BeginPlay();

	AFPSCharacter * Char = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Char->PlayerWarningDelegate.AddDynamic(this, &AFPSMiniCubeActor::PlayerCloseWarning);
}

void AFPSMiniCubeActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	//If this is hit by a special attack bullet
	if (OtherActor->ActorHasTag("Special"))
	{
		AFPSProjectile * HitProjectile = Cast<AFPSProjectile>(OtherActor);
		
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

			UGameplayStatics::SpawnEmitterAtLocation(this, Explosion, Overlap->GetActorLocation(),
                FRotator::ZeroRotator, FVector(HitProjectile->GetSpecialExplosionSize()));

			Overlap->Destroy();
		}

		UGameplayStatics::SpawnEmitterAtLocation(this, Explosion, GetActorLocation(),
                FRotator::ZeroRotator, FVector(HitProjectile->GetSpecialExplosionSize()));
	}
	else
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, Explosion, GetActorLocation());
	}

	Destroy();
}

void AFPSMiniCubeActor::PlayerCloseWarning(FVector PlayerLocation)
{
	//UE_LOG(LogTemp, Log, TEXT("Got Dynamic Delegate message!"));
	if (FVector::Dist(GetActorLocation(), PlayerLocation) < 1500.0f)
	{
		MeshComp->AddImpulse(FVector(0.0f, 0.0f, 500.0f), NAME_None, true);
	}
}

void AFPSMiniCubeActor::SetMaterial(UMaterialInterface* material) {
	MeshComp->SetMaterial(0, material);
}

// Called every frame
void AFPSMiniCubeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

