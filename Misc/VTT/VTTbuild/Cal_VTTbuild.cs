
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCal : IModule
  {
    public string Name
    {
      get { return "Cal"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Cal"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string calDir = System.IO.Path.Combine(model.Directory.BSW, "Cal");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(calDir);

        // Select all header files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(calDir, "*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
            model.Project.AddBswFile(file, Name);
        }

        // Select all source files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(calDir, "*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
            model.Project.AddBswFile(file, Name);
        }

        // Select all header files in GenData directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "Cal*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all source files in GenData directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "Cal*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

