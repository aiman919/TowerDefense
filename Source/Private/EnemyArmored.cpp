// EnemyArmored.cpp
#include "EnemyArmored.h"

AEnemyArmored::AEnemyArmored()
{
    MaxHealth    = 1200;
    Armor        = 80;      
    AttackDamage = 40;
    AttackRate   = 0.7f;
    MoveSpeed    = 300.f;

    CurrentHealth = MaxHealth;   
}

void AEnemyArmored::ReceiveDamage(int32 Amount, AController* DamageInstigator)
{
    
    const int32 Adjusted = FMath::Max(1, Amount - Armor);
    Super::ReceiveDamage(Adjusted, DamageInstigator);
}

void AEnemyArmored::ApplyDifficultyScaling(int32 WaveIndex, float StrengthMultiplier)
{
    
    Super::ApplyDifficultyScaling(WaveIndex, StrengthMultiplier);

    if (WaveIndex <= 1 || StrengthMultiplier <= 0.f)
        return;

    const float Factor = 1.f + 0.20f * (WaveIndex - 1) * StrengthMultiplier; // +20 %/волну
    const int32 OldArmor = Armor;

    Armor = FMath::Clamp(FMath::RoundToInt(Armor * Factor), 0, 2000);

    if (Armor != OldArmor)
        OnArmorChanged.Broadcast(Armor);
}
