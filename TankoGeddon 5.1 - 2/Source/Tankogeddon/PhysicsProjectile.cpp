#include "PhysicsProjectile.h"
#include "PhysicsComponent.h"
#include <Particles/ParticleSystemComponent.h>
#include <DrawDebugHelpers.h>
#include <Kismet/KismetMathLibrary.h>
#include "DamageTaker.h"

APhysicsProjectile::APhysicsProjectile()
{
    PhysicsComponent = CreateDefaultSubobject<UPhysicsComponent>(TEXT("PhysicsComponent"));

    TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail effect"));
    TrailEffect->SetupAttachment(RootComponent);
}

void APhysicsProjectile::Start()
{
    MoveVector = GetActorForwardVector() * TrajectorySimulationSpeed;
    CurrentTrajectory = PhysicsComponent->GenerateTrajectory(GetActorLocation(), MoveVector, TrajectorySimulationMaxTime, TrajectorySimulationTimeStep, 0);
    if (bShowTrajectory)
    {
        for (FVector Position : CurrentTrajectory)
        {
            DrawDebugSphere(GetWorld(), Position, 5, 8, FColor::Purple, true, 1, 0, 2);
        }
    }

    TragectoryPointIndex = 0;
    Super::Start();
}


void APhysicsProjectile::Move()
{
    FVector CurrentMoveVector = CurrentTrajectory[TragectoryPointIndex] - GetActorLocation();
    CurrentMoveVector.Normalize();
    FVector NewLocation = GetActorLocation() + CurrentMoveVector * MoveSpeed * MoveRate;
    SetActorLocation(NewLocation);
    if (FVector::Distance(NewLocation, CurrentTrajectory[TragectoryPointIndex]) <= MoveAccurency)
    {
        TragectoryPointIndex++;
        if (TragectoryPointIndex >= CurrentTrajectory.Num())
        {
            Explode();
            Destroy();
        }
        else
        {
            FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTrajectory[TragectoryPointIndex]);
            SetActorRotation(NewRotation);
        }
    }
}