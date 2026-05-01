#include "AIEnemy.h"
#include "EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "TopDownCharacter.h"
#include "Damageable.h"
#include "Net/UnrealNetwork.h"
#include "WaveManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyAnimInstance.h"
#include "Components/CapsuleComponent.h"


AAIEnemy::AAIEnemy()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// smooth rotation when moving
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	RightHandCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandCollision"));
	RightHandCollision->SetupAttachment(GetMesh(), TEXT("hand_r"));
	RightHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	RightHandCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	LeftHandCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandCollision"));
	LeftHandCollision->SetupAttachment(GetMesh(), TEXT("hand_l"));
	LeftHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	LeftHandCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AAIEnemy::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;

	RightHandCollision->OnComponentBeginOverlap.AddDynamic(this, &AAIEnemy::OnHandOverlap);
	LeftHandCollision->OnComponentBeginOverlap.AddDynamic(this, &AAIEnemy::OnHandOverlap);

	// set attack range in blackboard
	if (AEnemyAIController* AICon = Cast<AEnemyAIController>(GetController()))
	{
		AICon->SetAttackRange(AttackRange);
	}
}

void AAIEnemy::EnableHandCollision()
{
	HitActors.Empty();
	RightHandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LeftHandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AAIEnemy::DisableHandCollision()
{
	HitActors.Empty();
	RightHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAIEnemy::OnHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;

	if (OtherActor && OtherActor != this && !HitActors.Contains(OtherActor))
	{
		ATopDownCharacter* PlayerCharacter = Cast<ATopDownCharacter>(OtherActor);
		if (PlayerCharacter)
		{
			HitActors.Add(OtherActor);
			PlayerCharacter->ApplyDamage(AttackDamage);
		}
	}
}

void AAIEnemy::TakeDamage(float DamageAmount)
{
	if (!HasAuthority()) return;

	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0.0f && !IsDead)
	{
		IsDead = true;
		OnRep_IsDead();

		AEnemyAIController* AICon = Cast<AEnemyAIController>(GetController());

		if (AICon)
		{
			AICon->OnEnemyDeath();
		}

		// BroadCast we died so managers can safely hear it
		OnEnemyDiedDelegate.Broadcast(this);

		// STOP movement
		GetCharacterMovement()->DisableMovement();
	}
}

void AAIEnemy::Multicast_PlayAttackMontage_Implementation(UAnimMontage* Montage)
{
	if (Montage)
	{
		PlayAnimMontage(Montage);
	}
}

void AAIEnemy::OnRep_IsDead()
{
	if (IsDead)
	{
		StopAnimMontage();
		if (UEnemyAnimInstance* AnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance()))
		{
			AnimInstance->bIsDead = true;
		}

		// Disable all enemy collision and hitboxes so we don't accidentally hit the player after death
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		RightHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		LeftHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AAIEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAIEnemy, CurrentHealth);
	DOREPLIFETIME(AAIEnemy, IsDead);
}

