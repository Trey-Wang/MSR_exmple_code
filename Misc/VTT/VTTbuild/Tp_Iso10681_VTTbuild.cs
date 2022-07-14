
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleTp_Iso10681 : IModule
  {
    public string Name
    {
      get { return "FrTp"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/FrTp"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string tp_iso10681Dir = System.IO.Path.Combine(model.Directory.BSW, "Tp_Iso10681");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(tp_iso10681Dir);

        model.Project.AddBswFile(System.IO.Path.Combine(tp_iso10681Dir, "FrTp.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(tp_iso10681Dir, "FrTp.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(tp_iso10681Dir, "FrTp_FrIf.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(tp_iso10681Dir, "FrTp_Rsrc.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(tp_iso10681Dir, "FrTp_RxSm.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(tp_iso10681Dir, "FrTp_TxSm.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(tp_iso10681Dir, "FrTp_Util.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(tp_iso10681Dir, "FrTp_Common.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(tp_iso10681Dir, "FrTp_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(tp_iso10681Dir, "FrTp_FrIf.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(tp_iso10681Dir, "FrTp_Rsrc.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(tp_iso10681Dir, "FrTp_RxSm.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(tp_iso10681Dir, "FrTp_TxSm.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(tp_iso10681Dir, "FrTp_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(tp_iso10681Dir, "FrTp_Util.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FrTp_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FrTp_Pbcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FrTp_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FrTp_GlobCfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FrTp_Lcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "FrTp_Pbcfg.h"), Name);
      }
    }
  }
}

