

#ifndef HISI_AP_DRV_H
#define HISI_AP_DRV_H
#include <linux/module.h>
#include <linux/version.h>
/*************************************************************************
*
*   启动/加载/复位/校验
*
*************************************************************************/


#define PARTITION_MODEM_IMAGE_NAME       "modem"
#define PARTITION_MODEM_DSP_NAME         "modem_dsp"
#define PARTITION_MODEM_NVDLOAD_NAME     "modemnvm_update"
#define PARTITION_MODEM_NVDLOAD_CUST_NAME "modemnvm_cust"
#define PARTITION_MODEM_NVDEFAULT_NAME   "modemnvm_factory"
#define PARTITION_MODEM_NVBACKUP_NAME    "modemnvm_backup"
#define PARTITION_MODEM_NVSYS_NAME       "modemnvm_system"
#define PARTITION_MODEM_NVIMG_NAME       "modemnvm_img"
#define PARTITION_MODEM_NVPATCH_NAME     "modem_patch_nv"
#define PARTITION_MODEM_LOG_NAME         "modem_om"
#define PARTITION_MODEM_DTB_NAME	 "modem_dtb"
#define PARTITION_PTN_VRL_P_NAME 	 "vrl"
#define PARTITION_PTN_VRL_M_NAME	 "vrl_backup"

#define BSP_RESET_NOTIFY_REPLY_OK 		   0
#define BSP_RESET_NOTIFY_SEND_FAILED      -1
#define BSP_RESET_NOTIFY_TIMEOUT          -2

typedef enum
{
	BSP_START_MODE_CHARGING = 0,
	BSP_START_MODE_NORMAL,
	BSP_START_MODE_UPDATE,
	BSP_START_MODE_BUILT
} BSP_START_MODE_E;

typedef enum {
	BSP_CCORE = 0,
	BSP_HIFI,
	BSP_BBE,
	BSP_CDSP,
	BSP_CCORE_TAS,
	BSP_CCORE_WAS,
	BSP_CCORE_CAS,
	BSP_BUTT
} BSP_CORE_TYPE_E;

/*****************************************************************************
* 函 数 名  : flash_find_ptn
*
* 功能描述  : 根据分区名返回分区信息
*
* 输入参数  : str 分区名
*
* 输出参数  : 无
*
* 返 回 值  : 0 成功 -1 失败
*
* 其它说明  : kernel
*
*****************************************************************************/
int flash_find_ptn(const char* str, char* pblkname);

/*****************************************************************************
* 函 数 名  : bsp_need_loadmodem
*
* 功能描述  : 获取是否需要启动modem
*
* 输入参数  : 无
*
* 输出参数  : 无
*
* 返 回 值  : 1 需要启动
			  0 不需要启动
*
* 其它说明  : kernel
*
*****************************************************************************/
int bsp_need_loadmodem(void);

/*****************************************************************************
* 函 数 名  : bsp_reset_loadimage
*
* 功能描述  : Modem和HIFI 镜像加载
*
* 输入参数  : ecoretype 见BSP_CORE_TYPE_E
*
* 输出参数  : 无
*
* 返 回 值  : 0  处理成功
			  <0 处理失败
*
* 其它说明  : kernel
*
*****************************************************************************/
int bsp_reset_loadimage (BSP_CORE_TYPE_E ecoretype);

/*
 * Function name:bsp_reset_core_notify.
 * Description:notify the remote processor MODEM is going to reset.
 * Parameters:
 *      @ ecoretype: the core to be notified.
 *      @ cmdtype: to send to remote processor.
 *      @ timeout_ms: max time to wait, ms.
 *      @ retval: the ack's value get from the remote processor.
 *Called in:
 *      @ modem is going to reset. <reset_balong.c>
 *Return value:
 *      @ BSP_RESET_NOTIFY_REPLY_OK-->the remote processor give response in time.
 *      @ BSP_RESET_NOTIFY_SEND_FAILED-->the parameter is wrong or other rproc_sync_send's self error.
 *      @ BSP_RESET_NOTIFY_TIMEOUT-->after wait timeout_ms's time, the remote processor give no response.
*/
int bsp_reset_core_notify(BSP_CORE_TYPE_E ecoretype, unsigned int cmdtype, unsigned int timeout_ms, unsigned int *retval);



/*************************************************************************
*
*   RDR/Coresight
*
*************************************************************************/

#define STR_EXCEPTIONDESC_MAXLEN	48

/*与AP 约定定义成如下格式，如果变更，有AP变更*/
typedef enum
{
    CP_S_MODEMDMSS     = 0x70,
    CP_S_MODEMNOC      = 0x71,
    CP_S_MODEMAP       = 0x72,
    CP_S_EXCEPTION     = 0x73,
    CP_S_RESETFAIL     = 0x74,
    CP_S_NORMALRESET   = 0x75,
    CP_S_RILD_EXCEPTION= 0x76,
    CP_S_3RD_EXCEPTION = 0x77,
    CP_S_DRV_EXC       = 0x78,
    CP_S_PAM_EXC       = 0x79,
    CP_S_GUAS_EXC      = 0x7a,
    CP_S_CTTF_EXC      = 0x7b,
    CP_S_CAS_CPROC_EXC = 0x7c,
    CP_S_GUDSP_EXC     = 0x7d,
    CP_S_TLPS_EXC      = 0x7e,
    CP_S_TLDSP_EXC     = 0x7f,
    CP_S_CPHY_EXC      = 0x80,
    CP_S_GUCNAS_EXC    = 0x81,
} EXCH_SOURCE;

struct list_head_rdr {
	struct list_head_rdr *next, *prev;
};
typedef void (*rdr_e_callback)(unsigned int, void*);

/*
 *   struct list_head_rdr   e_list;
 *   u32 modid,			    exception id;
 *		if modid equal 0, will auto generation modid, and return it.
 *   u32 modid_end,		    can register modid region. [modid~modid_end];
		need modid_end >= modid,
 *		if modid_end equal 0, will be register modid only,
		but modid & modid_end cant equal 0 at the same time.
 *   u32 process_priority,	exception process priority
 *   u32 reboot_priority,	exception reboot priority
 *   u64 save_log_mask,		need save log mask
 *   u64 notify_core_mask,	need notify other core mask
 *   u64 reset_core_mask,	need reset other core mask
 *   u64 from_core,		    the core of happen exception
 *   u32 reentrant,		    whether to allow exception reentrant
 *   u32 exce_type,		    the type of exception
 *   char* from_module,		    the module of happen excption
 *   char* desc,		        the desc of happen excption
 *   rdr_e_callback callback,	will be called when excption has processed.
 *   u32 reserve_u32;		reserve u32
 *   void* reserve_p		    reserve void *
 */
struct rdr_exception_info_s {
	struct list_head_rdr e_list;	/*list_head_rdr instead of list_head  to solve the confliction between <linux/types.h>*/
	unsigned int	e_modid;
	unsigned int	e_modid_end;
	unsigned int	e_process_priority;
	unsigned int	e_reboot_priority;
	unsigned long long	e_notify_core_mask;
	unsigned long long	e_reset_core_mask;
	unsigned long long	e_from_core;
	unsigned int	e_reentrant;
	unsigned int    e_exce_type;
	unsigned int    e_exce_subtype;
	unsigned int    e_upload_flag;
	unsigned char	e_from_module[MODULE_NAME_LEN];
	unsigned char	e_desc[STR_EXCEPTIONDESC_MAXLEN];
	unsigned int	e_reserve_u32;
	void*	e_reserve_p;
	rdr_e_callback e_callback;
};

enum UPLOAD_FLAG {
    RDR_UPLOAD_YES = 0xff00fa00,
    RDR_UPLOAD_NO
};
enum REENTRANT {
    RDR_REENTRANT_ALLOW = 0xff00da00,
    RDR_REENTRANT_DISALLOW
};

enum REBOOT_PRI {
    RDR_REBOOT_NOW      = 0x01,
    RDR_REBOOT_WAIT,
    RDR_REBOOT_NO,
    RDR_REBOOT_MAX
};
enum CORE_LIST {
    RDR_AP      = 0x1,
    RDR_CP      = 0x2,
    RDR_HIFI    = 0x8,
    RDR_LPM3    = 0x10,
    RDR_MODEMAP = 0x200,
};
enum PROCESS_PRI {
    RDR_ERR      = 0x01,
    RDR_WARN,
    RDR_OTHER,
    RDR_PPRI_MAX
};


typedef void (*pfn_cb_dump_done)(unsigned int modid, unsigned long long coreid);
typedef void (*pfn_dump)(unsigned int modid, unsigned int etype, unsigned long long coreid, char* logpath, pfn_cb_dump_done fndone);
typedef void (*pfn_reset)(unsigned int modid, unsigned int etype, unsigned long long coreid);

struct rdr_module_ops_pub {
    pfn_dump    ops_dump;
    pfn_reset   ops_reset;
};

struct rdr_register_module_result {
    unsigned long long   log_addr;
    unsigned int         log_len;
    unsigned long long   nve;
};


#ifdef CONFIG_HISI_BB
/*****************************************************************************
* 函 数 名  : rdr_register_exception
*
* 功能描述  : RDR异常注册接口
*
* 输入参数  : rdr_exception_info_s
*
* 输出参数  : 无
*
* 返 回 值  : 0  处理失败
			  >0 e_modid_end
*
* 其它说明  : kernel
*
*****************************************************************************/
unsigned int rdr_register_exception (struct rdr_exception_info_s* e);

/*****************************************************************************
* 函 数 名  : rdr_register_module_ops
*
* 功能描述  : 注册异常处理函数
*
* 输入参数  : coreid  异常核ID
			  ops     异常处理函数与reset函数
*             retinfo 注册返回信息
* 输出参数  : 无
*
* 返 回 值  : 0  处理成功
			  <0 处理失败
*
* 其它说明  : kernel
*
*****************************************************************************/
int rdr_register_module_ops(unsigned long long coreid, struct rdr_module_ops_pub* ops, struct rdr_register_module_result* retinfo);


/*****************************************************************************
* 函 数 名  : rdr_system_error
*
* 功能描述  : 软件异常记录接口，完成异常文件记录之后主动复位单板
*
* 输入参数  :
*
* 输出参数  : 无
*
* 返 回 值  : 无
*
* 其它说明  : kernel
*
*****************************************************************************/
void rdr_system_error(unsigned int modid, unsigned int arg1, unsigned int arg2);
#else
static inline u32 rdr_register_exception(struct rdr_exception_info_s* e){ return 0;}
static inline int rdr_register_module_ops(unsigned long long coreid, struct rdr_module_ops_pub* ops,
				struct rdr_register_module_result* retinfo){ return -1; }
static inline void rdr_system_error(unsigned int modid, unsigned int arg1, unsigned int arg2) {}
#endif

/*****************************************************************************
* 函 数 名  : rdr_syserr_process_for_ap
*
* 功能描述  : 当已注册到rdr上的异常发生时，需要调用此接口触发保存log和reset流程。此接口供AP使用。
*
* 输入参数  : 无
*
* 输出参数  : 无
*
* 返 回 值  : 无
*
* 其它说明  : kernel
*
*****************************************************************************/
void rdr_syserr_process_for_ap(unsigned int modid, unsigned long long arg1, unsigned long long arg2);

/*
void top_tmc_enable(void);

void top_tmc_disable(void);

unsigned int rdr_register_soc_ops (RDR_U64 coreid, struct rdr_soc_ops_pub* ops,
        struct rdr_register_soc_result* retinfo);
unsigned long long rdr_unregister_soc_ops_info (RDR_U64 coreid);
*/


/*************************************************************************
*
*   eFuse
*
*************************************************************************/

/*****************************************************************************
* 函 数 名  : get_efuse_dieid_value
*
* 功能描述  : 返回die id 的值
*
* 输入参数  : u32Length 以字节为单位，并且要求最小是4个字节
			  timeout   超时时间
*
* 输出参数  : pu8Buffer 存储读取efuse的值
*
* 返 回 值  : 0  处理成功
			  <0 处理失败
*
* 其它说明  : kernel
*
*****************************************************************************/
int get_efuse_dieid_value(unsigned char *pu8Buffer, unsigned int u32Length, unsigned int timeout);

/*****************************************************************************
* 函 数 名  : get_efuse_chipid_value
*
* 功能描述  : 返回chip id 的值
*
* 输入参数  : u32Length 以字节为单位，并且要求最小是4个字节
			  timeout   超时时间
*
* 输出参数  : pu8Buffer 存储读取efuse的值
*
* 返 回 值  : 0  处理成功
			  <0 处理失败
*
* 其它说明  : kernel
*
*****************************************************************************/
int get_efuse_chipid_value(unsigned char *pu8Buffer, unsigned int u32Length, unsigned int timeout);

/*****************************************************************************
* 函 数 名  : get_efuse_kce_value
*
* 功能描述  : 返回kce的值
*
* 输入参数  : u32Length 以字节为单位，并且要求最小是4个字节
			  timeout   超时时间
*
* 输出参数  : pu8Buffer 存储读取efuse的值
*
* 返 回 值  : 0  处理成功
			  <0 处理失败
*
* 其它说明  : kernel
*
*****************************************************************************/
int get_efuse_kce_value(unsigned char *pu8Buffer, unsigned int u32Length, unsigned int timeout);

/*****************************************************************************
* 函 数 名  : set_efuse_kce_value
*
* 功能描述  : 设置kce的值
*
* 输入参数  : pu8Buffer 存储读取efuse的值
			  u32Length 以字节为单位，并且要求最小是4个字节
			  timeout   超时时间
*
* 输出参数  : 无
*
* 返 回 值  : 0  处理成功
			  <0 处理失败
*
* 其它说明  : kernel
*
*****************************************************************************/
int set_efuse_kce_value(unsigned char *pu8Buffer, unsigned int u32Length, unsigned int timeout);



/*************************************************************************
*
*   USB
*
*************************************************************************/
typedef void (*USB_ENABLE_CB_T)(void);
typedef void (*USB_DISABLE_CB_T)(void);

/*****************************************************************************
* 函 数 名  : bsp_acm_open
*
* 功能描述  : 打开设备
*
* 输入参数  : dev_id 要打开的设备id
*
* 输出参数  : 无
*
* 返 回 值  : 非0 返回的handle
			  NULL 错误
*
* 其它说明  : kernel
*
*****************************************************************************/
void *bsp_acm_open(unsigned int dev_id);

/*****************************************************************************
* 函 数 名  : bsp_acm_close
*
* 功能描述  : 关闭设备
*
* 输入参数  : 要关闭的设备handle
*
* 输出参数  : 无
*
* 返 回 值  : 0  处理成功
			  <0 处理失败
*
* 其它说明  : kernel
*
*****************************************************************************/
int bsp_acm_close(void *handle);

/*****************************************************************************
* 函 数 名  : bsp_acm_write
*
* 功能描述  : 数据写
*
* 输入参数  : handle 设备handle
			  buf  待写入的数据
			  size 代写入的数据大小
*
* 输出参数  : 无
*
* 返 回 值  : >0  处理成功的数据大小
			  <=0 处理失败
*
* 其它说明  : kernel
*
*****************************************************************************/
int bsp_acm_write(void *handle, void *buf, unsigned int size);

/*****************************************************************************
* 函 数 名  : bsp_acm_read
*
* 功能描述  : 数据写
*
* 输入参数  : handle 设备handle
			  size 代写入的数据大小
*
* 输出参数  : buf  待返回的数据
*
* 返 回 值  : >0  处理成功的数据大小
			  <=0 处理失败
*
* 其它说明  : kernel
*
*****************************************************************************/
int bsp_acm_read(void *handle, void *buf, unsigned int size);

/*****************************************************************************
* 函 数 名  : bsp_acm_ioctl
*
* 功能描述  : 根据命令码进行设置
*
* 输入参数  : handle 设备handle
			  cmd 命令码
			  para 命令参数
*
* 输出参数  : 无
*
* 返 回 值  : 0  处理成功
			  <0 处理失败
*
* 其它说明  : kernel
*
*****************************************************************************/
int bsp_acm_ioctl(void *handle, unsigned int cmd, void *para);

/*****************************************************************************
* 函 数 名  : BSP_USB_RegUdiEnableCB
*
* 功能描述  : 注册usb枚举完成回调
*
* 输入参数  : pFunc 回调指针
*
* 输出参数  : 无
*
* 返 回 值  : 0  处理成功
			  <0 处理失败
*
* 其它说明  : kernel
*
*****************************************************************************/
unsigned int BSP_USB_RegUdiEnableCB(USB_ENABLE_CB_T pFunc);

/*****************************************************************************
* 函 数 名  : BSP_USB_RegUdiDisableCB
*
* 功能描述  : 注册usb拔出完成回调
*
* 输入参数  : pFunc 回调指针
*
* 输出参数  : 无
*
* 返 回 值  : 0  处理成功
			  <0 处理失败
*
* 其它说明  : kernel
*
*****************************************************************************/
unsigned int BSP_USB_RegUdiDisableCB(USB_DISABLE_CB_T pFunc);

/*************************************************************************
*
*   PMU
*
*************************************************************************/


typedef  void   (*PMU_OCP_CALLBACK)( const char * name);
/*****************************************************************************
* 函 数 名  : BSP_USB_RegUdiDisableCB
*
* 功能描述  : 注册usb拔出完成回调
*
* 输入参数  : name OCP产生的电源名称，见pmu_mntn DTS定义
			  pFunc
*
* 输出参数  : 无
*
* 返 回 值  : 0  处理成功
			  <0 处理失败
*
* 其它说明  : kernel
*
*****************************************************************************/
int pmu_ocp_event_reg (const char * name, PMU_OCP_CALLBACK pFunc);

enum EDITION_KIND{
    EDITION_USER            = 1,
    EDITION_INTERNAL_BETA   = 2,
    EDITION_OVERSEA_BETA    = 3,
    EDITION_MAX
};
#ifdef CONFIG_HISI_BB
/*
 * func name: bbox_check_edition
 * func args:
 *   void
 * return:
 *   unsigned int:	return edition information
 *				0x01		User
 *				0x02		Internal BETA
 *				0x03		Oversea BETA
 */
unsigned int bbox_check_edition(void);
#else
static inline unsigned int bbox_check_edition(void){return EDITION_USER;}
#endif

/* hisi_pmic_special_ocp_register需要加入接口管控  */

int atfd_hisi_service_access_register_smc(unsigned long long main_fun_id, 
                                          unsigned long long buff_addr_phy, 
                                          unsigned long long data_len, 
                                          unsigned long long sub_fun_id);

void ipf_get_waking_pkt(void* data, unsigned int len);

/*****************************************************************************
* function     : get_kernel_build_time
* description  : get build date and build time of kernel
* input        : builddate: buffer for get build date
*                dtlen    : length of builddate buffer
*                buildtime: buffer for get build time
*                tmlen    : length of buildtime buffer
* output       : builddate : kernel build date string
*                buildtime : kernel build time string
* ret value  : 0  successfull
*               <0  failed to get date&time
*****************************************************************************/
int get_kernel_build_time(char* builddate, int dtlen, char* buildtime, int tmlen);


/********sim hotplug start************/
/*
status=1 means plug out;
status=0 means plug in;
*/
#define STATUS_PLUG_IN 0
#define STATUS_PLUG_OUT 1

#define MODULE_SD  0
#define MODULE_SIM  1


int get_sd2jtag_status(void);
u8 get_card1_type(void);
int sd_sim_detect_run(void *dw_mci_host, int status, int current_module, int need_sleep);

/********sim hotplug end************/

/********kirin pcie start************/
enum kirin_pcie_trigger {
	KIRIN_PCIE_TRIGGER_CALLBACK,
	KIRIN_PCIE_TRIGGER_COMPLETION,
};

enum kirin_pcie_event {
	KIRIN_PCIE_EVENT_MIN_INVALID = 0x0,		/*min invalid value*/
	KIRIN_PCIE_EVENT_LINKUP = 0x1,		/* linkup event  */
	KIRIN_PCIE_EVENT_LINKDOWN = 0x2,		/* linkdown event */
	KIRIN_PCIE_EVENT_WAKE = 0x4,	/* wake event*/
	KIRIN_PCIE_EVENT_L1SS = 0x8,	/* l1ss event*/
	KIRIN_PCIE_EVENT_CPL_TIMEOUT = 0x10,	/* completion timeout event */
	KIRIN_PCIE_EVENT_MAX_INVALID = 0x1F,	/* max invalid value*/
};

struct kirin_pcie_notify {
	enum kirin_pcie_event event;
	void *user;
	void *data;
	u32 options;
};

struct kirin_pcie_register_event {
	u32 events;
	void *user;
	enum kirin_pcie_trigger mode;
	void (*callback)(struct kirin_pcie_notify *notify);
	struct kirin_pcie_notify notify;
	struct completion *completion;
	u32 options;
};

/*****************************************************************************
* 函 数 名  : kirin_pcie_register_event
*
* 功能描述  : EP register hook fun for link event notification
*
* 输入参数  : reg register_event
*
* 输出参数  : 无
*
* 返 回 值  : 0  处理成功
*             非0 处理失败
*
* 其它说明  : kernel
*
*****************************************************************************/
int kirin_pcie_register_event(struct kirin_pcie_register_event *reg);

/*****************************************************************************
* 函 数 名  : kirin_pcie_deregister_event
*
* 功能描述  : EP deregister hook fun for link event notification
*
* 输入参数  : reg register_event
*
* 输出参数  : 无
*
* 返 回 值  : 0  处理成功
*             非0 处理失败
*
* 其它说明  : kernel
*
*****************************************************************************/
int kirin_pcie_deregister_event(struct kirin_pcie_register_event *reg);

/*****************************************************************************
* 函 数 名  : kirin_pcie_pm_control
*
* 功能描述  : ep控制rc上电接口
*
* 输入参数  : power_ops 0---PowerOff normally
*                       1---Poweron normally
*                       2---PowerOFF without PME
*                       3---Poweron without LINK
*             rc_idx ep对接的rc端口号
*
* 输出参数  : 无
*
* 返 回 值  : 0  处理成功
*             非0 处理失败
*
* 其它说明  : kernel
*
*****************************************************************************/
int kirin_pcie_pm_control(int power_ops, u32 rc_idx);

/*****************************************************************************
* 函 数 名  : kirin_pcie_ep_off
*
* 功能描述  : 获取ep设备下电状态接口
*
* 输入参数  : rc_idx ep对接的rc端口号
*
* 输出参数  : 无
*
* 返 回 值  : 0  未下电
*             1  已下电
*
* 其它说明  : kernel
*
*****************************************************************************/
int kirin_pcie_ep_off(u32 rc_idx);

/*****************************************************************************
* 函 数 名  : kirin_pcie_lp_ctrl
*
* 功能描述  : 打开关闭低功耗接口
*
* 输入参数  : rc_idx ep对接的rc端口号
*             enable 0---关闭
*                    1---打开
*
* 输出参数  : 无
*
* 返 回 值  : 0  处理成功
*             非0 处理失败
*
* 其它说明  : kernel
*
*****************************************************************************/
int kirin_pcie_lp_ctrl(u32 rc_idx, u32 enable);

/*****************************************************************************
* 函 数 名  : kirin_pcie_enumerate
*
* 功能描述  : pcie枚举接口
*
* 输入参数  : rc_idx ep对接的rc端口号
*
* 输出参数  : 无
*
* 返 回 值  : 0  处理成功
*             非0 处理失败
*
* 其它说明  : kernel
*
*****************************************************************************/
int kirin_pcie_enumerate(u32 rc_idx);

/*****************************************************************************
* 函 数 名  : kirin_pcie_remove_ep
*
* 功能描述  : pcie删除ep设备接口
*
* 输入参数  : rc_idx ep对接的rc端口号
*
* 输出参数  : 无
*
* 返 回 值  : 0  处理成功
*             非0 处理失败
*
* 其它说明  : kernel
*
*****************************************************************************/
int kirin_pcie_remove_ep(u32 rc_idx);

/*****************************************************************************
* 函 数 名  : kirin_pcie_rescan_ep
*
* 功能描述  : pcie扫描ep设备接口
*
* 输入参数  : rc_idx ep对接的rc端口号
*
* 输出参数  : 无
*
* 返 回 值  : 0  处理成功
*             非0 处理失败
*
* 其它说明  : kernel
*
*****************************************************************************/
int kirin_pcie_rescan_ep(u32 rc_idx);

/*****************************************************************************
* 函 数 名  : pcie_ep_link_ltssm_notify
*
* 功能描述  : 设置ep设备link状态接口
*
* 输入参数  : rc_id ep对接的rc端口号
*             link_status ep设备link状态
*
* 输出参数  : 无
*
* 返 回 值  : 0  处理成功
*             非0 处理失败
*
* 其它说明  : kernel
*
*****************************************************************************/
int pcie_ep_link_ltssm_notify(u32 rc_id, u32 link_status);

/*****************************************************************************
* 函 数 名  : kirin_pcie_power_notifiy_register
*
* 功能描述  : 注册pcie上电提供时钟后ep回调
*
* 输入参数  : rc_id ep对接的rc端口号
*             poweron ep上电回调指针
*             poweroff ep下电回调指针
*             data ep回调函数入参
*
* 输出参数  : 无
*
* 返 回 值  : 0  处理成功
*             非0 处理失败
*
* 其它说明  : kernel
*
*****************************************************************************/
int kirin_pcie_power_notifiy_register(u32 rc_id, int (*poweron)(void* data),
				int (*poweroff)(void* data), void* data);

/********kirin pcie end************/

/*****************************************************************************
* 函 数 名  : get_cmd_product_id
*
* 功能描述  : 从kernel的cmdline中获取product id
*
* 输入参数  : 无
*
* 输出参数  : 无
*
* 返 回 值  : 32bit无符号值，非0xFFFFFFFF  处理成功
*             0xFFFFFFFF 处理失败
*
* 其它说明  : kernel
*
*****************************************************************************/
unsigned int get_cmd_product_id(void);

/******************* modem temp start *************/
struct mdm_adc_s {
    unsigned int channel;
    unsigned int mode;
};
extern int hisi_mdm_adc_get_value_register(int (*func)(struct mdm_adc_s *mdm_adc_para));
/******************* modem temp end ****************/

#if (LINUX_VERSION_CODE > KERNEL_VERSION(4, 9, 0))
/* hifi reset notify modem */
enum DRV_RESET_CALLCBFUN_MOMENT {
	DRV_RESET_CALLCBFUN_RESET_BEFORE,
	DRV_RESET_CALLCBFUN_RESET_AFTER,
	DRV_RESET_CALLCBFUN_RESETING,
	DRV_RESET_CALLCBFUN_MOEMENT_INVALID
};

typedef int (*hifi_reset_cbfunc)(enum DRV_RESET_CALLCBFUN_MOMENT eparam, int userdata);

#ifdef CONFIG_HISI_HIFI_BB
int hifireset_regcbfunc(const char *pname, hifi_reset_cbfunc pcbfun, int userdata, int priolevel);
#else
static inline int hifireset_regcbfunc(const char *pname, hifi_reset_cbfunc pcbfun, int userdata, int priolevel)
{
	return 0;
}
#endif
#endif
#endif /* HISI_AP_DRV_H */


