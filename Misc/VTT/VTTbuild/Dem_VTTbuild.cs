
using System.Collections.Generic;
using System.Linq;
using System;
using System.IO;
using System.Text.RegularExpressions;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleDem : IModule
  {
    public string Name
    {
      get { return "Dem"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Dem"; }
    }

    private static IEnumerable<string> GetFiles(string path, string reFileNamePattern)
    {
        System.Text.RegularExpressions.Regex rePattern = new System.Text.RegularExpressions.Regex(reFileNamePattern);
        return System.IO.Directory.EnumerateFiles(path, "*", System.IO.SearchOption.TopDirectoryOnly)
                                  .Where(file => rePattern.IsMatch(Path.GetFileName(file)));
    }
    
    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string demDir = System.IO.Path.Combine(model.Directory.BSW, "Dem");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(demDir);

        /* Dem files */
        foreach(string file in GetFiles(demDir, @"^Dem.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

        /* GenData */
        foreach(string file in GetFiles(genDataDir, @"^Dem.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

