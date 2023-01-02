#include "TankFactory.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tankogeddon.h"
#include "MapLoader.h"
#include "DestroyedFactory.h"
ATankFactory::ATankFactory()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComp;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Mesh"));
	BuildingMesh->SetupAttachment(SceneComp);

	DestroyedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Destroyed Mesh"));
	DestroyedMesh->SetupAttachment(SceneComp);
	DestroyedMesh->SetVisibility(false);

	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	TankSpawnPoint->AttachToComponent(SceneComp, FAttachmentTransformRules::KeepRelativeTransform);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(SceneComp);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDie.AddDynamic(this, &ATankFactory::Die);
	HealthComponent->OnDamaged.AddDynamic(this, &ATankFactory::DamageTaked);
}

void ATankFactory::BeginPlay()
{
	Super::BeginPlay();

	BuildingMesh->SetVisibility(true);
	DestroyedMesh->SetVisibility(false);

	if (LinkedMapLoader)
	{
		LinkedMapLoader->SetIsActivated(false);
	}

	FTimerHandle TargetingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TargetingTimerHandle, this, &ATankFactory::SpawnNewTank, SpawnTankRate, true);
}

void ATankFactory::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void ATankFactory::Die()
{
	if (LinkedMapLoader)
	{
		LinkedMapLoader->SetIsActivated(true);
	}

	BuildingMesh->SetVisibility(false);
	DestroyedMesh->SetVisibility(true);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestuctionParticleSystem, GetActorTransform());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestructionSound, GetActorLocation());
}

void ATankFactory::DamageTaked(float DamageValue)
{
	UE_LOG(LogTankogeddon, Warning, TEXT("Factory %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetHealth());
}

void ATankFactory::SpawnNewTank()
{
	FTransform SpawnTransform(TankSpawnPoint->GetComponentRotation(), TankSpawnPoint->GetComponentLocation(), FVector(1));
	ATankPawn* NewTank = GetWorld()->SpawnActorDeferred<ATankPawn>(SpawnTankClass, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	NewTank->SetPatrollingPoints(TankWayPoints);
	
	UGameplayStatics::FinishSpawningActor(NewTank, SpawnTransform);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnParticleSystem, SpawnTransform);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SpawnSound, GetActorLocation());
}
