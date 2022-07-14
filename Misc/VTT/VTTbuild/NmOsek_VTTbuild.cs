
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleNmOsek : IModule
  {
    public string Name
    {
      get { return "NmOsek"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/NmOsek"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string nmOsekDir = System.IO.Path.Combine(model.Directory.BSW, "NmOsek");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(nmOsekDir);

        model.Project.AddBswFile(System.IO.Path.Combine(nmOsekDir, "NmOsek.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(nmOsekDir, "NmOsek.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(nmOsekDir, "NmOsek_Cbk.h"), Name);

        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "NmOsek_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "NmOsek_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "NmOsek_PBcfg.c"), Name);
      }
    }
  }
}

