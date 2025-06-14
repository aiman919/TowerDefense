#pragma once

#include "Enemy.h"
#include "EnemyFast.generated.h"

/**
 * @class AEnemyFast
 * @brief Lightweight and fast enemy with lower HP but higher speed and DPS.
 *
 * Notes:
 * - No need to override BeginPlay; movement logic is inherited from AEnemy.
 * - Public speed getter is exposed for Blueprint and UI use.
 */
UCLASS()
class MYPROJECT_API AEnemyFast : public AEnemy
{
    GENERATED_BODY()

public:
    /** Default constructor for the fast enemy. */
    AEnemyFast();

    /**
     * @brief Gets the current movement speed (MaxWalkSpeed).
     * 
     * @return Current movement speed of the enemy.
     */
    UFUNCTION(BlueprintPure, Category = "Movement")
    FORCEINLINE float GetMoveSpeedFast() const { return MoveSpeed; }
};
