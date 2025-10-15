#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"
#include "SpartaGameState.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"

// Sets default values
ASpartaCharacter::ASpartaCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/* ***** ī�޶� ***** */
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComps"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);

	/* ***** �̵� ���� �Ӽ� ***** */
	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.5f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed; // �ʱ� �̵� ���� (�ӵ�) -> �ȱ�

	// �ʱ� ü�� ����
	MaxHealth = 100.0f;
	Health = MaxHealth;
}

// Called when the game starts or when spawned
void ASpartaCharacter::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverheadHP();
	
}

// Called every frame
void ASpartaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASpartaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced Input Component�� ĳ����
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Input Action�� �������� ���� ���� ��Ʈ�ѷ��� SpartaController�� ĳ����
		if (ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				// Ű ���� �� Move() ȣ��
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::Move
				);
			}

			if (PlayerController->JumpAction)
			{
				// Ű ���� �� StartJump() ȣ��
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::StartJump
				);
				// Ű ���� �� StopJump() ȣ��
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&ASpartaCharacter::StopJump
				);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::Look
				);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::StartSprint
				);

				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&ASpartaCharacter::StopSprint
				);
			}
		}
	}
}


void ASpartaCharacter::Move(const FInputActionValue& value)
{
	// ��Ʈ�ѷ� ���� üũ
	// ���� ����� ���� ��Ʈ�ѷ� ���� Ȯ�� �ʼ�
	if (!Controller)
	{
		return;
	}

	// vlaue�� Axis2D�� ������ �Է°� ����
	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		// ĳ���Ͱ� �ٶ󺸴� ����(����)���� X�� �̵�
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		// ĳ������ ������ �������� Y�� �̵�
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void ASpartaCharacter::Look(const FInputActionValue& value)
{
	// ���콺�� X, Y ������ ��������
	FVector2D LookInput = value.Get<FVector2D>();

	// X�� �¿� ȸ�� (Yaw), Y�� ���� ȸ�� (Pitch)
	// �¿� ȸ��
	AddControllerYawInput(LookInput.X);
	// ���� ȸ��
	AddControllerPitchInput(LookInput.Y);
}

void ASpartaCharacter::StartJump(const FInputActionValue& value)
{
	// Pressed �϶� True
	if (value.Get<bool>())
	{
		// Character���� �⺻ �����Ǵ� Jump() �Լ�
		Jump();
	}
}

void ASpartaCharacter::StopJump(const FInputActionValue& value)
{
	// Completed �϶� False
	if (!value.Get<bool>())
	{
		// Character���� �⺻ �����Ǵ� StopJumping() �Լ�
		StopJumping();
	}
}

void ASpartaCharacter::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed; // �̵� ���� (�ӵ�) -> �ٱ�
	}
}

void ASpartaCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed; // �̵� ���� (�ӵ�) -> �ȱ�
	}
}

int32 ASpartaCharacter::GetHealth() const
{
	return Health;
}

void ASpartaCharacter::AddHealth(float Amount)
{
	// ü�� ȸ�� �� ȸ���� ���� (�ִ� ü�� �ʰ� X)
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
	UpdateOverheadHP();

	// UE_LOG(LogTemp, Log, TEXT("Health increased to: %f"), Health);
}

float ASpartaCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	UpdateOverheadHP();

	// UE_LOG(LogTemp, Warning, TEXT("Health decreased to: %f"), Health);

	if (Health <= 0.0f)
	{
		OnDeath();
	}

	return ActualDamage;
}

void ASpartaCharacter::OnDeath()
{
	//UE_LOG(LogTemp, Error, TEXT("Character is Dead!"));

	ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
	if (SpartaGameState)
	{
		SpartaGameState->OnGameOver();
	}
}

void ASpartaCharacter::UpdateOverheadHP()
{
	if (!OverheadWidget) return;

	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) return;

	if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
	{
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth)));
	}
}