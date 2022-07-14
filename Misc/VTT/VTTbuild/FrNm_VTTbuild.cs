
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleFrNm : IModule
  {
    public string Name
    {
      get { return "FrNm"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/FrNm"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string frnmDir = System.IO.Path.Combine(model.Directory.BSW, "FrNm");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(frnmDir);

        model.Project.AddBswFile(System.IO.Path.Combine(frnmDir, "FrNm_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(frnmDir, "FrNm.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(frnmDir, "FrNm.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FrNm_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FrNm_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FrNm_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FrNm_PBcfg.c"), Name);
      }
    }
  }
}

