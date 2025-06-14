// Base.cpp
#include "Base.h"
#include "Net/UnrealNetwork.h"

ABase::ABase()
{
    bReplicates = true;
    Health      = MaxHealth;     
}

float ABase::TakeDamage(float DamageAmount,
                        const FDamageEvent& /*DamageEvent*/,
                        AController* /*EventInstigator*/,
                        AActor* /*DamageCauser*/)
{
    const int32 Rounded = FMath::RoundToInt(DamageAmount);
    ApplyDamageInternal(Rounded);
    return static_cast<float>(Rounded);  
}

void ABase::ApplyDamageInternal(int32 Damage)
{
    if (Damage <= 0 || IsDestroyed())
        return;

    const int32 OldHealth = Health;
    Health = FMath::Clamp(Health - Damage, 0, MaxHealth);

    if (Health != OldHealth)            
    {
        OnRep_Health();                 
        if (Health <= 0)
            HandleDestroyed();
    }
}

void ABase::OnRep_Health()
{
    
}

void ABase::HandleDestroyed()
{
    Destroy();                          
}

void ABase::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps   
) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ABase, Health);
}
