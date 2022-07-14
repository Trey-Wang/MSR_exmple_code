
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleFrArTp : IModule
  {
    public string Name
    {
      get { return "FrArTp"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/FrArTp"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string frartpDir = System.IO.Path.Combine(model.Directory.BSW, "FrArTp");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(frartpDir);

        model.Project.AddBswFile(System.IO.Path.Combine(frartpDir, "FrArTp.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(frartpDir, "FrArTp.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(frartpDir, "FrArTp_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(frartpDir, "FrArTp_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FrArTp_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FrArTp_GlobCfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FrArTp_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FrArTp_Lcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FrArTp_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FrArTp_PBcfg.h"), Name);
      }
    }
  }
}

