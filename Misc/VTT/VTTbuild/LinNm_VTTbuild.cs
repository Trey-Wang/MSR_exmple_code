
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleLinNm : IModule
  {
    public string Name
    {
      get { return "LinNm"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/LinNm"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string linnmDir = System.IO.Path.Combine(model.Directory.BSW, "LinNm");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(linnmDir);

        model.Project.AddBswFile(System.IO.Path.Combine(linnmDir, "LinNm.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(linnmDir, "LinNm_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(linnmDir, "LinNm.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "LinNm_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "LinNm_Cfg.h"), Name);
      }
    }
  }
}

