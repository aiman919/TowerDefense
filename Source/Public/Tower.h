#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tower.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class AProjectile;
class AEnemy;
class ATDPlayerState;

/**
 * @class ATower
 * @brief Cannon tower that searches for the nearest enemy in range and fires periodically. Supports upgrades using player currency.
 *
 * Design Notes:
 * - Tick is disabled; uses a timer for firing logic.
 * - Contains safe early exits (e.g., null checks for UWorld).
 * - Provides public getters for Blueprint/UI and testing.
 */
UCLASS()
class MYPROJECT_API ATower : public AActor
{
	GENERATED_BODY()

public:
	/** Default constructor. Initializes default tower values. */
	ATower();

	/* ---------- Getters ---------- */

	/**
	 * @brief Gets the current upgrade level of the tower.
	 * @return The current tower level.
	 */
	UFUNCTION(BlueprintPure, Category = "Tower")
	FORCEINLINE int32 GetTowerLevel() const { return Level; }

	/**
	 * @brief Gets the damage dealt by each projectile.
	 * @return Projectile damage value.
	 */
	UFUNCTION(BlueprintPure, Category = "Tower")
	FORCEINLINE float GetDamage() const { return ProjectileDamage; }

	/**
	 * @brief Gets the interval between shots.
	 * @return Time between shots in seconds.
	 */
	UFUNCTION(BlueprintPure, Category = "Tower")
	FORCEINLINE float GetFireInterval() const { return FireInterval; }

	/**
	 * @brief Attempts to upgrade the tower using the player's resources.
	 * 
	 * @param PlayerState The player's state (used to deduct money).
	 * @return True if the upgrade was successful; false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Tower|Upgrade")
	bool Upgrade(ATDPlayerState* PlayerState);

protected:
	/** Called when the game starts. */
	virtual void BeginPlay() override;

	/* ---------- Components ---------- */

	/** Static mesh representing the tower. */
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* TowerMesh = nullptr;

	/** Scene component representing the muzzle (firing point). */
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Muzzle = nullptr;

	/* ---------- Combat Settings ---------- */

	/** Maximum range the tower can detect and fire at enemies. */
	UPROPERTY(EditAnywhere, Category = "Tower|Combat", meta = (ClampMin = "100"))
	float FireRange = 1500.f;

	/** Time interval between each projectile fired. */
	UPROPERTY(EditAnywhere, Category = "Tower|Combat", meta = (ClampMin = "0.05"))
	float FireInterval = 1.f;

	/** The class of projectile to spawn when firing. */
	UPROPERTY(EditAnywhere, Category = "Tower|Combat")
	TSubclassOf<AProjectile> ProjectileClass;

	/** Damage dealt by the tower's projectile. */
	UPROPERTY(EditAnywhere, Category = "Tower|Combat")
	float ProjectileDamage = 20.f;

	/** Speed at which the projectile travels. */
	UPROPERTY(EditAnywhere, Category = "Tower|Combat")
	float ProjectileSpeed = 2000.f;

	/* ---------- Upgrade Settings ---------- */

	/** Cost to upgrade the tower. */
	UPROPERTY(EditAnywhere, Category = "Tower|Upgrade")
	int32 UpgradeCost = 50;

	/** Current level of the tower. */
	UPROPERTY(VisibleAnywhere, Category = "Tower|Upgrade")
	int32 Level = 1;

private:
	/** Timer handle used for scheduling projectile firing. */
	FTimerHandle FireTimer;

	/**
	 * @brief Called on timer tick to fire at enemies if possible.
	 */
	void TryFire();

	/**
	 * @brief Finds the closest enemy within firing range.
	 * 
	 * @return Pointer to the closest enemy, or nullptr if none found.
	 */
	AEnemy* AcquireTarget() const;
};
