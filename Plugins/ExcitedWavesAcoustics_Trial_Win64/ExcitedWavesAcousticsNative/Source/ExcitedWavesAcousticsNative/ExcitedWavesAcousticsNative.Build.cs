using UnrealBuildTool;

public class ExcitedWavesAcousticsNative : ModuleRules
{
    public ExcitedWavesAcousticsNative(ReadOnlyTargetRules Target) : base(Target)
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
                "AudioMixer",
                "ExcitedWavesAcoustics"
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
