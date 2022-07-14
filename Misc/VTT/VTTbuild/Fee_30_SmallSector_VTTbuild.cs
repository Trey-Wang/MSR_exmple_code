
using System.Collections.Generic;
using System.Linq;
using System;

using DSI4 = GenTool_CsDataServerDomAsr4.Iface;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleFee_30_SmallSector : IModule
  {
    public string Name
    {
      /* not used anymore! */
      get { return ""; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Fee_30_SmallSector/Fee"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      DSI4.IEcucModuleConfigurationValues[] moduleArray;

      moduleArray = ecuc.FindModuleConfigurationsByDefinitionPath(DefinitionPath);

      if (moduleArray.Count() == 0)
      {
        //Console.WriteLine("INFO: SmallSectorFee is not part of this configuration.");
      }
      else if (moduleArray.Count() == 1)
      {
        string feeDir = System.IO.Path.Combine(model.Directory.BSW, "Fee_30_SmallSector");
        string genDataDir = model.Directory.GenData;
        string moduleName = moduleArray[0].ShortName;

        Console.WriteLine("INFO: SmallSectorFee is part of this configuration and added to VS Solution.");
        model.Project.AddIncludeDirectory(feeDir);

        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector.c"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector.h"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector_Cbk.h"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector_BlockHandler.c"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector_BlockHandler.h"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector_DatasetHandler.c"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector_DatasetHandler.h"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector_FlsCoordinator.c"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector_FlsCoordinator.h"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector_InstanceHandler.c"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector_InstanceHandler.h"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector_Layer1_Read.c"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector_Layer1_Read.h"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector_Layer1_Write.c"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector_Layer1_Write.h"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector_Layer2_DatasetEraser.c"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector_Layer2_DatasetEraser.h"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector_Layer2_InstanceFinder.c"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector_Layer2_InstanceFinder.h"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector_Layer2_WriteInstance.c"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector_Layer2_WriteInstance.h"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector_Layer3_ReadManagementBytes.c"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector_Layer3_ReadManagementBytes.h"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector_PartitionHandler.c"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector_PartitionHandler.h"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector_TaskManager.c"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_30_SmallSector_TaskManager.h"), moduleName);

        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Fee_30_SmallSector_Cfg.h"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Fee_30_SmallSector_Cfg.c"), moduleName);

      }
      else
      {

        uint count = 0;
        Console.WriteLine("ERROR: Invalid Number ({0}) of configurations with definition for SmallSectorFee!", moduleArray.Count());
        foreach (DSI4.IEcucModuleConfigurationValues element in moduleArray)
        {
          count++;
          Console.WriteLine(" - Name of element number {0}: {1}", count, element.ShortName);  
        }
        
      }
    }
  }
}

