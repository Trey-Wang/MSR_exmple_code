
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleSoAd : IModule
  {
    public string Name
    {
      get { return "SoAd"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/SoAd"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string soadDir = System.IO.Path.Combine(model.Directory.BSW, "SoAd");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(soadDir);

        model.Project.AddBswFile(System.IO.Path.Combine(soadDir, "SoAd.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(soadDir, "SoAd.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(soadDir, "SoAd_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(soadDir, "SoAd_EthSM.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(soadDir, "SoAd_Priv.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(soadDir, "SoAd_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "SoAd_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "SoAd_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "SoAd_PBcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "SoAd_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "SoAd_Lcfg.h"), Name);
      }
    }
  }
}

