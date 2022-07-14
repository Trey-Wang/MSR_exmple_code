
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCrypto : IModule
  {
    public string Name
    {
      get { return "Crypto"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Crypto"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {

      if ( (ecuc.IsConfigured) /* is part of the configuration */ )
      {
        string cryptoDir = System.IO.Path.Combine(model.Directory.BSW, "Crypto");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(cryptoDir);

        // Select all header-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(cryptoDir, "*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all C-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(cryptoDir, "*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all header-files in GenData directory
        // Set of generated file is SIP specific. Generator will overwrite unused files with empty ones.
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "Crypto_*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all source-files in GenData directory
        // Set of generated file is SIP specific. Generator will overwrite unused files with empty ones.
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "Crypto_*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

