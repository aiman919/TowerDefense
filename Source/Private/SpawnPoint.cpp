// SpawnPoint.cpp
#include "SpawnPoint.h"
#include "Enemy.h"
#include "TimerManager.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"

ASpawnPoint::ASpawnPoint()
{
    PrimaryActorTick.bCanEverTick = false;
    SpawnRoot   = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = SpawnRoot;
}

void ASpawnPoint::BeginPlay()
{
    Super::BeginPlay();
    if (bAutoStartWave)
        StartWave(DefaultWaveSize);
}

/* ---------- Public API ---------- */
void ASpawnPoint::StartWave(int32 Size, bool bSpawnBossAtEnd /*=false*/)
{
    if (!GetWorld() || Size <= 0)      // безопасно для unit-теста без UWorld
        return;

    WaveSize   = Size;
    SpawnedCnt = 0;
    bBossWave  = bSpawnBossAtEnd;

    GetWorldTimerManager().SetTimer(
        SpawnTimerHandle, this,
        &ASpawnPoint::SpawnOneEnemy,
        SpawnInterval, true, 0.f);
}

void ASpawnPoint::StopWave()
{
    if (GetWorld())
        GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
}

/* ---------- Helpers ---------- */
void ASpawnPoint::SpawnOneEnemy()
{
    if (!GetWorld() || SpawnedCnt >= WaveSize)
    {
        StopWave();
        StartNextWave();
        return;
    }

    /* ----- Choose class ----- */
    const bool bIsLast = SpawnedCnt == WaveSize - 1;
    const TSubclassOf<AEnemy> EnemyClass =
        (bBossWave && bIsLast) ? BossEnemyClass : ChooseEnemyClass();

    if (!EnemyClass) return;

    /* ----- Location ----- */
    const FVector Location =
        GetActorLocation() + FMath::VRand().GetSafeNormal2D() * SpawnOffsetDistance;
    const FTransform TM(GetActorRotation(), Location);

    /* ----- Spawn ----- */
    AEnemy* Enemy = GetWorld()->SpawnActorDeferred<AEnemy>(EnemyClass, TM, this);
    if (!Enemy) return;

    const float Multiplier =
        FMath::Pow(1.f + StrengthIncreasePerSpawn, SpawnedCnt);

    Enemy->ApplyDifficultyScaling(CurrentWave, Multiplier);
    UGameplayStatics::FinishSpawningActor(Enemy, TM);
    OnEnemySpawnedBP(Enemy, Multiplier);

    /* Босс — подписываемся на смерть, чтобы запустить следующую волну */
    if (bBossWave && bIsLast)
        Enemy->OnDestroyed.AddDynamic(this, &ASpawnPoint::OnBossDefeated);

    ++SpawnedCnt;
}

float ASpawnPoint::GetNextEnemyStrengthMultiplier() const
{
    return FMath::Pow(1.f + StrengthIncreasePerSpawn, SpawnedCnt);
}

/* Выбор врага: с учётом Armored/Fast вероятностей и взвешенных Common-типов */
TSubclassOf<AEnemy> ASpawnPoint::ChooseEnemyClass() const
{
    if (!FMath::IsNearlyZero(ArmoredChance) && FMath::FRand() < ArmoredChance && ArmoredEnemyClass)
        return ArmoredEnemyClass;

    if (!FMath::IsNearlyZero(FastChance) && FMath::FRand() < FastChance && FastEnemyClass)
        return FastEnemyClass;

    if (CommonEnemyTypes.Num() == 0) return nullptr;

    /* --- Weighted random, если заданы веса --- */
    if (CommonEnemyTypes.Num() == CommonWeights.Num() && CommonWeights.Num() > 0)
    {
        const int32 Sum = Algo::Accumulate(CommonWeights, 0);
        const int32 R   = FMath::RandRange(1, Sum);
        int32 Acc = 0;
        for (int32 i = 0; i < CommonWeights.Num(); ++i)
        {
            Acc += CommonWeights[i];
            if (R <= Acc)
                return CommonEnemyTypes[i];
        }
    }

    /* --- Иначе равновероятный выбор --- */
    const int32 Idx = FMath::RandRange(0, CommonEnemyTypes.Num() - 1);
    return CommonEnemyTypes[Idx];
}

void ASpawnPoint::OnBossDefeated(AActor* /*DestroyedActor*/)
{
    StartNextWave();
}

void ASpawnPoint::StartNextWave()
{
    ++CurrentWave;
    // Пример: каждую волну +2 врага
    // StartWave(DefaultWaveSize + CurrentWave * 2);
}
