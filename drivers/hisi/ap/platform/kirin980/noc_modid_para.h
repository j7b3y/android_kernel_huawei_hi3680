#ifndef __NOC_MODID_PARA_H__
#define __NOC_MODID_PARA_H__ 
#define MASTER_ID_MASK 0x3f
enum noc_error_bus {
  NOC_ERRBUS_SYS_CONFIG,
  NOC_ERRBUS_VCODEC,
  NOC_ERRBUS_VIVO,
  NOC_ERRBUS_NPU,
  NOC_ERRBUS_FCM,
  NOC_ERRBUS_BOTTOM,
};
#endif
