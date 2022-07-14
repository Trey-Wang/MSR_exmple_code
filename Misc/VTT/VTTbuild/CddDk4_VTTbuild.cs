
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCddDk4 : IModule
  {
    public string Name
    {
      get { return "CddDk4"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/CddDk4/Cdd"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string genDataDir = model.Directory.GenData;
        string cddDir = System.IO.Path.Combine(model.Directory.BSW, "CddDk4");
        
        model.Project.AddIncludeDirectory(cddDir);
        
        model.Project.AddBswFile(System.IO.Path.Combine(cddDir, "CddDk4.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(cddDir, "CddDk4.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(cddDir, "CddDk4_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(cddDir, "CddDk4_Types.h"), Name);
        
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "CddDk4_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "CddDk4_Cfg.h"), Name);
      }
    }
  }
}

