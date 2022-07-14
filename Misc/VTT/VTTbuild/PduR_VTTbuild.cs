
using System.Collections.Generic;
using System.Linq;

using DSI4 = GenTool_CsDataServerDomAsr4.Iface;

namespace Vector.VTT.VsProjectGenerator
{
  class ModulePduR : IModule
  {
    public string Name
    {
      get { return "PduR"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/PduR"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string pdurDir = System.IO.Path.Combine(model.Directory.BSW, "PduR");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(pdurDir);
        string[] pduRBswModules = 
          ecuc.EcucModuleConfigurationValues.ContainerList
          .OfType<DSI4.IEcucContainerValue>()
          .Where(x => x.DefinitionRef.Value == "/MICROSAR/PduR/PduRBswModules")
          .Select(x => x.ShortName)
          .ToArray();
        foreach (string moduleName in pduRBswModules)
        {
          model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "PduR_" + moduleName + ".h"), Name);
        }
        model.Project.AddBswFile(System.IO.Path.Combine(pdurDir, "PduR.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(pdurDir, "PduR.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "PduR_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "PduR_Lcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "PduR_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "PduR_PBcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "PduR_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "PduR_Types.h"), Name);
      }
    }
  }
}

