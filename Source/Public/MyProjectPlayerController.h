#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMyPlayerController, Log, All);

#include "Templates/SubclassOf.h"
#include "MyProjectPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UNiagaraSystem;
class ATower;

/**
 * @class AMyProjectPlayerController
 * @brief Player controller for Top-Down view; allows spawning towers via mouse clicks.
 *
 * Highlights:
 * - No overridden BeginPlay; clean and focused logic.
 * - Public API remains clean due to encapsulated helper methods.
 * - Fully supports Blueprint editing of TowerToSpawn.
 */
UCLASS()
class MYPROJECT_API AMyProjectPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/** Default constructor. Initializes controller state. */
	AMyProjectPlayerController();

	/**
	 * @brief The class of the tower to spawn when clicking.
	 * Can be edited via Blueprints.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower")
	TSubclassOf<ATower> TowerToSpawn;

	/**
	 * @brief Threshold time (in seconds) for detecting a short click.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float ShortPressThreshold = 0.2f;

	/**
	 * @brief Niagara FX system used to visualize cursor clicks.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	UNiagaraSystem* FXCursor = nullptr;

protected:
	/** 
	 * @brief Sets up input bindings.
	 * Called by the engine when initializing the input component.
	 */
	virtual void SetupInputComponent() override;

	/**
	 * @brief Input mapping context used by this controller.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext = nullptr;

	/**
	 * @brief Input action for spawning a tower.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* SpawnTowerAction = nullptr;

private:
	/**
	 * @brief Handles spawning a tower at the cursor location.
	 * Triggered by the input action.
	 */
	void HandleSpawnTower();

	/**
	 * @brief Tries to get a valid location under the cursor.
	 * 
	 * @param OutLocation The resulting world location under the cursor.
	 * @return True if a valid location was found, false otherwise.
	 */
	bool TryGetCursorLocation(FVector& OutLocation) const;
};
