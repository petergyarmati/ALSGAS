// Copyright mrgyarmati


#include "Player/AGXPlayerState.h"

#include "AbilitySystem/AGXAbilitySystemComponent.h"
#include "AbilitySystem/AGXAttributeSet.h"

AAGXPlayerState::AAGXPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAGXAbilitySystemComponent>("AGXAbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAGXAttributeSet>("AGXAttributeSet");
	
	NetUpdateFrequency = 100.0f;
}

UAbilitySystemComponent* AAGXPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
