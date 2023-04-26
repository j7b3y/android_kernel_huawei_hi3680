#ifndef __SOC_EFUSE_INTERFACE_H__
#define __SOC_EFUSE_INTERFACE_H__ 
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif
#define EFUSE_ATTR_READABLE (0x00000001UL)
#define EFUSE_ATTR_READ_DEVICE (0x00000002UL)
#define EFUSE_ATTR_READ_SRAM (0x00000004UL)
#define EFUSE_ATTR_READ_ZERO (0x00000008UL)
#define EFUSE_ATTR_WRITEABLE (0x00000100UL)
#define EFUSE_ATTR_WRITE_NOT_UPDATE (0x00000200UL)
#define EFUSE_ATTR_WRITE_CHECK (0x00000400UL)
typedef struct {
 unsigned int start_bit;
 unsigned int bit_cnt;
 unsigned int id;
 unsigned int attr;
} efuse_attr_t;
enum {
 EFUSE_DIEID_VALUE = 0,
 EFUSE_AUTH_KEY,
 EFUSE_INSE_FLAG,
 EFUSE_TRB_COUNTER,
 EFUSE_DJTAG_CTRL,
 EFUSE_HUKRD_DISABLE,
 EFUSE_SCPRD_DISABLE,
 EFUSE_DFTRD_DISABLE,
 EFUSE_SECDBG_CTRL,
 EFUSE_SECDBG_DBGEN,
 EFUSE_SECDBG_NIDEN = 10,
 EFUSE_SECDBG_SPIDEN,
 EFUSE_SECDBG_SPNIDEN,
 EFUSE_USB20PHY_VSEL,
 EFUSE_BURGLAR_PROOF,
 EFUSE_NTRB_COUNTER,
};
#define EFUSE_DIEID_VALUE_START 1184
#define EFUSE_DIEID_VALUE_SIZE 160
#define EFUSE_INSE_FLAG_START 2416
#define EFUSE_INSE_FLAG_SIZE 2
#define EFUSE_AUTH_KEY_START 3936
#define EFUSE_AUTH_KEY_SIZE 64
#define EFUSE_TRB_COUNTER_START 768
#define EFUSE_TRB_COUNTER_SIZE 32
#define EFUSE_DJTAG_CTRL_START 4050
#define EFUSE_DJTAG_CTRL_SIZE 2
#define EFUSE_HUKRD_DISABLE_START 4070
#define EFUSE_HUKRD_DISABLE_SIZE 1
#define EFUSE_SCPRD_DISABLE_START 4071
#define EFUSE_SCPRD_DISABLE_SIZE 1
#define EFUSE_DFTRD_DISABLE_START 4072
#define EFUSE_DFTRD_DISABLE_SIZE 1
#define EFUSE_SECDBG_CTRL_START 4073
#define EFUSE_SECDBG_CTRL_SIZE 2
#define EFUSE_SECDBG_DBGEN_START 4075
#define EFUSE_SECDBG_DBGEN_SIZE 1
#define EFUSE_SECDBG_NIDEN_START 4076
#define EFUSE_SECDBG_NIDEN_SIZE 1
#define EFUSE_SECDBG_SPIDEN_START 4077
#define EFUSE_SECDBG_SPIDEN_SIZE 1
#define EFUSE_SECDBG_SPNIDEN_START 4078
#define EFUSE_SECDBG_SPNIDEN_SIZE 1
#define EFUSE_USB20PHY_VSEL_START 4085
#define EFUSE_USB20PHY_VSEL_SIZE 1
#define EFUSE_BURGLAR_PROOF_START 2420
#define EFUSE_BURGLAR_PROOF_SIZE 1
#define EFUSE_NTRB_COUNTER_START 800
#define EFUSE_NTRB_COUNTER_SIZE 224
#define EFUSE_MAX_SIZE 4096
#define EFUSE_ITEM_MAX 160
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif