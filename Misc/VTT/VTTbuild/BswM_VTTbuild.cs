
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleBswM : IModule
  {
    public string Name
    {
      get { return "BswM"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/BswM"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string bswmDir = System.IO.Path.Combine(model.Directory.BSW, "BswM");
        string genDataDir = model.Directory.GenData;
        string sourceDir = model.Directory.Source;
        
        model.Project.AddIncludeDirectory(bswmDir);

        model.Project.AddBswFile(System.IO.Path.Combine(bswmDir, "BswM.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(bswmDir, "BswM.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "BswM_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "BswM_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "BswM_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "BswM_Private_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(sourceDir, "BswM_Callout_Stubs.c"), Name);
      }
    }
  }
}

