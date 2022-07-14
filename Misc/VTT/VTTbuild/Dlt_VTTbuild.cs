
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleDlt : IModule
  {
    public string Name
    {
      get { return "Dlt"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Dlt"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string dltDir = System.IO.Path.Combine(model.Directory.BSW, "Dlt");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(dltDir);

        // Select all header-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(dltDir, "*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
            model.Project.AddBswFile(file, Name);
        }

        // Select all C-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(dltDir, "*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
            model.Project.AddBswFile(file, Name);
        }

        // Select all header-files in GenData directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "Dlt*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
            model.Project.AddBswFile(file, Name);
        }

        // Select all source-files in GenData directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "Dlt*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
            model.Project.AddBswFile(file, Name);
        } 
      }
    }
  }
}

