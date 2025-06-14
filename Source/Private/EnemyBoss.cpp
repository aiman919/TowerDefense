// EnemyBoss.cpp
#include "EnemyBoss.h"
#include "Base.h"
#include "Kismet/GameplayStatics.h"

AEnemyBoss::AEnemyBoss()
{
   
    MaxHealth    = 5000;
    Armor        = 100;
    AttackDamage = 100;
    AttackRate   = 0.5f;
    MoveSpeed    = 400.f;

    CurrentHealth        = MaxHealth;          
    LastSpecialAttackTime = -SpecialAttackCooldown; 
}

void AEnemyBoss::Attack()
{
    Super::Attack();                           

    UWorld* World = GetWorld();
    if (!World || !HasAuthority())             
        return;

    const float Now = World->GetTimeSeconds();
    if (CanSpecialAttack(Now))
    {
        PerformSpecialAttack();
        LastSpecialAttackTime = Now;
    }
}

void AEnemyBoss::PerformSpecialAttack()
{
    
    UWorld* World = GetWorld();
    if (!HasAuthority() || !World)
        return;

    
    TArray<AActor*> Ignored;
    Ignored.Add(this);

    UGameplayStatics::ApplyRadialDamage(
        this,
        SpecialAttackDamage,
        GetActorLocation(),
        500.f,
        nullptr,                       
        Ignored,
        this,
        GetInstigatorController(),
        true                           
    );

   
}
