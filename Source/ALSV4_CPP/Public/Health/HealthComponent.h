#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, CurrentHealth, float, HealthDelta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALSV4_CPP_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	// Max health editable in editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float MaxHealth = 100.0f;

protected:
	virtual void BeginPlay() override;

	// replicated current health, notify on clients
	UPROPERTY(ReplicatedUsing=OnRep_CurrentHealth)
	float CurrentHealth = 100.0f;

	// Called on clients when CurrentHealth is updated
	UFUNCTION()
	void OnRep_CurrentHealth();

	// Internal server-side application of damage
	void HandleTakeDamage(float DamageAmount, AController* InstigatedBy = nullptr, AActor* DamageCauser = nullptr);

public:
	// Apply damage — should be called on server only. BlueprintCallable for convenience.
	UFUNCTION(BlueprintCallable, Category="Health")
	void ApplyDamage(float DamageAmount, AController* InstigatedBy = nullptr, AActor* DamageCauser = nullptr);

	// Heal (optional) — server only
	UFUNCTION(BlueprintCallable, Category="Health")
	void Heal(float HealAmount);

	// Event to subscribe in Blueprints/UI
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnHealthChangedSignature OnHealthChanged;

	// Replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
