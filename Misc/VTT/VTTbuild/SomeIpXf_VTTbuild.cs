
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleSomeIpXf : IModule
  {
    public string Name
    {
      get { return "SomeIpXf"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/SomeIpXf"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(genDataDir);

        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "SomeIpXf.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "SomeIpXf.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "SomeIpXf_Compiler_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "SomeIpXf_MemMap.h"), Name);
      }
    }
  }
}

