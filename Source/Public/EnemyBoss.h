#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "EnemyBoss.generated.h"

/**
 * @class AEnemyBoss
 * @brief The main boss enemy — an enhanced version of AEnemy with a special attack.
 *
 * Optimized features:
 * - No custom Tick or timers: relies on AEnemy logic with a simple cooldown.
 * - Handles nullptr UWorld for unit test compatibility.
 * - Inherits base stats and damage logic from AEnemy to avoid code duplication.
 */
UCLASS()
class MYPROJECT_API AEnemyBoss : public AEnemy
{
    GENERATED_BODY()

public:
    /** Default constructor for the boss enemy. */
    AEnemyBoss();

    /**
     * @brief Performs a normal attack plus a special attack if cooldown allows.
     * Overrides the base class attack.
     */
    virtual void Attack() override;

    /**
     * @brief Forces execution of the special attack.
     * Callable from Blueprints.
     */
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void PerformSpecialAttack();

    /* ---------- Getters ---------- */

    /**
     * @brief Gets the cooldown duration of the special attack.
     * @return The cooldown value in seconds.
     */
    UFUNCTION(BlueprintPure, Category = "Combat")
    FORCEINLINE float GetSpecialAttackCooldown() const { return SpecialAttackCooldown; }

    /**
     * @brief Gets the boss's maximum health value.
     * @return Max health value.
     */
    UFUNCTION(BlueprintPure, Category = "Combat")
    FORCEINLINE int32 GetMaxHealth() const { return MaxHealth; }

    /**
     * @brief Gets the time of the last special attack.
     * @return Timestamp of last special attack.
     */
    UFUNCTION(BlueprintPure, Category = "Combat")
    FORCEINLINE float GetLastSpecialAttackTime() const { return LastSpecialAttackTime; }

protected:
    /* ---------- Boss Stats ---------- */

    /**
     * @brief Damage value dealt by the special attack.
     */
    UPROPERTY(EditDefaultsOnly, Category = "Boss|Combat", meta = (ClampMin = "0"))
    int32 SpecialAttackDamage = 200;

    /**
     * @brief Cooldown duration between special attacks (in seconds).
     */
    UPROPERTY(EditDefaultsOnly, Category = "Boss|Combat", meta = (ClampMin = "0.1"))
    float SpecialAttackCooldown = 5.f;

    /**
     * @brief Time when the last special attack occurred.
     * Used to determine cooldown availability.
     */
    float LastSpecialAttackTime = -999.f;

private:
    /**
     * @brief Checks whether the special attack can be performed.
     * 
     * @param CurrentTime The current game time (passed externally).
     * @return True if cooldown has passed; false otherwise.
     */
    FORCEINLINE bool CanSpecialAttack(float CurrentTime) const
    {
        return (CurrentTime - LastSpecialAttackTime) >= SpecialAttackCooldown;
    }
};
