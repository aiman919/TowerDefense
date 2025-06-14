// Projectile.cpp
#include "Projectile.h"
#include "Enemy.h"
#include "Base.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	/* ---- Collision ---- */
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Collision->InitSphereRadius(8.f);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionObjectType(ECC_WorldDynamic);
	Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_Pawn,        ECR_Overlap);
	Collision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlap);
	RootComponent = Collision;

	/* ---- Movement ---- */
	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComp"));
	MoveComp->InitialSpeed            = 2000.f;
	MoveComp->MaxSpeed                = 2000.f;
	MoveComp->bRotationFollowsVelocity = true;
	MoveComp->ProjectileGravityScale   = 0.f;

	InitialLifeSpan = LifeSeconds;   // удалится автоматически
}

void AProjectile::InitProjectile(float InDamage, float InSpeed)
{
	if (InDamage > 0.f)
		Damage = InDamage;

	if (MoveComp && InSpeed > 0.f)
	{
		MoveComp->InitialSpeed = InSpeed;
		MoveComp->MaxSpeed     = InSpeed;
	}
}

void AProjectile::OnOverlap(UPrimitiveComponent* /*OverlappedComp*/,
                            AActor*              OtherActor,
                            UPrimitiveComponent* /*OtherComp*/,
                            int32 /*OtherBodyIndex*/,
                            bool /*bFromSweep*/,
                            const FHitResult& /*SweepResult*/)
{
	if (!OtherActor || OtherActor == this)
		return;

	if (AEnemy* Enemy = Cast<AEnemy>(OtherActor))
	{
		Enemy->ReceiveDamage(static_cast<int32>(Damage));
	}
	else if (ABase* B = Cast<ABase>(OtherActor))
	{
		B->ReceiveDamage(static_cast<int32>(Damage));
	}

	Destroy();
}
