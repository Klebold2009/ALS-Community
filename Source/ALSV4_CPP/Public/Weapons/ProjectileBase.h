#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class ALSV4_CPP_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AProjectileBase();

protected:
	virtual void BeginPlay() override;

	// Collision
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	// Movement
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovement;

	// Damage amount
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	float Damage = 20.0f;

	// Life span
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	float MaxLifeSeconds = 5.0f;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	           FVector NormalImpulse, const FHitResult& Hit);

public:
	// Replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
