// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

/**
 * 
 */
/*class containing the SlateStyle for the Mirror Animation System,
Adds the ability to use the Mirror Animation System's icons stored in the "resources" folder inside the plugin's directory*/

class FMirrorAnimationSystemStyle
{
public:
	static void Initialize();

	static void Shutdown();
	
	static FName GetStyleSetName();

private:
	static TSharedRef< class FSlateStyleSet > Create();

private:

	static TSharedPtr< class FSlateStyleSet > StyleInstance;
};
