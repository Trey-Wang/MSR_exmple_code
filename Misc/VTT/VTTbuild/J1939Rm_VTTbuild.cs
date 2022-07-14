
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleJ1939Rm : IModule
  {
    public string Name
    {
      get { return "J1939Rm"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/J1939Rm"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string j1939rmDir = System.IO.Path.Combine(model.Directory.BSW, "J1939Rm");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(j1939rmDir);

        model.Project.AddBswFile(System.IO.Path.Combine(j1939rmDir, "J1939Rm.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(j1939rmDir, "J1939Rm.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(j1939rmDir, "J1939Rm_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(j1939rmDir, "J1939Rm_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "J1939Rm_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "J1939Rm_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "J1939Rm_Lcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "J1939Rm_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "J1939Rm_PBcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "J1939Rm_PBcfg.c"), Name);
      }
    }
  }
}

