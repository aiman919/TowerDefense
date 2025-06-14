#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TDPlayerState.generated.h"

/**
 * @brief Delegate for notifying UI/Blueprints when the player's money changes.
 * @param NewMoney The updated money value.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoneyChanged, int32, NewMoney);

/**
 * @class ATDPlayerState
 * @brief Holds and replicates the player's resources (money) to clients.
 */
UCLASS()
class MYPROJECT_API ATDPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	/** Default constructor. */
	ATDPlayerState() = default;

	/**
	 * @brief Gets the current amount of money.
	 * @return Player's money.
	 */
	UFUNCTION(BlueprintPure, Category = "Resources")
	FORCEINLINE int32 GetMoney() const { return Money; }

	/**
	 * @brief Adds money to the player (only positive amounts).
	 * 
	 * @param Amount The amount to add.
	 */
	UFUNCTION(BlueprintCallable, Category = "Resources")
	void AddMoney(int32 Amount);

	/**
	 * @brief Tries to spend a specific amount of money.
	 * 
	 * @param Amount The amount to spend.
	 * @return True if the transaction was successful; false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Resources")
	bool SpendMoney(int32 Amount);

	/** Event broadcasted when money value changes (for UI). */
	UPROPERTY(BlueprintAssignable, Category = "Resources")
	FOnMoneyChanged OnMoneyChanged;

protected:
	/**
	 * @brief Replication callback for the Money variable.
	 * 
	 * @param OldMoney The previous value before update.
	 */
	UFUNCTION()
	void OnRep_Money(int32 OldMoney);

	/**
	 * @brief Sets up property replication for networking.
	 * 
	 * @param OutLifetimeProps The list of properties to replicate.
	 */
	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps
	) const override;

private:
	/**
	 * @brief Player's current money (replicated from server to clients).
	 */
	UPROPERTY(ReplicatedUsing = OnRep_Money, SaveGame)
	int32 Money = 0;

	/**
	 * @brief Locally broadcasts the money change delegate.
	 * Prevents unnecessary network broadcasts.
	 */
	void BroadcastMoneyChanged();

	#if WITH_AUTOMATION_TESTS
public:
	/**
	 * @brief For unit testing: forcibly sets the money value.
	 * 
	 * @param Amount The amount to set.
	 */
	void ForceSetMoney(int32 Amount) { Money = Amount; }
	#endif
};
