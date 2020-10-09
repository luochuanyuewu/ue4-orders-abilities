#include "AbilitySystem/RTSAttackAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Net/UnrealNetwork.h"

#include "AbilitySystem/RTSGlobalTags.h"


const float URTSAttackAttributeSet::MAX_DAMAGE = 10000.0f;
const float URTSAttackAttributeSet::MAX_COOLDOWN = 10000.0f;
const float URTSAttackAttributeSet::MAX_RANGE = 10000.0f;
const float URTSAttackAttributeSet::MAX_OUTGOING_DAMAGE_MULTIPLIER = 10000.0f;


void URTSAttackAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // DOREPLIFETIME(UMyAttributeSet, MyAttribute);
    // Chances are this is how you would ordinarily do it, however in the case of attributes this'll lead
    // to confusing and annoying replication errors, usually involving clientside ability prediction.
    // https://wiki.unrealengine.com/GameplayAbilities_and_You#AttributeSet
    // This is how it is done properly for attributes.
    DOREPLIFETIME_CONDITION_NOTIFY(URTSAttackAttributeSet, Damage, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URTSAttackAttributeSet, Cooldown, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URTSAttackAttributeSet, Range, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URTSAttackAttributeSet, OutgoingDamageMultiplier, COND_None, REPNOTIFY_Always);
}

void URTSAttackAttributeSet::OnRep_Damage(const FGameplayAttributeData& OldDamage)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URTSAttackAttributeSet, Damage, OldDamage);
}

void URTSAttackAttributeSet::OnRep_Cooldown(const FGameplayAttributeData& OldCoolDown)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URTSAttackAttributeSet, Cooldown, OldCoolDown);
}

void URTSAttackAttributeSet::OnRep_Range(const FGameplayAttributeData& OldRange)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URTSAttackAttributeSet, Range, OldRange);
}

void URTSAttackAttributeSet::OnRep_OutgoingDamageMultiplier(const FGameplayAttributeData& OldOutgoingDamageMultiplier)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URTSAttackAttributeSet, OutgoingDamageMultiplier, OldOutgoingDamageMultiplier);
}


bool URTSAttackAttributeSet::ShouldInitProperty(bool FirstInit, FProperty* PropertyToInit) const
{
    // We do not want the health property to change when the attribute sets properties where initialized using a curve
    // table.
    return (PropertyToInit != GetOutgoingDamageMultiplierAttribute().GetUProperty());
}

void URTSAttackAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
    if (Attribute == GetDamageAttribute())
    {
        // Note that we can use game play effects to heal units so we can clamp this value with min == 0.
        NewValue = FMath::Clamp(NewValue, 0.0f, MAX_DAMAGE);
    }

    if (Attribute == GetCooldownAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, MAX_COOLDOWN);
    }

    if (Attribute == GetRangeAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, MAX_RANGE);
    }

    if (Attribute == GetOutgoingDamageMultiplierAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, MAX_OUTGOING_DAMAGE_MULTIPLIER);
    }
}

void URTSAttackAttributeSet::PostInitializeProperties(bool bInitialInit)
{
    if (bInitialInit)
    {
        OutgoingDamageMultiplier = 1.0f;
    }
}

void URTSAttackAttributeSet::GetDefaultStatusTags(FGameplayTagContainer& OutStatusTags) const
{
    OutStatusTags.AddTag(URTSGlobalTags::Status_Permanent_CanAttack());
}
