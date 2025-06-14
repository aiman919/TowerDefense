#include "EnemyFast.h"

AEnemyFast::AEnemyFast()
{
    MaxHealth    = 600;
    Armor        = 0;
    MoveSpeed    = 700.f;   
    AttackDamage = 30;
    AttackRate   = 1.5f;    

    CurrentHealth = MaxHealth;
}