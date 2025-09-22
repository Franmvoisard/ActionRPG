// Public Domain - 2025 Franco Voisard.
// This code is provided for skill and knowledge demo purposes.
// No rights reserved. Use freely.


#include "FPlayerState.h"

#include "Net/UnrealNetwork.h"

void AFPlayerState::OnRep_Credits(int OldCredits)
{
	OnCreditsChanged.Broadcast(Credits, OldCredits - Credits);
}

AFPlayerState::AFPlayerState()
{
	Credits = 0;
}

int AFPlayerState::GetCredits() const
{
	return Credits;
}

void AFPlayerState::AddCredits(int Amount)
{
	Credits += Amount;
	OnCreditsChanged.Broadcast(Credits, Amount);
}

bool AFPlayerState::SpendCredits(int Amount)
{
	if (Credits < Amount)
	{
		return false;
	}
	
	Credits -= Amount;
	OnCreditsChanged.Broadcast(Credits, -Amount);
	return true;
}

void AFPlayerState::LoadPlayerState_Implementation(UFSaveGame* SaveObject)
{
	if (SaveObject)
	{
		Credits = SaveObject->Credits;
	}
}

void AFPlayerState::SavePlayerState_Implementation(UFSaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Credits = Credits;
	}
}

void AFPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFPlayerState, Credits);
}
