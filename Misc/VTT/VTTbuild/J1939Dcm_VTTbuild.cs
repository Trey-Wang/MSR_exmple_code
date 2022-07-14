
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleJ1939Dcm : IModule
  {
    public string Name
    {
      get { return "J1939Dcm"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/J1939Dcm"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string j1939nmDir = System.IO.Path.Combine(model.Directory.BSW, "J1939Dcm");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(j1939nmDir);

        model.Project.AddBswFile(System.IO.Path.Combine(j1939nmDir, "J1939Dcm.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(j1939nmDir, "J1939Dcm.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(j1939nmDir, "J1939Dcm_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(j1939nmDir, "J1939Dcm_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(j1939nmDir, "J1939Dcm_Priv.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "J1939Dcm_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "J1939Dcm_Lcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "J1939Dcm_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "J1939Dcm_PBcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "J1939Dcm_PBcfg.c"), Name);
      }
    }
  }
}

