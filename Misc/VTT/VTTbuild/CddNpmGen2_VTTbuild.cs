
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCddNpmGen2 : IModule
  {
    public string Name
    {
      get { return "CddNpmGen2"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/CddNpmGen2"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string genDataDir = model.Directory.GenData;
        string cddDir = System.IO.Path.Combine(model.Directory.BSW, "CddNpmGen2");
        
        model.Project.AddIncludeDirectory(cddDir);
        
        model.Project.AddBswFile(System.IO.Path.Combine(cddDir, "CddNpmGen2.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(cddDir, "CddNpmGen2.h"), Name);
        if (System.IO.File.Exists(System.IO.Path.Combine(cddDir, "CddNpmGen2_DCM.h")))
         model.Project.AddBswFile(System.IO.Path.Combine(cddDir, "CddNpmGen2_DCM.h"), Name);
        
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "CddNpmGen2_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "CddNpmGen2_Cfg.h"), Name);
      }
    }
  }
}

