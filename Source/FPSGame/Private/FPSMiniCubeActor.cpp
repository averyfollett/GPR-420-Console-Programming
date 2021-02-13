// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSMiniCubeActor.h"
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
	
}

void AFPSMiniCubeActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::SpawnEmitterAtLocation(this, Explosion, GetActorLocation());
	
	Destroy();
}

// Called every frame
void AFPSMiniCubeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

