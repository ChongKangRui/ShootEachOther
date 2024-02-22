// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ShootEachOther : ModuleRules
{
    public ShootEachOther(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
                "ShootEachOther"
            }
        );

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreOnline",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "GameplayTags",
            "GameplayTasks",
            "GameplayAbilities",
            "AIModule",
            "DataRegistry",
            "NetCore",
            "OnlineSubsystem",
            "OnlineSubsystemNull",
            "OnlineSubsystemSteam",
            "ReplicationGraph",
            "UMG",
            "Niagara"});
    }
}
