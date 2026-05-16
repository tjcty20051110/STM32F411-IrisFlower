/**
  ******************************************************************************
  * @file    network_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Sat May 16 08:17:10 2026
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#include "network_data_params.h"


/**  Activations Section  ****************************************************/
ai_handle g_network_activations_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(NULL),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};




/**  Weights Section  ********************************************************/
AI_ALIGNED(32)
const ai_u64 s_network_weights_array_u64[162] = {
  0x3dc49aa63e88236cU, 0x3f30cbaa3efa7077U, 0x3f467360bec1eae1U, 0xbf40c02fbe59a562U,
  0x3f5bc7f6bea8d082U, 0xbed884a7bf1e1d06U, 0xbecd2eb33eb0b690U, 0x3eaf560a3d8c7d28U,
  0xbdf6e6f0be4ea1b1U, 0x3eeb10713f797328U, 0xbece45c83e144f1eU, 0xbec5be343f243962U,
  0xbeaf44343eb4ea0dU, 0x3eac65683dc96ec6U, 0x3f0fd8583e6e1f81U, 0x3eaa1a0ebeae186aU,
  0xbf10ad463d594170U, 0x3f89aa813edc5794U, 0x3ed589d1beb9eb15U, 0xbf44ace7bef21d2aU,
  0x3e0e264f3dd61eecU, 0x3eecbde4bef04ed7U, 0xbdd468643eda2971U, 0xbdaebf8e3eb9d9b9U,
  0x3ba478043f10d158U, 0xbdc99d433f2e7bb0U, 0x3d9f660ebf644039U, 0xbebe2263beed02ccU,
  0xbe148ec3bdf76fa4U, 0x3f24a63a3f648107U, 0xbf0f69d73ce77979U, 0xbf3e0d68be2f51dbU,
  0x3f81cfdabd090439U, 0x3ed5e8b83f8bb9f1U, 0x3f11b30cbebd6631U, 0xbe8a66dd3f29bd26U,
  0x3e057aa83d1a95ccU, 0x3f0fc1ad3edb24b7U, 0x3e451fd5bee22abbU, 0x3f316803bed734d9U,
  0x3f28bee13d24022eU, 0xbe0c09343f2d1fecU, 0x3eabb00dbeadac93U, 0xbe060a6abda229f6U,
  0x3e95d8593c1e2fc8U, 0x3e3db4a43e34d075U, 0x3eb99dbfbd68614bU, 0x3f0db930bf0ec5e8U,
  0x3d44dad03eae24afU, 0x3ee03c04be739be7U, 0x3edabe1c3ea318cdU, 0x3e2253ae3f2f9d22U,
  0xbe52813a3ebc1cd3U, 0x3ee968043ed9fcf1U, 0xbdf333f03db6249cU, 0x3e3558103e6ff03dU,
  0x3cfff0363e940fa3U, 0x3ea68b8ebdc1fb08U, 0x3e5f0b0d3f2db06aU, 0x3e37cb3e3edd9c23U,
  0xbe96e3883e279400U, 0x3e8cbe303d72563aU, 0x3db9bfb43ee520faU, 0xbec0caeb3f1e39fdU,
  0x3ed4c9e53e85611eU, 0x3e08eecf3e26f450U, 0xbe572a493eda200fU, 0x3e139dd9bdf1985dU,
  0x3f2806863e54497cU, 0x3ca4724cbcdd4cdeU, 0x3f1bd3133dd281c2U, 0xbe1bcb7c3f423d0eU,
  0x3c9d2d543c8b5da1U, 0xbe79e428be96ee63U, 0xbdf3f4dc3e02976aU, 0xbe93a2b53b029516U,
  0xbdb093d8be7b9961U, 0xbe9ae9893dbee21eU, 0xbe4e00a43dc80a26U, 0xbdbf8ac8bd96a0d8U,
  0xbf21fd6a3eac8ec4U, 0x3d959072bf699b54U, 0x3d70f13c3f65204bU, 0x3df5b96ebe10a4c7U,
  0x3d2e3ad13ecb9c29U, 0x3dc4e4d4ba3eaac0U, 0x3ee8d5cf3e831280U, 0xbea94f103f0f6257U,
  0x3e502dec3ea8caedU, 0x3f0e876d3d300c46U, 0x3f547086be85bfd7U, 0x3cd6d8a93ef99c78U,
  0xbf2afce13ecc8a0fU, 0x3f2550a43e6c5c93U, 0xbecc04b13ea897a5U, 0x3f192620be927bf6U,
  0x3e4e0ff6be082764U, 0xbf4249963ee62d49U, 0xbe99eb5c3dbc6c31U, 0x3ea76494bf350c51U,
  0x3f188bf8beea10b3U, 0xbe840ff33ec7c6baU, 0x3f25da2b3e121b1cU, 0x3eae90babe952598U,
  0xbf2b1a363ebdc5e4U, 0xbd720c87bf6fe21eU, 0x3d6f8aef3f2b23faU, 0x3e75ec3fbde0abdfU,
  0x3db0ef833f1bf359U, 0xbd9a63d8bd78ae92U, 0x3f5192193de8214dU, 0xbda0a8393f832a22U,
  0xbeb251733efbc6d2U, 0xbd791a32beb14ae1U, 0xbe38c7b23f0bfd72U, 0x3ea3e79e3dfb45e7U,
  0xbc12ca003e97db2aU, 0xbdc1b8f23dc7888eU, 0x3e2caeb23e8150deU, 0xbe12e99a3f14da16U,
  0x3f5a2e623d426b9bU, 0x3c837c0b3f7cb510U, 0x3eb04046bed14457U, 0xbe04d589bd1b2c56U,
  0xbd8a8f08be42fa5bU, 0x3e951d913d551549U, 0x3daf43d3be309f55U, 0x3ebb4a1fbe73eaf5U,
  0x3d277b60be6455f8U, 0x3e691db6be4bfbe6U, 0x3df8e7143bc42100U, 0x3e438f78be3cd6faU,
  0xbcc3ff10be4d1786U, 0x3d1458b03d2e0290U, 0xbe136714bd680ef0U, 0xbe6e3d3abe5b7850U,
  0x3e8499573f2dbfe5U, 0xbeb10194beaa98e7U, 0xbe032053bcce50fdU, 0x3be6cec83f065d31U,
  0xbe445932bebb09e1U, 0x3cfc90003ee8e4fdU, 0xbf29786c3e743b05U, 0x3eb874acbe8cf208U,
  0xbd95b80ebe922e58U, 0x3eec6067bf39bbb5U, 0xbdb30ff6bda7fc4cU, 0x3e7b3ea03ecb3d64U,
  0x3e289f143dddd36cU, 0xbf382afa3e3fea91U, 0xbf0a5cf4be3142ecU, 0xbf3143dc3f16e0aeU,
  0x3d2df3d5bf762ea8U, 0xbe593f183f30ea68U, 0x3dcc801dbf2dd53aU, 0x3cd5a2893ee2fce5U,
  0x3f1196203d20a5adU, 0xbef538083dcfcc07U, 0x3f0e583b3f2d5075U, 0xbdce2498bf5ebd96U,
  0x3efa66c7bec5dec6U, 0xbebddf75U,
};


ai_handle g_network_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_network_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

