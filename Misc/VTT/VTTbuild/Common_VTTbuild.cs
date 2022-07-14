
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCommon : IModule
  {
    public string Name
    {
      get { return "Common"; }
    }

    public string DefinitionPath
    {
      get { return ""; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      string commonDir = System.IO.Path.Combine(model.Directory.BSW, "Vtt_Common");
      model.Project.AddIncludeDirectory(commonDir);      
      // Select all header files from the '_Common' directory, that do not start with an underscore.
      foreach (string file in System.IO.Directory.EnumerateFiles(commonDir, "*.h", System.IO.SearchOption.TopDirectoryOnly)
                      .Concat(System.IO.Directory.EnumerateFiles(commonDir, "*.c", System.IO.SearchOption.TopDirectoryOnly)))
      {
        if (!System.IO.Path.GetFileName(file).StartsWith("_")) 
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

