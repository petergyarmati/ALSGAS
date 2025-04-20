// Copyright mrgyarmati


#include "Characters/AGXCharacterBase.h"


AAGXCharacterBase::AAGXCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

UAbilitySystemComponent* AAGXCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
