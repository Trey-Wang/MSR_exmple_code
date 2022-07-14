
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleFrSM : IModule
  {
    public string Name
    {
      get { return "FrSM"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/FrSM"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string frsmDir = System.IO.Path.Combine(model.Directory.BSW, "FrSM");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(frsmDir);

        model.Project.AddBswFile(System.IO.Path.Combine(frsmDir, "FrSM_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(frsmDir, "FrSM.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(frsmDir, "FrSM.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FrSM_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FrSM_Lcfg.c"), Name);
      }
    }
  }
}

