
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleGenData : IModule
  {
    public string Name
    {
      get { return "GenData"; }
    }

    public string DefinitionPath
    {
      get { return ""; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      string genDataDir = model.Directory.GenData;
      string componentsDir = model.Directory.GenData + "/Components";
      model.Project.AddIncludeDirectory(genDataDir);
      if (System.IO.Directory.Exists(componentsDir))
      {
        model.Project.AddIncludeDirectory(componentsDir);
      }
    }
  }
}

