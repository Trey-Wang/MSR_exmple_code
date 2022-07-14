
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleFrTSyn : IModule
  {
    public string Name
    {
      get { return "FrTSyn"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/FrTSyn"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string FrTSyn = System.IO.Path.Combine(model.Directory.BSW, "FrTSyn");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(FrTSyn);
        
        
        // Select all header-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(FrTSyn, "*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all header-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(FrTSyn, "*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all header-files in GenData directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "FrTSyn*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all source-files in GenData directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "FrTSyn*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

