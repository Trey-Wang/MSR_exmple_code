
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleFvM : IModule
  {
    public string Name
    {
      get { return "FvM"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/FvM"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string FvMDir = System.IO.Path.Combine(model.Directory.BSW, "FvM");
        string genDataDir = model.Directory.GenData;
        
        model.Project.AddIncludeDirectory(FvMDir);
        
        model.Project.AddBswFile(System.IO.Path.Combine(FvMDir, "FvM.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(FvMDir, "FvM.h"), Name);

        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FvM_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FvM_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FvM_Lcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FvM_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FvM_PBcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FvM_Types.h"), Name);
      }
    }
  }
}
