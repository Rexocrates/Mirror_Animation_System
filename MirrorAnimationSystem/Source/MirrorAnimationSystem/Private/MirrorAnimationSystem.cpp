// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MirrorAnimationSystem.h"

#define LOCTEXT_NAMESPACE "FMirrorAnimationSystemModule"

void FMirrorAnimationSystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
#if WITH_EDITOR
	FModuleManager::Get().LoadModule(TEXT("BlueprintGraph"));
	//FModuleManager::Get().LoadModule(TEXT("AnimGraph"));
	FModuleManager::Get().LoadModule(TEXT("MirrorAnimationSystemEditor"));
#endif
}

void FMirrorAnimationSystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMirrorAnimationSystemModule, MirrorAnimationSystem)