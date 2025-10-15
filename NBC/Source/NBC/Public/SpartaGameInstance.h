
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SpartaGameInstance.generated.h"


UCLASS()
class NBC_API USpartaGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	USpartaGameInstance();

	// ���� ��ü ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;
	// ���� ���� �ε��� (GameState������ ������ �� ������, �� ��ȯ �Ŀ��� �츮�� �ʹٸ� GameInstance�� ������ �� ����)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex;

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount);
};
