// MyProjectCharacter.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyProjectCharacter.generated.h"

/**
 * @class AMyProjectCharacter
 * @brief Top-down playable character with a spring-arm mounted camera.
 * 
 * Design notes:
 * - Tick is disabled to save performance as no per-frame logic exists yet.
 * - Public getters are exposed for Blueprint and UI use.
 * - Components are initialized directly at declaration (C++17 style).
 */
UCLASS(Blueprintable)
class MYPROJECT_API AMyProjectCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	/** Default constructor. Sets up camera and spring arm components. */
	AMyProjectCharacter();

	/**
	 * @brief Returns the top-down camera component.
	 * @return Pointer to the UCameraComponent.
	 */
	FORCEINLINE UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }

	/**
	 * @brief Returns the spring arm component that positions the camera.
	 * @return Pointer to the USpringArmComponent.
	 */
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:
	/**
	 * @brief Top-down camera attached to the spring arm.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* TopDownCameraComponent;

	/**
	 * @brief Spring arm used to offset and rotate the camera above the character.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
};
