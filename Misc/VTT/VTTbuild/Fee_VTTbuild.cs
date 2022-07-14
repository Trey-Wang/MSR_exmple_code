
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleFee : IModule
  {
    public string Name
    {
      get { return "Fee"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Fee"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string feeDir = System.IO.Path.Combine(model.Directory.BSW, "Fee");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(feeDir);

        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_ChunkInfo.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_ChunkInfo.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_Int.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_IntBase.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_Partition.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_Partition.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_Sector.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_Sector.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_Types.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_Processing.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_ChunkInfoDefs.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_InitEx.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_JobParams.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_PartitionDefs.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(feeDir, "Fee_SectorDefs.h"), Name);
        
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Fee_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Fee_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Fee_PrivateCfg.h"), Name);
      }
    }
  }
}

