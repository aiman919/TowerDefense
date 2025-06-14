#include "EnemyFast.h"

AEnemyFast::AEnemyFast()
{
    MaxHealth    = 600;
    Armor        = 0;
    MoveSpeed    = 700.f;   // быстрее базового врага
    AttackDamage = 30;
    AttackRate   = 1.5f;    // 1.5 атаки в секунду

    CurrentHealth = MaxHealth; // обновляем после изменения MaxHealth
}