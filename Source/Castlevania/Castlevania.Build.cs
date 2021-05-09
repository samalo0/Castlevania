// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

using System.IO;

public class Castlevania : ModuleRules
{
	public Castlevania(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Paper2D" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		if(Target.Platform == UnrealTargetPlatform.Android)
		{
			var manifestFile = Path.Combine(ModuleDirectory, "AndroidSanitizePermissions_UPL.xml");
			AdditionalPropertiesForReceipt.Add("AndroidPlugin", manifestFile);	
		}
	}
}
