// Copyright Stuart Hancock

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraCharacterEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacterEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterEnemy();
	
	virtual void HightlightActor() override;

	virtual void UnHightlightActor() override;

	
};
