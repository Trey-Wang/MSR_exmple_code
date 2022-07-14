
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleJ1939Tp : IModule
  {
    public string Name
    {
      get { return "J1939Tp"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/J1939Tp"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string j1939tpDir = System.IO.Path.Combine(model.Directory.BSW, "J1939Tp");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(j1939tpDir);

        model.Project.AddBswFile(System.IO.Path.Combine(j1939tpDir, "J1939Tp.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(j1939tpDir, "J1939Tp.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(j1939tpDir, "J1939Tp_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(j1939tpDir, "J1939Tp_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(j1939tpDir, "J1939Tp_Bam.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(j1939tpDir, "J1939Tp_Bam.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(j1939tpDir, "J1939Tp_Cmdt.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(j1939tpDir, "J1939Tp_Cmdt.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "J1939Tp_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "J1939Tp_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "J1939Tp_Lcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "J1939Tp_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "J1939Tp_PBcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "J1939Tp_PBcfg.c"), Name);
       
        if (System.IO.File.Exists(System.IO.Path.Combine(j1939tpDir, "J1939Tp_Direct.c")))
        {
          model.Project.AddBswFile(System.IO.Path.Combine(j1939tpDir, "J1939Tp_Direct.c"), Name);
        }

        if (System.IO.File.Exists(System.IO.Path.Combine(j1939tpDir, "J1939Tp_Direct.h")))
        {
          model.Project.AddBswFile(System.IO.Path.Combine(j1939tpDir, "J1939Tp_Direct.h"), Name);
        }
       
        if (System.IO.File.Exists(System.IO.Path.Combine(j1939tpDir, "J1939Tp_Fpp.c")))
        {
          model.Project.AddBswFile(System.IO.Path.Combine(j1939tpDir, "J1939Tp_Fpp.c"), Name);
        }
       
        if (System.IO.File.Exists(System.IO.Path.Combine(j1939tpDir, "J1939Tp_Fpp.h")))
        {
          model.Project.AddBswFile(System.IO.Path.Combine(j1939tpDir, "J1939Tp_Fpp.h"), Name);
        }
      }
    }
  }
}

