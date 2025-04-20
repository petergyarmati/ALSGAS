// Copyright mrgyarmati


#include "Actors/AGXEffectActor.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystem/AGXAttributeSet.h"
#include "Components/BoxComponent.h"

AAGXEffectActor::AAGXEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->SetupAttachment(GetRootComponent());
}

void AAGXEffectActor::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AAGXEffectActor::OnOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AAGXEffectActor::EndOverlap);
}

void AAGXEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UAGXAttributeSet* AGXAttributeSet = Cast<UAGXAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UAGXAttributeSet::StaticClass()));
		UAGXAttributeSet* MutableAttributeSet = const_cast<UAGXAttributeSet*>(AGXAttributeSet);
		MutableAttributeSet->SetHealth(AGXAttributeSet->GetHealth() - 25.f);
		MutableAttributeSet->SetStamina(AGXAttributeSet->GetStamina() - 25.f);
	}
}

void AAGXEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


