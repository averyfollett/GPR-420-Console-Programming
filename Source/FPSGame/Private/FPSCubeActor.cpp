// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCubeActor.h"

#include "FPSMiniCubeActor.h"

// Sets default values
AFPSCubeActor::AFPSCubeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;

	CubeMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/Environment/M_Cube.M_Cube'")).Object;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetStaticMesh(CubeMesh);
	MeshComp->SetMaterial(0, CubeMaterial);
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
	const FVector Location = GetActorLocation();
	const FRotator Rotation = GetActorRotation();
	const FActorSpawnParameters SpawnParameters;
	GetWorld()->SpawnActor<AFPSMiniCubeActor>(Location + FVector(1.0f, 0.0f, 0.0f), Rotation, SpawnParameters);
	GetWorld()->SpawnActor<AFPSMiniCubeActor>(Location + FVector(-1.0f, 0.0f, 0.0f), Rotation, SpawnParameters);
	GetWorld()->SpawnActor<AFPSMiniCubeActor>(Location + FVector(0.0f, 1.0f, 0.0f), Rotation, SpawnParameters);
	GetWorld()->SpawnActor<AFPSMiniCubeActor>(Location + FVector(0.0f, -1.0f, 0.0f), Rotation, SpawnParameters);
	
	Destroy();
}

// Called every frame
void AFPSCubeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

