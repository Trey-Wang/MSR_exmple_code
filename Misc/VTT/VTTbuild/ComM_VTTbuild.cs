
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleComM : IModule
  {
    public string Name
    {
      get { return "ComM"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/ComM"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string commDir = System.IO.Path.Combine(model.Directory.BSW, "ComM");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(commDir);

        model.Project.AddBswFile(System.IO.Path.Combine(commDir, "ComM.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(commDir, "ComM.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(commDir, "ComM_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(commDir, "ComM_BusSM.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(commDir, "ComM_Nm.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(commDir, "ComM_EcuMBswM.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(commDir, "ComM_Dcm.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "ComM_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "ComM_Lcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "ComM_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "ComM_GenTypes.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "ComM_PBcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "ComM_PBcfg.c"), Name);
      }
    }
  }
}

