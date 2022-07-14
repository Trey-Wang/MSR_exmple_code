
using System.Collections.Generic;
using System.Linq;
using System;

using DSI4 = GenTool_CsDataServerDomAsr4.Iface;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVTTWdg : IModule
  {
    public string Name
    {
      /* not used anymore! */
      get { return ""; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTWdg"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      DSI4.IEcucModuleConfigurationValues[] moduleArray;

      moduleArray = ecuc.FindModuleConfigurationsByDefinitionPath(DefinitionPath);

      if (moduleArray.Count() == 0)
      {
        //Console.WriteLine("INFO: Wdg is not part of this configuration.");
      }
      else if (moduleArray.Count() == 1)
      {
        string wdgDir = System.IO.Path.Combine(model.Directory.BSW, "VttWdg");
        string vttCntrlDir = System.IO.Path.Combine(model.Directory.BSW, "VttCntrl");
        string genDataDir = model.Directory.GenData;
        string moduleName = moduleArray[0].ShortName;

        Console.WriteLine("INFO: Wdg is part of this configuration and added to VS Solution.");
        
        model.Project.AddIncludeDirectory(wdgDir);

        model.Project.AddBswFile(System.IO.Path.Combine(wdgDir, "Wdg.h"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(wdgDir, "Wdg.c"), moduleName);

        if (System.IO.Directory.Exists(genDataDir))
        {
          foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "*.h", System.IO.SearchOption.TopDirectoryOnly))
          {
            if (System.IO.Path.GetFileName(file).StartsWith("Wdg_"))
            {
              model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, file), moduleName);
            }
          }
          foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "*.c", System.IO.SearchOption.TopDirectoryOnly))
          {
            if (System.IO.Path.GetFileName(file).StartsWith("Wdg_"))
            {
              model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, file), moduleName);
            }
          }
        }

        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Wdg.c"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Wdg.h"), moduleName);
        
        model.CANoeEmu.AddStateChangeFunction("VttWdg_OnStateChange", "VttCntrl_Wdg.h");
      }
      else
      {

        uint count = 0;
        Console.WriteLine("ERROR: Invalid Number ({0}) of configurations with definition for Wdg!", moduleArray.Count());
        foreach (DSI4.IEcucModuleConfigurationValues element in moduleArray)
        {
          count++;
          Console.WriteLine(" - Name of element number {0}: {1}", count, element.ShortName);  
        }
        
      }
    }
  }
}

