
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCanIf : IModule
  {
    public string Name
    {
      get { return "CanIf"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/CanIf"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string canifDir = System.IO.Path.Combine(model.Directory.BSW, "CanIf");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(canifDir);

        model.Project.AddBswFile(System.IO.Path.Combine(canifDir, "CanIf.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(canifDir, "CanIf.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(canifDir, "CanIf_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(canifDir, "CanIf_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(canifDir, "CanIf_GeneralTypes.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(canifDir, "CanIf_Hooks.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "CanIf_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "CanIf_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "CanIf_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "CanIf_CanTrcv.h"), Name);
        
        string dataChecksumFile = System.IO.Path.Combine(canifDir, "CanIf_DataChecksum.c");
        if (System.IO.File.Exists(dataChecksumFile))
        {
          model.Project.AddBswFile(dataChecksumFile, Name);
        }
      }
    }
  }
}

