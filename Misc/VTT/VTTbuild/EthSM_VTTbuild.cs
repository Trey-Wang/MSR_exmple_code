
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleEthSM : IModule
  {
    public string Name
    {
      get { return "EthSM"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/EthSM"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string ethsmDir = System.IO.Path.Combine(model.Directory.BSW, "EthSM");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(ethsmDir);

        model.Project.AddBswFile(System.IO.Path.Combine(ethsmDir, "EthSM.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(ethsmDir, "EthSM.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(ethsmDir, "EthSM_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(ethsmDir, "EthSM_Priv.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "EthSM_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "EthSM_Lcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "EthSM_Lcfg.c"), Name);
      }
    }
  }
}

