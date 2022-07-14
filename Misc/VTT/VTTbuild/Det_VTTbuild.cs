
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleDet : IModule
  {
    public string Name
    {
      get { return "Det"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Det"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string detDir = System.IO.Path.Combine(model.Directory.BSW, "Det");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(detDir);

        model.Project.AddBswFile(System.IO.Path.Combine(detDir, "Det.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(detDir, "Det.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Det_cfg.h"), Name);

        if (System.IO.File.Exists(System.IO.Path.Combine(genDataDir, "Det_cfg.c")))
        {
          model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Det_cfg.c"), Name);
        }
      }
    }
  }
}

