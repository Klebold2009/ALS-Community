#include "WeaponHitscanComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "HealthComponent.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"

UWeaponHitscanComponent::UWeaponHitscanComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UWeaponHitscanComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UWeaponHitscanComponent::ServerFire_Validate(const FVector& TraceStart, const FVector& TraceEnd)
{
	// Could add validation (rate limiting, distance checks)
	return true;
}

void UWeaponHitscanComponent::ServerFire_Implementation(const FVector& TraceStart, const FVector& TraceEnd)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	Params.bReturnPhysicalMaterial = true;

	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params))
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			// Prefer HealthComponent if present
			if (UHealthComponent* HC = HitActor->FindComponentByClass<UHealthComponent>())
			{
				AController* InstigatorController = nullptr;
				if (APawn* Pawn = Cast<APawn>(GetOwner()))
				{
					InstigatorController = Pawn->GetController();
				}
				HC->ApplyDamage(Damage, InstigatorController, GetOwner());
			}
			else
			{
				UGameplayStatics::ApplyPointDamage(HitActor, Damage, (TraceEnd - TraceStart).GetSafeNormal(), Hit, nullptr, GetOwner(), nullptr);
			}

			// Play impact effect for all clients
			MulticastPlayImpactEffect(Hit.ImpactPoint, Hit.ImpactNormal);
		}
	}
	else
	{
		// Optional: replicate trace miss effects (tracer to TraceEnd)
	}
}

void UWeaponHitscanComponent::MulticastPlayImpactEffect_Implementation(const FVector& ImpactPoint, const FVector& ImpactNormal)
{
	if (ImpactEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, ImpactPoint, ImpactNormal.Rotation());
	}
	// Optionally: Draw debug impact
	// DrawDebugSphere(GetWorld(), ImpactPoint, 6.0f, 8, FColor::Red, false, 2.0f);
}
