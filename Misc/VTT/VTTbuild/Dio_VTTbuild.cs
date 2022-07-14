
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVTTDio : IModule
  {
    public string Name
    {
      get { return "VTTDio"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTDio"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string dioDir = System.IO.Path.Combine(model.Directory.BSW, "VttDio");
        string vttCntrlDir = System.IO.Path.Combine(model.Directory.BSW, "VTTCntrl");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(dioDir);

        model.Project.AddBswFile(System.IO.Path.Combine(dioDir, "Dio.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(dioDir, "Dio.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Dio_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Dio_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Dio.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Dio.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Dio_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Dio_Cfg.h"), Name);

        model.CANoeEmu.AddStateChangeFunction("VttDio_OnStateChange", "VttCntrl_Dio.h");
      }
    }
  }
}

