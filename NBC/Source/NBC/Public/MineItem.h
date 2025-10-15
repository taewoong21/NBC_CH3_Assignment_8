// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "MineItem.generated.h"

UCLASS()
class NBC_API AMineItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	AMineItem();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* ExplosionCollision; // Æø¹ß ¹üÀ§ (ÄÝ¸®Àü)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	float ExplosionDelay; // Æø¹ß µô·¹ÀÌ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	float ExplosionRadius; // Æø¹ß ¹üÀ§
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	float ExplosionDamage; // Æø¹ß µ¥¹ÌÁö

	FTimerHandle ExplosionTimerHandle; // Æø¹ß ¿©ºÎ

	virtual void ActivateItem(AActor* Activator) override;

	void Explode();
};
