#include "TankAIController.h"
#include "TankPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

void ATankAIController::BeginPlay()
{
    Super::BeginPlay();
    Initalize();
}
void ATankAIController::Initalize()
{
    TankPawn = Cast<ATankPawn>(GetPawn());
    if (TankPawn)
    {
        PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
        PatrollingPoints = TankPawn->GetPatrollingPoints();
        MovementAccurency = TankPawn->GetMovementAccurency();
      
        CurrentPatrolPointIndex = PatrollingPoints.Num() == 0 ? INDEX_NONE : 0;
    }
}

void ATankAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!TankPawn)
    {
        Initalize();
    }
    if (!TankPawn)
    {
        return;
    }
    if (CurrentPatrolPointIndex == INDEX_NONE)
    {
        TankPawn->MoveForward(0.f);
        return;
    }
    TankPawn->MoveForward(1.f);

    float RotationValue = GetRotationgValue();
    TankPawn->RotateRight(RotationValue);

    Targeting();
}

float ATankAIController::GetRotationgValue()
{

    FVector CurrentPoint = PatrollingPoints[CurrentPatrolPointIndex];
    FVector PawnLocation = TankPawn->GetActorLocation();
    if (FVector::Distance(CurrentPoint, PawnLocation) <= MovementAccurency)
    {
        CurrentPatrolPointIndex++;
        if (CurrentPatrolPointIndex >= PatrollingPoints.Num())
            CurrentPatrolPointIndex = 0;
    }

    FVector moveDirection = CurrentPoint - PawnLocation;
    moveDirection.Normalize();
    FVector ForwardDirection = TankPawn->GetActorForwardVector();
    FVector RightDirection = TankPawn->GetActorRightVector();

    DrawDebugLine(GetWorld(), PawnLocation, CurrentPoint, FColor::Green, false, 0.1f, 0, 5);

    float ForwardAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ForwardDirection, moveDirection)));
    float RightAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(RightDirection, moveDirection)));

    float RotationValue = 0;
    if (ForwardAngle > 5.f)
    {
        RotationValue = 1.f;
    }
    if (RightAngle > 90.f)
    {
        RotationValue = -RotationValue;
    }

    return RotationValue;
}

void ATankAIController::Targeting()
{
    if (DetectCanFire())
    {
        Fire();
    }
    else
    {
        RotateToPlayer();
    }
}

void ATankAIController::RotateToPlayer()
{
    if (IsPlayerInRange())
    {
        TankPawn->RotateTurretTo(PlayerPawn->GetActorLocation());
    }
}

bool ATankAIController::IsPlayerInRange()
{
    return FVector::Distance(TankPawn->GetActorLocation(), PlayerPawn->GetActorLocation()) <= TargetingRange;
}

bool ATankAIController::DetectCanFire()
{
    if (!DetectPlayerVisibility())
    {
        return false;
    }

    FVector TargetingDir = TankPawn->GetTurretForwardVector();
    FVector DirToPlayer = PlayerPawn->GetActorLocation() - TankPawn->GetActorLocation();
    DirToPlayer.Normalize();
    float AimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(TargetingDir, DirToPlayer)));
    return AimAngle <= Accurency;
}

void ATankAIController::Fire()
{
    TankPawn->Fire();
}

bool ATankAIController::DetectPlayerVisibility()
{
    FVector PlayerPos = PlayerPawn->GetActorLocation();
    FVector EyesPos = TankPawn->GetEyesPosition();

    FHitResult HitResult;
    FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
    TraceParams.bTraceComplex = true;
    TraceParams.AddIgnoredActor(TankPawn);
    TraceParams.bReturnPhysicalMaterial = false;

    if (GetWorld()->LineTraceSingleByChannel(HitResult, EyesPos, PlayerPos, ECollisionChannel::ECC_Visibility, TraceParams))
    {
        
        if (HitResult.GetActor())
        {
            DrawDebugLine(GetWorld(), EyesPos, HitResult.Location, FColor::Cyan, false, 0.5f, 0, 10);
            return HitResult.GetActor() == PlayerPawn;
        }
    }
    DrawDebugLine(GetWorld(), EyesPos, PlayerPos, FColor::Cyan, false, 0.5f, 0, 10);
    return false;
}
