
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCryIf : IModule
  {
    public string Name
    {
      get { return "CryIf"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/CryIf"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string cryIfDir = System.IO.Path.Combine(model.Directory.BSW, "CryIf");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(cryIfDir);

        model.Project.AddBswFile(System.IO.Path.Combine(cryIfDir, "CryIf.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(cryIfDir, "CryIf.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(cryIfDir, "CryIf_Cbk.h"), Name);

        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "CryIf_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "CryIf_Cfg.h"), Name);
        
        if (System.IO.File.Exists(System.IO.Path.Combine(genDataDir, "CryIf_PBcfg.c")))
        {
          model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "CryIf_PBcfg.c"), Name);
        }
      }
    }
  }
}

