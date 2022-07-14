
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleSrp : IModule
  {
    public string Name
    {
      get { return "Srp"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Srp"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string srpDir = System.IO.Path.Combine(model.Directory.BSW, "Srp");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(srpDir);

        model.Project.AddBswFile(System.IO.Path.Combine(srpDir, "Srp.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(srpDir, "Srp.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(srpDir, "Srp_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(srpDir, "Srp_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(srpDir, "Srp_Priv.h"), Name);
       
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Srp_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Srp_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Srp_Lcfg.h"), Name); 
      }
    }
  }
}

