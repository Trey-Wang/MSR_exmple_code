
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleIssm : IModule
  {
    public string Name
    {
      get { return "Issm"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Issm"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string IssmDir = System.IO.Path.Combine(model.Directory.BSW, "Issm");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(IssmDir);

        model.Project.AddBswFile(System.IO.Path.Combine(IssmDir, "Issm.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(IssmDir, "Issm.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(IssmDir, "Issm_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Issm_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Issm_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Issm_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Issm_PBcfg.h"), Name);
      }
    }
  }
}

