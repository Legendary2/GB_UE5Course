#include "DestroyedFactory.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

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

void ADestroyedFactory::BeginPlay()
{
	Super::BeginPlay();
}