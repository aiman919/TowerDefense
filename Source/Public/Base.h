// Base.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Base.generated.h"

/**
 * @class ABase
 * @brief Base class with replicated health logic.
 *
 * This actor represents a destructible base with replicated health.
 * - Health and MaxHealth are initialized in the constructor.
 * - Internal damage logic is separated for reusability between different damage types.
 */
UCLASS()
class MYPROJECT_API ABase : public AActor
{
    GENERATED_BODY()

public:
    /** Constructor that initializes default health values. */
    ABase();

    /**
     * @brief Applies float damage using Unreal's damage system.
     * 
     * @param DamageAmount Amount of damage to apply.
     * @param DamageEvent Details about the damage event.
     * @param EventInstigator The controller that instigated the damage.
     * @param DamageCauser The actor that caused the damage.
     * @return The actual amount of damage applied.
     */
    virtual float TakeDamage(float DamageAmount,
                             const FDamageEvent& DamageEvent,
                             AController* EventInstigator,
                             AActor* DamageCauser) override;

    /**
     * @brief Applies integer damage (useful for Blueprints).
     * 
     * @param Amount The amount of integer damage to apply.
     */
    UFUNCTION(BlueprintCallable, Category = "Defense")
    FORCEINLINE void ReceiveDamage(int32 Amount)
    {
        ApplyDamageInternal(Amount);
    }

    /**
     * @brief Returns the current health of the base.
     * 
     * @return The current health value.
     */
    UFUNCTION(BlueprintPure, Category = "Defense")
    FORCEINLINE int32 GetHealth() const { return Health; }

    /**
     * @brief Checks if the base is considered destroyed.
     * 
     * @return True if Health is 0 or below, false otherwise.
     */
    UFUNCTION(BlueprintPure, Category = "Defense")
    FORCEINLINE bool IsDestroyed() const { return Health <= 0; }

protected:
    /**
     * @brief The current health value (replicated and saved).
     */
    UPROPERTY(ReplicatedUsing = OnRep_Health,
              VisibleAnywhere,
              Category = "Defense",
              SaveGame)
    int32 Health = 100;

    /**
     * @brief The maximum health value (can be set in Inspector).
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defense")
    int32 MaxHealth = 100;

    /**
     * @brief Callback triggered when Health is updated on clients.
     */
    UFUNCTION()
    void OnRep_Health();

    /**
     * @brief Specifies which properties are replicated over the network.
     * 
     * @param OutLifetimeProps The list to populate with replicated properties.
     */
    virtual void GetLifetimeReplicatedProps(
        TArray<FLifetimeProperty>& OutLifetimeProps
    ) const override;

private:
    /**
     * @brief Internal logic to apply integer damage and update health.
     * 
     * @param Damage The amount of damage to apply.
     */
    void ApplyDamageInternal(int32 Damage);

    /**
     * @brief Handles logic when the base is destroyed (Health <= 0).
     */
    void HandleDestroyed();
};
