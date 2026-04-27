using UnrealBuildTool;

public class ExcitedWavesAcoustics : ModuleRules
{
    public ExcitedWavesAcoustics(ReadOnlyTargetRules Target) : base(Target)
    {
		bUsePrecompiled = true;
        PrecompileForTargets = PrecompileTargetsType.Any;

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
			}
        );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "DeveloperSettings",
                "AudioMixer",
                "AudioExtensions"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Slate",
                "SlateCore",
                "ExcitedWavesAcousticsSDK",
                "PhysicsCore",
                "Projects",
                "Json"
            }
        );

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
			}
        );
    }
}
