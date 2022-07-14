
using System.Collections.Generic;
using System.Linq;
using System.IO;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleE2E : IModule
  {
    public string Name
    {
      get { return "E2E"; }
    }

    public string DefinitionPath
    {
      get { return ""; }
    }

    public void AddBswFileIfExists(IGeneratorModel model, string file)
    {
        if (File.Exists(file))
        {
            model.Project.AddBswFile(file, Name);
        }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
        if ((ecuc.FindModuleConfigurationsByDefinitionPath("/MICROSAR/E2EPW").Length > 0) || (ecuc.FindModuleConfigurationsByDefinitionPath("/MICROSAR/E2EXf").Length > 0))
        {
            string eepDir = System.IO.Path.Combine(model.Directory.BSW, "E2E");
            string genDataDir = model.Directory.GenData;

            model.Project.AddIncludeDirectory(eepDir);

            AddBswFileIfExists(model, Path.Combine(eepDir, "E2E.c"));
            AddBswFileIfExists(model, Path.Combine(eepDir, "E2E_P01.c"));
            AddBswFileIfExists(model, Path.Combine(eepDir, "E2E_P02.c"));
            AddBswFileIfExists(model, Path.Combine(eepDir, "E2E_P03.c"));
            AddBswFileIfExists(model, Path.Combine(eepDir, "E2E_P04.c"));
            AddBswFileIfExists(model, Path.Combine(eepDir, "E2E_P05.c"));
            AddBswFileIfExists(model, Path.Combine(eepDir, "E2E_P06.c"));
            AddBswFileIfExists(model, Path.Combine(eepDir, "E2E_P07.c"));
            AddBswFileIfExists(model, Path.Combine(eepDir, "E2E_SM.c"));

            AddBswFileIfExists(model, Path.Combine(eepDir, "E2E.h"));
            AddBswFileIfExists(model, Path.Combine(eepDir, "E2E_P01.h"));
            AddBswFileIfExists(model, Path.Combine(eepDir, "E2E_P02.h"));
            AddBswFileIfExists(model, Path.Combine(eepDir, "E2E_P03.h"));
            AddBswFileIfExists(model, Path.Combine(eepDir, "E2E_P04.h"));
            AddBswFileIfExists(model, Path.Combine(eepDir, "E2E_P05.h"));
            AddBswFileIfExists(model, Path.Combine(eepDir, "E2E_P06.h"));
            AddBswFileIfExists(model, Path.Combine(eepDir, "E2E_P07.h"));
            AddBswFileIfExists(model, Path.Combine(eepDir, "E2E_SM.h"));
        }
    }
  }
}

