#include "TopDownCharacter.h"
#include "EngineUtils.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Net/UnrealNetwork.h"
#include "HealthPickupDelegate.h"
#include "HealthWidget.h"


// Sets default values
ATopDownCharacter::ATopDownCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;


	// Spring arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.f;
	//SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));

	// To make the camera fixed and stop rotation
	//SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	//SpringArm->bInheritPitch = false;
	//SpringArm->bInheritYaw = false;
	//SpringArm->bInheritRoll = false;


	// Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;


	// movement defaults
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
}

// Called when the game starts or when spawned
void ATopDownCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}

	// Bind to health pickup delegate
	//for (TActorIterator<AHealthPickupDelegate> It(GetWorld()); It; ++It)
	//{
	//	if (*It)
	//	{
	//		It->OnPickedUp.AddDynamic(this, &ATopDownCharacter::MyHeal);
	//		UE_LOG(LogTemp, Warning, TEXT("Bound to health pickup delegate!"));
	//	}
	//}

	if (IsLocallyControlled())
	{
		HealthWidget = CreateWidget<UUserWidget>(Cast<APlayerController>(GetController()), HealthWidgetClass);

		if (HealthWidget)
			HealthWidget->AddToViewport();
	}

	OnRep_Health();
}

// Called to bind functionality to input
void ATopDownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopDownCharacter::Move);

	Input->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	Input->BindAction(SprintAction, ETriggerEvent::Started, this, &ATopDownCharacter::StartSprint);
	Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &ATopDownCharacter::StopSprint);
	Input->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ATopDownCharacter::Look);

	// Looking
	Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATopDownCharacter::Look);
}

void ATopDownCharacter::Move(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();

	if (Camera)
	{
		float Yaw = Camera->GetComponentRotation().Yaw;
		FRotator YawRotation(0.f, Yaw, 0.f);

		FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Forward, Input.Y);
		AddMovementInput(Right, Input.X);
	}
}

void ATopDownCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ATopDownCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

// server authoritave approach
void ATopDownCharacter::Server_StartSprint_Implementation()
{
	StartSprint();
}

void ATopDownCharacter::Server_StopSprint_Implementation()
{
	StopSprint();
}

void ATopDownCharacter::StartSprint()
{
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed * SprintMultiplier;

	if(!HasAuthority())
	{
		Server_StartSprint();
	}
}

void ATopDownCharacter::StopSprint()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;

	if(!HasAuthority())
	{
		Server_StopSprint();
	}
}

void ATopDownCharacter::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATopDownCharacter, Health);
}


// Health
void ATopDownCharacter::Heal_Implementation(float Amount)
{
	if (HasAuthority())
	{
		Health = FMath::Clamp(Health + Amount, 0.f, MaxHealth);
		OnRep_Health();
	}
	else
	{
		Server_Heal(Amount);
	}
}

void ATopDownCharacter::Server_Heal_Implementation(float Amount)
{
	// heals on the server
	MyHeal(Amount);
}

void ATopDownCharacter::MyHeal(float Amount)
{
	IHealInterface::Execute_Heal(this, Amount); // calls interface
}

void ATopDownCharacter::OnRep_Health()
{
	UE_LOG(LogTemp, Warning, TEXT("Health Updated: %f"), Health);

	float Percent = Health / MaxHealth;

	UHealthWidget* HW = Cast<UHealthWidget>(HealthWidget);

	if (HW)
	{
		HW->SetHealthPercent(Percent); // update local UI
	}
}

// Weapon
void ATopDownCharacter::EquipWeapon()
{
	if (!CurrentWeapon) {
		//Find weapon in the world and equip it
		for (TActorIterator<AWeapon> It(GetWorld()); It; ++It)
		{
			if (*It)
			{
				CurrentWeapon = *It;
				break;
			}
		}
	}

	if (!CurrentWeapon) return;

	CurrentWeapon->Equip();

	CurrentWeapon->AttachToComponent(
		GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		TEXT("HandGrip_R")
	);
}

void ATopDownCharacter::UnequipWeapon()
{
	if (!CurrentWeapon) return;

	CurrentWeapon->Unequip();

	CurrentWeapon->DetachFromActor(
		FDetachmentTransformRules::KeepWorldTransform
	);
}

// Damage
void ATopDownCharacter::ApplyDamage(float Damage)
{
	if (HasAuthority())
	{
		Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
		OnRep_Health();
	}
	else
	{
		Server_ApplyDamage(Damage);
	}
}

void ATopDownCharacter::Server_ApplyDamage_Implementation(float Damage)
{
	ApplyDamage(Damage);
}