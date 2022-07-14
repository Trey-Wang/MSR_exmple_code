
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCrypto_30_LibCv : IModule
  {
    public string Name
    {
      get { return "Crypto_30_LibCv"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Crypto_30_LibCv/Crypto"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string cryptoDir = System.IO.Path.Combine(model.Directory.BSW, "Crypto_30_LibCv");
        string cryptoCvDir = System.IO.Path.Combine(model.Directory.BSW, "SecMod");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(cryptoDir);

        // Select all header-files in BSW directory except templates
        foreach (string file in System.IO.Directory.EnumerateFiles(cryptoDir, "Crypto_30_LibCv*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all C-files in BSW directory except templates
        foreach (string file in System.IO.Directory.EnumerateFiles(cryptoDir, "Crypto_30_LibCv*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Crypto_30_LibCv_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Crypto_30_LibCv_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Crypto_30_LibCv_PBcfg.c"), Name);

        if (System.IO.Directory.Exists(cryptoCvDir))
        {
          model.Project.AddIncludeDirectory(cryptoCvDir);

          // Select all header-files in BSW directory
          foreach (string file in System.IO.Directory.EnumerateFiles(cryptoCvDir, "*.h", System.IO.SearchOption.TopDirectoryOnly))
          {
            model.Project.AddBswFile(file, Name);
          }

          // Select all C-files in BSW directory
          foreach (string file in System.IO.Directory.EnumerateFiles(cryptoCvDir, "*.c", System.IO.SearchOption.TopDirectoryOnly))
          {
            model.Project.AddBswFile(file, Name);
          }
        }
      }
    }
  }
}

