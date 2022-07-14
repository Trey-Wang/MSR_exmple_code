
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleJ1939Nm : IModule
  {
    public string Name
    {
      get { return "J1939Nm"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/J1939Nm"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string j1939nmDir = System.IO.Path.Combine(model.Directory.BSW, "J1939Nm");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(j1939nmDir);

        model.Project.AddBswFile(System.IO.Path.Combine(j1939nmDir, "J1939Nm.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(j1939nmDir, "J1939Nm.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(j1939nmDir, "J1939Nm_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(j1939nmDir, "J1939Nm_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "J1939Nm_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "J1939Nm_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "J1939Nm_Lcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "J1939Nm_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "J1939Nm_PBcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "J1939Nm_PBcfg.c"), Name);

        {
          string j1939nm_dynamic_c = System.IO.Path.Combine(j1939nmDir, "J1939Nm_Dynamic.c");
          if (System.IO.File.Exists(j1939nm_dynamic_c))
          {
            model.Project.AddBswFile(j1939nm_dynamic_c, Name);
          }
        }
        {
          string j1939nm_dynamic_h = System.IO.Path.Combine(j1939nmDir, "J1939Nm_Dynamic.h");
          if (System.IO.File.Exists(j1939nm_dynamic_h))
          {
            model.Project.AddBswFile(j1939nm_dynamic_h, Name);
          }
        }
      }
    }
  }
}

