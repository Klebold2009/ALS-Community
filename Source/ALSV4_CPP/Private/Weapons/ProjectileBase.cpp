#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	InitialLifeSpan = 10.0f;
	bReplicates = true;
	SetReplicateMovement(true);
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(MaxLifeSeconds);
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                            FVector NormalImpulse, const FHitResult& Hit)
{
	if (!HasAuthority()) return; // server only

	if (OtherActor && OtherActor != GetOwner())
	{
		// Apply damage via HealthComponent if present
		if (UHealthComponent* HC = OtherActor->FindComponentByClass<UHealthComponent>())
		{
			AController* InstigatorController = nullptr;
			if (APawn* Pawn = Cast<APawn>(GetOwner()))
			{
				InstigatorController = Pawn->GetController();
			}
			HC->ApplyDamage(Damage, InstigatorController, this);
		}
		else
		{
			UGameplayStatics::ApplyPointDamage(OtherActor, Damage, GetVelocity().GetSafeNormal(), Hit, nullptr, this, nullptr);
		}

		// Optionally spawn impact effect via multicast or replicate an event (not implemented here)
		Destroy();
	}
}

void AProjectileBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// nothing extra currently; add variables here if you replicate Damage etc.
}
