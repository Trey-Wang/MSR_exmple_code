
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleWdgIf_TTTech : IModule
  {
    public string Name
    {
      get { return "WdgIf"; }
    }

    public string DefinitionPath
    {
      get { return "/TTTECH/WdgIf"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string wdgifDir = System.IO.Path.Combine(model.Directory.BSW, "WdgIf");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(wdgifDir);

        model.Project.AddBswFile(System.IO.Path.Combine(wdgifDir, "WdgIf.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(wdgifDir, "WdgIf.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(wdgifDir, "WdgIf_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "WdgIf_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "WdgIf_Cfg_Features.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "WdgIf_Lcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "WdgIf_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "WdgIf_MemMap.h"), Name);
      }
    }
  }
}

