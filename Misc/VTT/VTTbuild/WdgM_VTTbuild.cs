
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleWdgM : IModule
  {
    public string Name
    {
      get { return "WdgM"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/WdgM"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string wdgmDir = System.IO.Path.Combine(model.Directory.BSW, "WdgM");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(wdgmDir);

        model.Project.AddBswFile(System.IO.Path.Combine(wdgmDir, "WdgM.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(wdgmDir, "WdgM.h"), Name);
        if (System.IO.File.Exists(System.IO.Path.Combine(wdgmDir, "WdgM_Checkpoint.c")))
         model.Project.AddBswFile(System.IO.Path.Combine(wdgmDir, "WdgM_Checkpoint.c"), Name);
       
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "WdgM_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "WdgM_PBcfg.c"), Name);
        if (System.IO.File.Exists(System.IO.Path.Combine(genDataDir, "WdgM_PBcfg.h")))
         model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "WdgM_PBcfg.h"), Name);
        
        if (System.IO.File.Exists(System.IO.Path.Combine(genDataDir, "WdgM_PrivateCfg.h")))
         model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "WdgM_PrivateCfg.h"), Name);


         if (System.IO.File.Exists(System.IO.Path.Combine(genDataDir, "WdgM_Rte_Includes.h")))
         model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "WdgM_Rte_Includes.h"), Name);
       
        if (System.IO.File.Exists(System.IO.Path.Combine(genDataDir, "WdgM_Cfg_Features.h")))
         model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "WdgM_Cfg_Features.h"), Name);
        
     
      if (System.IO.File.Exists(System.IO.Path.Combine(genDataDir, "WdgM_OSMemMap.h")))
         model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "WdgM_OSMemMap.h"), Name);
     
      }
    }
  }
}

