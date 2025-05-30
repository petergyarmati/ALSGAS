﻿// Copyright mrgyarmati

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ALS/Public/AlsCharacter.h"
#include "AGXCharacterBase.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class ALSABILITYCORE_API AAGXCharacterBase : public AAlsCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
public:
	AAGXCharacterBase();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAttributeSet* GetAttributeSet() const { return AttributeSet; };
};
