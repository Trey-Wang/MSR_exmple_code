
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVTTIcu : IModule
  {
    public string Name
    {
      get { return "VTTIcu"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTIcu"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string icuDir = System.IO.Path.Combine(model.Directory.BSW, "VttIcu");
        string vttCntrlDir = System.IO.Path.Combine(model.Directory.BSW, "VTTCntrl");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(icuDir);
        model.CANoeEmu.AddStateChangeFunction("VttIcu_OnStateChange", "VttCntrl_Icu.h");
        model.Project.AddBswFile(System.IO.Path.Combine(icuDir, "Icu_Irq.c"), Name);
        if (System.IO.File.Exists(System.IO.Path.Combine(icuDir, "Icu_Irq.h")))
        {
          model.Project.AddBswFile(System.IO.Path.Combine(icuDir, "Icu_Irq.h"), Name);
        }
        model.Project.AddBswFile(System.IO.Path.Combine(icuDir, "Icu.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(icuDir, "Icu.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Icu_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Icu_PBCfg.c"), Name);        
        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Icu.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Icu.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Icu_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Icu_Cfg.h"), Name);
      }
    }
  }
}

