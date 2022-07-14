
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVTTEthTrcv : IModule
  {
    public string Name
    {
      get { return "VTTEthTrcv"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTEthTrcv"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string ethtrcvDir = System.IO.Path.Combine(model.Directory.BSW, "VttEthTrcv_30_Vtt");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(ethtrcvDir);

        // Select all header-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(ethtrcvDir, "*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }
        foreach (string file in System.IO.Directory.EnumerateFiles(ethtrcvDir, "*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "EthTrcv_30_*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "EthTrcv_30_*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

