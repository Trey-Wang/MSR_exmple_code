
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleDiagXf : IModule
  {
    public string Name
    {
      get { return "DiagXf"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/DiagXf"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string diagXfDir = System.IO.Path.Combine(model.Directory.BSW, "DiagXf");
        string genDataDir = model.Directory.GenData;
        
        model.Project.AddIncludeDirectory(diagXfDir);

        if (System.IO.File.Exists(System.IO.Path.Combine(diagXfDir, "DiagXf.c")))
        {
          model.Project.AddBswFile(System.IO.Path.Combine(diagXfDir, "DiagXf.c"), Name);
        }

        if (System.IO.File.Exists(System.IO.Path.Combine(genDataDir, "DiagXf.c")))
        {
          model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "DiagXf.c"), Name);
        }
      }
    }
  }
}

