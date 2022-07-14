
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleComXf : IModule
  {
    public string Name
    {
      get { return "ComXf"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/ComXf"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(genDataDir);

        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "ComXf.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "ComXf.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "ComXf_Compiler_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "ComXf_MemMap.h"), Name);
      }
    }
  }
}

