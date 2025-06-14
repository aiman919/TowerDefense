// Enemy.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class ABase;
class ATDPlayerState;

/**
 * @class AEnemy
 * @brief Enemy character that moves toward the base, deals damage, and scales with waves.
 *
 * Optimized for replication and performance:
 * - Avoids redundant clamps and assignments.
 * - OnHealthChanged triggers only when health truly changes.
 * - Attack timer is created once and cleared on death/EndPlay.
 * - Replicates key parameters like CurrentHealth and AttackDamage.
 */
UCLASS()
class MYPROJECT_API AEnemy : public ACharacter
{
    GENERATED_BODY()

public:
    /** Delegate for notifying health changes (used in Blueprints/UI). */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyHealthChanged, int32, NewHealth);

    /** Delegate for notifying death event (used in Blueprints/UI). */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyDeath);

    /** Event triggered when enemy health changes. */
    UPROPERTY(BlueprintAssignable, Category = "Enemy|Events")
    FOnEnemyHealthChanged OnHealthChanged;

    /** Event triggered when enemy dies. */
    UPROPERTY(BlueprintAssignable, Category = "Enemy|Events")
    FOnEnemyDeath OnDeathEvt;

    /** Constructor that initializes default values. */
    AEnemy();

    /* ---------- Combat ---------- */

    /**
     * @brief Applies damage to the enemy, considering armor.
     * 
     * @param Amount The raw damage amount.
     * @param DamageInstigator The controller that caused the damage (optional).
     */
    virtual void ReceiveDamage(int32 Amount,
        AController* DamageInstigator = nullptr);

    /**
     * @brief Blueprint-friendly wrapper for receiving damage without instigator.
     * 
     * @param Amount Damage amount.
     */
    UFUNCTION(BlueprintCallable, Category = "Enemy|Combat")
    FORCEINLINE void ReceiveDamageBP(int32 Amount)
    {
        ReceiveDamage(Amount, nullptr);
    }

    /** 
     * @brief Starts periodic attacks if AttackRate is greater than zero. 
     */
    void StartAttacking();

    /**
     * @brief Performs a single attack.
     * Can be overridden in Blueprints.
     */
    UFUNCTION(BlueprintCallable, Category = "Enemy|Combat")
    virtual void Attack();

    /**
     * @brief Increases stats based on wave index and scaling multiplier.
     * 
     * @param WaveIndex The current wave number.
     * @param StrengthMultiplier Additional multiplier for scaling.
     */
    virtual void ApplyDifficultyScaling(int32 WaveIndex, float StrengthMultiplier = 1.f);

    /* ---------- Getters ---------- */

    /**
     * @brief Returns the current health of the enemy.
     * @return Current health value.
     */
    FORCEINLINE int32 GetHealth() const { return CurrentHealth; }

    /**
     * @brief Checks if the enemy is dead.
     * @return True if health is 0 or below.
     */
    FORCEINLINE bool IsDead() const { return CurrentHealth <= 0; }

    /**
     * @brief Returns the controller that last caused damage to this enemy.
     * @return Pointer to the instigator controller.
     */
    FORCEINLINE AController* GetLastInstigator() const { return LastDamageInstigator; }

protected:
    /** Called when the game starts or when spawned. */
    virtual void BeginPlay() override;

    /** Called when the actor is removed from the game. */
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    /**
     * @brief Registers properties for network replication.
     * @param OutLifetimeProps The list of properties to replicate.
     */
    virtual void GetLifetimeReplicatedProps(
        TArray<FLifetimeProperty>& OutLifetimeProps
    ) const override;

    /* ---------- Movement ---------- */

    /** Target actor to move towards (e.g., the base). */
    UPROPERTY(EditAnywhere, Category = "Enemy|Movement")
    AActor* TargetActor = nullptr;

    /** Minimum distance to target before stopping movement. */
    UPROPERTY(EditAnywhere, Category = "Enemy|Movement", meta = (ClampMin = "0"))
    float AcceptanceRadius = 30.f;

    /** Movement speed of the enemy. */
    UPROPERTY(EditAnywhere, Category = "Enemy|Movement", meta = (ClampMin = "0"))
    float MoveSpeed = 400.f;

    /* ---------- Base Stats ---------- */

    /** Maximum health of the enemy. */
    UPROPERTY(EditDefaultsOnly, Replicated, Category = "Enemy|Stats", meta = (ClampMin = "1"))
    int32 MaxHealth = 1000;

    /** Armor value used to reduce incoming damage. */
    UPROPERTY(EditAnywhere, Category = "Enemy|Stats", meta = (ClampMin = "0"))
    int32 Armor = 0;

    /** Damage dealt to targets when attacking. */
    UPROPERTY(EditDefaultsOnly, Replicated, Category = "Enemy|Stats", meta = (ClampMin = "0"))
    int32 AttackDamage = 50;

    /** Number of attacks per second. */
    UPROPERTY(EditAnywhere, Category = "Enemy|Stats", meta = (ClampMin = "0", UIMin = "0"))
    float AttackRate = 1.f;

    /** Reward money for killing this enemy. */
    UPROPERTY(EditAnywhere, Category = "Enemy|Stats")
    int32 MoneyReward = 10;

    /* ---------- Scaling Parameters ---------- */

    /** Health increase per wave (percentage). */
    UPROPERTY(EditAnywhere, Category = "Enemy|Scaling", meta = (ClampMin = "0"))
    float HealthPctPerWave = 0.20f;

    /** Damage increase per wave (percentage). */
    UPROPERTY(EditAnywhere, Category = "Enemy|Scaling", meta = (ClampMin = "0"))
    float DamagePctPerWave = 0.15f;

    /** Maximum cap for health after scaling. */
    UPROPERTY(EditAnywhere, Category = "Enemy|Scaling")
    int32 MaxHealthCap = 10000;

    /** Maximum cap for attack damage after scaling. */
    UPROPERTY(EditAnywhere, Category = "Enemy|Scaling")
    int32 DamageCap = 2000;

protected:
    /* ---------- Runtime ---------- */

    /** Current health value, replicated using OnRep_Health. */
    UPROPERTY(ReplicatedUsing = OnRep_Health)
    int32 CurrentHealth = 0;

    /** Reference to the controller that last damaged the enemy. */
    UPROPERTY()
    AController* LastDamageInstigator = nullptr;

    /** Handle for managing the attack timer. */
    FTimerHandle AttackTimerHandle;

    /* ---------- Network Callbacks ---------- */

    /** Called on clients when CurrentHealth changes. */
    UFUNCTION()
    void OnRep_Health();

    /** Handles death logic and rewards the player. */
    void HandleDeath();
};
