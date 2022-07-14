
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleStbM : IModule
  {
    public string Name
    {
      get { return "StbM"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/StbM"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string stbmDir = System.IO.Path.Combine(model.Directory.BSW, "StbM");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(stbmDir);

        model.Project.AddBswFile(System.IO.Path.Combine(stbmDir, "StbM.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(stbmDir, "StbM.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "StbM_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "StbM_Cfg.h"), Name);
      }
    }
  }
}

