#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponHitscanComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALSV4_CPP_API UWeaponHitscanComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponHitscanComponent();

	// Damage per hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	float Damage = 25.0f;

	// Impact effect (optional) - Blueprint implementable or particle template
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects")
	UParticleSystem* ImpactEffect;

protected:
	virtual void BeginPlay() override;

public:
	// Client should call local fire visuals, then call ServerFire to apply damage on server.
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire(const FVector& TraceStart, const FVector& TraceEnd);

	// Multicast effect to show impact to all clients
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayImpactEffect(const FVector& ImpactPoint, const FVector& ImpactNormal);
};
