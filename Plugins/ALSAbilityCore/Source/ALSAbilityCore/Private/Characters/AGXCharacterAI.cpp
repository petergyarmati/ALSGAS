// Copyright mrgyarmati


#include "Characters/AGXCharacterAI.h"

#include "AbilitySystem/AGXAbilitySystemComponent.h"
#include "AbilitySystem/AGXAttributeSet.h"


AAGXCharacterAI::AAGXCharacterAI()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAGXAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAGXAttributeSet>(TEXT("AttributeSet"));
}

void AAGXCharacterAI::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);	
}

void AAGXCharacterAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


