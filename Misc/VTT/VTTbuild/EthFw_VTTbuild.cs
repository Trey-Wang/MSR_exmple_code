
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleEthFw : IModule
  {
    public string Name
    {
      get { return "EthFw"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/EthFw"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string staticDir = System.IO.Path.Combine(model.Directory.BSW, "EthFw");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(staticDir);

        model.Project.AddBswFile(System.IO.Path.Combine(staticDir, "EthFw.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(staticDir, "EthFw.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(staticDir, "EthFw_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(staticDir, "EthFw_Priv.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(staticDir, "EthFw_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "SchM_EthFw_Type.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "EthFw_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "EthFw_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "EthFw_Lcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "EthFw_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "EthFw_PBcfg.h"), Name);
      }
    }
  }
}

