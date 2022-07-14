
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleLinTp : IModule
  {
    public string Name
    {
      get { return "LinTp"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/LinTp"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string genDataDir = model.Directory.GenData;
        string linifDir = System.IO.Path.Combine(model.Directory.BSW, "LinIf");

        model.Project.AddBswFile(System.IO.Path.Combine(linifDir, "LinTp_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "LinTp_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "LinTp_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "LinTp_Cfg.h"), Name);
      }
    }
  }
}

