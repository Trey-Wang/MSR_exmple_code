
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCsm : IModule
  {
    public string Name
    {
      get { return "Csm"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Csm"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string csmDir = System.IO.Path.Combine(model.Directory.BSW, "Csm");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(csmDir);

        model.Project.AddBswFile(System.IO.Path.Combine(csmDir, "Csm.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(csmDir, "Csm.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(csmDir, "Csm_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(csmDir, "Csm_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Csm_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Csm_Cfg.h"), Name);

        if (System.IO.File.Exists(System.IO.Path.Combine(genDataDir, "Csm_PBcfg.c")))
        {
          model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Csm_PBcfg.c"), Name);
        }
      }
    }
  }
}

