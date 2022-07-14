
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleIoHwAb : IModule
  {
    public string Name
    {
      get { return "IoHwAb"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/IoHwAb"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string iohwabDir = System.IO.Path.Combine(model.Directory.BSW, "IoHwAb");
        string genDataDir = model.Directory.GenData;
        string sourceDir = model.Directory.Source;

        model.Project.AddIncludeDirectory(iohwabDir);

        model.Project.AddBswFile(System.IO.Path.Combine(iohwabDir, "IoHwAb.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(sourceDir, "IoHwAb_30.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "IoHwAb_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "IoHwAb_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "IoHwAb_Types.h"), Name);
      }
    }
  }
}

