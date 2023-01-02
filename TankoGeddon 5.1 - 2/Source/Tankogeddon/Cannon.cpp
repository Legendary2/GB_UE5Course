// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Projectile.h"
#include "DrawDebugHelpers.h"
#include "DamageTaker.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"

ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);

	ShootEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Shoot effect"));
	ShootEffect->SetupAttachment(ProjectileSpawnPoint);

	ShotAudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio effect"));
	ShotAudioEffect->SetupAttachment(ProjectileSpawnPoint);
}

void ACannon::Fire()
{
	if (Ammunition == 0)
	{
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Red, "Ammunition is empty");
		return;
	}
	if (!bReadyToFire)
	{
		return;
	}
	bReadyToFire = false;

	ShootEffect->ActivateSystem();
	ShotAudioEffect->Play();

	if (GetOwner() && GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		if (ShootForceEffect)
		{
			FForceFeedbackParameters ShootForceEffectParams;
			ShootForceEffectParams.bLooping = false;
			ShootForceEffectParams.Tag = "ShootForceEffectParams";
			GetWorld()->GetFirstPlayerController()->ClientPlayForceFeedback(ShootForceEffect, ShootForceEffectParams);
		}

		if (ShootShake)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(ShootShake);
		}
	}

	if (Type == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Fire - projectile");

		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		if (Projectile)
		{
			Projectile->SetInstigator(GetInstigator());
			Projectile->Start();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Special fire"));
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Fire - trace");

		FHitResult HitResult;
		FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
		TraceParams.bTraceComplex = true;
		TraceParams.bReturnPhysicalMaterial = false;

		FVector Start = ProjectileSpawnPoint->GetComponentLocation();
		FVector End = ProjectileSpawnPoint->GetForwardVector() * FireRange + Start;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, TraceParams))
		{
			DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red, false, 0.5f, 0, 5);
			if (IDamageTaker* DamageTaker = Cast<IDamageTaker>(HitResult.GetActor()))
			{
				AActor* MyInstigator = GetInstigator();
				if (HitResult.GetActor() != MyInstigator)
				{
					FDamageData DamageData;
					DamageData.DamageValue = FireDamage;
					DamageData.DamageMaker = this;
					DamageData.Instigator = MyInstigator;
					DamageTaker->TakeDamage(DamageData);
				}
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.5f, 0, 5);
		}
	}
	Ammunition--;
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
}
void ACannon::FireSpecial()
{
	if (Ammunition == 0)
	{
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Red, "Ammunition is empty");
		return;
	}
	if (!bReadyToFire)
	{
		return;
	}
	bReadyToFire = false;
	CurrentQueue = FireSpecialNumber;
	GetWorldTimerManager().SetTimer(QueueTimerHandle, this, &ACannon::Special, 1.0f / FireSpecialRate, true);
	Ammunition--;
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
}
void ACannon::Special()
{
	if (--CurrentQueue <= 0)
	{
		GetWorldTimerManager().ClearTimer(QueueTimerHandle);
	}
	if (Type == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Fire - projectile");

		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		if (Projectile)
		{
			Projectile->Start();
		}
		UE_LOG(LogTemp, Warning, TEXT("Special fire"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Special fire"));
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Fire - trace");

		FHitResult HitResult;
		FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
		TraceParams.bTraceComplex = true;
		TraceParams.bReturnPhysicalMaterial = false;

		FVector Start = ProjectileSpawnPoint->GetComponentLocation();
		FVector End = ProjectileSpawnPoint->GetForwardVector() * FireRange + Start;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, TraceParams))
		{
			DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red, false, 0.5f, 0, 5);
			DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red, false, 0.5f, 0, 5);
			if (IDamageTaker* DamageTaker = Cast<IDamageTaker>(HitResult.GetActor()))
			{
				AActor* MyInstigator = GetInstigator();
				if (HitResult.GetActor() != MyInstigator)
				{
					FDamageData DamageData;
					DamageData.DamageValue = FireDamage;
					DamageData.DamageMaker = this;
					DamageData.Instigator = MyInstigator;
					DamageTaker->TakeDamage(DamageData);
				}
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.5f, 0, 5);
		}
	}
}
bool ACannon::IsReadyToFire()
{
	return bReadyToFire;
}

void ACannon::Reload()
{
	bReadyToFire = true;
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();
}
void ACannon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
}
int ACannon::GetAmmunition()
{
	return Ammunition;
}
void ACannon::SetAmmunition(int SavedAmmunition)
{
	Ammunition = SavedAmmunition;
}