
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleFwEth : IModule
  {
    public string Name
    {
      get { return "FwEth"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/FwEth"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string staticDir = System.IO.Path.Combine(model.Directory.BSW, "FwEth");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(staticDir);

        model.Project.AddBswFile(System.IO.Path.Combine(staticDir, "FwEth.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(staticDir, "FwEth.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(staticDir, "FwEth_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(staticDir, "FwEth_Priv.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(staticDir, "FwEth_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "SchM_FwEth_Type.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FwEth_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FwEth_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FwEth_Lcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FwEth_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FwEth_PBcfg.h"), Name);
      }
    }
  }
}

