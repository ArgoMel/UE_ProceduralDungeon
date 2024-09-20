// Copyright Epic Games, Inc. All Rights Reserved.

#include "Pixel3D.h"

#define LOCTEXT_NAMESPACE "FPixel3DModule"

void FPixel3DModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FPixel3DModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPixel3DModule, Pixel3D)