
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleRtm : IModule
  {
    public string Name
    {
      get { return "Rtm"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Rtm"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string rtmDir = System.IO.Path.Combine(model.Directory.BSW, "Rtm");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(rtmDir);

        model.Project.AddBswFile(System.IO.Path.Combine(rtmDir, "Rtm.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(rtmDir, "Rtm.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(rtmDir, "Rtm_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Rtm_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Rtm_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Rtm_Cfg.h"), Name);
      }
    }
  }
}

