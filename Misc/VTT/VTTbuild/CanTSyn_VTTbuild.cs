
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCanTSyn : IModule
  {
    public string Name
    {
      get { return "CanTSyn"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/CanTSyn"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string CanTSyn = System.IO.Path.Combine(model.Directory.BSW, "CanTSyn");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(CanTSyn);
        
        
        // Select all header-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(CanTSyn, "*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all header-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(CanTSyn, "*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all header-files in GenData directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "CanTSyn*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
            model.Project.AddBswFile(file, Name);
        }

        // Select all source-files in GenData directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "CanTSyn*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
            model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

