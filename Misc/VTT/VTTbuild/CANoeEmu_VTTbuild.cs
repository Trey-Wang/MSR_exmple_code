
using System.Collections.Generic;
using System.Linq;


namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVTTCANoeEmu : IModule
  {
    public string Name
    {
      get { return "CANoeEmu"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTOs"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      string CANoeEmuLibDir = System.IO.Path.Combine(model.Directory.BSW, "VttCANoeEmu");
      string genDataDir = model.Directory.GenData;

      model.Project.AddIncludeDirectory(CANoeEmuLibDir);
      model.Project.AddLibraryDirectory(CANoeEmuLibDir);

      switch (model.Project.Compiler)
      {
        case CompilerVersion.eVC100:
          model.Project.AddLibrary("CanoeEmu_VC100_R.lib", ProjectConfiguration.eRelease);
          model.Project.AddLibrary("CanoeEmu_VC100_D.lib", ProjectConfiguration.eDebug);
          break;
        case CompilerVersion.eVC120:
          model.Project.AddLibrary("CanoeEmu_VC120_R.lib", ProjectConfiguration.eRelease);
          model.Project.AddLibrary("CanoeEmu_VC120_D.lib", ProjectConfiguration.eDebug);
          break;
#if (VTT_VC141)
        case CompilerVersion.eVC141:
          model.Project.AddLibrary("CanoeEmu_VC141_R.lib", ProjectConfiguration.eRelease);
          model.Project.AddLibrary("CanoeEmu_VC141_D.lib", ProjectConfiguration.eDebug);
          break;
#endif
        default:
          throw new System.Exception("Compiler Version: The selected compiler version is not supported by CANoeEmu library");
      }

      model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "CANoeEmu_cfg.c"), Name);
	  if (System.IO.File.Exists(System.IO.Path.Combine(CANoeEmuLibDir, "CANoeEmu_DllMain.cpp")))
      	model.Project.AddBswFile(System.IO.Path.Combine(CANoeEmuLibDir, "CANoeEmu_DllMain.cpp"), Name);
      model.Project.AddBswFile(System.IO.Path.Combine(CANoeEmuLibDir, "CANoeAPI.h"), Name);
      model.Project.AddBswFile(System.IO.Path.Combine(CANoeEmuLibDir, "CANoeEmuProcessor.h"), Name);
      if (System.IO.File.Exists(System.IO.Path.Combine(CANoeEmuLibDir, "CANoeEmuLin.h")))
         model.Project.AddBswFile(System.IO.Path.Combine(CANoeEmuLibDir, "CANoeEmuLin.h"), Name);
      model.Project.SetModuleDefinitionFile(System.IO.Path.Combine(CANoeEmuLibDir, "CANoeEmu.def"));
    }
  }
}

