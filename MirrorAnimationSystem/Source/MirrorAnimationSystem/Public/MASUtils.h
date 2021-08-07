// Copyright 2017-2021 Rexocrates. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class MIRRORANIMATIONSYSTEM_API FMASUtils
{
public:
	static bool TwinSubstring(const FName BoneName, const FString Substring_A, const FString Substring_B, FName& OutTwinBoneName);
	static void CSMirrorSettings(const FReferenceSkeleton& RefSkeleton, 
		const EAxis::Type MirrorAxis, const FString Substring_A, const FString Substring_B,
		TArray<FIntPoint>& TwinPairs, TArray<int32>& NonTwinIDs, TArray<EAxis::Type>& NonTwinFlipAxis);
};
