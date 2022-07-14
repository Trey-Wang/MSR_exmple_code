
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVTTFr : IModule
  {
    public string Name
    {
      get { return "VTTFr"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTFr"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string frDir = System.IO.Path.Combine(model.Directory.BSW, "VttFr");
        string genDataDir = model.Directory.GenData;
        string vttCntrlDir = System.IO.Path.Combine(model.Directory.BSW, "VttCntrl");

        model.Project.AddIncludeDirectory(frDir);

        model.Project.AddBswFile(System.IO.Path.Combine(frDir, "Fr.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(frDir, "Fr.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(frDir, "Fr_ERay.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(frDir, "Fr_Ext.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(frDir, "Fr_Irq.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(frDir, "Fr_Priv.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(frDir, "Fr_Timer.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Fr_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Fr_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Fr_PBcfg.c"), Name);

        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Fr.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Fr.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Fr_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Fr_Cfg.c"), Name);

        /* Add Fr Transceiver if available */
        /* Hint: The Fr Transceiver is added to the Fr Driver, as there is no static definition path for Fr Transceiver. */
        if (System.IO.File.Exists(System.IO.Path.Combine(genDataDir, "VttCntrl_FrTrcv.c")))
        {
          model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_FrTrcv.c"), Name);
        }

        model.CANoeEmu.AddStateChangeFunction("VttFr_OnStateChange", "VttCntrl_Fr.h");
      }
    }
  }
}

