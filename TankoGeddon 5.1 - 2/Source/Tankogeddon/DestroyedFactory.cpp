// Fill out your copyright notice in the Description page of Project Settings.


#include "DestroyedFactory.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
// Sets default values
ADestroyedFactory::ADestroyedFactory()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComp;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Mesh"));
	BuildingMesh->SetupAttachment(SceneComp);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(SceneComp);

}

// Called when the game starts or when spawned
void ADestroyedFactory::BeginPlay()
{
	Super::BeginPlay();
}