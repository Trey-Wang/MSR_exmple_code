
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleLinSM : IModule
  {
    public string Name
    {
      get { return "LinSM"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/LinSM"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string linsmDir = System.IO.Path.Combine(model.Directory.BSW, "LinSM");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(linsmDir);

        model.Project.AddBswFile(System.IO.Path.Combine(linsmDir, "LinSM.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(linsmDir, "LinSM_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(linsmDir, "LinSM_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(linsmDir, "LinSM.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "LinSM_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "LinSM_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "LinSM_PBcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "LinSM_PBcfg.c"), Name);
      }
    }
  }
}

