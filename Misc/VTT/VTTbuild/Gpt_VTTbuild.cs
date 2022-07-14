
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVTTGpt : IModule
  {
    public string Name
    {
      get { return "VTTGpt"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTGpt"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string gptDir = System.IO.Path.Combine(model.Directory.BSW, "VttGpt");
        string vttCntrlDir = System.IO.Path.Combine(model.Directory.BSW, "VTTCntrl");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(gptDir);

        model.Project.AddBswFile(System.IO.Path.Combine(gptDir, "Gpt.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(gptDir, "Gpt.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(gptDir, "Gpt_Irq.c"), Name);
        
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Gpt_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Gpt_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Gpt.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Gpt.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Gpt_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Gpt_Cfg.h"), Name);

        model.CANoeEmu.AddStateChangeFunction("VttGpt_OnStateChange", "VttCntrl_Gpt.h");
      }
    }
  }
}

