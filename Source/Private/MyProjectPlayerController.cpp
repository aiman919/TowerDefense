#include "MyProjectPlayerController.h"

DEFINE_LOG_CATEGORY(LogMyPlayerController);

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Tower.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/LogMacros.h"


AMyProjectPlayerController::AMyProjectPlayerController()
{
	bShowMouseCursor   = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AMyProjectPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	
	if (DefaultMappingContext)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Sub =
			 ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Sub->AddMappingContext(DefaultMappingContext, /*Priority=*/0);
		}
	}

	
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (SpawnTowerAction)
		{
			EIC->BindAction(SpawnTowerAction,
			                ETriggerEvent::Completed,
			                this,
			                &AMyProjectPlayerController::HandleSpawnTower);
		}
	}
}

/* ---------- Private helpers ---------- */

bool AMyProjectPlayerController::TryGetCursorLocation(FVector& OutLocation) const
{
	FHitResult Hit;
	if (!GetHitResultUnderCursor(ECC_Visibility, false, Hit) || !Hit.bBlockingHit)
		return false;

	OutLocation = Hit.Location;
	return true;
}

void AMyProjectPlayerController::HandleSpawnTower()
{
	if (!TowerToSpawn)
	{
		UE_LOG(LogPlayerController, Warning, TEXT("TowerToSpawn not set"));
		return;
	}

	FVector SpawnLoc;
	if (!TryGetCursorLocation(SpawnLoc))
		return;

	const FRotator SpawnRot = FRotator::ZeroRotator;

	GetWorld()->SpawnActor<ATower>(TowerToSpawn, SpawnLoc, SpawnRot);

	// FX
	if (FXCursor)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this, FXCursor, SpawnLoc, SpawnRot, FVector(1.f));
	}
}
