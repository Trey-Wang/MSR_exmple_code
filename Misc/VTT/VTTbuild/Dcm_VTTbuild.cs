
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleDcm : IModule
  {
    public string Name
    {
      get { return "Dcm"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Dcm"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string dcmDir = System.IO.Path.Combine(model.Directory.BSW, "Dcm");
        string genDataDir = model.Directory.GenData;
        string sourceDir = model.Directory.Source;

        model.Project.AddIncludeDirectory(dcmDir);

        // Number of Gendata Files depend on the use case, therefor a pattern matching is used:
        foreach (string file in System.IO.Directory.EnumerateFiles(dcmDir, "Dcm*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        foreach (string file in System.IO.Directory.EnumerateFiles(dcmDir, "Dcm*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "Dcm*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "Dcm*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

