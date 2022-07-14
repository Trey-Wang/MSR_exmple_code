
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVTTCan : IModule
  {
    public string Name
    {
      get { return "VTTCan"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTCan"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string canDir = System.IO.Path.Combine(model.Directory.BSW, "VttCan");
        string genDataDir = model.Directory.GenData;
        string vttCntrlDir = System.IO.Path.Combine(model.Directory.BSW, "VttCntrl");

        model.Project.AddIncludeDirectory(canDir);

        model.Project.AddBswFile(System.IO.Path.Combine(canDir, "Can.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(canDir, "Can_Local.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(canDir, "Can.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(canDir, "Can_Irq.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(canDir, "Can_Hooks.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Can.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Can.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Can_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Can_DrvGeneralTypes.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Can_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Can_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Can_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Can_Cfg.h"), Name);

        /* Add Can Transceiver if available */
        /* Hint: The Can Transceiver is added to the Can Driver, as there is no static definition path for Can Transceiver. */
        if (System.IO.File.Exists(System.IO.Path.Combine(genDataDir, "VttCntrl_CanTrcv.c")))
        {
          model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_CanTrcv.c"), Name);
        }
        model.CANoeEmu.AddStateChangeFunction("VttCan_OnStateChange", "VttCntrl_Can.h");
      }
    }
  }
}

