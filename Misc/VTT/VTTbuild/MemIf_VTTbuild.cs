
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleMemIf : IModule
  {
    public string Name
    {
      get { return "MemIf"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/MemIf"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string memifDir = System.IO.Path.Combine(model.Directory.BSW, "MemIf");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(memifDir);

        model.Project.AddBswFile(System.IO.Path.Combine(memifDir, "MemIf.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(memifDir, "MemIf.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(memifDir, "MemIf_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "MemIf_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "MemIf_Cfg.c"), Name);
      }
    }
  }
}

