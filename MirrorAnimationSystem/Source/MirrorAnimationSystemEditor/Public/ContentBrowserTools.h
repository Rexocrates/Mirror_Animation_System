// Copyright 2017-2021 Rexocrates. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
/**
 * 
 */
/*Class that adds the "Mirror AnimAsset" and the "Mirror Table From Skeleton" tool's buttons
when an Animation Asset or a Skeleton asset is right clicked*/
class FContentBrowserTools
{
public:
	static void InstallHooks();
	static void RemoveHooks();
};
