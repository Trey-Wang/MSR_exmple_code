
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleWdgM_TTTech : IModule
  {
    public string Name
    {
      get { return "WdgM"; }
    }

    public string DefinitionPath
    {
      get { return "/TTTECH/WdgM"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string wdgmDir = System.IO.Path.Combine(model.Directory.BSW, "WdgM");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(wdgmDir);

        model.Project.AddBswFile(System.IO.Path.Combine(wdgmDir, "WdgM.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(wdgmDir, "WdgM_Checkpoint.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(wdgmDir, "WdgM.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "WdgM_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "WdgM_Cfg_Features.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "WdgM_PBcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "WdgM_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "WdgM_OSMemMap.h"), Name);
      }
    }
  }
}

