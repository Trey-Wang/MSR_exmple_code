
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCry_30_LibCv : IModule
  {
    public string Name
    {
      get { return "Cry"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Cry_30_LibCv/Cry"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string cryDir = System.IO.Path.Combine(model.Directory.BSW, "Cry");
        string cryptoCvDir = System.IO.Path.Combine(model.Directory.BSW, "SecMod");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(cryDir);

        // Select all header-files in BSW directory except templates
        foreach (string file in System.IO.Directory.EnumerateFiles(cryDir, "Cry*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all C-files in BSW directory except templates
        foreach (string file in System.IO.Directory.EnumerateFiles(cryDir, "Cry*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Cry_30_LibCv_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Cry_30_LibCv_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Cry_30_LibCv_PBcfg.c"), Name);

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

