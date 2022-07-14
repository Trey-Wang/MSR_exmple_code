
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModulePwm : IModule
  {
    public string Name
    {
      get { return "VTTPwm"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTPwm"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string pwmDir = System.IO.Path.Combine(model.Directory.BSW, "VTTPwm");
        string vttCntrlDir = System.IO.Path.Combine(model.Directory.BSW, "VTTCntrl");
        string genDataDir = model.Directory.GenData;
        string sourceDir = model.Directory.Source;

        model.Project.AddIncludeDirectory(pwmDir);
        model.Project.AddIncludeDirectory(vttCntrlDir);

        model.Project.AddBswFile(System.IO.Path.Combine(pwmDir, "Pwm.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(pwmDir, "Pwm.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(pwmDir, "Pwm_Irq.c"), Name);
        if (System.IO.File.Exists(System.IO.Path.Combine(pwmDir, "Pwm_Irq.h")))
        {
          model.Project.AddBswFile(System.IO.Path.Combine(pwmDir, "Pwm_Irq.h"), Name);
        }
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Pwm_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Pwm_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Pwm.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Pwm.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Pwm_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Pwm_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(sourceDir, "VttCntrl_Pwm_Callout_Stubs.c"), Name);

        model.CANoeEmu.AddStateChangeFunction("VttPwm_OnStateChange", "VttCntrl_Pwm.h");
      }
    }
  }
}

