
using System.Collections.Generic;
using System.Linq;
using System;

using DSI4 = GenTool_CsDataServerDomAsr4.Iface;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVTTFls : IModule
  {
    public string Name
    {
      /* not used anymore! */
      get { return ""; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTFls"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      DSI4.IEcucModuleConfigurationValues[] moduleArray;

      moduleArray = ecuc.FindModuleConfigurationsByDefinitionPath(DefinitionPath);

      if (moduleArray.Count() == 0)
      {
        //Console.WriteLine("INFO: Fls is not part of this configuration.");
      }
      else if (moduleArray.Count() == 1)
      {
        string flsDir = System.IO.Path.Combine(model.Directory.BSW, "VttFls");
        string vttCntrlDir = System.IO.Path.Combine(model.Directory.BSW, "VTTCntrl");
        string genDataDir = model.Directory.GenData;
        string moduleName = moduleArray[0].ShortName;

        Console.WriteLine("INFO: Fls is part of this configuration and added to VS Solution.");
        
        model.Project.AddIncludeDirectory(flsDir);

        model.Project.AddBswFile(System.IO.Path.Combine(flsDir, "Fls.c"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(flsDir, "Fls.h"), moduleName);
        
        string f;
        
        f = System.IO.Path.Combine(flsDir, "Fls_17_Pmu.c");
        if (System.IO.File.Exists(f))
        {
            model.Project.AddBswFile(f, moduleName);
        }
        
        f = System.IO.Path.Combine(flsDir, "Fls_17_Pmu.h");
        if (System.IO.File.Exists(f))
        {
            model.Project.AddBswFile(f, moduleName);
        }


        if (System.IO.Directory.Exists(genDataDir))
        {
          foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "*.h", System.IO.SearchOption.TopDirectoryOnly))
          {
            if (System.IO.Path.GetFileName(file).StartsWith("Fls"))
            {
              model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, file), moduleName);
            }
          }
          foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "*.c", System.IO.SearchOption.TopDirectoryOnly))
          {
            if (System.IO.Path.GetFileName(file).StartsWith("Fls"))
            {
              model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, file), moduleName);
            }
          }
        }

        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Fls.c"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Fls.h"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Fls_Cfg.c"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Fls_Cfg.h"), moduleName);

        model.CANoeEmu.AddStateChangeFunction("VttFls_OnStateChange", "VttCntrl_Fls.h");
      }
      else
      {

        uint count = 0;
        Console.WriteLine("ERROR: Invalid Number ({0}) of configurations with definition for Fls!", moduleArray.Count());
        foreach (DSI4.IEcucModuleConfigurationValues element in moduleArray)
        {
          count++;
          Console.WriteLine(" - Name of element number {0}: {1}", count, element.ShortName);  
        }
        
      }
    }
  }
}

