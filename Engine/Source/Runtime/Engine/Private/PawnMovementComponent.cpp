// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "EnginePrivate.h"
#include "GameFramework/PawnMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogPawnMovementComponent, Log, All);


//----------------------------------------------------------------------//
// UPawnMovementComponent
//----------------------------------------------------------------------//
UPawnMovementComponent::UPawnMovementComponent(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
}

void UPawnMovementComponent::SetUpdatedComponent(UPrimitiveComponent* NewUpdatedComponent)
{
	if (NewUpdatedComponent)
	{
		if (!ensureMsgf(Cast<APawn>(NewUpdatedComponent->GetOwner()), TEXT("%s must update a component owned by a Pawn"), *GetName()))
		{
			return;
		}
	}

	Super::SetUpdatedComponent(NewUpdatedComponent);

	PawnOwner = NewUpdatedComponent ? CastChecked<APawn>(NewUpdatedComponent->GetOwner()) : NULL;
}

APawn* UPawnMovementComponent::GetPawnOwner() const
{
	return PawnOwner;
}

bool UPawnMovementComponent::IsMoveInputIgnored() const
{
	if (UpdatedComponent)
	{
		if (PawnOwner)
		{
			return PawnOwner->IsMoveInputIgnored();
		}
	}

	// No UpdatedComponent or Pawn, no movement.
	return true;
}

void UPawnMovementComponent::AddInputVector(FVector WorldAccel, bool bForce /*=false*/)
{
	if (PawnOwner)
	{
		PawnOwner->Internal_AddMovementInput(WorldAccel, bForce);
	}
}

FVector UPawnMovementComponent::GetInputVector() const
{
	return PawnOwner ? PawnOwner->Internal_GetMovementInputVector() : FVector::ZeroVector;
}

FVector UPawnMovementComponent::ConsumeInputVector()
{
	return PawnOwner ? PawnOwner->Internal_ConsumeMovementInputVector() : FVector::ZeroVector;
}
