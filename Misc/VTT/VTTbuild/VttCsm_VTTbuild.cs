
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVttCsm : IModule
  {
    public string Name
    {
      get { return "VTTCsm"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTCsm"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string sourceDir = System.IO.Path.Combine(model.Directory.BSW, "VttCry");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(sourceDir);

        // Select all header-files in BSW directory except templates
        foreach (string file in System.IO.Directory.EnumerateFiles(sourceDir, "VTT*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all C-files in BSW directory except templates
        foreach (string file in System.IO.Directory.EnumerateFiles(sourceDir, "VTT*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VTTCsm_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VTTCsm_Cfg.h"), Name);
      }
    }
  }
}

