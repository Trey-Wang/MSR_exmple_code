
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVTTSpi : IModule
  {
    public string Name
    {
      get { return "VTTSpi"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTSpi"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string spiDir = System.IO.Path.Combine(model.Directory.BSW, "VttSpi");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(spiDir);

        model.Project.AddBswFile(System.IO.Path.Combine(spiDir, "Spi.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(spiDir, "Spi.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(spiDir, "Spi_Irq.c"), Name);
        if (System.IO.File.Exists(System.IO.Path.Combine(spiDir, "Spi_Irq.h")))
        {
          model.Project.AddBswFile(System.IO.Path.Combine(spiDir, "Spi_Irq.h"), Name);
        }
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Spi_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Spi_PBcfg.c"), Name);
      }
    }
  }
}

