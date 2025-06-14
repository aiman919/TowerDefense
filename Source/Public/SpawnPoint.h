#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPoint.generated.h"

class AEnemy;

/**
 * @class ASpawnPoint
 * @brief Manages enemy wave spawning logic, including types, timing, and strength scaling.
 *
 * Features:
 * - Supports automatic wave launching and wave customization.
 * - Dynamically chooses enemy types with probability weights.
 * - Supports boss waves and strength scaling per enemy spawn.
 */
UCLASS()
class MYPROJECT_API ASpawnPoint : public AActor
{
    GENERATED_BODY()

public:
    /** Default constructor. */
    ASpawnPoint();

    /**
     * @brief Starts an enemy wave of specified size.
     * 
     * @param Size Number of enemies to spawn in the wave.
     * @param bSpawnBossAtEnd Whether to spawn a boss at the end of the wave.
     */
    UFUNCTION(BlueprintCallable, Category = "Spawn")
    void StartWave(int32 Size, bool bSpawnBossAtEnd = false);

    /**
     * @brief Stops the currently active wave.
     */
    UFUNCTION(BlueprintCallable, Category = "Spawn")
    void StopWave();

    /* ---------- Design-time ---------- */

    /**
     * @brief If true, wave starts automatically on BeginPlay.
     */
    UPROPERTY(EditAnywhere, Category = "Spawn")
    bool bAutoStartWave = false;

    /**
     * @brief Default size of the wave if auto-start is enabled.
     */
    UPROPERTY(EditAnywhere, Category = "Spawn", meta = (EditCondition = "bAutoStartWave"))
    int32 DefaultWaveSize = 5;

    /**
     * @brief Strength multiplier added for each newly spawned enemy.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Scaling", meta = (ClampMin = "0"))
    float StrengthIncreasePerSpawn = 0.05f;

    /* ---------- Blueprint helpers ---------- */

    /**
     * @brief Calculates the strength multiplier for the next enemy to spawn.
     * @return Multiplier as a float.
     */
    UFUNCTION(BlueprintPure, Category = "Spawn|Scaling")
    float GetNextEnemyStrengthMultiplier() const;

    /**
     * @brief Event triggered in Blueprints when an enemy is spawned.
     * 
     * @param Enemy The spawned enemy instance.
     * @param AppliedMultiplier The strength multiplier applied to this enemy.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Spawn")
    void OnEnemySpawnedBP(AEnemy* Enemy, float AppliedMultiplier);

protected:
    /** Called when the game starts. */
    virtual void BeginPlay() override;

    /* ---------- Enemy pools ---------- */

    /** List of regular enemy types that can be spawned. */
    UPROPERTY(EditAnywhere, Category = "Spawn|Types")
    TArray<TSubclassOf<AEnemy>> CommonEnemyTypes;

    /** Optional weighting list corresponding to CommonEnemyTypes. */
    UPROPERTY(EditAnywhere, Category = "Spawn|Types", AdvancedDisplay)
    TArray<int32> CommonWeights;

    /** Armored enemy class. */
    UPROPERTY(EditAnywhere, Category = "Spawn|Types")
    TSubclassOf<AEnemy> ArmoredEnemyClass;

    /** Fast enemy class. */
    UPROPERTY(EditAnywhere, Category = "Spawn|Types")
    TSubclassOf<AEnemy> FastEnemyClass;

    /** Boss enemy class. */
    UPROPERTY(EditAnywhere, Category = "Spawn|Types")
    TSubclassOf<AEnemy> BossEnemyClass;

    /* ---------- Probabilities ---------- */

    /** Chance of spawning an armored enemy (0.0 to 1.0). */
    UPROPERTY(EditAnywhere, Category = "Spawn|Probabilities", meta = (ClampMin = "0", ClampMax = "1"))
    float ArmoredChance = 0.15f;

    /** Chance of spawning a fast enemy (0.0 to 1.0). */
    UPROPERTY(EditAnywhere, Category = "Spawn|Probabilities", meta = (ClampMin = "0", ClampMax = "1"))
    float FastChance = 0.15f;

    /* ---------- Timing / Location ---------- */

    /** Delay between enemy spawns during a wave. */
    UPROPERTY(EditAnywhere, Category = "Spawn|Timing", meta = (ClampMin = "0.05"))
    float SpawnInterval = 1.f;

    /** Distance offset used for enemy spawn positioning. */
    UPROPERTY(EditAnywhere, Category = "Spawn|Location")
    float SpawnOffsetDistance = 100.f;

    /** Scene component used as the root for spawn positioning. */
    UPROPERTY(VisibleAnywhere, Category = "Spawn")
    USceneComponent* SpawnRoot = nullptr;

private:
    /* ---------- Runtime ---------- */

    /** Timer used to manage spawn intervals. */
    FTimerHandle SpawnTimerHandle;

    /** Current wave number. */
    int32 CurrentWave = 1;

    /** Number of enemies to spawn in the current wave. */
    int32 WaveSize = 0;

    /** Number of enemies spawned so far in the current wave. */
    int32 SpawnedCnt = 0;

    /** Whether the current wave includes a boss. */
    bool bBossWave = false;

    /* ---------- Helpers ---------- */

    /** Spawns a single enemy instance. */
    void SpawnOneEnemy();

    /**
     * @brief Chooses the appropriate enemy class based on defined chances and weights.
     * @return The enemy class to spawn.
     */
    TSubclassOf<AEnemy> ChooseEnemyClass() const;

    /** Begins the next wave. */
    void StartNextWave();

    /**
     * @brief Callback triggered when the boss is defeated.
     * 
     * @param DestroyedActor The boss actor that was destroyed.
     */
    UFUNCTION()
    void OnBossDefeated(AActor* DestroyedActor);
};
