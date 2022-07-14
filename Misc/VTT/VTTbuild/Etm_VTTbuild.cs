
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleEtm : IModule
  {
    public string Name
    {
      get { return "Etm"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Etm"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string etmDir = System.IO.Path.Combine(model.Directory.BSW, "Etm");
        string genDataDir = model.Directory.GenData;
        
        model.Project.AddIncludeDirectory(etmDir);
        
        model.Project.AddBswFile(System.IO.Path.Combine(etmDir, "Etm.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(etmDir, "Etm.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(etmDir, "Etm_Cbk.h"), Name);
        if (System.IO.File.Exists(System.IO.Path.Combine(etmDir, "Etm_Priv.h")))
          model.Project.AddBswFile(System.IO.Path.Combine(etmDir, "Etm_Priv.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(etmDir, "Etm_Types.h"), Name);
        if (System.IO.File.Exists(System.IO.Path.Combine(etmDir, "Etm_Types_Priv.h")))
          model.Project.AddBswFile(System.IO.Path.Combine(etmDir, "Etm_Types_Priv.h"), Name);
        
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Etm_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Etm_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Etm_Lcfg.h"), Name);
      }
    }
  }
}
