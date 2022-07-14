
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVTTPort : IModule
  {
    public string Name
    {
      get { return "VTTPort"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTPort"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string portDir = System.IO.Path.Combine(model.Directory.BSW, "VttPort");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(portDir);

        model.Project.AddBswFile(System.IO.Path.Combine(portDir, "Port.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(portDir, "Port.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Port_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Port_PBcfg.c"), Name);
      }
    }
  }
}

