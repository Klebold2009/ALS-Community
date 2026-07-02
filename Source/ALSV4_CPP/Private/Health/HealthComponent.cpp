#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		CurrentHealth = MaxHealth;
	}
}

void UHealthComponent::OnRep_CurrentHealth()
{
	// On clients: broadcast change (Delta not available here, pass 0)
	OnHealthChanged.Broadcast(CurrentHealth, 0.0f);
}

void UHealthComponent::ApplyDamage(float DamageAmount, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return; // only server

	if (DamageAmount <= 0.f) return;

	HandleTakeDamage(DamageAmount, InstigatedBy, DamageCauser);
}

void UHealthComponent::HandleTakeDamage(float DamageAmount, AController* InstigatedBy, AActor* DamageCauser)
{
	const float OldHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);
	const float Delta = CurrentHealth - OldHealth;

	// Notify server-side listeners (and will replicate to clients)
	OnHealthChanged.Broadcast(CurrentHealth, Delta);

	if (CurrentHealth <= 0.0f)
	{
		// Owner died — server-side death handling should be here or in Character class
		AActor* Owner = GetOwner();
		if (Owner)
		{
			// Example: disable collisions / input can be dispatched to owner character
			// (Keep logic minimal here — forward to owning character if needed)
		}
	}
}

void UHealthComponent::Heal(float HealAmount)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return; // only server

	if (HealAmount <= 0.f) return;

	const float OldHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.0f, MaxHealth);
	const float Delta = CurrentHealth - OldHealth;
	OnHealthChanged.Broadcast(CurrentHealth, Delta);
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}
