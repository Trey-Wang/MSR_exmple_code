
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCrc : IModule
  {
    public string Name
    {
      get { return "Crc"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Crc"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string crcDir = System.IO.Path.Combine(model.Directory.BSW, "Crc");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(crcDir);

        // Select all header-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(crcDir, "*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
            model.Project.AddBswFile(file, Name);
        }

        // Select all C-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(crcDir, "*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
            model.Project.AddBswFile(file, Name);
        }

        // Select all header-files in GenData directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "Crc_*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
            model.Project.AddBswFile(file, Name);
        }

        // Select all source-files in GenData directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "Crc_*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
            model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

