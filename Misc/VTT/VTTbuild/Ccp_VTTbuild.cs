
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCcp : IModule
  {
    public string Name
    {
      get { return "Ccp"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Ccp"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string ccpDir = System.IO.Path.Combine(model.Directory.BSW, "Ccp");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(ccpDir);

        model.Project.AddBswFile(System.IO.Path.Combine(ccpDir, "Ccp.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(ccpDir, "Ccp.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(ccpDir, "Ccp_Tl.c"), Name);

        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Ccp_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Ccp_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Ccp_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Ccp_PBcfg.h"), Name);
      }
    }
  }
}

