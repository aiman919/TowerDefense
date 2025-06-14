#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

/**
 * @class AProjectile
 * @brief Simple projectile that travels in a straight line without gravity, dealing damage to enemies and the base.
 * 
 * Optimizations:
 * - Removed empty BeginPlay to avoid unnecessary virtual calls.
 * - Safe runtime use with null checks in InitProjectile.
 * - Inline getters available for testing and Blueprints.
 */
UCLASS()
class MYPROJECT_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	/** Default constructor. */
	AProjectile();

	/**
	 * @brief Initializes the projectile's parameters before firing.
	 * 
	 * @param InDamage Amount of damage this projectile will deal.
	 * @param InSpeed Initial movement speed of the projectile.
	 */
	void InitProjectile(float InDamage, float InSpeed);

	/* ---------- Getters ---------- */

	/**
	 * @brief Gets the damage value of the projectile.
	 * @return Damage as a float.
	 */
	FORCEINLINE float GetDamage() const { return Damage; }

	/**
	 * @brief Gets the initial speed of the projectile.
	 * @return Speed value if movement component exists, otherwise 0.
	 */
	FORCEINLINE float GetSpeed() const { return MoveComp ? MoveComp->InitialSpeed : 0.f; }

	/**
	 * @brief Gets the total lifetime of the projectile before it auto-destroys.
	 * @return Lifetime in seconds.
	 */
	FORCEINLINE float GetLifeTime() const { return InitialLifeSpan; }

protected:
	/**
	 * @brief Collision component for detecting overlaps.
	 */
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Collision = nullptr;

	/**
	 * @brief Handles movement logic for the projectile.
	 */
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* MoveComp = nullptr;

	/**
	 * @brief Damage inflicted by the projectile upon hitting a target.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float Damage = 20.f;

	/**
	 * @brief Lifetime duration before the projectile is destroyed.
	 */
	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (ClampMin = "0.1"))
	float LifeSeconds = 5.f;

private:
	/**
	 * @brief Called when the projectile overlaps another actor.
	 * 
	 * @param OverlappedComp The component that was overlapped.
	 * @param OtherActor The actor that was hit.
	 * @param OtherComp The component that was hit.
	 * @param OtherBodyIndex Index of the body hit.
	 * @param bFromSweep True if the overlap was from a sweep.
	 * @param SweepResult Detailed hit result.
	 */
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp,
	               AActor* OtherActor,
	               UPrimitiveComponent* OtherComp,
	               int32 OtherBodyIndex,
	               bool bFromSweep,
	               const FHitResult& SweepResult);
};
