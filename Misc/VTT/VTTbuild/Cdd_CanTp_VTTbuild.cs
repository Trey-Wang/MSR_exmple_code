
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCddCanTp : IModule
  {
    public string Name
    {
      get { return "Cdd_CanTp"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Cdd_AsrCdd/Cdd"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string genDataDir = model.Directory.GenData;
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Cdd_CanTp_Cbk.h"), Name);
      }
    }
  }
}

