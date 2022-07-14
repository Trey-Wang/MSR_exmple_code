
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleAdc : IModule
  {
    public string Name
    {
      get { return "VTTAdc"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTAdc"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string adcDir = System.IO.Path.Combine(model.Directory.BSW, "VttAdc");
        string vttCntrlDir = System.IO.Path.Combine(model.Directory.BSW, "VTTCntrl");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(adcDir);

        model.Project.AddBswFile(System.IO.Path.Combine(adcDir, "Adc.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(adcDir, "Adc.h"), Name);
        if (System.IO.File.Exists(System.IO.Path.Combine(adcDir, "Adc_Irq.h")))
        {
          model.Project.AddBswFile(System.IO.Path.Combine(adcDir, "Adc_Irq.h"), Name);
        }
        model.Project.AddBswFile(System.IO.Path.Combine(adcDir, "Adc_Irq.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Adc_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Adc_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Adc.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Adc.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Adc_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Adc_Cfg.h"), Name);
        
        model.CANoeEmu.AddStateChangeFunction("VttAdc_OnStateChange", "VttCntrl_Adc.h");
      }
    }
  }
}

