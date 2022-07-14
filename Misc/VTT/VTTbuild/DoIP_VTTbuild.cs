
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleDoIP : IModule
  {
    public string Name
    {
      get { return "DoIP"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/DoIP"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string doipDir = System.IO.Path.Combine(model.Directory.BSW, "DoIP");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(doipDir);

        model.Project.AddBswFile(System.IO.Path.Combine(doipDir, "DoIP.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(doipDir, "DoIP.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(doipDir, "DoIP_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "DoIP_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "DoIP_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "DoIP_Lcfg.h"), Name);
      }
    }
  }
}

