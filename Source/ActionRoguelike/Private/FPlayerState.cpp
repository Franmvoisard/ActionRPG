// Public Domain - 2025 Franco Voisard.
// This code is provided for skill and knowledge demo purposes.
// No rights reserved. Use freely.


#include "FPlayerState.h"

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
