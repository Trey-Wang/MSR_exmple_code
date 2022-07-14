
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCryptoCv : IModule
  {
    public string Name
    {
      get { return "SecMod"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/CryptoCv"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {

      if ( (ecuc.IsConfigured) /* is part of the configuration */ )
      {
        string cryptocvDir = System.IO.Path.Combine(model.Directory.BSW, "SecMod");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(cryptocvDir);

        // Select all header-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(cryptocvDir, "*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all C-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(cryptocvDir, "*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all header-files in GenData directory
        // Set of generated file is SIP specific. Generator will overwrite unused files with empty ones.
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "CryptoCv_*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all source-files in GenData directory
        // Set of generated file is SIP specific. Generator will overwrite unused files with empty ones.
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "CryptoCv_*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
            model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

