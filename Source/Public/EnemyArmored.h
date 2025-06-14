#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "EnemyArmored.generated.h"

/**
 * @class AEnemyArmored
 * @brief Armored enemy type with higher HP and scaling armor.
 * 
 * This enemy variant has:
 * - Increased base HP.
 * - Armor that reduces incoming damage but guarantees at least 1 damage.
 * - Armor scaling logic based on wave index.
 */
UCLASS()
class MYPROJECT_API AEnemyArmored : public AEnemy
{
    GENERATED_BODY()

public:
    /**
     * @brief Delegate for notifying armor changes (used in UI or Blueprints).
     * @param NewArmor The updated armor value.
     */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArmorChanged, int32, NewArmor);

    /** Event triggered when the armor value changes. */
    UPROPERTY(BlueprintAssignable, Category = "Enemy|Events")
    FOnArmorChanged OnArmorChanged;

    /** Default constructor for the armored enemy. */
    AEnemyArmored();

    /**
     * @brief Applies damage while enforcing the "minimum 1 damage after armor" rule.
     * 
     * @param Amount The raw damage value.
     * @param DamageInstigator The controller responsible for the damage (optional).
     */
    virtual void ReceiveDamage(int32 Amount,
                               AController* DamageInstigator = nullptr) override;

protected:
    /**
     * @brief Increases armor stats additionally during wave-based scaling.
     * 
     * @param WaveIndex The index of the current wave.
     * @param StrengthMultiplier Optional multiplier for scaling strength.
     */
    virtual void ApplyDifficultyScaling(int32 WaveIndex,
                                        float StrengthMultiplier = 1.f) override;
};
