// Copyright mrgyarmati


#include "AbilitySystem/AGXAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "Characters/AGXCharacterPlayer.h"

UAGXAttributeSet::UAGXAttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
	InitStamina(100.f);
	InitMaxStamina(100.f);
}

void UAGXAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAGXAttributeSet, Health, OldHealth);
}

void UAGXAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAGXAttributeSet, MaxHealth, OldMaxHealth);
}

void UAGXAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAGXAttributeSet, Stamina, OldStamina);
}

void UAGXAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAGXAttributeSet, MaxStamina, OldMaxStamina);
}

void UAGXAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAGXAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGXAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGXAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGXAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}

void UAGXAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
		UE_LOG(LogTemp, Warning, TEXT("Stamina after effect: %f"), GetStamina());
		
		if (AAGXCharacterPlayer* Character = Cast<AAGXCharacterPlayer>(Data.EffectSpec.GetContext().GetOriginalInstigatorAbilitySystemComponent()->AbilityActorInfo->AvatarActor.Get()))
		{
			// Auto-stop sprinting if stamina drops too low
			if (Character->GetDesiredGait() == AlsGaitTags::Sprinting &&
				GetStamina() < 10.0f) // Assuming GetMinSprintStamina() returns MinSprintStamina
			{
				UE_LOG(LogTemp, Warning, TEXT("Auto canceling sprint due to low stamina"));
				Character->SetDesiredGait(AlsGaitTags::Running);
			}

			UE_LOG(LogTemp, Warning, TEXT("Character is sprinting with stamina: %f"), GetStamina());
		}
	}
}

void UAGXAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
}