// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARotatingDoor.generated.h"

UCLASS()
class ADVANCEDUNREALCPP_API AARotatingDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARotatingDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	// Door mesh
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* DoorMesh;

	// Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OpenAngle = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OpenSpeed = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsOpen = false;

private:
	float CurrentYaw;
};
