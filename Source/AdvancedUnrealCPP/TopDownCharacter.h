#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "HealInterface.h"
#include "Weapon.h"
#include "TopDownCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UUserWidget;
class UChildActorComponent;
class UAnimMontage;

UCLASS()
class ADVANCEDUNREALCPP_API ATopDownCharacter : public ACharacter, public IHealInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATopDownCharacter();

    UPROPERTY(EditAnywhere)
    TSubclassOf<UUserWidget> HealthWidgetClass;

    UUserWidget* HealthWidget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    UChildActorComponent* WeaponComponent;

    UPROPERTY(ReplicatedUsing = OnRep_Health, EditAnywhere, BlueprintReadWrite, Category = "Health")
    float Health = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth = 100.f;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Heal_Implementation(float Amount) override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION()
    void MyHeal(float Amount);

    // 3 hit combo system
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TArray<UAnimMontage*> AttackMontages;

    UPROPERTY(BlueprintReadWrite, Category = "Combat")
    int32 ComboIndex = 0;

    UPROPERTY(BlueprintReadWrite, Category = "Combat")
    bool bIsAttacking = false;

    UPROPERTY(BlueprintReadWrite, Category = "Combat")
    bool bCanCombo = false;

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void Attack();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void EnableCombo();

    void PlayAttackMontageShared(int32 Index);

    UFUNCTION(Server, Reliable)
    void Server_PlayAttackMontage(int32 Index);

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_PlayAttackMontage(int32 Index);

    UFUNCTION()
    void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

    // Same with healing and damaging as sprinting
    UFUNCTION()
    void OnRep_Health();

    UFUNCTION(Server, Reliable)
    void Server_ApplyDamage(float Damage);
    
    UFUNCTION(Server, Reliable)
    void Server_Heal(float Amount);

    void ApplyDamage(float Damage);

    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void DoLook(float Yaw, float Pitch);

private:

    // Camera
    UPROPERTY(VisibleAnywhere)
    USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere)
    UCameraComponent* Camera;

    // Input
    UPROPERTY(EditAnywhere, Category = "Input")
    UInputMappingContext* MappingContext;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* SprintAction;

    /** Look Input Action */
    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* LookAction;

    /** Mouse Look Input Action */
    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* MouseLookAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* AttackAction;

    // Movement
    UPROPERTY(EditAnywhere, Category = "Movement")
    float BaseSpeed = 400.f;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float SprintMultiplier = 2.f;


    bool bIsSprinting = false;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

    void StartSprint();
    void StopSprint();

    // sprinting wasn't working on client so I had to update bIsSprinting on the server
    UFUNCTION(Server, Reliable)
    void Server_StartSprint();

    UFUNCTION(Server, Reliable)
    void Server_StopSprint();


};
