#pragma once

#include "CoreMinimal.h"
#include "GameStructs.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TANKOGEDDON_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, DamageValue);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDie);
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health values")
    float MaxHealth = 10.f;

    UPROPERTY()
    float CurrentHealth;

public:
    UPROPERTY(BlueprintAssignable)
    FOnDie OnDie;

    UPROPERTY(BlueprintAssignable)
    FOnHealthChanged OnDamaged;

public:
    UHealthComponent();

    void TakeDamage(FDamageData DamageData);

    float GetHealth() const;

    float GetHealthState() const;

    void AddHealth(float AddiditionalHealthValue);

    virtual void BeginPlay() override;
};
