
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleDns : IModule
  {
    public string Name
    {
      get { return "Dns"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Dns"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string dnsDir = System.IO.Path.Combine(model.Directory.BSW, "Dns");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(dnsDir);

        // Select all header-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(dnsDir, "*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all C-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(dnsDir, "*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all header-files in GenData directory
        // Set of generated file is SIP specific. Generator will overwrite unused files with empty ones.
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "Dns_*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all source-files in GenData directory
        // Set of generated file is SIP specific. Generator will overwrite unused files with empty ones.
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "Dns_*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
            model.Project.AddBswFile(file, Name);
        }

		/* Comment Section
		
		End of Comment Section */ 
      }
    }
  }
}

