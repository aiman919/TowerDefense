// MyProjectCharacter.cpp
#include "MyProjectCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h" 
#include "GameFramework/CharacterMovementComponent.h"

AMyProjectCharacter::AMyProjectCharacter()
{
	/* ---- Collider ---- */
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	/* ---- Rotation / Movement ---- */
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw   = false;
	bUseControllerRotationRoll  = false;

	UCharacterMovementComponent* Move = GetCharacterMovement();
	Move->bOrientRotationToMovement = true;
	Move->RotationRate             = {0.f, 640.f, 0.f};
	Move->bConstrainToPlane        = true;
	Move->bSnapToPlaneAtStart      = true;

	/* ---- Camera boom ---- */
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength   = 800.f;
	CameraBoom->SetRelativeRotation({-60.f, 0.f, 0.f});
	CameraBoom->bDoCollisionTest  = false;

	/* ---- Camera ---- */
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	/* ---- Tick ---- */
	PrimaryActorTick.bCanEverTick = false;   
}
