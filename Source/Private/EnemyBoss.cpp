// EnemyBoss.cpp
#include "EnemyBoss.h"
#include "Base.h"
#include "Kismet/GameplayStatics.h"

AEnemyBoss::AEnemyBoss()
{
    // Усиленные параметры босса
    MaxHealth    = 5000;
    Armor        = 100;
    AttackDamage = 100;
    AttackRate   = 0.5f;
    MoveSpeed    = 400.f;

    CurrentHealth        = MaxHealth;          // поле из AEnemy
    LastSpecialAttackTime = -SpecialAttackCooldown; // «готово сразу»
}

void AEnemyBoss::Attack()
{
    Super::Attack();                           // стандартный удар

    UWorld* World = GetWorld();
    if (!World || !HasAuthority())             // без мира / на клиенте — ничего не делаем
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
    // Защита от вызова на клиентах или без мира
    UWorld* World = GetWorld();
    if (!HasAuthority() || !World)
        return;

    // Массовый урон по радиусу 500 uu
    TArray<AActor*> Ignored;
    Ignored.Add(this);

    UGameplayStatics::ApplyRadialDamage(
        this,
        SpecialAttackDamage,
        GetActorLocation(),
        500.f,
        nullptr,                       // DamageType
        Ignored,
        this,
        GetInstigatorController(),
        true                           // DoFullDamage
    );

    // Здесь можно вызвать FX / SFX / Blueprint-ивент
}
