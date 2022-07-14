
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleEthIf : IModule
  {
    public string Name
    {
      get { return "EthIf"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/EthIf"; }
    }

    private IEnumerable<string> GetFiles(string path, string reFileNamePattern)
    {
      System.Text.RegularExpressions.Regex rePattern = new System.Text.RegularExpressions.Regex(reFileNamePattern);
      return System.IO.Directory.EnumerateFiles(path, "*", System.IO.SearchOption.TopDirectoryOnly)
                                .Where(filePath => rePattern.IsMatch(System.IO.Path.GetFileName(filePath)));
    }
    
    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string ethIfStaticFilePattern = @"^EthIf.*\.(h|c)$";
        string ipBaseStaticFilePattern = @"^IpBase.*\.(h|c)$";
        string ethIfGenDataFilePattern = @"^EthIf_.*\.(h|c)$";
        
        // EthIf directory
        string ethifDir = System.IO.Path.Combine(model.Directory.BSW, "EthIf");
        // IpBase directory
        string ipbaseDir = System.IO.Path.Combine(model.Directory.BSW, "IpBase");
        // GenData directory
        string genDataDir = model.Directory.GenData;
        
        // add both IpBase and EthIf directory to include path
        model.Project.AddIncludeDirectory(ethifDir);
        model.Project.AddIncludeDirectory(ipbaseDir);

        // get static files of EthIf and add them to compilation list
        foreach(string file in GetFiles(ethifDir, ethIfStaticFilePattern))
        {
          model.Project.AddBswFile(file, "EthIf");
          System.Console.WriteLine(file);
        }
        
        // get static files of IpBase and add them to compilation list
        foreach(string file in GetFiles(ipbaseDir, ipBaseStaticFilePattern))
        {
          model.Project.AddBswFile(file, "IpBase");
          System.Console.WriteLine(file);
        }

        // get generated files of EthIf and add them to compilation list
        foreach(string file in GetFiles(genDataDir, ethIfGenDataFilePattern))
        {
          model.Project.AddBswFile(file, "EthIf");
          System.Console.WriteLine(file);
        }
      }
    }
  }
}

