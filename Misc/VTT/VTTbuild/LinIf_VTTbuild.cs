
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleLinIf : IModule
  {
    public string Name
    {
      get { return "LinIf"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/LinIf"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string linifDir = System.IO.Path.Combine(model.Directory.BSW, "LinIf");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(linifDir);

        model.Project.AddBswFile(System.IO.Path.Combine(linifDir, "LinIf.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(linifDir, "LinIf.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(linifDir, "LinIf_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "LinIf_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "LinIf_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "LinIf_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "LinIf_GeneralTypes.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "LinIf_LinTrcv.h"), Name);
      }
    }
  }
}

