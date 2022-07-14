
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleEcuM : IModule
  {
    public string Name
    {
      get { return "EcuM"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/EcuM"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string ecumDir = System.IO.Path.Combine(model.Directory.BSW, "EcuM");
        string genDataDir = model.Directory.GenData;
        string sourceDir = model.Directory.Source;

        model.Project.AddIncludeDirectory(ecumDir);

        model.Project.AddBswFile(System.IO.Path.Combine(ecumDir, "EcuM.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(ecumDir, "EcuM.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(ecumDir, "EcuM_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(ecumDir, "EcuM_Error.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "EcuM_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "EcuM_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "EcuM_Generated_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "EcuM_Init_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "EcuM_Init_PBcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "EcuM_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "EcuM_PrivateCfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(sourceDir, "EcuM_Callout_Stubs.c"), Name);

        string ecum_init_cfgH = System.IO.Path.Combine(genDataDir, "EcuM_Init_Cfg.h");
        string ecum_init_cfgC = System.IO.Path.Combine(genDataDir, "EcuM_Init_Cfg.c");
        if (System.IO.File.Exists(ecum_init_cfgC))
        {
            model.Project.AddBswFile(ecum_init_cfgC, Name);
            model.Project.AddBswFile(ecum_init_cfgH, Name);
        }
      }
    }
  }
}

