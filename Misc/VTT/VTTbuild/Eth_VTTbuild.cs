
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVTTEth : IModule
  {
    public string Name
    {
      get { return "VTTEth"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTEth"; }
    }

    private IEnumerable<string> GetFiles(string path, string reFileNamePattern)
    {
      System.Text.RegularExpressions.Regex rePattern = new System.Text.RegularExpressions.Regex(reFileNamePattern);
      return System.IO.Directory.EnumerateFiles(path, "*", System.IO.SearchOption.TopDirectoryOnly)
                                .Where(file => rePattern.IsMatch(file));
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string ethDir = System.IO.Path.Combine(model.Directory.BSW, "VttEth");
        string genDataDir = model.Directory.GenData;
        string vttCntrlDir = System.IO.Path.Combine(model.Directory.BSW, "VttCntrl");

        model.Project.AddIncludeDirectory(ethDir);

        // get static files and add them
        foreach (string file in GetFiles(ethDir, ".*.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
          System.Console.WriteLine(file);
        }

        // get generated files and add them
        foreach (string file in GetFiles(genDataDir, "Eth(_30_.*)?.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Eth_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Eth_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Eth.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Eth.h"), Name);

        model.CANoeEmu.AddStateChangeFunction("VttEth_OnStateChange", "VttCntrl_Eth.h");
      }
    }
  }
}
