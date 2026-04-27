using UnrealBuildTool;
using System;
using System.IO;

public class ExcitedWavesAcousticsSDK : ModuleRules
{
    public ExcitedWavesAcousticsSDK(ReadOnlyTargetRules Target) : base(Target)
    {
		bUsePrecompiled = true;
        PrecompileForTargets = PrecompileTargetsType.Any;

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
            }
        );

        PrivateIncludePaths.AddRange(
            new string[] {
            }
        );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
            }
        );

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
            }
        );

        SharedPCHHeaderFile = "Public/ExcitedWavesAcousticsSDK.h";

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            string pluginDir = Path.GetFullPath(Path.Combine(ModuleDirectory, "..", ".."));
            string binariesDir = Path.Combine(pluginDir, "Binaries", "Win64");

            UnrealTargetConfiguration ueConfig = Target.Configuration;
            UnrealTargetConfiguration sdkFolderConfig = ueConfig;
            if (ueConfig == UnrealTargetConfiguration.DebugGame || ueConfig == UnrealTargetConfiguration.Debug)
            {
                sdkFolderConfig = UnrealTargetConfiguration.Development;
            }

            string ueConfigName = ueConfig.ToString();
            string sdkFolderName = sdkFolderConfig.ToString();
            string configDir = Path.Combine(binariesDir, sdkFolderName);
            string resolvedLibPath = Path.Combine(configDir, "ExcitedWavesAcousticsSDK.lib");
            string resolvedDllPath = Path.Combine(configDir, "ExcitedWavesAcousticsSDK.dll");

            if (!File.Exists(resolvedLibPath))
            {
                string mappingNote = (ueConfigName != sdkFolderName)
                    ? "  UBT configuration: " + ueConfigName + " (uses SDK folder: " + sdkFolderName + ")\n"
                    : "  UBT configuration: " + ueConfigName + "\n";
                string msg =
                    "ExcitedWavesAcousticsSDK: Missing native SDK for this build configuration.\n" +
                    "  Expected: " + resolvedLibPath + "\n" +
                    mappingNote +
                    "  Build the SDK, e.g. run:\n" +
                    "    Plugins\\ExcitedWavesAcoustics\\SDK\\build_sdk.bat --static\n" +
                    "  (produces Binaries\\Win64\\Development, Release, Shipping).";
                Console.WriteLine(msg);
                throw new BuildException(msg);
            }

            PublicAdditionalLibraries.Add(resolvedLibPath);
            if (ueConfigName != sdkFolderName)
            {
                Console.WriteLine(
                    "ExcitedWavesAcousticsSDK: UBT " + ueConfigName +
                    " -> SDK folder " + sdkFolderName + ": " + resolvedLibPath);
            }
            else
            {
                Console.WriteLine($"ExcitedWavesAcousticsSDK: Using library: {resolvedLibPath}");
            }

            bool bIsStatic = !File.Exists(resolvedDllPath);

            if (bIsStatic)
            {
                PublicDefinitions.Add("EWSDK_STATIC=1");
                Console.WriteLine("ExcitedWavesAcousticsSDK: Static linking mode (no DLL)");
            }
            else
            {
                PublicDelayLoadDLLs.Add("ExcitedWavesAcousticsSDK.dll");
                RuntimeDependencies.Add(resolvedDllPath, StagedFileType.NonUFS);
                Console.WriteLine($"ExcitedWavesAcousticsSDK: Dynamic linking mode, DLL: {resolvedDllPath}");
            }
        }
        else
        {
            Console.WriteLine("ExcitedWavesAcousticsSDK: platform not supported for external SDK.");
        }
    }
}
