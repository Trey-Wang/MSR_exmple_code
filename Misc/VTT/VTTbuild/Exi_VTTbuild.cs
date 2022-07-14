
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleExi : IModule
  {
    public string Name
    {
      get { return "Exi"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Exi"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string ExiDir = System.IO.Path.Combine(model.Directory.BSW, "Exi");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(ExiDir);

        // Select all header-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(ExiDir, "*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all C-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(ExiDir, "*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all header-files in GenData directory
        // Set of generated file is SIP specific. Generator will overwrite unused files with empty ones.
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "Exi_*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all source-files in GenData directory
        // Set of generated file is SIP specific. Generator will overwrite unused files with empty ones.
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "Exi_*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

