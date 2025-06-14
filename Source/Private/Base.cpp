// Base.cpp
#include "Base.h"
#include "Net/UnrealNetwork.h"

ABase::ABase()
{
    bReplicates = true;
    Health      = MaxHealth;     // На старте — полный запас.
}

float ABase::TakeDamage(float DamageAmount,
                        const FDamageEvent& /*DamageEvent*/,
                        AController* /*EventInstigator*/,
                        AActor* /*DamageCauser*/)
{
    const int32 Rounded = FMath::RoundToInt(DamageAmount);
    ApplyDamageInternal(Rounded);
    return static_cast<float>(Rounded);   // «Фактический» урон.
}

void ABase::ApplyDamageInternal(int32 Damage)
{
    if (Damage <= 0 || IsDestroyed())
        return;

    const int32 OldHealth = Health;
    Health = FMath::Clamp(Health - Damage, 0, MaxHealth);

    if (Health != OldHealth)            // Здоровье действительно изменилось
    {
        OnRep_Health();                 // локальный отклик
        if (Health <= 0)
            HandleDestroyed();
    }
}

void ABase::OnRep_Health()
{
    // Здесь можно вызвать BP-ивент, проиграть звук/FX и т.п.
}

void ABase::HandleDestroyed()
{
    Destroy();                          // Сервер удалит, клиенты узнают через репликацию
}

void ABase::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps   // ← было Out
) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ABase, Health);
}
