
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCanNm : IModule
  {
    public string Name
    {
      get { return "CanNm"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/CanNm"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string cannmDir = System.IO.Path.Combine(model.Directory.BSW, "CanNm");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(cannmDir);

        model.Project.AddBswFile(System.IO.Path.Combine(cannmDir, "CanNm_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(cannmDir, "CanNm.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(cannmDir, "CanNm.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "CanNm_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "CanNm_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "CanNm_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "CanNm_PBcfg.c"), Name);
      }
    }
  }
}

