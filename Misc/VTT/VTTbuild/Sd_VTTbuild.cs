
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleSd : IModule
  {
    public string Name
    {
      get { return "Sd"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Sd"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string sdDir = System.IO.Path.Combine(model.Directory.BSW, "Sd");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(sdDir);

        model.Project.AddBswFile(System.IO.Path.Combine(sdDir, "Sd.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(sdDir, "Sd.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(sdDir, "Sd_Priv.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(sdDir, "Sd_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "SchM_Sd_Type.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Sd_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Sd_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Sd_PBcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Sd_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Sd_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Sd_Lcfg.h"), Name);
      }
    }
  }
}

