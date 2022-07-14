
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleNm : IModule
  {
    public string Name
    {
      get { return "Nm"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Nm"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string nmDir = System.IO.Path.Combine(model.Directory.BSW, "Nm");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(nmDir);

        model.Project.AddBswFile(System.IO.Path.Combine(nmDir, "Nm_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(nmDir, "Nm.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(nmDir, "Nm.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(nmDir, "NmStack_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Nm_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Nm_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Nm_Cfg.h"), Name);
      }
    }
  }
}

