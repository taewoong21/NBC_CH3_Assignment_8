
#include "MineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AMineItem::AMineItem()
{
	ExplosionDelay = 2.0f;
	ExplosionRadius = 300.0f;
	ExplosionDamage = 50.0f;

	ItemType = "Mine";

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(Scene);
}

void AMineItem::ActivateItem(AActor* Activator)
{
	// Ÿ�̸� ���� -> ���� ����
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AMineItem::Explode, ExplosionDelay);
}

void AMineItem::Explode()
{
	TArray<AActor*> OverlappingActors;
	ExplosionCollision->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Player damaged %d by MineItem"), ExplosionDamage));
			UGameplayStatics::ApplyDamage(
				Actor, // �������� ���� ����
				ExplosionDamage, // ���� ������
				nullptr,	// �������� ����(��ġ)�� ��ü
				this,	// �������� ������ ������Ʈ
				UDamageType::StaticClass() // ������ ����
			);
		}
	}
	DestroyItem();
}