
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModulevCanCcGbt : IModule
  {
    public string Name
    {
      get { return "vCanCcGbt"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/vCanCcGbt"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string vCanCcGbtDir = System.IO.Path.Combine(model.Directory.BSW, "vCanCcGbt");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(vCanCcGbtDir);

        model.Project.AddBswFile(System.IO.Path.Combine(vCanCcGbtDir, "vCanCcGbt.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vCanCcGbtDir, "vCanCcGbt.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vCanCcGbtDir, "vCanCcGbt_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vCanCcGbtDir, "vCanCcGbt_Callout.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vCanCcGbtDir, "vCanCcGbt_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vCanCcGbtDir, "vCanCcGbt._Int.h"), Name);

        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "vCanCcGbt_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "vCanCcGbt_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "vCanCcGbt_LCfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "vCanCcGbt_LCfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "vCanCcGbt_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "vCanCcGbt_PBcfg.h"), Name);
      }
    }
  }
}

