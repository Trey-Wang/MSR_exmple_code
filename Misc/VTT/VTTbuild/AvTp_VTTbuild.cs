
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleAvTp : IModule
  {
    public string Name
    {
      get { return "AvTp"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/AvTp"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string avtpDir = System.IO.Path.Combine(model.Directory.BSW, "AvTp");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(avtpDir);

        model.Project.AddBswFile(System.IO.Path.Combine(avtpDir, "AvTp.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(avtpDir, "AvTp.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(avtpDir, "AvTp_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(avtpDir, "AvTp_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(avtpDir, "AvTp_Priv.h"), Name);
       
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "AvTp_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "AvTp_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "AvTp_Lcfg.h"), Name);
      }
    }
  }
}

