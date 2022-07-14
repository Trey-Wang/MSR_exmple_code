
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVTTLin : IModule
  {
    public string Name
    {
      get { return "VTTLin"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTLin"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string linDir = System.IO.Path.Combine(model.Directory.BSW, "VttLin");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(linDir);

        model.Project.AddBswFile(System.IO.Path.Combine(linDir, "Lin.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(linDir, "Lin.h"), Name);
        if (System.IO.File.Exists(System.IO.Path.Combine(linDir, "Lin_Irq.c")))
        {
          model.Project.AddBswFile(System.IO.Path.Combine(linDir, "Lin_Irq.c"), Name);
        }
        model.Project.AddBswFile(System.IO.Path.Combine(linDir, "Lin_GeneralTypes.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(linDir, "Lin_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Lin_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Lin_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Lin_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Lin.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Lin.h"), Name);

        /* Add Lin Transceiver if available */
        /* Hint: The Lin Transceiver is added to the Lin Driver, as there is no static definition path for Lin Transceiver. */
        if (System.IO.File.Exists(System.IO.Path.Combine(genDataDir, "VttCntrl_LinTrcv.c")))
        {
          model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_LinTrcv.c"), Name);
        }

        model.CANoeEmu.AddStateChangeFunction("VttLin_OnStateChange", "VttCntrl_Lin.h");
      }
    }
  }
}

