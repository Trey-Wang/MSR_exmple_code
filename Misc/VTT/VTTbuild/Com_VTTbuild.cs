
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCom : IModule
  {
    public string Name
    {
      get { return "Com"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Com"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string comDir = System.IO.Path.Combine(model.Directory.BSW, "Com");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(comDir);

        model.Project.AddBswFile(System.IO.Path.Combine(comDir, "Com.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(comDir, "Com.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Com_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Com_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Com_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Com_Lcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Com_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Com_PBcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Com_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Appl_Cbk.h"), Name);

        // Es koennen weitere Headerdateien (durch Anlegen von User Callback Header) vorhanden sein, die vom Com generiert werden. 
      }
    }
  }
}

