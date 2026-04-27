// Copyright Excited Waves. All Rights Reserved.

using UnrealBuildTool;

/**
 * ExcitedWavesAcousticsChaos Module
 * 
 * Chaos Destruction integration for Excited Waves Acoustics
 * 
 * UE5+ ONLY - automatically disabled in UE 4.27 (missing dependencies)
 */
public class ExcitedWavesAcousticsChaos : ModuleRules
{
    public ExcitedWavesAcousticsChaos(ReadOnlyTargetRules Target) : base(Target)
    {
		bUsePrecompiled = true;
        PrecompileForTargets = PrecompileTargetsType.Any;

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "ExcitedWavesAcoustics",          // Main plugin module
				"GeometryCollectionEngine",       // Chaos Destruction
				"ChaosSolverEngine",              // Chaos solver
				"Chaos"                           // Chaos physics
			}
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "DeveloperSettings"
            }
        );

        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "PropertyEditor",
                    "Slate",
                    "SlateCore"
                }
            );
        }
    }
}

