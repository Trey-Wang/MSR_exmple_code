
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleUdpNm : IModule
  {
    public string Name
    {
      get { return "UdpNm"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/UdpNm"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string udpnmDir = System.IO.Path.Combine(model.Directory.BSW, "UdpNm");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(udpnmDir);

        model.Project.AddBswFile(System.IO.Path.Combine(udpnmDir, "UdpNm.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(udpnmDir, "UdpNm.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(udpnmDir, "UdpNm_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(udpnmDir, "UdpNm_Priv.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(udpnmDir, "UdpNm_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "UdpNm_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "UdpNm_Lcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "UdpNm_Lcfg.c"), Name);
      }
    }
  }
}

