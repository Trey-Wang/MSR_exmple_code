
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVStdLib : IModule
  {
    public string Name
    {
      get { return "VStdLib"; }
    }

    public string DefinitionPath
    {
      get { return ""; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      string vstdlibDir = System.IO.Path.Combine(model.Directory.BSW, "VStdLib");
      string genDataDir = model.Directory.GenData;

      if (System.IO.Directory.Exists(vstdlibDir))
      {   
        model.Project.AddIncludeDirectory(vstdlibDir);

        model.Project.AddBswFile(System.IO.Path.Combine(vstdlibDir, "vstdlib.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vstdlibDir, "vstdlib.h"), Name);
      }
    }
  }
}

