
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleNvM : IModule
  {
    public string Name
    {
      get { return "NvM"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/NvM"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string nvmDir = System.IO.Path.Combine(model.Directory.BSW, "NvM");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(nvmDir);

        model.Project.AddBswFile(System.IO.Path.Combine(nvmDir, "NvM.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(nvmDir, "NvM_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(nvmDir, "NvM_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(nvmDir, "NvM.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(nvmDir, "NvM_Act.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(nvmDir, "NvM_Act.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(nvmDir, "NvM_Crc.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(nvmDir, "NvM_Crc.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(nvmDir, "NvM_JobProc.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(nvmDir, "NvM_JobProc.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(nvmDir, "NvM_Qry.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(nvmDir, "NvM_Qry.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(nvmDir, "NvM_Queue.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(nvmDir, "NvM_Queue.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "NvM_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "NvM_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "NvM_PrivateCfg.h"), Name);
      }
    }
  }
}

