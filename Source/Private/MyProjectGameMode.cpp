// MyProjectGameMode.cpp
#include "MyProjectGameMode.h"
#include "SpawnPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

int32 AMyProjectGameMode::CalcInitialWaveSize(const ASpawnPoint* SpawnPoint)
{
	if (!SpawnPoint)
		return 0;

	return SpawnPoint->bAutoStartWave ? SpawnPoint->DefaultWaveSize : 10;
}

void AMyProjectGameMode::BeginPlay()
{
	Super::BeginPlay();

	
	UWorld* World = GetWorld();
	if (!World || !HasAuthority())
		return;

	TArray<AActor*> SpawnActors;
	UGameplayStatics::GetAllActorsOfClass(World, ASpawnPoint::StaticClass(), SpawnActors);

	for (AActor* Actor : SpawnActors)
	{
		if (ASpawnPoint* SP = Cast<ASpawnPoint>(Actor))
		{
			const int32 Count = CalcInitialWaveSize(SP);
			if (Count > 0)
			{
				SP->StartWave(Count);
			}
		}
	}
}
