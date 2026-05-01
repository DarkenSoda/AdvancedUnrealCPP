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
#include "Components/ChildActorComponent.h"
#include "Weapon.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.h"
#include "Damageable.h"

// Sets default values
ATopDownCharacter::ATopDownCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	// Weapon Component
	WeaponComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("WeaponComponent"));
	WeaponComponent->SetupAttachment(GetMesh(), TEXT("HandGrip_R"));
	WeaponComponent->SetChildActorClass(AWeapon::StaticClass());

	// Spring arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.f;
	//SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));

	// To make the camera fixed and stop rotation
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
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 480.f);

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

	if (AttackAction)
	{
		Input->BindAction(AttackAction, ETriggerEvent::Started, this, &ATopDownCharacter::Attack);
	}

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

	if (!HasAuthority())
	{
		Server_StartSprint();
	}
}

void ATopDownCharacter::StopSprint()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;

	if (!HasAuthority())
	{
		Server_StopSprint();
	}
}

void ATopDownCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATopDownCharacter, Health);
	DOREPLIFETIME(ATopDownCharacter, bIsDead);
}

void ATopDownCharacter::Jump()
{
	if (bIsAttacking) return;
	Super::Jump();
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

// Combat
void ATopDownCharacter::Attack()
{
	if (GetCharacterMovement()->IsFalling()) return;
	if (AttackMontages.Num() == 0) return;

	if (bIsAttacking)
	{
		// Only allow combo if the notify fired AND we're not on the last hit
		if (bCanCombo && ComboIndex < AttackMontages.Num() - 1)
		{
			bCanCombo = false;
			ComboIndex++;
			PlayAttackMontageShared(ComboIndex);
		}
		// If on last hit or combo window not open — do nothing, let it finish
	}
	else
	{
		// Fresh attack
		bIsAttacking = true;
		bCanCombo = false;
		ComboIndex = 0;
		PlayAttackMontageShared(ComboIndex);
	}
}

void ATopDownCharacter::PlayAttackMontageShared(int32 Index)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontages.IsValidIndex(Index))
	{
		AnimInstance->Montage_Play(AttackMontages[Index]);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &ATopDownCharacter::OnAttackMontageEnded);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontages[Index]);
	}

	if (!HasAuthority())
	{
		Server_PlayAttackMontage(Index);
	}
	else
	{
		Multicast_PlayAttackMontage(Index);
	}
}

void ATopDownCharacter::Server_PlayAttackMontage_Implementation(int32 Index)
{
	Multicast_PlayAttackMontage(Index);
}

void ATopDownCharacter::Multicast_PlayAttackMontage_Implementation(int32 Index)
{
	if (IsLocallyControlled()) return;

	bIsAttacking = true;
	ComboIndex = Index;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontages.IsValidIndex(Index))
	{
		AnimInstance->Montage_Play(AttackMontages[Index]);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &ATopDownCharacter::OnAttackMontageEnded);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontages[Index]);
	}
}

void ATopDownCharacter::EnableCombo()
{
	bCanCombo = true;
}

void ATopDownCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!AttackMontages.IsValidIndex(ComboIndex)) return;

	DisableWeaponCollision(); // Ensure it's disabled if montage ends.

	// If interrupted (i.e. we blended into the next combo hit), ignore the reset
	if (bInterrupted) return;

	// Only reset if this is actually the current montage finishing naturally
	if (Montage == AttackMontages[ComboIndex])
	{
		bIsAttacking = false;
		ComboIndex = 0;
		bCanCombo = false;
	}
}

void ATopDownCharacter::EnableWeaponCollision()
{
	if (WeaponComponent && WeaponComponent->GetChildActor())
	{
		AWeapon* Weapon = Cast<AWeapon>(WeaponComponent->GetChildActor());
		if (Weapon)
		{
			Weapon->EnableWeapon();
		}
	}
}

void ATopDownCharacter::DisableWeaponCollision()
{
	if (WeaponComponent && WeaponComponent->GetChildActor())
	{
		AWeapon* Weapon = Cast<AWeapon>(WeaponComponent->GetChildActor());
		if (Weapon)
		{
			Weapon->DisableWeapon();
		}
	}
}

// Damage
void ATopDownCharacter::ApplyDamage(float Damage)
{
	if (HasAuthority())
	{
		Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
		OnRep_Health();

		if (Health <= 0.f && !bIsDead)
		{
			bIsDead = true;
			OnRep_IsDead();

			// Check if all players are dead
			bool bAllDead = true;
			for (TActorIterator<ATopDownCharacter> It(GetWorld()); It; ++It)
			{
				if (!It->bIsDead)
				{
					bAllDead = false;
					break;
				}
			}

			if (bAllDead)
			{
				OnAllPlayersDeadDelegate.Broadcast();
			}
		}
	}
	else
	{
		Server_ApplyDamage(Damage);
	}
}

void ATopDownCharacter::OnRep_IsDead()
{
	if (bIsDead)
	{
		StopAnimMontage();
		GetCharacterMovement()->DisableMovement();

		if (IsLocallyControlled())
		{
			if (APlayerController* PC = Cast<APlayerController>(GetController()))
			{
				DisableInput(PC);
			}
		}
	}
}

void ATopDownCharacter::Server_ApplyDamage_Implementation(float Damage)
{
	ApplyDamage(Damage);
}

void ATopDownCharacter::Server_DealDamage_Implementation(AActor* TargetActor, float Damage)
{
	if (TargetActor)
	{
		IDamageable* DamageableActor = Cast<IDamageable>(TargetActor);
		if (DamageableActor)
		{
			DamageableActor->TakeDamage(Damage);
		}
	}
}