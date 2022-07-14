
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleFrIf : IModule
  {
    public string Name
    {
      get { return "FrIf"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/FrIf"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string frifDir = System.IO.Path.Combine(model.Directory.BSW, "FrIf");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(frifDir);

        model.Project.AddBswFile(System.IO.Path.Combine(frifDir, "FrIf.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(frifDir, "FrIf.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(frifDir, "FrIf_LCfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(frifDir, "FrIf_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(frifDir, "FrIf_AbsTimer.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(frifDir, "FrIf_Rx.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(frifDir, "FrIf_Time.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(frifDir, "FrIf_Trcv.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(frifDir, "FrIf_Tx.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FrIf_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FrIf_LCfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FrIf_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FrIf_PBcfg.h"), Name); 
      }
    }
  }
}

