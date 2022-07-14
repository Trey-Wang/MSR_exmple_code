
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleIpduM : IModule
  {
    public string Name
    {
      get { return "IpduM"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/IpduM"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string ipdumDir = System.IO.Path.Combine(model.Directory.BSW, "IpduM");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(ipdumDir);

        model.Project.AddBswFile(System.IO.Path.Combine(ipdumDir, "IpduM.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(ipdumDir, "IpduM.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(ipdumDir, "IpduM_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "IpduM_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "IpduM_Lcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "IpduM_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "IpduM_PBcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "IpduM_PBcfg.c"), Name);
      }
    }
  }
}

