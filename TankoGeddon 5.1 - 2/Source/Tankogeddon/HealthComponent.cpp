#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentHealth = MaxHealth;
}

void UHealthComponent::TakeDamage(FDamageData DamageData)
{
	float TakedDamageValue = DamageData.DamageValue;
	CurrentHealth -= TakedDamageValue;

	if (CurrentHealth <= 0)
	{
		if (OnDie.IsBound())
		{
			OnDie.Broadcast();
		}
	}
	else
	{
		if (OnDamaged.IsBound())
		{
			OnDamaged.Broadcast(TakedDamageValue);
		}
	}
}

float UHealthComponent::GetHealth() const
{
	return CurrentHealth;
}

float UHealthComponent::GetHealthState() const
{
	return CurrentHealth / MaxHealth;
}

void UHealthComponent::AddHealth(float AddiditionalHealthValue)
{
	CurrentHealth += AddiditionalHealthValue;
	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}