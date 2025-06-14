// MyProjectGameMode.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyProjectGameMode.generated.h"

class ASpawnPoint;

/**
 * @class AMyProjectGameMode
 * @brief Custom game mode that initiates enemy waves at all spawn points when the match starts.
 *
 * Features:
 * - Uses static helper function to avoid hardcoded values.
 * - Validates `HasAuthority()` and `GetWorld()` to ensure logic runs only on the server.
 * - Avoids code duplication in wave size logic.
 */
UCLASS()
class MYPROJECT_API AMyProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	/**
	 * @brief Called when the game begins (server-side only).
	 * Automatically starts waves at all registered spawn points.
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief Calculates the initial wave size based on the specified spawn point.
	 * 
	 * @param SpawnPoint A pointer to the spawn point to evaluate.
	 * @return The number of enemies in the first wave.
	 */
	UFUNCTION(BlueprintPure, Category = "Waves")
	static int32 CalcInitialWaveSize(const ASpawnPoint* SpawnPoint);
};
