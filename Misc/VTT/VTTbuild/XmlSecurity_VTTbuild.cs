
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleXmlSecurity : IModule
  {
    public string Name
    {
      get { return "XmlSecurity"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/XmlSecurity"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string XmlSecurityDir = System.IO.Path.Combine(model.Directory.BSW, "XmlSecurity");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(XmlSecurityDir);

        // Select all header-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(XmlSecurityDir, "*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all C-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(XmlSecurityDir, "*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all header-files in GenData directory
        // Set of generated file is SIP specific. Generator will overwrite unused files with empty ones.
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "XmlSecurity_*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all source-files in GenData directory
        // Set of generated file is SIP specific. Generator will overwrite unused files with empty ones.
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "XmlSecurity_*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

