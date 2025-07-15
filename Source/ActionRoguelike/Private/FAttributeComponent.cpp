// Fill out your copyright notice in the Description page of Project Settings.


#include "FAttributeComponent.h"

UFAttributeComponent::UFAttributeComponent()
{
	Health = 100.0f;
	MaxHealth = 100.0f;
}

bool UFAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UFAttributeComponent::ApplyHealthChange(float Delta)
{
	if (Delta > 0.0f) //Healing
	{
		//Ignore healing if our health is already above the MaxHealth
		if (Health >= MaxHealth) return false;
	}
	else
	{
		//Ignore damage if our health is already 0
		if (Health <= 0.0f) return false;
	}
	
	Health += Delta;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
	OnHealthChange.Broadcast(nullptr, this, Health, Delta);
	return true;
}
