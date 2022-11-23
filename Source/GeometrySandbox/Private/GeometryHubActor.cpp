// Fill out your copyright notice in the Description page of Project Settings.


#include "GeometryHubActor.h"

DEFINE_LOG_CATEGORY_STATIC(LogGeometryHub, All, All);

// Sets default values
AGeometryHubActor::AGeometryHubActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGeometryHubActor::BeginPlay()
{
	Super::BeginPlay();

	SpawnGeometryActors();
}

// Called every frame
void AGeometryHubActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGeometryHubActor::SpawnGeometryActors()
{
	for (const FGeometryPayload& Payload : GeometryPayloads)
	{
		if (ABaseGeometryActor* GeometryActor = GetWorld()->SpawnActorDeferred<ABaseGeometryActor>(
			Payload.GeometryClass, Payload.InitialTransform))
		{
			GeometryActor->SetGeometryData(Payload.Data);
			GeometryActor->OnColorChanged.AddDynamic(this, &AGeometryHubActor::OnColorChanged);
			GeometryActor->OnTimerFinished.AddUObject(this, &AGeometryHubActor::OnTimerFinished);
			GeometryActor->FinishSpawning(Payload.InitialTransform);
		}
	}
}

void AGeometryHubActor::OnColorChanged(const FLinearColor& Color, const FString& Name)
{
	UE_LOG(LogGeometryHub, Display, TEXT("Actor name: %s | Color: %s"), *Name, *Color.ToString())
}

void AGeometryHubActor::OnTimerFinished(AActor* Actor)
{
	if(!Actor)
	{
		return;
	}
	UE_LOG(LogGeometryHub, Display, TEXT("Timer finished: %s"), *Actor->GetName());

	if(ABaseGeometryActor* GeometryActor = Cast<ABaseGeometryActor>(Actor))
	{
		GeometryActor->Destroy();
	}
}
