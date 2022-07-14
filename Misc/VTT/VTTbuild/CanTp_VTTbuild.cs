
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCanTp : IModule
  {
    public string Name
    {
      get { return "CanTp"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/CanTp"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string cantpDir = System.IO.Path.Combine(model.Directory.BSW, "CanTp");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(cantpDir);

        model.Project.AddBswFile(System.IO.Path.Combine(cantpDir, "CanTp.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(cantpDir, "CanTp.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(cantpDir, "CanTp_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(cantpDir, "CanTp_Priv.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(cantpDir, "CanTp_Types.h"), Name);

        // Number of Gendata Files depends on the use case, therefor a pattern matching is used:
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "CanTp_*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "CanTp_*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

