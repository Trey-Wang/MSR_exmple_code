
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleRtp : IModule
  {
    public string Name
    {
      get { return "Rtp"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Rtp"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string sdDir = System.IO.Path.Combine(model.Directory.BSW, "Rtp");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(sdDir);

        model.Project.AddBswFile(System.IO.Path.Combine(sdDir, "Rtp.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(sdDir, "Rtp.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(sdDir, "Rtp_Priv.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(sdDir, "Rtp_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "SchM_Rtp_Type.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Rtp_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Rtp_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Rtp_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Rtp_Lcfg.h"), Name);
      }
    }
  }
}

