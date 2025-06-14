// Tower.cpp
#include "Tower.h"
#include "Projectile.h"
#include "Enemy.h"
#include "TDPlayerState.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/World.h"

ATower::ATower()
{
	PrimaryActorTick.bCanEverTick = false;

	/* ---- Geometry ---- */
	TowerMesh   = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerMesh"));
	RootComponent = TowerMesh;

	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(TowerMesh);
	Muzzle->SetRelativeLocation({0.f, 0.f, 100.f});
}

void ATower::BeginPlay()
{
	Super::BeginPlay();

	if (FireInterval > 0.f && GetWorld())
	{
		GetWorldTimerManager().SetTimer(
			FireTimer, this, &ATower::TryFire, FireInterval, true);
	}
}

/* ---------- Combat ---------- */
void ATower::TryFire()
{
	if (!ProjectileClass)                       return;
	if (!GetWorld())                            return;

	if (AEnemy* Target = AcquireTarget())
	{
		const FVector Start = Muzzle->GetComponentLocation();
		const FVector Dir   = (Target->GetActorLocation() - Start).GetSafeNormal();
		const FRotator Rot  = Dir.Rotation();

		FActorSpawnParameters Params;
		Params.Owner = this;
		Params.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		if (AProjectile* Proj = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Start, Rot, Params))
		{
			Proj->InitProjectile(ProjectileDamage, ProjectileSpeed);
		}
	}
}

AEnemy* ATower::AcquireTarget() const
{
	if (!GetWorld()) return nullptr;

	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), Found);

	AEnemy* Best = nullptr;
	float BestD2 = FireRange * FireRange;

	for (AActor* A : Found)
	{
		const float D2 = FVector::DistSquared(GetActorLocation(), A->GetActorLocation());
		if (D2 < BestD2)
		{
			BestD2 = D2;
			Best   = Cast<AEnemy>(A);
		}
	}
	return Best;
}

/* ---------- Upgrade ---------- */
bool ATower::Upgrade(ATDPlayerState* PlayerState)
{
	if (!PlayerState)              return false;
	if (!PlayerState->SpendMoney(UpgradeCost))
		return false;               // недостаточно средств

	++Level;
	ProjectileDamage += 15.f;
	FireInterval      = FMath::Max(0.1f, FireInterval * 0.85f);
	UpgradeCost       = FMath::RoundToInt(UpgradeCost * 1.75f);

	// Перезапускаем таймер с новым FireInterval
	if (GetWorld())
	{
		GetWorldTimerManager().ClearTimer(FireTimer);
		GetWorldTimerManager().SetTimer(FireTimer, this, &ATower::TryFire,
		                                FireInterval, true);
	}
	return true;
}
