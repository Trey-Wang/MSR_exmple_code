/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *          File:  TSC_CtSaDoor.h
 *        Config:  MyECU.dpa
 *   ECU-Project:  MyECU
 *
 *     Generator:  MICROSAR RTE Generator Version 4.16.0
 *                 RTE Core Version 1.16.0
 *       License:  Unlimited license CBD0000000 for N/A
 *
 *   Description:  Header of wrapper software component for Bte-based Rte test cases
 *********************************************************************************************************************/


/** Sender receiver - explicit write services */
Std_ReturnType TSC_CtSaDoor_Rte_Write_PpDoorState_DeDoorState(P2CONST(struct Rte_CDS_CtSaDoor, AUTOMATIC, RTE_CONST) self, IdtDoorState data);

/** Client server interfaces */
Std_ReturnType TSC_CtSaDoor_Rte_Call_PpDoorIoHwAb_ReadChannel(const struct Rte_CDS_CtSaDoor* self, IdtDioValueType *value);




/** Sender receiver - explicit write services */
Std_ReturnType TSC_CtSaDoor_Rte_Write_PpDoorState_DeDoorState(P2CONST(struct Rte_CDS_CtSaDoor, AUTOMATIC, RTE_CONST) self, IdtDoorState data);

/** Client server interfaces */
Std_ReturnType TSC_CtSaDoor_Rte_Call_PpDoorIoHwAb_ReadChannel(const struct Rte_CDS_CtSaDoor* self, IdtDioValueType *value);




