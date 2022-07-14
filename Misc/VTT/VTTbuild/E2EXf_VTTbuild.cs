
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleE2EXf : IModule
  {
    public string Name
    {
      get { return "E2EXf"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/E2EXf"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string eepDir = System.IO.Path.Combine(model.Directory.BSW, "E2EXf");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(eepDir);

        model.Project.AddBswFile(System.IO.Path.Combine(eepDir, "E2EXf.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eepDir, "E2EXf.h"), Name);

        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "E2EXf_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "E2EXf_LCfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "E2EXf_LCfg.h"), Name);
      }
    }
  }
}

