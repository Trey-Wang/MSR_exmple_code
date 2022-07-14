
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCanSM : IModule
  {
    public string Name
    {
      get { return "CanSM"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/CanSM"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string cansmDir = System.IO.Path.Combine(model.Directory.BSW, "CanSM");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(cansmDir);

        model.Project.AddBswFile(System.IO.Path.Combine(cansmDir, "CanSM_BswM.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(cansmDir, "CanSM_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(cansmDir, "CanSM_ComM.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(cansmDir, "CanSM_Dcm.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(cansmDir, "CanSM_EcuM.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(cansmDir, "CanSM_TxTimeoutException.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(cansmDir, "CanSM.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(cansmDir, "CanSM.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "CanSM_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "CanSM_Lcfg.c"), Name);
      }
    }
  }
}

