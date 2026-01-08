/*******************************************************************************
** Copyright 2015-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** Example of an ADI setup using two simple UINT16 ADIs representing speed and
** reference speed of a motor. Both are mapped as cyclical process data
** parameters.
**
** Make sure that the following definitions, if they exist in
** abcc_driver_config.h, are set to the following:
**    ABCC_CFG_STRUCT_DATA_TYPE_ENABLED     0
**    ABCC_CFG_ADI_GET_SET_CALLBACK_ENABLED 0
**
**
** Example extension: Assembly mapping object
** Utilizing two additional ADI, i.e., a total of four ADI to create 3 different
** write asseblies and 2 different read assemblies.
**
** Make sure that the following definitions in
** abcc_driver_config.h are set to the following:
**    ASM_OBJ_ENABLE 1
**    ABCC_CFG_REMAP_SUPPORT_ENABLED 1
**    ASM_IA_NAME_ENABLE 1
**    ASM_IA_MAX_NUM_ADI_MAPS_ENABLE 1
**
** In case of running EtherNet/IP, make sure that EtherNet/IP host object is
** enabled and that instance attributes 7 and 8 are supported. For example,
** entered into ABCC_API_COMMAND_RESPONSE_LIST like this:
**    ABCC_ETHERNETIP_OBJ_PROD_INSTANCE_GET_VALUE("\x64\x00\x65\x00\x66\x00", 6), \
**    ABCC_ETHERNETIP_OBJ_CONS_INSTANCE_GET_VALUE("\x96\x00\x97\x00", 4)
**
** Corresponding to producing instances 0x0064, 0x0065, and 0x0066, and
** consuming instances 0x0096 and 0x0097.
********************************************************************************
*/

#include "stdint.h"
#include "abcc_api.h"
#include "abp_asm.h"
#include "abcc_application_data_interface.h"

#if ( ABCC_CFG_STRUCT_DATA_TYPE_ENABLED || ABCC_CFG_ADI_GET_SET_CALLBACK_ENABLED )
   #error ABCC_CFG_ADI_GET_SET_CALLBACK_ENABLED must be set to 0 and ABCC_CFG_STRUCT_DATA_TYPE_ENABLED set to 0 in order to run this example
#endif

/*------------------------------------------------------------------------------
** Data holder for the network data parameters (ADI)
**------------------------------------------------------------------------------
*/
static uint16_t appl_iSpeed     = 0x0000;
static uint16_t appl_iRefSpeed  = 0x0000;
static uint16_t appl_iTorque    = 0x0000;
static uint16_t appl_iRefTorque = 0x0000;

/*------------------------------------------------------------------------------
** Network data parameters (ADI) properties (min, max, default values)
**------------------------------------------------------------------------------
*/
static AD_UINT16Type appl_sSpeedProp     = { { 0, 0xFFFF, 0x0000 } };
static AD_UINT16Type appl_sRefSpeedProp  = { { 0, 0xFFFF, 0x0000 } };
static AD_UINT16Type appl_sTorqueProp    = { { 0, 0xFFFF, 0x0000 } };
static AD_UINT16Type appl_sRefTorqueProp = { { 0, 0xFFFF, 0x0000 } };

/*
** The ADI entry list (AD_AdiEntryType):
** ----------------------------------------------------------------------------------------------------------------------------
** | 1. iInstance | 2. pabName | 3. bDataType | 4. bNumOfElements | 5. bDesc | 6.pxValuePtr | 7. pxValuePropPtr | 8. psStruct |
** ----------------------------------------------------------------------------------------------------------------------------
*/
const AD_AdiEntryType ABCC_API_asAdiEntryList[] =
{
   { 0x1, "SPEED",      ABP_UINT32, 1, AD_ADI_DESC___W_G, { { &appl_iSpeed,     &appl_sSpeedProp } } },
   { 0x2, "REF_SPEED",  ABP_SINT32, 1, AD_ADI_DESC__R_SG, { { &appl_iRefSpeed,  &appl_sRefSpeedProp } } },
   { 0x3, "TORQUE",     ABP_UINT16, 1, AD_ADI_DESC___W_G, { { &appl_iTorque,    &appl_sTorqueProp } } },
   { 0x4, "REF_TORQUE", ABP_SINT16, 1, AD_ADI_DESC__R_SG, { { &appl_iRefTorque, &appl_sRefTorqueProp } } }
};

/*
** Default map (AD_MapType):
**-------------------------------------------------------------
** | 1. AdiIndex | 2. Direction | 3. NumElem  | 4. StartIndex |
**-------------------------------------------------------------
*/
const AD_MapType ABCC_API_asAdObjDefaultMap[] =
{
   { 1, PD_WRITE, AD_MAP_ALL_ELEM, 0 }, /* Speed */
   { 2, PD_READ,  AD_MAP_ALL_ELEM, 0 }, /* RefSpeed */
   { AD_MAP_END_ENTRY }
};

#if ASM_OBJ_ENABLE

#if !ABCC_CFG_REMAP_SUPPORT_ENABLED
   #error "Remap support must be enabled for this example."
#endif

#if !ASM_IA_NAME_ENABLE
   #error "Assembly instance names must be supported for this example."
#endif

#if !ASM_IA_MAX_NUM_ADI_MAPS_ENABLE
   #error "Max number of ADI maps must be supported for this example."
#endif

/*------------------------------------------------------------------------------
** Example assembly write maps.
**------------------------------------------------------------------------------
*/
const AD_MapType ABCC_API_asAsmWriteMap1[] =
{
   { 1, PD_WRITE, AD_MAP_ALL_ELEM, 0 }, /* Speed */
   { 3, PD_WRITE, AD_MAP_ALL_ELEM, 0 }, /* Torque */
   { AD_MAP_END_ENTRY }
};

const AD_MapType ABCC_API_asAsmWriteMap2[] =
{
   { 1, PD_WRITE, AD_MAP_ALL_ELEM, 0 }, /* Speed */
   { AD_MAP_END_ENTRY }
};

const AD_MapType ABCC_API_asAsmWriteMap3[] =
{
   { 3, PD_WRITE, AD_MAP_ALL_ELEM, 0 }, /* Torque */
   { AD_MAP_END_ENTRY }
};

/*------------------------------------------------------------------------------
** Example assembly read maps.
**------------------------------------------------------------------------------
*/
const AD_MapType ABCC_API_asAsmReadMap1[] =
{
   { 2, PD_READ, AD_MAP_ALL_ELEM, 0 }, /* RefSpeed */
   { 4, PD_READ, AD_MAP_ALL_ELEM, 0 }, /* RefTorque */
   { AD_MAP_END_ENTRY }
};

const AD_MapType ABCC_API_asAsmReadMap2[] =
{
   { 2, PD_READ, AD_MAP_ALL_ELEM, 0 }, /* RefTorque */
   { AD_MAP_END_ENTRY }
};

/*------------------------------------------------------------------------------
** Assembly mapping instances.
**------------------------------------------------------------------------------
*/
const ASM_InstanceType ABCC_API_sAsmWriteMapInst1 =
{
   ABP_ASM_IA_DESC_WRITE | ABP_ASM_IA_DESC_STATIC | ABP_ASM_IA_DESC_PD_MAPPABLE,
   ABCC_API_asAsmWriteMap1,
   "Write mappable assembly 1",
   sizeof( ABCC_API_asAsmWriteMap1 ) / sizeof( AD_MapType ) - 1
};

const ASM_InstanceType ABCC_API_sAsmWriteMapInst2 =
{
   ABP_ASM_IA_DESC_WRITE | ABP_ASM_IA_DESC_STATIC | ABP_ASM_IA_DESC_PD_MAPPABLE,
   ABCC_API_asAsmWriteMap2,
   "Write mappable assembly 2",
   sizeof( ABCC_API_asAsmWriteMap2 ) / sizeof( AD_MapType ) - 1
};

const ASM_InstanceType ABCC_API_sAsmWriteMapInst3 =
{
   ABP_ASM_IA_DESC_WRITE | ABP_ASM_IA_DESC_STATIC | ABP_ASM_IA_DESC_PD_MAPPABLE,
   ABCC_API_asAsmWriteMap3,
   "Write mappable assembly 3",
   sizeof( ABCC_API_asAsmWriteMap3 ) / sizeof( AD_MapType ) - 1
};

const ASM_InstanceType ABCC_API_sAsmReadMapInst1 =
{
   ABP_ASM_IA_DESC_READ | ABP_ASM_IA_DESC_STATIC | ABP_ASM_IA_DESC_PD_MAPPABLE,
   ABCC_API_asAsmReadMap1,
   "Read mappable assembly 1",
   sizeof( ABCC_API_asAsmReadMap1 ) / sizeof( AD_MapType ) - 1
};

const ASM_InstanceType ABCC_API_sAsmReadMapInst2 =
{
   ABP_ASM_IA_DESC_READ | ABP_ASM_IA_DESC_STATIC | ABP_ASM_IA_DESC_PD_MAPPABLE,
   ABCC_API_asAsmReadMap2,
   "Read mappable assembly 2",
   sizeof( ABCC_API_asAsmReadMap2 ) / sizeof( AD_MapType ) - 1
};

/*------------------------------------------------------------------------------
** Array of all assembly mapping instances.
**------------------------------------------------------------------------------
*/
const ASM_InstanceType* ABCC_API_aasAsmInstances[] =
{
   &ABCC_API_sAsmWriteMapInst1,
   &ABCC_API_sAsmWriteMapInst2,
   &ABCC_API_sAsmWriteMapInst3,
   &ABCC_API_sAsmReadMapInst1,
   &ABCC_API_sAsmReadMapInst2
};

const ASM_InstanceType** ABCC_API_CbfGetAsmInstances( void )
{
   return( ABCC_API_aasAsmInstances );
}

UINT16 ABCC_API_CbfGetNumAsmInstances( void )
{
   return( sizeof( ABCC_API_aasAsmInstances ) / sizeof( ASM_InstanceType* ) );
}
#endif /* ASM_OBJ_ENABLE */


UINT16 ABCC_API_CbfGetNumAdi( void )
{
   return( sizeof( ABCC_API_asAdiEntryList ) / sizeof( AD_AdiEntryType ) );
}

/*------------------------------------------------------------------------------
** Example - electric motor speed control loop
**------------------------------------------------------------------------------
*/
void ABCC_API_CbfCyclicalProcessing()
{
   if( ABCC_API_AnbState() == ABP_ANB_STATE_PROCESS_ACTIVE )
   {
      /*
      ** An example of ADI data handling.
      */
      if( appl_iSpeed > appl_iRefSpeed )
      {
         /*
         ** Do something that lowers speed.
         */
         appl_iSpeed -= 1;
      }
      else if( appl_iSpeed < appl_iRefSpeed )
      {
         /*
         ** Do something that increases speed.
         */
         appl_iSpeed += 1;
      }
   }
   else
   {
      /*
      ** We are not in process active, the default should be that the motor
      ** should not run.
      */
      appl_iSpeed = 0;
   }
}