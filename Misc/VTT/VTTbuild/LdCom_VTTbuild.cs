
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleLdCom : IModule
  {
    public string Name
    {
      get { return "LdCom"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/LdCom"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string LdComDir = System.IO.Path.Combine(model.Directory.BSW, "LdCom");
        string genDataDir = model.Directory.GenData;
        string sourceDir = model.Directory.Source;

        model.Project.AddIncludeDirectory(LdComDir);

        model.Project.AddBswFile(System.IO.Path.Combine(LdComDir, "LdCom.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(LdComDir, "LdCom.h"), Name);

        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "LdCom_Cbk.h"), Name); 
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "LdCom_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "LdCom_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "LdCom_Lcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "LdCom_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "LdCom_PBcfg.h"), Name);
      }
    }
  }
}

