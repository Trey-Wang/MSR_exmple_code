
using System.Collections.Generic;
using System.Linq;

using DSI4 = GenTool_CsDataServerDomAsr4.Iface;

namespace Vector.VTT.VsProjectGenerator
{
    class ModuleCpl : IModule
    {
        public string Name
        {
            get { return "Cpl"; }
        }

        public string DefinitionPath
        {
            get { return "/MICROSAR/Cpl"; }
        }

        private static bool CheckIfCplPrimitiveIsConfigured(DSI4.IEcucModuleConfigurationValues ecucModuleConfigurationValues, string cplPrimitiveConfigurationContainerDefinition, string calReferenceParameterDefinition)
        {
            // get the Cpl primitive configuration 
            var cplPrimitiveConfigurationContainers = ecucModuleConfigurationValues.GetContainersByDefinition(cplPrimitiveConfigurationContainerDefinition).OfType<DSI4.IARObject>();
            if (cplPrimitiveConfigurationContainers.Count() > 0)
            {
                foreach (var cplPrimitiveConfigurationARObject in cplPrimitiveConfigurationContainers)
                {
                    var cplPrimitiveConfigurationContainer = (DSI4.IEcucContainerValue)cplPrimitiveConfigurationARObject;
                    var referenceList = cplPrimitiveConfigurationContainer.ReferencedFromList;
                    foreach (DSI4.IARRef reference in referenceList)
                    {
                        var owner = reference.Owner;
                        DSI4.IEcucReferenceValue referenceValue = owner as DSI4.IEcucReferenceValue;
                        if (referenceValue != null)
                        {
                            if (referenceValue.Definition.Value == calReferenceParameterDefinition)
                            {
                                return true;
                            }
                        }
                    }
                }
            }
            return false;
        }

        public void Configure(IEcucProvider ecuc, IGeneratorModel model)
        {
            if (ecuc.IsConfigured)
            {
                string cplDir = System.IO.Path.Combine(model.Directory.BSW, "Cpl");
                string genDataDir = model.Directory.GenData;

                model.Project.AddIncludeDirectory(cplDir);

                model.Project.AddBswFile(System.IO.Path.Combine(cplDir, "Cpl.h"), Name);
                model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Cpl_Cfg.h"), Name);
                model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Cpl_Cfg.c"), Name);

                var ecucModuleConfigurationValues = ecuc.EcucModuleConfigurationValues;

                if (CheckIfCplPrimitiveIsConfigured(ecucModuleConfigurationValues, "CplAesCmacGenerate/CplAesCmacGenerateConfig",
                    "/MICROSAR/Cal/CalMacGenerate/CalMacGenerateConfig/CalMacGenerateCplRef")
                    || CheckIfCplPrimitiveIsConfigured(ecucModuleConfigurationValues, "CplAesCmacVerify/CplAesCmacVerifyConfig",
                    "/MICROSAR/Cal/CalMacVerify/CalMacVerifyConfig/CalMacVerifyCplRef"))
                {
                    model.Project.AddBswFile(System.IO.Path.Combine(cplDir, "Cpl_AesCmac.c"), Name);
                    model.Project.AddBswFile(System.IO.Path.Combine(cplDir, "Cpl_AesCmac.h"), Name);
                }


                if (CheckIfCplPrimitiveIsConfigured(ecucModuleConfigurationValues, "CplAesCtrDrbg/CplAesCtrDrbgConfig",
                    "/MICROSAR/Cal/CalRandomGenerate/CalRandomGenerateConfig/CalRandomGenerateCplRef")
                    || CheckIfCplPrimitiveIsConfigured(ecucModuleConfigurationValues, "CplAesCtrDrbg/CplAesCtrDrbgConfig",
                    "/MICROSAR/Cal/CalRandomSeed/CalRandomSeedConfig/CalRandomSeedCplRef"))
                {
                    model.Project.AddBswFile(System.IO.Path.Combine(cplDir, "Cpl_AesCtrDrbg.c"), Name);
                    model.Project.AddBswFile(System.IO.Path.Combine(cplDir, "Cpl_AesCtrDrbg.h"), Name);
                }

                if (CheckIfCplPrimitiveIsConfigured(ecucModuleConfigurationValues, "CplFips186/CplFips186Config",
                    "/MICROSAR/Cal/CalRandomGenerate/CalRandomGenerateConfig/CalRandomGenerateCplRef")
                    || CheckIfCplPrimitiveIsConfigured(ecucModuleConfigurationValues, "CplFips186/CplFips186Config",
                    "/MICROSAR/Cal/CalRandomSeed/CalRandomSeedConfig/CalRandomSeedCplRef"))
                {
                    model.Project.AddBswFile(System.IO.Path.Combine(cplDir, "Cpl_Fips186.c"), Name);
                    model.Project.AddBswFile(System.IO.Path.Combine(cplDir, "Cpl_Fips186.h"), Name);
                }

                if (CheckIfCplPrimitiveIsConfigured(ecucModuleConfigurationValues, "CplAesDecrypt128/CplAesDecrypt128Config",
                    "/MICROSAR/Cal/CalSymDecrypt/CalSymDecryptConfig/CalSymDecryptCplRef"))
                {
                    model.Project.AddBswFile(System.IO.Path.Combine(cplDir, "Cpl_AesDecrypt128.c"), Name);
                    model.Project.AddBswFile(System.IO.Path.Combine(cplDir, "Cpl_AesDecrypt128.h"), Name);
                }

                if (CheckIfCplPrimitiveIsConfigured(ecucModuleConfigurationValues, "CplAesEncrypt128/CplAesEncrypt128Config",
                    "/MICROSAR/Cal/CalSymEncrypt/CalSymEncryptConfig/CalSymEncryptCplRef"))
                {
                    model.Project.AddBswFile(System.IO.Path.Combine(cplDir, "Cpl_AesEncrypt128.c"), Name);
                    model.Project.AddBswFile(System.IO.Path.Combine(cplDir, "Cpl_AesEncrypt128.h"), Name);
                }

                if (CheckIfCplPrimitiveIsConfigured(ecucModuleConfigurationValues, "CplRsaSha256Verify/CplRsaSha256VerifyConfig",
                    "/MICROSAR/Cal/CalSignatureVerify/CalSignatureVerifyConfig/CalSignatureVerifyCplRef"))
                {
                    model.Project.AddBswFile(System.IO.Path.Combine(cplDir, "Cpl_RsaSha256Verify.c"), Name);
                    model.Project.AddBswFile(System.IO.Path.Combine(cplDir, "Cpl_RsaSha256Verify.h"), Name);
                }
            }
        }
    }
}