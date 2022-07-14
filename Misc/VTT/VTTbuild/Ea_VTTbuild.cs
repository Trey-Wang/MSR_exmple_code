
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleEa : IModule
  {
    public string Name
    {
      get { return "Ea"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Ea"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string eaDir = System.IO.Path.Combine(model.Directory.BSW, "Ea");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(eaDir);

        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_BlockHandler.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_BlockHandler.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_DatasetHandler.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_DatasetHandler.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_EepCoordinator.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_EepCoordinator.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_InstanceHandler.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_InstanceHandler.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_Layer1_Erase.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_Layer1_Erase.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_Layer1_Invalidate.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_Layer1_Invalidate.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_Layer1_Read.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_Layer1_Read.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_Layer1_Write.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_Layer1_Write.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_Layer2_InstanceFinder.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_Layer2_InstanceFinder.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_Layer2_InvalidateInstance.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_Layer2_InvalidateInstance.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_Layer2_WriteInstance.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_Layer2_WriteInstance.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_Layer3_ReadManagementBytes.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_Layer3_ReadManagementBytes.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_PartitionHandler.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_PartitionHandler.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_TaskManager.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(eaDir, "Ea_TaskManager.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Ea_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Ea_Cfg.c"), Name);
      }
    }
  }
}

