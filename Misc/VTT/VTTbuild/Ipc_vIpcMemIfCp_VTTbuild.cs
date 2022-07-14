
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModulevIpcMemIfCp : IModule
  {
    public string Name
    {
      get { return "vIpcMemIfCp"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/vIpcMemIfCp"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
          string vIpcMemIfCpDir = System.IO.Path.Combine(model.Directory.BSW, "vIpcMemIfCp");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(vIpcMemIfCpDir);

        // Select all header-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(vIpcMemIfCpDir, "*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all C-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(vIpcMemIfCpDir, "*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all header-files in GenData directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "vIpcMemIfCp_*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all source-files in GenData directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "vIpcMemIfCp_*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

