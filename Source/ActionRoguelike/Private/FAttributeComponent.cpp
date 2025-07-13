// Fill out your copyright notice in the Description page of Project Settings.


#include "FAttributeComponent.h"

UFAttributeComponent::UFAttributeComponent()
{
	Health = 100.0f;
}

bool UFAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UFAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;
	OnHealthChange.Broadcast(nullptr, this, Health, Delta);
	return true;
}
