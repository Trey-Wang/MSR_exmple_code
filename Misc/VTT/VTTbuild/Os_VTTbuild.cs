
using System.Collections.Generic;
using System.Linq;
using System;
using System.IO;
using System.Text.RegularExpressions;

using DSI4 = GenTool_CsDataServerDomAsr4.Iface;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVTTOs : IModule
  {
    public string Name
    {
      get { return "VTTOs"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTOs"; }
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
        var containerList = ecuc.EcucModuleConfigurationValues.GetContainersByDefinition("OsPublishedInformation").OfType<DSI4.IARObject>();

        if (containerList.Count() == 1)
        {
          ConfigureMultiCoreVTTOs(ecuc, model, containerList);
        }
        else
        {
          ConfigureLegacyVTTOs(ecuc, model);
        }
      }
    }

    void ConfigureMultiCoreVTTOs(IEcucProvider ecuc, IGeneratorModel model, IEnumerable<DSI4.IARObject> containerList)
    {
      string osDir = System.IO.Path.Combine(model.Directory.BSW, "VttOs");
      string genDataDir = model.Directory.GenData;
      string templateDir = model.Directory.Source;

      model.Project.AddIncludeDirectory(osDir);

      DSI4.IEcucParameterValue parameterValue = ((DSI4.IEcucContainerValue)containerList.First()).GetParameterValueByDefinition("/MICROSAR/VTT/VTTOs/OsPublishedInformation/OsKernelVersion", false);
      if (parameterValue != null)
      {
        int kernelVersion = 0;
        DSI4.IEcucNumericalParamValue numValue = (DSI4.IEcucNumericalParamValue)parameterValue;
        if (numValue != null)
        {
          kernelVersion = Int32.Parse(numValue.Value.Value);
        }

        /* OS files */
        foreach(string file in GetFiles(osDir, @"^Os.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

        /* GenData */
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Ioc.h"), Name);
        foreach(string file in GetFiles(genDataDir, @"^Os.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }


        Regex regexNumberOfCores = new Regex(@"^\s*OS_COREID_COUNT\s*=.*?(\d+)");
        Regex regexNumberOfCat2ISRs = new Regex(@"^\s*OS_ISRID_COUNT\s*=.*?(\d+)");
        Regex regexNumberOfCat1ISRs = new Regex(@"^\s*OS_ISRCAT1ID_COUNT\s*=.*?(\d+)");

        Regex regexNumberOfInterrupts = new Regex(@"^\s*#\s*define\s+OS_CANOE_IRQ_COUNT.*?(\d+)");
        Regex regexNumberOfIntLevels = new Regex(@"^\s*#\s*define\s+OS_CFG_MAX_INT_LEVEL.*?(\d+)");
        Regex regexNumberOfHRTs = new Regex(@"^\s*OS_CANOE_HRT_COUNT\s*=.*?(\d+)");

        Match m;
        string line;

        int numberOfCores = -1;
        int numberOfCat2ISRs = -1;

        int numberOfHRTs = -1;

        int numberOfInterrupts = -1;
        int numberOfIntLevels = -1;

        using (StreamReader r = new StreamReader(System.IO.Path.Combine(genDataDir, "Os_Types_Lcfg.h")))
        {
          while ((line = r.ReadLine()) != null)
          {
            if ((m = regexNumberOfCat2ISRs.Match(line)).Success)
              numberOfCat2ISRs = Int32.Parse(m.Groups[1].Value);
          }
        }

        using (StreamReader r = new StreamReader(System.IO.Path.Combine(genDataDir, "Os_Hal_Lcfg.h")))
        {
          while ((line = r.ReadLine()) != null)
          {
            if ((m = regexNumberOfCores.Match(line)).Success)
              numberOfCores = Int32.Parse(m.Groups[1].Value);
          }
        }

        using (StreamReader r = new StreamReader(System.IO.Path.Combine(genDataDir, "Os_Hal_Cfg.h")))
        {
          while ((line = r.ReadLine()) != null)
          {
            if ((m = regexNumberOfInterrupts.Match(line)).Success)
              numberOfInterrupts = Int32.Parse(m.Groups[1].Value);

            if ((m = regexNumberOfIntLevels.Match(line)).Success)
              numberOfIntLevels = Int32.Parse(m.Groups[1].Value);

            if ((m = regexNumberOfHRTs.Match(line)).Success)
              numberOfHRTs = Int32.Parse(m.Groups[1].Value);
          }
        }

        if (numberOfCores < 0)
          throw new Exception("Found errors in GenData (numberOfCores)");

        if (numberOfCat2ISRs < 0)
          throw new Exception("Found errors in GenData (numberOfCat2ISRs)");

        if (numberOfInterrupts < 0)
          throw new Exception("Found errors in GenData (numberOfInterrupts)");

        if (numberOfIntLevels < 0)
          throw new Exception("Found errors in GenData (numberOfIntLevels)");

        if (numberOfHRTs < 0)
          throw new Exception("Found errors in GenData (numberOfHRTs)");


        // Set the number of processor cores (range 1-32).
        model.CANoeEmu.SetNumberOfCores(numberOfCores);

        // Set the number of interrupts (range 1-10000).
        model.CANoeEmu.SetNumberOfInterrupts(numberOfInterrupts);

        // Set the number of interrupt levels (range 1-200).
        model.CANoeEmu.SetNumberOfInterruptLevels(numberOfIntLevels);

        // Set the number of non-maskable interrupt levels (range from 0 to numberOfInterruptLevels-1).
        model.CANoeEmu.SetNumberOfNmiLevels(0);

        // Set the number of system timers (range 0-1)
        model.CANoeEmu.SetNumberOfSystemTimers(0);

        // Set the Number of High Resolution Timers (range 0-256)
        model.CANoeEmu.SetNumberOfHighResolutionTimers(numberOfHRTs);

        // Configure state change handler
        if (System.IO.File.Exists(System.IO.Path.Combine(osDir, "Os_Hal_Processor.h")))
        {
          model.CANoeEmu.AddStateChangeFunction("VttOs_OnStateChange", "Os_Hal_Processor.h");
        }
      }
      else
      {
        throw new Exception("Missing parameter: /MICROSAR/VTT/VTTOs/OsPublishedInformation/OsKernelVersion");
      }
    }

    void ConfigureLegacyVTTOs(IEcucProvider ecuc, IGeneratorModel model)
    {
      string osDir = System.IO.Path.Combine(model.Directory.BSW, "VttOs");
      string genDataDir = model.Directory.GenData;
      string templateDir = model.Directory.Source;

      model.Project.AddIncludeDirectory(osDir);
      model.Project.AddLibraryDirectory(osDir);

      bool isOsStatusExtended = false;
      {
        var containerList = ecuc.EcucModuleConfigurationValues.GetContainersByDefinition("OsOS").OfType<DSI4.IARObject>();
        if (containerList.Count() == 1)
        {
          DSI4.IEcucContainerValue containerValue = (DSI4.IEcucContainerValue)containerList.First();
          DSI4.IEcucParameterValue parameterValue = containerValue.GetParameterValueByDefinition("/MICROSAR/VTT/VTTOs/OsOS/OsStatus", false);
          DSI4.IEcucTextualParamValue textualParamValue = (DSI4.IEcucTextualParamValue)parameterValue;
          if (textualParamValue != null)
          {
            isOsStatusExtended = (textualParamValue.ValueSpecified && textualParamValue.Value == "EXTENDED");
          }
        }
      }

      switch (model.Project.Compiler)
      {
        case CompilerVersion.eVC100:
        case CompilerVersion.eVC120:
#if (VTT_VC141)
        case CompilerVersion.eVC141:
#endif
          if (isOsStatusExtended)
          {
            model.Project.AddLibrary("OsekEmu_VC100_XR.lib", ProjectConfiguration.eRelease);
            model.Project.AddLibrary("OsekEmu_VC100_XD.lib", ProjectConfiguration.eDebug);
          }
          else
          {
            model.Project.AddLibrary("OsekEmu_VC100_SR.lib", ProjectConfiguration.eRelease);
            model.Project.AddLibrary("OsekEmu_VC100_SD.lib", ProjectConfiguration.eDebug);
          }
          break;
        default:
          throw new Exception("Compiler Version: The selected compiler version is not supported by VTTOs");
      }

      model.Project.AddBswFile(System.IO.Path.Combine(osDir, "Os.h"), Name);
      model.Project.AddBswFile(System.IO.Path.Combine(osDir, "Os_cfg.h"), Name);
      model.Project.AddBswFile(System.IO.Path.Combine(osDir, "osek.h"), Name);
      model.Project.AddBswFile(System.IO.Path.Combine(osDir, "osekasrt.h"), Name);
      model.Project.AddBswFile(System.IO.Path.Combine(osDir, "osekcov.h"), Name);
      model.Project.AddBswFile(System.IO.Path.Combine(osDir, "osekerr.h"), Name);
      model.Project.AddBswFile(System.IO.Path.Combine(osDir, "osekext.h"), Name);
      model.Project.AddBswFile(System.IO.Path.Combine(osDir, "vrm.h"), Name);
      model.Project.AddBswFile(System.IO.Path.Combine(osDir, "emptymac.h"), Name);
      model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "tcb.c"), Name);
      model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "tcb.h"), Name);
      model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "msg.c"), Name);
      model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "msg.h"), Name);
      model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "trustfct.h"), Name);
      model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "trustfct.c"), Name);
      model.Project.AddBswFile(System.IO.Path.Combine(templateDir, "Os_Callout_Stubs.c"), Name);

      /* Add special files for multicore configuration */
      model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VTTOs_Hardware_Timer.c"), Name);

      string osMemMapHeader = System.IO.Path.Combine(osDir, "Os_MemMap.h");
      if (System.IO.File.Exists(osMemMapHeader))
      {
        /* Either Memory Protection or Multicore Support is configured */
        model.Project.AddBswFile(osMemMapHeader, Name);
        string osMemMapImpl = System.IO.Path.Combine(osDir, "Os_MemMap.c");
        if (System.IO.File.Exists(osMemMapImpl))
        {
          model.Project.AddBswFile(osMemMapImpl, Name);
          model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VTTOs_Multicore_Cfg.h"), Name);
          model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VTTOs_Multicore_Cfg.c"), Name);
          model.Project.AddBswFile(System.IO.Path.Combine(templateDir, "VTTOs_Multicore_Callout_Stubs.c"), Name);
        }
      }
      model.CANoeEmu.UseVariableOskNumberOfInterruptSources();
    }
  }

}

