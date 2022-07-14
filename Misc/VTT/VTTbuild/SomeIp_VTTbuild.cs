
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleSomeIp : IModule
  {
    public string Name
    {
      get { return "SomeIp"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/SomeIp"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string soadDir = System.IO.Path.Combine(model.Directory.BSW, "SomeIp");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(soadDir);

        model.Project.AddBswFile(System.IO.Path.Combine(soadDir, "SomeIp.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(soadDir, "SomeIp.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(soadDir, "SomeIp_Priv.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(soadDir, "SomeIp_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "SchM_SomeIp.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "SchM_SomeIp_Type.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "SomeIp_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "PduR_SomeIp_Cdd.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "SomeIp_Cdd_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "SomeIp_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "SomeIp_Lcfg.h"), Name);
      }
    }
  }
}

