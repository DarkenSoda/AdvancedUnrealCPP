// Fill out your copyright notice in the Description page of Project Settings.


#include "ARotatingDoor.h"

// Sets default values
AARotatingDoor::AARotatingDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AARotatingDoor::BeginPlay()
{
	Super::BeginPlay();

	CurrentYaw = GetActorRotation().Yaw;
}

// Called every frame
void AARotatingDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	float TargetYaw = bIsOpen ? OpenAngle : 0.f;

	CurrentYaw = FMath::FInterpTo(
		CurrentYaw,
		TargetYaw,
		DeltaTime,
		OpenSpeed
	);

	FRotator NewRotation = DoorMesh->GetRelativeRotation();
	NewRotation.Yaw = CurrentYaw;

	DoorMesh->SetRelativeRotation(NewRotation);
}

