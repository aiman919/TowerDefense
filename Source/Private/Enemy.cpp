// Enemy.cpp
#include "Enemy.h"
#include "Base.h"
#include "AIController.h"
#include "TDPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

AEnemy::AEnemy()
{
    PrimaryActorTick.bCanEverTick = false;
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    SetReplicates(true);
    CurrentHealth = MaxHealth;            // моментальный full-HP
}

void AEnemy::BeginPlay()
{
    Super::BeginPlay();

    // Настраиваем скорость
    if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
    {
        MoveComp->MaxWalkSpeed = MoveSpeed;
    }

    // Цель слежения (если не задана вручную)
    if (!TargetActor)
    {
        TargetActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABase::StaticClass());
    }

    if (AAIController* AI = Cast<AAIController>(GetController()))
    {
        AI->MoveToActor(TargetActor, AcceptanceRadius, true);
    }

    StartAttacking();
}

void AEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    GetWorldTimerManager().ClearTimer(AttackTimerHandle);
    Super::EndPlay(EndPlayReason);
}

/* ---------- Combat ---------- */
void AEnemy::ReceiveDamage(int32 Amount, AController* DamageInstigator)
{
    if (Amount <= 0 || IsDead()) return;

    LastDamageInstigator = DamageInstigator ? DamageInstigator : LastDamageInstigator;

    const int32 Effective = FMath::Max(Amount - Armor, 0);
    const int32 NewHP     = FMath::Clamp(CurrentHealth - Effective, 0, MaxHealth);

    if (NewHP == CurrentHealth) return;       // урон «в ноль»

    CurrentHealth = NewHP;
    OnHealthChanged.Broadcast(CurrentHealth);

    if (IsDead())
    {
        HandleDeath();
    }
}

void AEnemy::StartAttacking()
{
    if (AttackRate <= 0.f) return;
    if (!GetWorldTimerManager().IsTimerActive(AttackTimerHandle))
    {
        GetWorldTimerManager()
            .SetTimer(AttackTimerHandle, this, &AEnemy::Attack, 1.f / AttackRate, true);
    }
}

void AEnemy::Attack()
{
    if (ABase* Base = Cast<ABase>(TargetActor))
    {
        Base->ReceiveDamage(AttackDamage);
    }
}

void AEnemy::ApplyDifficultyScaling(int32 WaveIndex, float StrengthMultiplier)
{
    if (WaveIndex <= 1 || StrengthMultiplier <= 0.f) return;

    const float HFactor = 1.f + HealthPctPerWave  * (WaveIndex - 1) * StrengthMultiplier;
    const float DFactor = 1.f + DamagePctPerWave  * (WaveIndex - 1) * StrengthMultiplier;

    const int32 NewMaxHP  = FMath::Clamp(FMath::RoundToInt(MaxHealth   * HFactor), 1, MaxHealthCap);
    const int32 NewDamage = FMath::Clamp(FMath::RoundToInt(AttackDamage * DFactor), 1, DamageCap);

    MaxHealth      = NewMaxHP;
    AttackDamage   = NewDamage;
    CurrentHealth  = MaxHealth;
}

/* ---------- Net ---------- */
void AEnemy::OnRep_Health()
{
    OnHealthChanged.Broadcast(CurrentHealth);
    if (IsDead())
    {
        HandleDeath();
    }
}

void AEnemy::HandleDeath()
{
    GetWorldTimerManager().ClearTimer(AttackTimerHandle);

    if (LastDamageInstigator)
    {
        if (ATDPlayerState* PS = LastDamageInstigator->GetPlayerState<ATDPlayerState>())
        {
            PS->AddMoney(MoneyReward);
        }
    }

    OnDeathEvt.Broadcast();
    Destroy();
}

void AEnemy::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps
) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AEnemy, CurrentHealth);
    DOREPLIFETIME(AEnemy, AttackDamage);
}