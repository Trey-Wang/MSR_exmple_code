
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleFiM : IModule
  {
    public string Name
    {
      get { return "FiM"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/FiM"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string fimDir = System.IO.Path.Combine(model.Directory.BSW, "FiM");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(fimDir);

        model.Project.AddBswFile(System.IO.Path.Combine(fimDir, "FiM.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(fimDir, "FiM.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(fimDir, "FiM_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FiM_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FiM_Lcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FiM_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FiM_PBcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FiM_PBcfg.c"), Name);
      }
    }
  }
}

