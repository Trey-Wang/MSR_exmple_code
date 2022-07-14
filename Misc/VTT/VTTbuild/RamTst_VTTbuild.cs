
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleRamTst : IModule
  {
    public string Name
    {
      get { return "RamTst"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/RamTst"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string ramtstDir = System.IO.Path.Combine(model.Directory.BSW, "RamTst");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(ramtstDir);

        model.Project.AddBswFile(System.IO.Path.Combine(ramtstDir, "RamTst.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(ramtstDir, "RamTst.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "RamTst_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "RamTst_LCfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "RamTst_PrivateCfg.h"), Name);
      }
    }
  }
}

