// TDPlayerState.cpp
#include "TDPlayerState.h"
#include "Net/UnrealNetwork.h"

void ATDPlayerState::AddMoney(int32 Amount)
{
	if (!HasAuthority() || Amount <= 0) return;

	Money += Amount;
	BroadcastMoneyChanged();          
}

bool ATDPlayerState::SpendMoney(int32 Amount)
{
	if (!HasAuthority() || Amount <= 0 || Money < Amount)
		return false;

	Money -= Amount;
	BroadcastMoneyChanged();
	return true;
}

void ATDPlayerState::OnRep_Money(int32 /*OldMoney*/)
{
	BroadcastMoneyChanged();
}

void ATDPlayerState::BroadcastMoneyChanged()
{
	OnMoneyChanged.Broadcast(Money);
	
}

void ATDPlayerState::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps
) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATDPlayerState, Money);
}