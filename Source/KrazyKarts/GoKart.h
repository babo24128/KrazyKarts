// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKart.generated.h"


USTRUCT()
struct FGoKartMove
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float Throttle;

	UPROPERTY()
	float SteeringThrow;

	UPROPERTY()
	float DeltaTime;

	UPROPERTY()
	float Time;
};

USTRUCT()
struct FGorKartState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	FGoKartMove LastMove;
};

UCLASS()
class KRAZYKARTS_API AGoKart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGoKart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void SimulateMove(FGoKartMove Move);

	FGoKartMove CreateMove(float DeltaTime);
	void ClearAcknowledgeMoves(FGoKartMove LastMove);

	FVector GetAirResistance();
	FVector GetRollingResistance();

	void ApplyRotation(float DeltaTime, float SteeringThrow);

	void UpdateLocationFromVelocity(float DeltaTime);

	// ���� �߷�
	UPROPERTY(EditAnywhere)
	float Mass = 1000;

	// ����Ʋ�� ������ �������� �� ���� �������� ��
	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;

	// ������ ���� ���¿��� �ڵ��� ȸ�� �ݰ��� �ּ� ������(m)
	UPROPERTY(EditAnywhere)
	float MinTurningRadius = 10;

	// �������� �� ���� �׷�
	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16;

	// �������� ���������� ũ��
	UPROPERTY(EditAnywhere)
	float RollingResistanceCoefficient = 0.015;

	void MoveForward(float Value);
	void MoveRight(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(FGoKartMove Move);

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FGorKartState ServerState;

	FVector Velocity;

	UFUNCTION()
	void OnRep_ServerState();

	float Throttle;
	float SteeringThrow;

	TArray<FGoKartMove> UnacknowledgedMoves;
};
