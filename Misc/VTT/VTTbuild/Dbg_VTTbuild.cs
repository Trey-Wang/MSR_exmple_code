
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleDbg : IModule
  {
    public string Name
    {
      get { return "Dbg"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Dbg"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string dbgDir = System.IO.Path.Combine(model.Directory.BSW, "Dbg");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(dbgDir);

        model.Project.AddBswFile(System.IO.Path.Combine(dbgDir, "Dbg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(dbgDir, "Dbg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Dbg_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Dbg_Cfg.c"), Name);
      }
    }
  }
}

