/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_BLKDEV_H
#define _LINUX_BLKDEV_H

#include <linux/sched.h>
#include <linux/sched/clock.h>

#ifdef CONFIG_BLOCK

#include <linux/major.h>
#include <linux/genhd.h>
#include <linux/list.h>
#include <linux/llist.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/pagemap.h>
#include <linux/backing-dev-defs.h>
#include <linux/wait.h>
#include <linux/mempool.h>
#include <linux/pfn.h>
#include <linux/bio.h>
#include <linux/stringify.h>
#include <linux/gfp.h>
#include <linux/bsg.h>
#include <linux/smp.h>
#include <linux/rcupdate.h>
#include <linux/percpu-refcount.h>
#include <linux/scatterlist.h>
#include <linux/blkzoned.h>

struct module;
struct scsi_ioctl_command;
struct Scsi_Host;

struct request_queue;
struct elevator_queue;
struct blk_trace;
struct request;
struct sg_io_hdr;
struct bsg_job;
struct blkcg_gq;
struct blk_flush_queue;
struct pr_ops;
struct rq_wb;
struct blk_queue_stats;
struct blk_stat_callback;

#define BLKDEV_MIN_RQ	4
#define BLKDEV_MAX_RQ	128	/* Default maximum */

/* Must be consisitent with blk_mq_poll_stats_bkt() */
#define BLK_MQ_POLL_STATS_BKTS 16

#define BLK_MIN_BG_DEPTH	2
#define BLK_MIN_DEPTH_ON	16

/*
 * Maximum number of blkcg policies allowed to be registered concurrently.
 * Defined here to simplify include dependency.
 */
#define BLKCG_MAX_POLS		3

#if defined(CONFIG_QOS_BLKIO) || defined(CONFIG_ROW_VIP_QUEUE)
#ifndef BLKIO_QOS_HIGH
#define BLKIO_QOS_HIGH	VALUE_QOS_HIGH
#endif
#ifndef BLKIO_QOS_DEFAULT
#define BLKIO_QOS_DEFAULT	VALUE_QOS_NORMAL
#endif
#ifndef QOS_BLKIO_DEBUG
#define QOS_BLKIO_DEBUG
#endif
#endif

typedef void (rq_end_io_fn)(struct request *, blk_status_t);

#define BLK_RL_SYNCFULL		(1U << 0)
#define BLK_RL_ASYNCFULL	(1U << 1)

struct request_list {
	struct request_queue	*q;	/* the queue this rl belongs to */
#ifdef CONFIG_BLK_CGROUP
	struct blkcg_gq		*blkg;	/* blkg this request pool belongs to */
#endif
	/*
	 * count[], starved[], and wait[] are indexed by
	 * BLK_RW_SYNC/BLK_RW_ASYNC
	 */
	int			count[2];
	int			starved[2];
	mempool_t		*rq_pool;
	wait_queue_head_t	wait[2];
	unsigned int		flags;
};

/*
 * request flags */
typedef __u32 __bitwise req_flags_t;

/* elevator knows about this request */
#define RQF_SORTED		((__force req_flags_t)(1 << 0))
/* drive already may have started this one */
#define RQF_STARTED		((__force req_flags_t)(1 << 1))
/* uses tagged queueing */
#define RQF_QUEUED		((__force req_flags_t)(1 << 2))
/* may not be passed by ioscheduler */
#define RQF_SOFTBARRIER		((__force req_flags_t)(1 << 3))
/* request for flush sequence */
#define RQF_FLUSH_SEQ		((__force req_flags_t)(1 << 4))
/* merge of different types, fail separately */
#define RQF_MIXED_MERGE		((__force req_flags_t)(1 << 5))
/* track inflight for MQ */
#define RQF_MQ_INFLIGHT		((__force req_flags_t)(1 << 6))
/* don't call prep for this one */
#define RQF_DONTPREP		((__force req_flags_t)(1 << 7))
/* set for "ide_preempt" requests and also for requests for which the SCSI
   "quiesce" state must be ignored. */
#define RQF_PREEMPT		((__force req_flags_t)(1 << 8))
/* contains copies of user pages */
#define RQF_COPY_USER		((__force req_flags_t)(1 << 9))
/* vaguely specified driver internal error.  Ignored by the block layer */
#define RQF_FAILED		((__force req_flags_t)(1 << 10))
/* don't warn about errors */
#define RQF_QUIET		((__force req_flags_t)(1 << 11))
/* elevator private data attached */
#define RQF_ELVPRIV		((__force req_flags_t)(1 << 12))
/* account I/O stat */
#define RQF_IO_STAT		((__force req_flags_t)(1 << 13))
/* request came from our alloc pool */
#define RQF_ALLOCED		((__force req_flags_t)(1 << 14))
/* runtime pm request */
#define RQF_PM			((__force req_flags_t)(1 << 15))
/* on IO scheduler merge hash */
#define RQF_HASHED		((__force req_flags_t)(1 << 16))
/* IO stats tracking on */
#define RQF_STATS		((__force req_flags_t)(1 << 17))
/* Look at ->special_vec for the actual data payload instead of the
   bio chain. */
#define RQF_SPECIAL_PAYLOAD	((__force req_flags_t)(1 << 18))

/* flags that prevent us from merging requests: */
#define RQF_NOMERGE_FLAGS \
	(RQF_STARTED | RQF_SOFTBARRIER | RQF_FLUSH_SEQ | RQF_SPECIAL_PAYLOAD)

#ifdef CONFIG_MAS_BLK
enum customer_rq_flag_bits {
	__REQ_HEAD_OF_QUEUE = 0,
	__REQ_COMMAND_PRIO,
	__REQ_COMMAND_ORDER,
	__REQ_TURBO_ZONE,
};
int get_mq_all_tag_used(struct request_queue *q);
int get_mq_prio_tag_used(struct request_queue *q);

#define CUST_REQ_HEAD_OF_QUEUE (1ULL << __REQ_HEAD_OF_QUEUE)
#define CUST_REQ_COMMAND_PRIO (1ULL << __REQ_COMMAND_PRIO)
#define CUST_REQ_ORDER (1ULL << __REQ_COMMAND_ORDER)
#define CUST_REQ_TURBO_ZONE (1ULL << __REQ_TURBO_ZONE)

#define req_hoq(req)                                                           \
	((req)->mas_req.mas_featrue_flag & CUST_REQ_HEAD_OF_QUEUE)
#define req_cp(req) ((req)->mas_req.mas_featrue_flag & CUST_REQ_COMMAND_PRIO)
#define req_tz(req) ((req)->mas_req.mas_featrue_flag & CUST_REQ_TURBO_ZONE)

#define req_order(req) \
		((req)->mas_req.mas_featrue_flag & CUST_REQ_ORDER)

enum requeue_reason_enum {
	REQ_REQUEUE_IO_NO_REQUEUE = 0,
	REQ_REQUEUE_IO_HW_LIMIT,
	REQ_REQUEUE_IO_SW_LIMIT,
	REQ_REQUEUE_IO_CP_LIMIT,
	REQ_REQUEUE_IO_HW_PENDING,
};

#if defined(CONFIG_MAS_DEBUG_FS) || defined(CONFIG_MAS_BLK_DEBUG)
enum blk_ft_rq_sim_mode {
	BLK_FT_RQ_SIM_NONE = 0, /* The value can't be changed! */
	BLK_FT_RQ_SIM_TIMEOUT_HANDLED,
	BLK_FT_RQ_SIM_RESET_TIMER,
	BLK_FT_RQ_SIM_EXPECT_REQUEUE,
};
#endif

/*
 * This struct defines all the variable in vendor block layer.
 */
struct blk_req_cust {
	/* Dispatch IO process task struct */
	struct task_struct *dispatch_task;
	/* Dispatch IO process PID */
	pid_t task_pid;
	/* Dispatch IO process TGID */
	pid_t task_tgid;
	/* Dispatch IO process name */
	char task_comm[TASK_COMM_LEN];

	/* mas feature flag */
	unsigned long long mas_featrue_flag;
	/* io comes from fs or not */
	unsigned char fs_io_flag;
	/* The reason for IO requeue */
	enum requeue_reason_enum requeue_reason;
	/* The CTX which make the request */
	struct blk_mq_ctx *mq_ctx_generate;
	/* Non-FS request endup call back function */
	rq_end_io_fn *uplayer_end_io;
#ifdef CONFIG_MAS_QOS_MQ
	unsigned char mas_rq_qos;
	unsigned int slot_cpu;
#endif
	ktime_t req_stage_ktime[REQ_PROC_STAGE_MAX];

#ifdef CONFIG_MAS_UNISTORE_PRESERVE
	/*
	 * Member for Vertical Opti
	 */
	unsigned char stream_type;
	unsigned char cp_tag;
	unsigned long data_ino;
	unsigned long data_idx;
	bool fsync_ind;
	bool slc_mode;
	bool fg_io;
	unsigned int protocol_nr_cnt;
#endif

	unsigned int make_req_nr;
	unsigned int protocol_nr;

	/*
	 * Below info for debug info
	 */

#if defined(CONFIG_MAS_DEBUG_FS) || defined(CONFIG_MAS_BLK_DEBUG)
	atomic_t req_used;
	enum blk_ft_rq_sim_mode simulate_mode;
#endif
};

#ifdef CONFIG_MAS_QOS_MQ
enum mas_mq_qos_val {
	MAS_MQ_QOS_0 = 0,
	MAS_MQ_QOS_1,
	MAS_MQ_QOS_2,
	MAS_MQ_QOS_3,
	MAS_MQ_QOS_4,
	MAS_MQ_QOS_5,
	MAS_MQ_QOS_6,
	MAS_MQ_QOS_7,
};
#endif /* CONFIG_MAS_QOS_MQ */
#endif /* CONFIG_MAS_BLK */

/*
 * Try to put the fields that are referenced together in the same cacheline.
 *
 * If you modify this structure, make sure to update blk_rq_init() and
 * especially blk_mq_rq_ctx_init() to take care of the added fields.
 */
struct request {
	struct list_head queuelist;
	struct list_head fg_bg_list;
	union {
		struct __call_single_data csd;
		u64 fifo_time;
	};

	struct request_queue *q;
	struct blk_mq_ctx *mq_ctx;

	int cpu;
	unsigned int cmd_flags;		/* op and common flags */
	req_flags_t rq_flags;

	int internal_tag;

	unsigned long atomic_flags;

	/* the following two fields are internal, NEVER access directly */
	unsigned int __data_len;	/* total data len */
	int tag;
	sector_t __sector;		/* sector cursor */

	struct bio *bio;
	struct bio *biotail;

	/*
	 * The hash is used inside the scheduler, and killed once the
	 * request reaches the dispatch list. The ipi_list is only used
	 * to queue the request for softirq completion, which is long
	 * after the request has been unhashed (and even removed from
	 * the dispatch list).
	 */
	union {
		struct hlist_node hash;	/* merge hash */
		struct list_head ipi_list;
	};

	/*
	 * The rb_node is only used inside the io scheduler, requests
	 * are pruned when moved to the dispatch queue. So let the
	 * completion_data share space with the rb_node.
	 */
	union {
		struct rb_node rb_node;	/* sort/lookup */
		struct bio_vec special_vec;
		void *completion_data;
		int error_count; /* for legacy drivers, don't use */
	};

	/*
	 * Three pointers are available for the IO schedulers, if they need
	 * more they have to dynamically allocate it.  Flush requests are
	 * never put on the IO scheduler. So let the flush fields share
	 * space with the elevator data.
	 */
	union {
		struct {
			struct io_cq		*icq;
			void			*priv[2];
		} elv;

		struct {
			unsigned int		seq;
			struct list_head	list;
			rq_end_io_fn		*saved_end_io;
		} flush;
	};

	struct gendisk *rq_disk;
	struct hd_struct *part;
	unsigned long start_time;
#ifdef CONFIG_MAS_BLK
#ifdef CONFIG_MMC_MQ_CQ_HCI
	struct list_head cmdq_list;
#endif
	unsigned long mas_cmd_flags;

	struct blk_req_cust mas_req;
	/*
	 * Below is for MAS IO Scheduler
	 */
	struct list_head async_list;
	/*
	 * Below is for MAS Block Debug purpose
	 */
	struct list_head cnt_list;
#endif /* CONFIG_MAS_BLK */

	/*
	 * Below info for inline crypto
	 */
	void *ci_key;
	int ci_key_len;
	int ci_key_index;
	u8 *ci_metadata;

	struct blk_issue_stat issue_stat;
#ifdef CONFIG_BLK_CGROUP
	struct request_list *rl;		/* rl this rq is alloced from */
	unsigned long long start_time_ns;
	unsigned long long io_start_time_ns;    /* when passed to hardware */
#endif
	/* Number of scatter-gather DMA addr+len pairs after
	 * physical address coalescing is performed.
	 */
	unsigned short nr_phys_segments;
#if defined(CONFIG_BLK_DEV_INTEGRITY)
	unsigned short nr_integrity_segments;
#endif

	unsigned short ioprio;

	unsigned int timeout;

	void *special;		/* opaque pointer available for LLD use */

	unsigned int extra_len;	/* length of alignment and padding */

	unsigned short write_hint;

	unsigned long deadline;
	struct list_head timeout_list;

	/*
	 * completion callback.
	 */
	rq_end_io_fn *end_io;
	void *end_io_data;

	/* for bidi */
	struct request *next_rq;
};

static inline bool blk_op_is_scsi(unsigned int op)
{
	return op == REQ_OP_SCSI_IN || op == REQ_OP_SCSI_OUT;
}

static inline bool blk_op_is_private(unsigned int op)
{
	return op == REQ_OP_DRV_IN || op == REQ_OP_DRV_OUT;
}

static inline bool blk_rq_is_scsi(struct request *rq)
{
	return blk_op_is_scsi(req_op(rq));
}

static inline bool blk_rq_is_private(struct request *rq)
{
	return blk_op_is_private(req_op(rq));
}

static inline bool blk_rq_is_passthrough(struct request *rq)
{
	return blk_rq_is_scsi(rq) || blk_rq_is_private(rq);
}

static inline bool bio_is_passthrough(struct bio *bio)
{
	unsigned op = bio_op(bio);

	return blk_op_is_scsi(op) || blk_op_is_private(op);
}

static inline unsigned short req_get_ioprio(struct request *req)
{
	return req->ioprio;
}

#include <linux/elevator.h>

struct blk_queue_ctx;

typedef void (request_fn_proc) (struct request_queue *q);
typedef blk_qc_t (make_request_fn) (struct request_queue *q, struct bio *bio);
typedef int (prep_rq_fn) (struct request_queue *, struct request *);
typedef void (unprep_rq_fn) (struct request_queue *, struct request *);

struct bio_vec;
typedef void (softirq_done_fn)(struct request *);
typedef int (dma_drain_needed_fn)(struct request *);
typedef int (lld_busy_fn) (struct request_queue *q);
typedef int (bsg_job_fn) (struct bsg_job *);
typedef int (init_rq_fn)(struct request_queue *, struct request *, gfp_t);
typedef void (exit_rq_fn)(struct request_queue *, struct request *);

enum blk_eh_timer_return {
	BLK_EH_NOT_HANDLED,
	BLK_EH_HANDLED,
	BLK_EH_RESET_TIMER,
#ifdef CONFIG_MAS_BLK
	BLK_EH_REQUEUE,
#endif
};

typedef enum blk_eh_timer_return (rq_timed_out_fn)(struct request *);

enum blk_queue_state {
	Queue_down,
	Queue_up,
};

enum blk_dump_scene {
	BLK_DUMP_WARNING = 0,
	BLK_DUMP_PANIC,
};

static inline char *mas_blk_prefix_str(enum blk_dump_scene s)
{
	return (s == BLK_DUMP_PANIC) ? "dump" : "io_latency";
}

enum blk_freeze_obj_type {
	BLK_LLD = 0,
	BLK_QUEUE,
};

#ifdef CONFIG_MAS_UNISTORE_PRESERVE
#define STREAM_TYPE_RPMB    0xf0
#define STREAM_TYPE_INVALID 0xff
#define BLK_ORDER_STREAM_NUM 4
#define DATA_MOVE_STREAM_NUM 2
#define MAX_RESCUE_SEG_CNT 240
#define DATA_MOVE_MAX_NUM 768

#if defined(CONFIG_MAS_DEBUG_FS) || defined(CONFIG_MAS_BLK_DEBUG)
enum reset_debug {
	RESET_DEBUG_NO = 0,
	RESET_DEBUG_100E,
	RESET_DEBUG_700D,
	RESET_DEBUG_CLOSE,
};
#endif

enum stream_type {
	BLK_STREAM_META = 0,
	BLK_STREAM_COLD_NODE,
	BLK_STREAM_COLD_DATA,
	BLK_STREAM_HOT_NODE,
	BLK_STREAM_HOT_DATA,
	BLK_STREAM_HP,

	/* add new stream above */
	BLK_STREAM_MAX_STRAM,
};

enum bkops_fs_work_result {
	BKOPS_START_SUC = 0,
	BKOPS_DEV_NOT_SUPPORT,
	BKOPS_INPUT_ERR,
	BKOPS_FUNC_NOT_SUPPORT,
	BKOPS_STATE_NOT_IDLE,
	BKOPS_ALREADY_START,
	BKOPS_QUERY_ERR,
	BKOPS_NO_NEED_START,
	BKOPS_NEED_START,
	BKOPS_START_ERR,
};

enum pwron_type {
	DM_STRAM0_TYPE = 0,
	DM_STRAM1_TYPE,
	RECOVERY_TYPE,

	PWRON_MAX_TYPE,
};

typedef void (stor_dev_pwron_info_done_fn)(int, u8);

struct stor_dev_pwron_info_done_info {
	stor_dev_pwron_info_done_fn *done;
	u8 pwron_type;
};

#define STREAM_SECTION_NUM 5

struct stor_dev_pwron_info {
	unsigned int dev_stream_addr[BLK_STREAM_MAX_STRAM];
	unsigned int rescue_seg_cnt;
	unsigned int *rescue_seg;
	unsigned char pe_limit_status;
	unsigned int dm_stream_addr[DATA_MOVE_STREAM_NUM];
	unsigned char stream_lun_info[BLK_STREAM_MAX_STRAM];
	unsigned char dm_lun_info[DATA_MOVE_STREAM_NUM];
	unsigned char io_slc_mode_status;
	unsigned char dm_slc_mode_status;
	unsigned int section_info[BLK_ORDER_STREAM_NUM][STREAM_SECTION_NUM];
	struct stor_dev_pwron_info_done_info done_info;
};

struct stor_dev_stream_info {
	unsigned char stream_id;
	bool dm_stream;
	unsigned int stream_start_lba;
};

struct stor_dev_stream_oob_info {
	unsigned long data_ino;
	unsigned long data_idx;
};

struct stor_dev_verify_info {
	unsigned int next_to_be_verify_4k_lba;
	unsigned int next_available_write_4k_lba;
	unsigned char lun_info;
	unsigned char verify_done_status;
	unsigned char verify_fail_reason;
	unsigned int pu_size;
};

typedef void (stor_dev_data_move_done_fn)(struct stor_dev_verify_info, void *);

struct stor_dev_data_move_done_info {
	stor_dev_data_move_done_fn *done;
	void *private_data;
	sector_t start_addr;
};

struct stor_dev_data_move_source_addr {
	unsigned int data_move_source_addr;
	unsigned char src_lun;
	unsigned int source_length;
};

struct stor_dev_data_move_source_inode {
	unsigned int data_move_source_inode;
	unsigned int data_move_source_offset;
};

struct stor_dev_data_move_info {
	unsigned int data_move_total_length;
	unsigned int dest_4k_lba;
	unsigned char dest_lun_info;
	unsigned char dest_stream_id;
	unsigned char dest_blk_mode;
	unsigned char force_flush_option;
	unsigned char repeat_option;
	unsigned char error_injection;
	unsigned int source_addr_num;
	struct stor_dev_data_move_source_addr *source_addr;
	unsigned int source_inode_num;
	struct stor_dev_data_move_source_inode *source_inode;
	struct stor_dev_verify_info verify_info;
	struct stor_dev_data_move_done_info done_info;
};

struct stor_dev_sync_read_verify_info {
	unsigned char stream_id;
	unsigned int cp_verify_l4k;
	unsigned int cp_open_l4k;
	unsigned int cp_cache_l4k;
	unsigned char error_injection;
	struct stor_dev_verify_info verify_info;
};

struct stor_dev_bad_block_info {
	unsigned short tlc_total_block_num;
	unsigned char tlc_bad_block_num;
};

struct stor_dev_program_size {
	unsigned short tlc_program_size;
	unsigned short slc_program_size;
};

struct stor_dev_reset_ftl {
	unsigned char op_type; /* 0:reset ftl, 1:close section */
	unsigned char stream_type; /* 0: normal, 1:datamove */
	unsigned char stream_id;
};

enum {
	PARTITION_TYPE_META0, /* first 4K-mapping region */
	PARTITION_TYPE_META1, /* second 4K-mapping region */
	PARTITION_TYPE_USER0, /* first 2M-mapping region */
	PARTITION_TYPE_USER1, /* second 2M-mapping region */
	PARTITION_TYPE_MAX
};

struct stor_dev_mapping_partition {
	unsigned int partion_start[PARTITION_TYPE_MAX];
	unsigned int partion_size[PARTITION_TYPE_MAX];
};

struct stor_dev_section_info {
	unsigned int section_start_lba;
	int flash_mode;
	unsigned char stream_type;
};

typedef void (*blk_dev_bad_block_notify_fn)(
		struct stor_dev_bad_block_info, void *);
typedef int (*lld_dev_pwron_info_sync_fn)(struct request_queue *,
		struct stor_dev_pwron_info *, unsigned int rescue_seg_size);
typedef int (*lld_dev_stream_oob_info_fetch_fn)(
		struct request_queue *, struct stor_dev_stream_info,
		unsigned int, struct stor_dev_stream_oob_info *);
typedef int (*lld_dev_reset_ftl_fn)(struct request_queue *,
		struct stor_dev_reset_ftl *);
typedef int (*lld_dev_read_section_fn)(
		struct request_queue *, unsigned int *);
typedef int (*lld_dev_read_mapping_partition_fn)(
		struct request_queue *, struct stor_dev_mapping_partition *);
typedef int (*lld_dev_config_mapping_partition_fn)(
		struct request_queue *, struct stor_dev_mapping_partition *);
typedef int (*lld_dev_fs_sync_done_fn)(struct request_queue *);
typedef int (*lld_dev_data_move_fn)(struct request_queue *,
		struct stor_dev_data_move_info *);
typedef int (*lld_dev_slc_mode_configuration_fn)(
		struct request_queue *, int *);
typedef int (*lld_dev_sync_read_verify_fn)(struct request_queue *,
		struct stor_dev_sync_read_verify_info *);
typedef int (*lld_dev_get_bad_block_info_fn)(
		struct request_queue *, struct stor_dev_bad_block_info *);
typedef int (*lld_dev_get_program_size_fn)(
		struct request_queue *, struct stor_dev_program_size *);
typedef int (*lld_dev_read_op_size_fn)(struct request_queue *, int *);
typedef int (*lld_dev_read_lrb_in_use_fn)(
		struct request_queue *, unsigned long *);
typedef int (*lld_dev_bad_block_notify_register_fn)(
		struct request_queue *, void (*func)(struct Scsi_Host *host,
		struct stor_dev_bad_block_info *bad_block_info));
#ifdef CONFIG_MAS_DEBUG_FS
typedef int (*lld_dev_rescue_block_inject_data_fn)(
		struct request_queue *, unsigned int);
typedef int (*lld_dev_bad_block_error_inject_fn)(
		struct request_queue *, unsigned char, unsigned char);
#endif
#endif

typedef void (*lld_dump_status_fn)(
	struct request_queue *, enum blk_dump_scene);
typedef int (*lld_tz_query_fn)(struct request_queue *q, u32 type, u8 *buf,
			       u32 buf_len);
typedef int (*lld_tz_ctrl_fn)(
	struct request_queue *q, int desc_id, uint8_t index);
typedef int (*blk_direct_flush_fn)(struct request_queue *);

#ifdef CONFIG_HP_CORE
typedef int (*lld_query_health_fn)(struct request_queue *q,
	u8 *pre_eol_info, u8 *life_time_est_a, u8 *life_time_est_b);
#endif

#ifdef CONFIG_MAS_BLK
enum blk_lld_base {
	BLK_LLD_QUEUE_BASE = 0,
	BLK_LLD_QUEUE_TAG_BASE,
	BLK_LLD_TAGSET_BASE,
};

enum blk_busyidle_callback_ret {
	/* Event Proc won't trigger IO */
	BUSYIDLE_NO_IO = 0,
	/* Event Proc will trigger new IO */
	BUSYIDLE_IO_TRIGGERED,
	/* Event Proc meets errors */
	BUSYIDLE_ERR,
};

enum blk_idle_notify_state {
	BLK_BUSY_NOTIFY = 0, /* IO Busy Event */
	BLK_IDLE_NOTIFY,     /* IO Idle Event */
#ifdef CONFIG_MAS_UNISTORE_PRESERVE
	BLK_FG_BUSY_NOTIFY,  /* FG IO Busy Event */
	BLK_FG_IDLE_NOTIFY,  /* FG IO Idle Event */
#endif
};

enum blk_io_state {
	BLK_IO_BUSY = 0,
	BLK_IO_IDLE,
};

#ifdef CONFIG_MAS_UNISTORE_PRESERVE
enum blk_fg_io_state {
	BLK_FG_IO_BUSY = 0,
	BLK_FG_IO_IDLE,
};
#endif
enum blk_idle_dur_enum {
	BLK_IDLE_DUR_IDX_100MS,
	BLK_IDLE_DUR_IDX_500MS,
	BLK_IDLE_DUR_IDX_1000MS,
	BLK_IDLE_DUR_IDX_2000MS,
	BLK_IDLE_DUR_IDX_4000MS,
	BLK_IDLE_DUR_IDX_6000MS,
	BLK_IDLE_DUR_IDX_8000MS,
	BLK_IDLE_DUR_IDX_10000MS,
	BLK_IDLE_DUR_IDX_FOR_AGES,
	BLK_IDLE_DUR_IDX_DUR_NUM,
};

struct blk_dev_lld;

#define SUBSCRIBER_NAME_LEN 32
struct blk_busyidle_event_node {
	/* Identify a subscriber uniquely */
	char subscriber[SUBSCRIBER_NAME_LEN];
	/* should be provided by subscriber module */
	enum blk_busyidle_callback_ret (*busyidle_callback)(
		struct blk_busyidle_event_node *, enum blk_idle_notify_state);
	/* optional */
	void *param_data;

	/* busy idle private data, don't touch it */
	struct blk_dev_lld *lld;
};

struct blk_idle_state {
	/* Hardware idle support or not */
	bool idle_intr_support;
	/* Idle event process worker */
	struct delayed_work idle_notify_worker;
	/* delay avoid jitter */
	unsigned int idle_notify_delay_ms;

	/* list of subcribed events */
	struct list_head subscribed_list;
	struct blocking_notifier_head nh;
	atomic_t io_count; /* io count variable */
	struct mutex io_count_mutex;
	/* busy idle state*/
	enum blk_io_state idle_state;

#if defined(CONFIG_MAS_DEBUG_FS) || defined(CONFIG_MAS_BLK_DEBUG)
	/*
	 * for busy idle statistic purpose
	 */
	ktime_t last_idle_ktime;
	ktime_t last_busy_ktime;
	ktime_t total_busy_ktime;
	ktime_t total_idle_ktime;
	unsigned long long total_idle_count;
	/* statistic for idle time */
	s64 blk_idle_dur[BLK_IDLE_DUR_IDX_DUR_NUM];
	/* max idle time */
	s64 max_idle_dur;
	struct blk_busyidle_event_node busy_idle_test_node;
	struct blk_busyidle_event_node busy_idle_test_nodes[5];
#endif

#ifdef CONFIG_MAS_UNISTORE_PRESERVE
	/* foreground io idle event process worker */
	struct delayed_work fg_io_idle_notify_worker;
	unsigned int fg_io_idle_notify_delay_ms;
	/* foreground io count variable */
	atomic_t fg_io_count;
	struct mutex fg_io_count_mutex;
	/* foreground io busy idle state*/
	enum blk_fg_io_state fg_io_idle_state;
	atomic_t idle_trigger_runtime_change;
#endif

	/*
	 * Below info is just for busy idle debug purpose!
	 */
	/* The number of bios have been counted */
	u64 bio_count;
	/* The number of reqs have been counted */
	u64 req_count;
	/* The list for all the counted bios */
	struct list_head bio_list;
	/* The list for all the counted reqs */
	struct list_head req_list;
	spinlock_t counted_list_lock;
};

#ifdef CONFIG_MAS_UNISTORE_PRESERVE
struct mas_unistore_ops {
	lld_dev_pwron_info_sync_fn dev_pwron_info_sync;
	lld_dev_stream_oob_info_fetch_fn dev_stream_oob_info_fetch;
	lld_dev_reset_ftl_fn dev_reset_ftl;
	lld_dev_read_section_fn dev_read_section;
	lld_dev_config_mapping_partition_fn dev_config_mapping_partition;
	lld_dev_read_mapping_partition_fn dev_read_mapping_partition;
	lld_dev_fs_sync_done_fn dev_fs_sync_done;
	lld_dev_data_move_fn dev_data_move;
	lld_dev_slc_mode_configuration_fn dev_slc_mode_configuration;
	lld_dev_sync_read_verify_fn dev_sync_read_verify;
	lld_dev_get_bad_block_info_fn dev_get_bad_block_info;
	blk_dev_bad_block_notify_fn dev_bad_block_notfiy_fn;
	void *dev_bad_block_notfiy_param_data;
	lld_dev_get_program_size_fn dev_get_program_size;
	lld_dev_bad_block_notify_register_fn dev_bad_block_notify_register;
	lld_dev_read_op_size_fn dev_read_op_size;
	lld_dev_read_lrb_in_use_fn dev_read_lrb_in_use;
#ifdef CONFIG_MAS_DEBUG_FS
	lld_dev_rescue_block_inject_data_fn dev_rescue_block_inject_data;
	lld_dev_bad_block_error_inject_fn dev_bad_block_err_inject;
#endif
};
#endif

struct blk_dev_lld {
	/* Magic Number for the struct*/
	unsigned int init_magic;
	/* The object on queue, tag or tagset*/
	enum blk_lld_base type;
	/* Private data */
	void *data;
	/* is hw idle enabled */
	atomic_t hw_idle_en;
#define MAS_BLK_LLD_IDLE_INTR_EN (1 << 3)
	/* LLD Feature flag bit */
	unsigned long features;
#define MAS_BLK_LLD_IDLE_INTR_CAP (1 << 3)
	unsigned long lld_cap;
	lld_dump_status_fn dump_fn;
	/* IO Latency warning threshold */
	unsigned int latency_warning_threshold_ms;
	/* Emergency Flush Operation */
	blk_direct_flush_fn flush_fn;
	/* query api of turbu zone */
	lld_tz_query_fn tz_query;
	lld_tz_ctrl_fn tz_ctrl;

#ifdef CONFIG_MAS_UNISTORE_PRESERVE
	struct mas_unistore_ops unistore_ops;

	unsigned char lock_map[NR_CPUS];

	unsigned int mas_sec_size;
	unsigned int mas_pu_size;
	atomic_t bad_block_atomic;
	struct work_struct bad_block_work;
	struct stor_dev_bad_block_info bad_block_info;
	unsigned char last_stream_type;
	bool	fsync_ind;
	spinlock_t fsync_ind_lock;
	struct mas_bkops *bkops;
	spinlock_t expected_lba_lock[BLK_ORDER_STREAM_NUM];
	sector_t	expected_lba[BLK_ORDER_STREAM_NUM];
	sector_t	expected_pu[BLK_ORDER_STREAM_NUM];
	sector_t	current_pu_size[BLK_ORDER_STREAM_NUM];
	ktime_t expected_refresh_time[BLK_ORDER_STREAM_NUM];
	sector_t old_section[BLK_ORDER_STREAM_NUM];
	struct list_head section_list[BLK_ORDER_STREAM_NUM];

	atomic_t reset_cnt;
	atomic_t recovery_flag;
	atomic_t recovery_pwron_flag;
	atomic_t recovery_pwron_inprocess_flag;
	struct rw_semaphore recovery_rwsem;
	struct mutex recovery_mutex;
	struct list_head buf_bio_list[BLK_ORDER_STREAM_NUM + 1];
	spinlock_t buf_bio_list_lock[BLK_ORDER_STREAM_NUM + 1];
	unsigned int buf_bio_size[BLK_ORDER_STREAM_NUM + 1];
	unsigned int buf_bio_num[BLK_ORDER_STREAM_NUM + 1];
	unsigned int buf_page_num[BLK_ORDER_STREAM_NUM + 1];
	atomic_t replaced_page_cnt[BLK_ORDER_STREAM_NUM + 1];
	sector_t max_recovery_size;

	unsigned int write_curr_cnt;
	unsigned int write_pre_cnt;
#endif

	/* For busy idle feature */
	struct blk_idle_state blk_idle;
	/* accumulated write len of the whole device */
	unsigned long write_len;
	/* accumulated discard len of the whole device */
	unsigned long discard_len;
	bool dev_order_en;
	unsigned int write_num;
	spinlock_t write_num_lock;
	unsigned int make_req_nr;
	spinlock_t make_req_nr_lock;
	struct list_head lld_list;
	/*
	 * MAS IO Scheduler private data
	 */
	bool sched_ds_lld_inited;
	void *sched_ds_lld;
	int sqr_v;
	int sqw_v;
};
#endif /* CONFIG_MAS_BLK */

struct blk_queue_tag {
	struct request **tag_index;	/* map of busy tags */
	unsigned long *tag_map;		/* bit map of free/busy tags */
	int max_depth;			/* what we will send to device */
	int real_max_depth;		/* what the array can hold */
	/* what we will send to device from bg thread */
	int max_bg_depth;
	atomic_t refcnt;		/* map can be shared */
	int alloc_policy;		/* tag allocation policy */
	int next_tag;			/* next tag */
#ifdef CONFIG_MAS_BLK
	struct mutex		tag_list_lock;
	struct list_head	tag_list;
	struct blk_dev_lld lld_func;
#endif
};
#define BLK_TAG_ALLOC_FIFO 0 /* allocate starting from 0 */
#define BLK_TAG_ALLOC_RR 1 /* allocate starting from last allocated tag */

#define BLK_SCSI_MAX_CMDS	(256)
#define BLK_SCSI_CMD_PER_LONG	(BLK_SCSI_MAX_CMDS / (sizeof(long) * 8))

/*
 * Zoned block device models (zoned limit).
 */
enum blk_zoned_model {
	BLK_ZONED_NONE,	/* Regular block device */
	BLK_ZONED_HA,	/* Host-aware zoned block device */
	BLK_ZONED_HM,	/* Host-managed zoned block device */
};

struct queue_limits {
	unsigned long		bounce_pfn;
	unsigned long		seg_boundary_mask;
	unsigned long		virt_boundary_mask;

	unsigned int		max_hw_sectors;
	unsigned int		max_dev_sectors;
	unsigned int		chunk_sectors;
	unsigned int		max_sectors;
	unsigned int		max_segment_size;
	unsigned int		physical_block_size;
	unsigned int		logical_block_size;
	unsigned int		alignment_offset;
	unsigned int		io_min;
	unsigned int		io_opt;
	unsigned int		max_discard_sectors;
	unsigned int		max_hw_discard_sectors;
	unsigned int		max_write_same_sectors;
	unsigned int		max_write_zeroes_sectors;
	unsigned int		discard_granularity;
	unsigned int		discard_alignment;

	unsigned short		max_segments;
	unsigned short		max_integrity_segments;
	unsigned short		max_discard_segments;

	unsigned char		misaligned;
	unsigned char		discard_misaligned;
	unsigned char		cluster;
	unsigned char		raid_partial_stripes_expensive;
	enum blk_zoned_model	zoned;
};

#ifdef CONFIG_BLK_DEV_ZONED

struct blk_zone_report_hdr {
	unsigned int	nr_zones;
	u8		padding[60];
};

extern int blkdev_report_zones(struct block_device *bdev,
			       sector_t sector, struct blk_zone *zones,
			       unsigned int *nr_zones, gfp_t gfp_mask);
extern int blkdev_reset_zones(struct block_device *bdev, sector_t sectors,
			      sector_t nr_sectors, gfp_t gfp_mask);

extern int blkdev_report_zones_ioctl(struct block_device *bdev, fmode_t mode,
				     unsigned int cmd, unsigned long arg);
extern int blkdev_reset_zones_ioctl(struct block_device *bdev, fmode_t mode,
				    unsigned int cmd, unsigned long arg);

#else /* CONFIG_BLK_DEV_ZONED */

static inline int blkdev_report_zones_ioctl(struct block_device *bdev,
					    fmode_t mode, unsigned int cmd,
					    unsigned long arg)
{
	return -ENOTTY;
}

static inline int blkdev_reset_zones_ioctl(struct block_device *bdev,
					   fmode_t mode, unsigned int cmd,
					   unsigned long arg)
{
	return -ENOTTY;
}

#endif /* CONFIG_BLK_DEV_ZONED */

#ifdef CONFIG_MAS_BLK
struct blk_queue_ops;

/*
 * This struct defines all the variable in vendor block layer.
 */
struct blk_queue_cust {
	/* The disk struct of the request queue */
	struct gendisk *queue_disk;
	/* The request queue has the partition table or not */
	bool blk_part_tbl_exist;
	unsigned long usr_ctrl_n;

#if defined(CONFIG_MAS_DEBUG_FS) || defined(CONFIG_MAS_BLK_DEBUG)
	bool io_prio_sim;
	unsigned long tz_write_bytes;
#endif

	/*
	 * Flush Optimise
	 */
	struct delayed_work flush_work;
	atomic_t flush_work_trigger;
	atomic_t write_after_flush;
	struct list_head flush_queue_node;
	int flush_optimize;
	/*
	 * IO latency statistic function
	 */
	int io_latency_enable;
	unsigned int io_lat_warning_thresh;
	struct list_head dump_list;
	/*
	 * IO Latency for test purpose only
	 */
	unsigned long sr_l; /* Seq Read Latency */
	unsigned long sw_l; /* Seq Write Latency */
	unsigned long rr_l; /* Rand Read Latency */
	unsigned long rw_l; /* Rand Write Latency */
	struct timer_list limit_setting_protect_timer;
	/*
	 * MAS IO Scheduler private data
	 */
	void *cust_queuedata;
};
#endif /* CONFIG_MAS_BLK */

struct request_queue {
	/*
	 * Together with queue_head for cacheline sharing
	 */
	struct list_head	queue_head;
	struct list_head	fg_head;
	struct list_head	bg_head;
	struct request		*last_merge;
	struct elevator_queue	*elevator;
	int			nr_rqs[2];	/* # allocated [a]sync rqs */
	int			nr_rqs_elvpriv;	/* # allocated rqs w/ elvpriv */

	atomic_t		shared_hctx_restart;

	struct blk_queue_stats	*stats;
	struct rq_wb		*rq_wb;

	/*
	 * If blkcg is not used, @q->root_rl serves all requests.  If blkcg
	 * is used, root blkg allocates from @q->root_rl and all other
	 * blkgs from their own blkg->rl.  Which one to use should be
	 * determined using bio_request_list().
	 */
	struct request_list	root_rl;

	request_fn_proc		*request_fn;
	make_request_fn		*make_request_fn;
	prep_rq_fn		*prep_rq_fn;
	unprep_rq_fn		*unprep_rq_fn;
	softirq_done_fn		*softirq_done_fn;
	rq_timed_out_fn		*rq_timed_out_fn;
	dma_drain_needed_fn	*dma_drain_needed;
	lld_busy_fn		*lld_busy_fn;
	/* Called just after a request is allocated */
	init_rq_fn		*init_rq_fn;
	/* Called just before a request is freed */
	exit_rq_fn		*exit_rq_fn;
	/* Called from inside blk_get_request() */
	void (*initialize_rq_fn)(struct request *rq);

	const struct blk_mq_ops	*mq_ops;

	unsigned int		*mq_map; /* map cpu to hw queue */

	/* sw queues */
	struct blk_mq_ctx __percpu	*queue_ctx;
	unsigned int		nr_queues;

	unsigned int		queue_depth;

	/* hw dispatch queues */
	struct blk_mq_hw_ctx	**queue_hw_ctx;
	unsigned int		nr_hw_queues;

	/*
	 * Dispatch queue sorting
	 */
	sector_t		end_sector;
	struct request		*boundary_rq;

	/*
	 * Delayed queue handling
	 */
	struct delayed_work	delay_work;

	struct backing_dev_info	*backing_dev_info;

	/*
	 * The queue owner gets to use this for whatever they like.
	 * ll_rw_blk doesn't touch it.
	 */
	void			*queuedata;

	/*
	 * various queue flags, see QUEUE_* below
	 */
	unsigned long		queue_flags;

	/*
	 * ida allocated id for this queue.  Used to index queues from
	 * ioctx.
	 */
	int			id;

	/*
	 * queue needs bounce pages for pages above this limit
	 */
	gfp_t			bounce_gfp;

	/*
	 * protects queue structures from reentrancy. ->__queue_lock should
	 * _never_ be used directly, it is queue private. always use
	 * ->queue_lock.
	 */
	spinlock_t		__queue_lock;
	spinlock_t		*queue_lock;

	/*
	 * queue kobject
	 */
	struct kobject kobj;

	/*
	 * mq queue kobject
	 */
	struct kobject mq_kobj;

#ifdef  CONFIG_BLK_DEV_INTEGRITY
	struct blk_integrity integrity;
#endif	/* CONFIG_BLK_DEV_INTEGRITY */

#ifdef CONFIG_PM
	struct device		*dev;
	int			rpm_status;
	unsigned int		nr_pending;
#endif

	/*
	 * queue settings
	 */
	unsigned long		nr_requests;	/* Max # of requests */
	unsigned int		nr_congestion_on;
	unsigned int		nr_congestion_off;
	unsigned int		nr_batching;

	unsigned int		dma_drain_size;
	void			*dma_drain_buffer;
	unsigned int		dma_pad_mask;
	unsigned int		dma_alignment;

	struct blk_queue_tag	*queue_tags;
	struct list_head	tag_busy_list;

	unsigned int		nr_sorted;
	unsigned int		in_flight[4];

	/*
	 * Number of active block driver functions for which blk_drain_queue()
	 * must wait. Must be incremented around functions that unlock the
	 * queue_lock internally, e.g. scsi_request_fn().
	 */
	unsigned int		request_fn_active;

	unsigned int		rq_timeout;
	int			poll_nsec;

	struct blk_stat_callback	*poll_cb;
	struct blk_rq_stat	poll_stat[BLK_MQ_POLL_STATS_BKTS];

	struct timer_list	timeout;
	struct work_struct	timeout_work;
	struct list_head	timeout_list;

	struct list_head	icq_list;
#ifdef CONFIG_SCSI_UFS_INLINE_CRYPTO
	int inline_crypt_support;
#endif
#ifdef CONFIG_HP_CORE
	/* query api of health */
	lld_query_health_fn health_query;
#endif

#ifdef CONFIG_MAS_BLK
	struct blk_queue_cust mas_queue;
	struct blk_queue_ops *mas_queue_ops;
	struct blk_dev_lld lld_func;
#endif

#ifdef CONFIG_BLK_CGROUP
	DECLARE_BITMAP		(blkcg_pols, BLKCG_MAX_POLS);
	struct blkcg_gq		*root_blkg;
	struct list_head	blkg_list;
#endif

	struct queue_limits	limits;

	/*
	 * sg stuff
	 */
	unsigned int		sg_timeout;
	unsigned int		sg_reserved_size;
	int			node;
#ifdef CONFIG_BLK_DEV_IO_TRACE
	struct blk_trace	*blk_trace;
	struct mutex		blk_trace_mutex;
#endif
	/*
	 * for flush operations
	 */
	struct blk_flush_queue	*fq;

	struct list_head	requeue_list;
	spinlock_t		requeue_lock;
	struct delayed_work	requeue_work;

	struct mutex		sysfs_lock;

	int			bypass_depth;
	atomic_t		mq_freeze_depth;

#if defined(CONFIG_BLK_DEV_BSG)
	bsg_job_fn		*bsg_job_fn;
	struct bsg_class_device bsg_dev;
#endif

#ifdef CONFIG_BLK_DEV_THROTTLING
	/* Throttle data */
	struct throtl_data *td;
	/* for turbo zone */
	struct block_device *tz_weighted_bdev;
#endif
	struct rcu_head		rcu_head;
	wait_queue_head_t	mq_freeze_wq;
	struct percpu_ref	q_usage_counter;
	struct list_head	all_q_node;

	struct blk_mq_tag_set	*tag_set;
	struct list_head	tag_set_list;
	struct bio_set		*bio_split;

#ifdef CONFIG_BLK_DEBUG_FS
	struct dentry		*debugfs_dir;
	struct dentry		*sched_debugfs_dir;
#endif

	bool			mq_sysfs_init_done;

	size_t			cmd_size;
	void			*rq_alloc_data;

	struct work_struct	release_work;

#define BLK_MAX_WRITE_HINTS	5
	u64			write_hints[BLK_MAX_WRITE_HINTS];

	unsigned long		bw_timestamp;
	unsigned long		last_ticks;
	sector_t		last_sects[2];
	unsigned long		last_ios[2];
	sector_t		disk_bw;
	unsigned long		disk_iops;
};

#define QUEUE_FLAG_QUEUED	0	/* uses generic tag queueing */
#define QUEUE_FLAG_STOPPED	1	/* queue is stopped */
#define QUEUE_FLAG_DYING	2	/* queue being torn down */
#define QUEUE_FLAG_BYPASS	3	/* act as dumb FIFO queue */
#define QUEUE_FLAG_BIDI		4	/* queue supports bidi requests */
#define QUEUE_FLAG_NOMERGES     5	/* disable merge attempts */
#define QUEUE_FLAG_SAME_COMP	6	/* complete on same CPU-group */
#define QUEUE_FLAG_FAIL_IO	7	/* fake timeout */
#define QUEUE_FLAG_STACKABLE	8	/* supports request stacking */
#define QUEUE_FLAG_NONROT	9	/* non-rotational device (SSD) */
#define QUEUE_FLAG_VIRT        QUEUE_FLAG_NONROT /* paravirt device */
#define QUEUE_FLAG_IO_STAT     10	/* do IO stats */
#define QUEUE_FLAG_DISCARD     11	/* supports DISCARD */
#define QUEUE_FLAG_NOXMERGES   12	/* No extended merges */
#define QUEUE_FLAG_ADD_RANDOM  13	/* Contributes to random pool */
#define QUEUE_FLAG_SECERASE    14	/* supports secure erase */
#define QUEUE_FLAG_SAME_FORCE  15	/* force complete on same CPU */
#define QUEUE_FLAG_DEAD        16	/* queue tear-down finished */
#define QUEUE_FLAG_INIT_DONE   17	/* queue is initialized */
#define QUEUE_FLAG_NO_SG_MERGE 18	/* don't attempt to merge SG segments*/
#define QUEUE_FLAG_POLL	       19	/* IO polling enabled if set */
#define QUEUE_FLAG_WC	       20	/* Write back caching */
#define QUEUE_FLAG_FUA	       21	/* device supports FUA writes */
#define QUEUE_FLAG_FLUSH_NQ    22	/* flush not queueuable */
#define QUEUE_FLAG_DAX         23	/* device supports DAX */
#define QUEUE_FLAG_STATS       24	/* track rq completion times */
#define QUEUE_FLAG_POLL_STATS  25	/* collecting stats for hybrid polling */
#define QUEUE_FLAG_REGISTERED  26	/* queue has been registered to a disk */
#define QUEUE_FLAG_SCSI_PASSTHROUGH 27	/* queue supports SCSI commands */
#define QUEUE_FLAG_QUIESCED    28	/* queue has been quiesced */
#define QUEUE_FLAG_QOS		   29		 /* device supports qos on off*/

#define QUEUE_FLAG_DEFAULT	((1 << QUEUE_FLAG_IO_STAT) |		\
				 (1 << QUEUE_FLAG_STACKABLE)	|	\
				 (1 << QUEUE_FLAG_SAME_COMP)	|	\
				 (1 << QUEUE_FLAG_ADD_RANDOM))

#define QUEUE_FLAG_MQ_DEFAULT	((1 << QUEUE_FLAG_IO_STAT) |		\
				 (1 << QUEUE_FLAG_STACKABLE)	|	\
				 (1 << QUEUE_FLAG_SAME_COMP)	|	\
				 (1 << QUEUE_FLAG_POLL))

/*
 * @q->queue_lock is set while a queue is being initialized. Since we know
 * that no other threads access the queue object before @q->queue_lock has
 * been set, it is safe to manipulate queue flags without holding the
 * queue_lock if @q->queue_lock == NULL. See also blk_alloc_queue_node() and
 * blk_init_allocated_queue().
 */
static inline void queue_lockdep_assert_held(struct request_queue *q)
{
	if (q->queue_lock)
		lockdep_assert_held(q->queue_lock);
}

static inline void queue_flag_set_unlocked(unsigned int flag,
					   struct request_queue *q)
{
	__set_bit(flag, &q->queue_flags);
}

static inline int queue_flag_test_and_clear(unsigned int flag,
					    struct request_queue *q)
{
	queue_lockdep_assert_held(q);

	if (test_bit(flag, &q->queue_flags)) {
		__clear_bit(flag, &q->queue_flags);
		return 1;
	}

	return 0;
}

static inline int queue_flag_test_and_set(unsigned int flag,
					  struct request_queue *q)
{
	queue_lockdep_assert_held(q);

	if (!test_bit(flag, &q->queue_flags)) {
		__set_bit(flag, &q->queue_flags);
		return 0;
	}

	return 1;
}

static inline void queue_flag_set(unsigned int flag, struct request_queue *q)
{
	queue_lockdep_assert_held(q);
	__set_bit(flag, &q->queue_flags);
}

static inline void queue_flag_clear_unlocked(unsigned int flag,
					     struct request_queue *q)
{
	__clear_bit(flag, &q->queue_flags);
}

static inline int queue_in_flight(struct request_queue *q)
{
	return q->in_flight[0] + q->in_flight[1];
}

static inline void queue_flag_clear(unsigned int flag, struct request_queue *q)
{
	queue_lockdep_assert_held(q);
	__clear_bit(flag, &q->queue_flags);
}

#ifdef CONFIG_BLK_DEV_HI_PRIO_FOR_FG
static inline void queue_throtl_add_request(struct request_queue *q,
					    struct request *rq, bool front)
{
	struct list_head *head;

	if (rq->cmd_flags & REQ_FG)
		head = &q->fg_head;
	else
		head = &q->bg_head;

	if (front)
		list_add(&rq->fg_bg_list, head);
	else
		list_add_tail(&rq->fg_bg_list, head);
}

static inline void queue_throtl_add_inflight(struct request_queue *q,
					     struct request *rq)
{
	if (rq->cmd_flags & REQ_FG)
		q->in_flight[BLK_RW_FG]++;
	else
		q->in_flight[BLK_RW_BG]++;
}

static inline void queue_throtl_dec_inflight(struct request_queue *q,
					     struct request *rq)
{
	if (rq->cmd_flags & REQ_FG)
		q->in_flight[BLK_RW_FG]--;
	else
		q->in_flight[BLK_RW_BG]--;
}
#else
static inline void queue_throtl_add_request(struct request_queue *q,
					    struct request *rq, bool front)
{
}

static inline void queue_throtl_add_inflight(struct request_queue *q,
					     struct request *rq)
{
}

static inline void queue_throtl_dec_inflight(struct request_queue *q,
					     struct request *rq)
{
}
#endif

#define blk_queue_tagged(q)	test_bit(QUEUE_FLAG_QUEUED, &(q)->queue_flags)
#define blk_queue_stopped(q)	test_bit(QUEUE_FLAG_STOPPED, &(q)->queue_flags)
#define blk_queue_dying(q)	test_bit(QUEUE_FLAG_DYING, &(q)->queue_flags)
#define blk_queue_dead(q)	test_bit(QUEUE_FLAG_DEAD, &(q)->queue_flags)
#define blk_queue_bypass(q)	test_bit(QUEUE_FLAG_BYPASS, &(q)->queue_flags)
#define blk_queue_init_done(q)	test_bit(QUEUE_FLAG_INIT_DONE, &(q)->queue_flags)
#define blk_queue_nomerges(q)	test_bit(QUEUE_FLAG_NOMERGES, &(q)->queue_flags)
#define blk_queue_noxmerges(q)	\
	test_bit(QUEUE_FLAG_NOXMERGES, &(q)->queue_flags)
#define blk_queue_nonrot(q)	test_bit(QUEUE_FLAG_NONROT, &(q)->queue_flags)
#define blk_queue_io_stat(q)	test_bit(QUEUE_FLAG_IO_STAT, &(q)->queue_flags)
#define blk_queue_add_random(q)	test_bit(QUEUE_FLAG_ADD_RANDOM, &(q)->queue_flags)
#define blk_queue_stackable(q)	\
	test_bit(QUEUE_FLAG_STACKABLE, &(q)->queue_flags)
#define blk_queue_discard(q)	test_bit(QUEUE_FLAG_DISCARD, &(q)->queue_flags)
#define blk_queue_secure_erase(q) \
	(test_bit(QUEUE_FLAG_SECERASE, &(q)->queue_flags))
#define blk_queue_dax(q)	test_bit(QUEUE_FLAG_DAX, &(q)->queue_flags)
#define blk_queue_qos_on(q)	test_bit(QUEUE_FLAG_QOS, &(q)->queue_flags)
#define blk_queue_scsi_passthrough(q)	\
	test_bit(QUEUE_FLAG_SCSI_PASSTHROUGH, &(q)->queue_flags)

#define blk_noretry_request(rq) \
	((rq)->cmd_flags & (REQ_FAILFAST_DEV|REQ_FAILFAST_TRANSPORT| \
			     REQ_FAILFAST_DRIVER))
#define blk_queue_quiesced(q)	test_bit(QUEUE_FLAG_QUIESCED, &(q)->queue_flags)

static inline bool blk_account_rq(struct request *rq)
{
	return (rq->rq_flags & RQF_STARTED) && !blk_rq_is_passthrough(rq);
}

#define blk_rq_cpu_valid(rq)	((rq)->cpu != -1)
#define blk_bidi_rq(rq)		((rq)->next_rq != NULL)
/* rq->queuelist of dequeued request must be list_empty() */
#define blk_queued_rq(rq)	(!list_empty(&(rq)->queuelist))

#define list_entry_rq(ptr)	list_entry((ptr), struct request, queuelist)
#ifdef CONFIG_MMC_MQ_CQ_HCI
#define cmdq_list_entry_rq(ptr)	list_entry((ptr), struct request, cmdq_list)
#endif
#define rq_data_dir(rq)		(op_is_write(req_op(rq)) ? WRITE : READ)

/*
 * Driver can handle struct request, if it either has an old style
 * request_fn defined, or is blk-mq based.
 */
static inline bool queue_is_rq_based(struct request_queue *q)
{
	return q->request_fn || q->mq_ops;
}

static inline unsigned int blk_queue_cluster(struct request_queue *q)
{
	return q->limits.cluster;
}

static inline enum blk_zoned_model
blk_queue_zoned_model(struct request_queue *q)
{
	return q->limits.zoned;
}

static inline bool blk_queue_is_zoned(struct request_queue *q)
{
	switch (blk_queue_zoned_model(q)) {
	case BLK_ZONED_HA:
	case BLK_ZONED_HM:
		return true;
	default:
		return false;
	}
}

static inline unsigned int blk_queue_zone_sectors(struct request_queue *q)
{
	return blk_queue_is_zoned(q) ? q->limits.chunk_sectors : 0;
}

static inline bool rq_is_sync(struct request *rq)
{
	return op_is_sync(rq->cmd_flags);
}

#ifdef CONFIG_ROW_VIP_QUEUE
static inline bool rq_is_vip(struct request *rq)
{
	return rq->cmd_flags & REQ_VIP;
}
#endif

static inline bool blk_rl_full(struct request_list *rl, bool sync)
{
	unsigned int flag = sync ? BLK_RL_SYNCFULL : BLK_RL_ASYNCFULL;

	return rl->flags & flag;
}

static inline void blk_set_rl_full(struct request_list *rl, bool sync)
{
	unsigned int flag = sync ? BLK_RL_SYNCFULL : BLK_RL_ASYNCFULL;

	rl->flags |= flag;
}

static inline void blk_clear_rl_full(struct request_list *rl, bool sync)
{
	unsigned int flag = sync ? BLK_RL_SYNCFULL : BLK_RL_ASYNCFULL;

	rl->flags &= ~flag;
}

static inline bool rq_mergeable(struct request *rq)
{
	if (blk_rq_is_passthrough(rq))
		return false;

	if (req_op(rq) == REQ_OP_FLUSH)
		return false;

	if (req_op(rq) == REQ_OP_WRITE_ZEROES)
		return false;

	if (rq->cmd_flags & REQ_NOMERGE_FLAGS)
		return false;
	if (rq->rq_flags & RQF_NOMERGE_FLAGS)
		return false;

#ifdef CONFIG_MAS_BLK
	if (rq->rq_flags & RQF_DONTPREP)
		return false;
#endif

	return true;
}

static inline bool blk_write_same_mergeable(struct bio *a, struct bio *b)
{
	if (bio_page(a) == bio_page(b) &&
	    bio_offset(a) == bio_offset(b))
		return true;

	return false;
}

static inline unsigned int blk_queue_depth(struct request_queue *q)
{
	if (q->queue_depth)
		return q->queue_depth;

	return q->nr_requests;
}

/*
 * q->prep_rq_fn return values
 */
enum {
	BLKPREP_OK,		/* serve it */
	BLKPREP_KILL,		/* fatal error, kill, return -EIO */
	BLKPREP_DEFER,		/* leave on queue */
	BLKPREP_INVALID,	/* invalid command, kill, return -EREMOTEIO */
};

extern unsigned long blk_max_low_pfn, blk_max_pfn;

/*
 * standard bounce addresses:
 *
 * BLK_BOUNCE_HIGH	: bounce all highmem pages
 * BLK_BOUNCE_ANY	: don't bounce anything
 * BLK_BOUNCE_ISA	: bounce pages above ISA DMA boundary
 */

#if BITS_PER_LONG == 32
#define BLK_BOUNCE_HIGH		((u64)blk_max_low_pfn << PAGE_SHIFT)
#else
#define BLK_BOUNCE_HIGH		-1ULL
#endif
#define BLK_BOUNCE_ANY		(-1ULL)
#define BLK_BOUNCE_ISA		(DMA_BIT_MASK(24))

/*
 * default timeout for SG_IO if none specified
 */
#define BLK_DEFAULT_SG_TIMEOUT	(60 * HZ)
#define BLK_MIN_SG_TIMEOUT	(7 * HZ)

struct rq_map_data {
	struct page **pages;
	int page_order;
	int nr_entries;
	unsigned long offset;
	int null_mapped;
	int from_user;
};

struct req_iterator {
	struct bvec_iter iter;
	struct bio *bio;
};

/* This should not be used directly - use rq_for_each_segment */
#define for_each_bio(_bio)		\
	for (; _bio; _bio = _bio->bi_next)
#define __rq_for_each_bio(_bio, rq)	\
	if ((rq->bio))			\
		for (_bio = (rq)->bio; _bio; _bio = _bio->bi_next)

#define rq_for_each_segment(bvl, _rq, _iter)			\
	__rq_for_each_bio(_iter.bio, _rq)			\
		bio_for_each_segment(bvl, _iter.bio, _iter.iter)

#define rq_iter_last(bvec, _iter)				\
		(_iter.bio->bi_next == NULL &&			\
		 bio_iter_last(bvec, _iter.iter))

#ifndef ARCH_IMPLEMENTS_FLUSH_DCACHE_PAGE
# error	"You should define ARCH_IMPLEMENTS_FLUSH_DCACHE_PAGE for your platform"
#endif
#if ARCH_IMPLEMENTS_FLUSH_DCACHE_PAGE
extern void rq_flush_dcache_pages(struct request *rq);
#else
static inline void rq_flush_dcache_pages(struct request *rq)
{
}
#endif

#ifdef CONFIG_PRINTK
#define vfs_msg(sb, level, fmt, ...)				\
	__vfs_msg(sb, level, fmt, ##__VA_ARGS__)
#else
#define vfs_msg(sb, level, fmt, ...)				\
do {								\
	no_printk(fmt, ##__VA_ARGS__);				\
	__vfs_msg(sb, "", " ");					\
} while (0)
#endif

extern int blk_register_queue(struct gendisk *disk);
extern void blk_unregister_queue(struct gendisk *disk);
extern blk_qc_t generic_make_request(struct bio *bio);
extern void blk_rq_init(struct request_queue *q, struct request *rq);
extern void blk_init_request_from_bio(struct request *req, struct bio *bio);
extern void blk_put_request(struct request *);
extern void __blk_put_request(struct request_queue *, struct request *);
extern struct request *blk_get_request(struct request_queue *, unsigned int op,
				       gfp_t gfp_mask);
extern void blk_requeue_request(struct request_queue *, struct request *);
extern int blk_lld_busy(struct request_queue *q);
extern int blk_rq_prep_clone(struct request *rq, struct request *rq_src,
			     struct bio_set *bs, gfp_t gfp_mask,
			     int (*bio_ctr)(struct bio *, struct bio *, void *),
			     void *data);
extern void blk_rq_unprep_clone(struct request *rq);
extern blk_status_t blk_insert_cloned_request(struct request_queue *q,
				     struct request *rq);
extern int blk_rq_append_bio(struct request *rq, struct bio **bio);
extern void blk_delay_queue(struct request_queue *, unsigned long);
extern void blk_queue_split(struct request_queue *, struct bio **);
extern void blk_recount_segments(struct request_queue *, struct bio *);
extern int scsi_verify_blk_ioctl(struct block_device *, unsigned int);
extern int scsi_cmd_blk_ioctl(struct block_device *, fmode_t,
			      unsigned int, void __user *);
extern int scsi_cmd_ioctl(struct request_queue *, struct gendisk *, fmode_t,
			  unsigned int, void __user *);
extern int sg_scsi_ioctl(struct request_queue *, struct gendisk *, fmode_t,
			 struct scsi_ioctl_command __user *);

extern int blk_queue_enter(struct request_queue *q, bool nowait);
extern void blk_queue_exit(struct request_queue *q);
extern void blk_start_queue(struct request_queue *q);
extern void blk_start_queue_async(struct request_queue *q);
extern void blk_stop_queue(struct request_queue *q);
extern void blk_sync_queue(struct request_queue *q);
extern void __blk_stop_queue(struct request_queue *q);
extern void __blk_run_queue(struct request_queue *q);
extern void __blk_run_queue_uncond(struct request_queue *q);
extern void blk_run_queue(struct request_queue *);
extern void blk_run_queue_async(struct request_queue *q);
extern int blk_rq_map_user(struct request_queue *, struct request *,
			   struct rq_map_data *, void __user *, unsigned long,
			   gfp_t);
extern int blk_rq_unmap_user(struct bio *);
extern int blk_rq_map_kern(struct request_queue *, struct request *, void *, unsigned int, gfp_t);
extern int blk_rq_map_user_iov(struct request_queue *, struct request *,
			       struct rq_map_data *, const struct iov_iter *,
			       gfp_t);
extern void blk_execute_rq(struct request_queue *, struct gendisk *,
			  struct request *, int);
extern void blk_execute_rq_nowait(struct request_queue *, struct gendisk *,
				  struct request *, int, rq_end_io_fn *);

int blk_status_to_errno(blk_status_t status);
blk_status_t errno_to_blk_status(int errno);

bool blk_mq_poll(struct request_queue *q, blk_qc_t cookie);

static inline struct request_queue *bdev_get_queue(struct block_device *bdev)
{
	return bdev->bd_disk->queue;	/* this is never NULL */
}

/*
 * blk_rq_pos()			: the current sector
 * blk_rq_bytes()		: bytes left in the entire request
 * blk_rq_cur_bytes()		: bytes left in the current segment
 * blk_rq_err_bytes()		: bytes left till the next error boundary
 * blk_rq_sectors()		: sectors left in the entire request
 * blk_rq_cur_sectors()		: sectors left in the current segment
 */
static inline sector_t blk_rq_pos(const struct request *rq)
{
	return rq->__sector;
}

static inline unsigned int blk_rq_bytes(const struct request *rq)
{
	return rq->__data_len;
}

static inline int blk_rq_cur_bytes(const struct request *rq)
{
	return rq->bio ? bio_cur_bytes(rq->bio) : 0;
}

extern unsigned int blk_rq_err_bytes(const struct request *rq);

static inline unsigned int blk_rq_sectors(const struct request *rq)
{
	return blk_rq_bytes(rq) >> 9;
}

static inline unsigned int blk_rq_cur_sectors(const struct request *rq)
{
	return blk_rq_cur_bytes(rq) >> 9;
}

/*
 * Some commands like WRITE SAME have a payload or data transfer size which
 * is different from the size of the request.  Any driver that supports such
 * commands using the RQF_SPECIAL_PAYLOAD flag needs to use this helper to
 * calculate the data transfer size.
 */
static inline unsigned int blk_rq_payload_bytes(struct request *rq)
{
	if (rq->rq_flags & RQF_SPECIAL_PAYLOAD)
		return rq->special_vec.bv_len;
	return blk_rq_bytes(rq);
}

static inline unsigned int blk_queue_get_max_sectors(struct request_queue *q,
						     int op)
{
	if (unlikely(op == REQ_OP_DISCARD || op == REQ_OP_SECURE_ERASE))
		return min(q->limits.max_discard_sectors, UINT_MAX >> 9);

	if (unlikely(op == REQ_OP_WRITE_SAME))
		return q->limits.max_write_same_sectors;

	if (unlikely(op == REQ_OP_WRITE_ZEROES))
		return q->limits.max_write_zeroes_sectors;

	return q->limits.max_sectors;
}

/*
 * Return maximum size of a request at given offset. Only valid for
 * file system requests.
 */
static inline unsigned int blk_max_size_offset(struct request_queue *q,
					       sector_t offset)
{
	if (!q->limits.chunk_sectors)
		return q->limits.max_sectors;

	return min(q->limits.max_sectors, (unsigned int)(q->limits.chunk_sectors -
			(offset & (q->limits.chunk_sectors - 1))));
}

static inline unsigned int blk_rq_get_max_sectors(struct request *rq,
						  sector_t offset)
{
	struct request_queue *q = rq->q;

	if (blk_rq_is_passthrough(rq))
		return q->limits.max_hw_sectors;

	if (!q->limits.chunk_sectors ||
	    req_op(rq) == REQ_OP_DISCARD ||
	    req_op(rq) == REQ_OP_SECURE_ERASE)
		return blk_queue_get_max_sectors(q, req_op(rq));

	return min(blk_max_size_offset(q, offset),
			blk_queue_get_max_sectors(q, req_op(rq)));
}

static inline unsigned int blk_rq_count_bios(struct request *rq)
{
	unsigned int nr_bios = 0;
	struct bio *bio;

	__rq_for_each_bio(bio, rq)
		nr_bios++;

	return nr_bios;
}

/*
 * Request issue related functions.
 */
extern struct request *blk_peek_request(struct request_queue *q);
extern void blk_start_request(struct request *rq);
extern struct request *blk_fetch_request(struct request_queue *q);

/*
 * Request completion related functions.
 *
 * blk_update_request() completes given number of bytes and updates
 * the request without completing it.
 *
 * blk_end_request() and friends.  __blk_end_request() must be called
 * with the request queue spinlock acquired.
 *
 * Several drivers define their own end_request and call
 * blk_end_request() for parts of the original function.
 * This prevents code duplication in drivers.
 */
extern bool blk_update_request(struct request *rq, blk_status_t error,
			       unsigned int nr_bytes);
extern void blk_finish_request(struct request *rq, blk_status_t error);
extern bool blk_end_request(struct request *rq, blk_status_t error,
			    unsigned int nr_bytes);
extern void blk_end_request_all(struct request *rq, blk_status_t error);
extern bool __blk_end_request(struct request *rq, blk_status_t error,
			      unsigned int nr_bytes);
extern void __blk_end_request_all(struct request *rq, blk_status_t error);
extern bool __blk_end_request_cur(struct request *rq, blk_status_t error);

extern void blk_complete_request(struct request *);
extern void __blk_complete_request(struct request *);
extern void blk_abort_request(struct request *);
extern void blk_unprep_request(struct request *);

/*
 * Access functions for manipulating queue properties
 */
extern struct request_queue *blk_init_queue_node(request_fn_proc *rfn,
					spinlock_t *lock, int node_id);
extern struct request_queue *blk_init_queue(request_fn_proc *, spinlock_t *);
extern int blk_init_allocated_queue(struct request_queue *);
extern void blk_cleanup_queue(struct request_queue *);
extern void blk_queue_make_request(struct request_queue *, make_request_fn *);
extern void blk_queue_bounce_limit(struct request_queue *, u64);
extern void blk_queue_max_hw_sectors(struct request_queue *, unsigned int);
extern void blk_queue_chunk_sectors(struct request_queue *, unsigned int);
extern void blk_queue_max_segments(struct request_queue *, unsigned short);
extern void blk_queue_max_discard_segments(struct request_queue *,
		unsigned short);
extern void blk_queue_max_segment_size(struct request_queue *, unsigned int);
extern void blk_queue_max_discard_sectors(struct request_queue *q,
		unsigned int max_discard_sectors);
extern void blk_queue_max_write_same_sectors(struct request_queue *q,
		unsigned int max_write_same_sectors);
extern void blk_queue_max_write_zeroes_sectors(struct request_queue *q,
		unsigned int max_write_same_sectors);
extern void blk_queue_logical_block_size(struct request_queue *, unsigned int);
extern void blk_queue_physical_block_size(struct request_queue *, unsigned int);
extern void blk_queue_alignment_offset(struct request_queue *q,
				       unsigned int alignment);
extern void blk_limits_io_min(struct queue_limits *limits, unsigned int min);
extern void blk_queue_io_min(struct request_queue *q, unsigned int min);
extern void blk_limits_io_opt(struct queue_limits *limits, unsigned int opt);
extern void blk_queue_io_opt(struct request_queue *q, unsigned int opt);
extern void blk_set_queue_depth(struct request_queue *q, unsigned int depth);
extern void blk_set_default_limits(struct queue_limits *lim);
extern void blk_set_stacking_limits(struct queue_limits *lim);
extern int blk_stack_limits(struct queue_limits *t, struct queue_limits *b,
			    sector_t offset);
extern int bdev_stack_limits(struct queue_limits *t, struct block_device *bdev,
			    sector_t offset);
extern void disk_stack_limits(struct gendisk *disk, struct block_device *bdev,
			      sector_t offset);
extern void blk_queue_stack_limits(struct request_queue *t, struct request_queue *b);
extern void blk_queue_dma_pad(struct request_queue *, unsigned int);
extern void blk_queue_update_dma_pad(struct request_queue *, unsigned int);
extern int blk_queue_dma_drain(struct request_queue *q,
			       dma_drain_needed_fn *dma_drain_needed,
			       void *buf, unsigned int size);
extern void blk_queue_lld_busy(struct request_queue *q, lld_busy_fn *fn);
extern void blk_queue_segment_boundary(struct request_queue *, unsigned long);
extern void blk_queue_virt_boundary(struct request_queue *, unsigned long);
extern void blk_queue_prep_rq(struct request_queue *, prep_rq_fn *pfn);
extern void blk_queue_unprep_rq(struct request_queue *, unprep_rq_fn *ufn);
extern void blk_queue_dma_alignment(struct request_queue *, int);
extern void blk_queue_update_dma_alignment(struct request_queue *, int);
extern void blk_queue_softirq_done(struct request_queue *, softirq_done_fn *);
extern void blk_queue_rq_timed_out(struct request_queue *, rq_timed_out_fn *);
extern void blk_queue_rq_timeout(struct request_queue *, unsigned int);
extern void blk_queue_flush_queueable(struct request_queue *q, bool queueable);
extern void blk_queue_write_cache(struct request_queue *q, bool enabled, bool fua);

/*
 * Number of physical segments as sent to the device.
 *
 * Normally this is the number of discontiguous data segments sent by the
 * submitter.  But for data-less command like discard we might have no
 * actual data segments submitted, but the driver might have to add it's
 * own special payload.  In that case we still return 1 here so that this
 * special payload will be mapped.
 */
static inline unsigned short blk_rq_nr_phys_segments(struct request *rq)
{
	if (rq->rq_flags & RQF_SPECIAL_PAYLOAD)
		return 1;
	return rq->nr_phys_segments;
}

/*
 * Number of discard segments (or ranges) the driver needs to fill in.
 * Each discard bio merged into a request is counted as one segment.
 */
static inline unsigned short blk_rq_nr_discard_segments(struct request *rq)
{
	return max_t(unsigned short, rq->nr_phys_segments, 1);
}

extern int blk_rq_map_sg(struct request_queue *, struct request *, struct scatterlist *);
extern void blk_dump_rq_flags(struct request *, char *);
extern long nr_blockdev_pages(void);

bool __must_check blk_get_queue(struct request_queue *);
struct request_queue *blk_alloc_queue(gfp_t);
struct request_queue *blk_alloc_queue_node(gfp_t, int);
extern void blk_put_queue(struct request_queue *);
extern void blk_set_queue_dying(struct request_queue *);

/*
 * block layer runtime pm functions
 */
#ifdef CONFIG_PM
extern void blk_pm_runtime_init(struct request_queue *q, struct device *dev);
extern int blk_pre_runtime_suspend(struct request_queue *q);
extern void blk_post_runtime_suspend(struct request_queue *q, int err);
extern void blk_pre_runtime_resume(struct request_queue *q);
extern void blk_post_runtime_resume(struct request_queue *q, int err);
extern void blk_set_runtime_active(struct request_queue *q);
#else
static inline void blk_pm_runtime_init(struct request_queue *q,
	struct device *dev) {}
static inline int blk_pre_runtime_suspend(struct request_queue *q)
{
	return -ENOSYS;
}
static inline void blk_post_runtime_suspend(struct request_queue *q, int err) {}
static inline void blk_pre_runtime_resume(struct request_queue *q) {}
static inline void blk_post_runtime_resume(struct request_queue *q, int err) {}
static inline void blk_set_runtime_active(struct request_queue *q) {}
#endif

/*
 * blk_plug permits building a queue of related requests by holding the I/O
 * fragments for a short period. This allows merging of sequential requests
 * into single larger request. As the requests are moved from a per-task list to
 * the device's request_queue in a batch, this results in improved scalability
 * as the lock contention for request_queue lock is reduced.
 *
 * It is ok not to disable preemption when adding the request to the plug list
 * or when attempting a merge, because blk_schedule_flush_list() will only flush
 * the plug list when the task sleeps by itself. For details, please see
 * schedule() where blk_schedule_flush_plug() is called.
 */
struct blk_plug {
	struct list_head list; /* requests */
	struct list_head mq_list; /* blk-mq requests */
	struct list_head cb_list; /* md requires an unplug callback */
#ifdef CONFIG_MAS_BLK
	struct list_head mas_blk_list;
	void (*flush_plug_list_fn)(struct blk_plug *, bool);
#endif
};
#define BLK_MAX_REQUEST_COUNT 16
#define BLK_PLUG_FLUSH_SIZE (128 * 1024)

struct blk_plug_cb;
typedef void (*blk_plug_cb_fn)(struct blk_plug_cb *, bool);
struct blk_plug_cb {
	struct list_head list;
	blk_plug_cb_fn callback;
	void *data;
};
extern struct blk_plug_cb *blk_check_plugged(blk_plug_cb_fn unplug,
					     void *data, int size);
extern void blk_start_plug(struct blk_plug *);
extern void blk_finish_plug(struct blk_plug *);
extern void blk_flush_plug_list(struct blk_plug *, bool);

static inline void blk_flush_plug(struct task_struct *tsk)
{
	struct blk_plug *plug = tsk->plug;

	if (plug)
		blk_flush_plug_list(plug, false);
}

static inline void blk_schedule_flush_plug(struct task_struct *tsk)
{
	struct blk_plug *plug = tsk->plug;

	if (plug)
		blk_flush_plug_list(plug, true);
}

static inline bool blk_needs_flush_plug(struct task_struct *tsk)
{
	struct blk_plug *plug = tsk->plug;

	return plug &&
		(!list_empty(&plug->list) ||
#ifdef CONFIG_MAS_BLK
		!list_empty(&plug->mas_blk_list) ||
#endif
		 !list_empty(&plug->mq_list) ||
		 !list_empty(&plug->cb_list));
}

/*
 * tag stuff
 */
extern int blk_queue_start_tag(struct request_queue *, struct request *);
extern struct request *blk_queue_find_tag(struct request_queue *, int);
extern void blk_queue_end_tag(struct request_queue *, struct request *);
extern int blk_queue_init_tags(struct request_queue *, int, struct blk_queue_tag *, int);
extern void blk_queue_free_tags(struct request_queue *);
extern int blk_queue_resize_tags(struct request_queue *, int);
extern void blk_queue_invalidate_tags(struct request_queue *);
extern struct blk_queue_tag *blk_init_tags(int, int);
extern void blk_free_tags(struct blk_queue_tag *);

static inline struct request *blk_map_queue_find_tag(struct blk_queue_tag *bqt,
						int tag)
{
	if (unlikely(bqt == NULL || tag >= bqt->real_max_depth))
		return NULL;
	return bqt->tag_index[tag];
}

extern int blkdev_issue_flush(struct block_device *, gfp_t, sector_t *);
extern int blkdev_issue_write_same(struct block_device *bdev, sector_t sector,
		sector_t nr_sects, gfp_t gfp_mask, struct page *page);

#define BLKDEV_DISCARD_SECURE	(1 << 0)	/* issue a secure erase */

extern int blkdev_issue_discard(struct block_device *bdev, sector_t sector,
		sector_t nr_sects, gfp_t gfp_mask, unsigned long flags);
extern int __blkdev_issue_discard(struct block_device *bdev, sector_t sector,
		sector_t nr_sects, gfp_t gfp_mask, int flags,
		struct bio **biop);

#define BLKDEV_ZERO_NOUNMAP	(1 << 0)  /* do not free blocks */
#define BLKDEV_ZERO_NOFALLBACK	(1 << 1)  /* don't write explicit zeroes */

extern int __blkdev_issue_zeroout(struct block_device *bdev, sector_t sector,
		sector_t nr_sects, gfp_t gfp_mask, struct bio **biop,
		unsigned flags);
extern int blkdev_issue_zeroout(struct block_device *bdev, sector_t sector,
		sector_t nr_sects, gfp_t gfp_mask, unsigned flags);

static inline int sb_issue_discard(struct super_block *sb, sector_t block,
		sector_t nr_blocks, gfp_t gfp_mask, unsigned long flags)
{
	return blkdev_issue_discard(sb->s_bdev, block << (sb->s_blocksize_bits - 9),
				    nr_blocks << (sb->s_blocksize_bits - 9),
				    gfp_mask, flags);
}
static inline int sb_issue_zeroout(struct super_block *sb, sector_t block,
		sector_t nr_blocks, gfp_t gfp_mask)
{
	return blkdev_issue_zeroout(sb->s_bdev,
				    block << (sb->s_blocksize_bits - 9),
				    nr_blocks << (sb->s_blocksize_bits - 9),
				    gfp_mask, 0);
}

extern int blk_verify_command(unsigned char *cmd, fmode_t has_write_perm);

enum blk_default_limits {
	BLK_MAX_SEGMENTS	= 128,
	BLK_SAFE_MAX_SECTORS	= 255,
	BLK_DEF_MAX_SECTORS	= 2560,
	BLK_MAX_SEGMENT_SIZE	= 65536,
	BLK_SEG_BOUNDARY_MASK	= 0xFFFFFFFFUL,
};

#define blkdev_entry_to_request(entry) list_entry((entry), struct request, queuelist)

static inline unsigned long queue_segment_boundary(struct request_queue *q)
{
	return q->limits.seg_boundary_mask;
}

static inline unsigned long queue_virt_boundary(struct request_queue *q)
{
	return q->limits.virt_boundary_mask;
}

static inline unsigned int queue_max_sectors(struct request_queue *q)
{
	return q->limits.max_sectors;
}

static inline unsigned int queue_max_hw_sectors(struct request_queue *q)
{
	return q->limits.max_hw_sectors;
}

static inline unsigned short queue_max_segments(struct request_queue *q)
{
	return q->limits.max_segments;
}

static inline unsigned short queue_max_discard_segments(struct request_queue *q)
{
	return q->limits.max_discard_segments;
}

static inline unsigned int queue_max_segment_size(struct request_queue *q)
{
	return q->limits.max_segment_size;
}

static inline unsigned queue_logical_block_size(struct request_queue *q)
{
	int retval = 512;

	if (q && q->limits.logical_block_size)
		retval = q->limits.logical_block_size;

	return retval;
}

static inline unsigned int bdev_logical_block_size(struct block_device *bdev)
{
	return queue_logical_block_size(bdev_get_queue(bdev));
}

static inline unsigned int queue_physical_block_size(struct request_queue *q)
{
	return q->limits.physical_block_size;
}

static inline unsigned int bdev_physical_block_size(struct block_device *bdev)
{
	return queue_physical_block_size(bdev_get_queue(bdev));
}

static inline unsigned int queue_io_min(struct request_queue *q)
{
	return q->limits.io_min;
}

static inline int bdev_io_min(struct block_device *bdev)
{
	return queue_io_min(bdev_get_queue(bdev));
}

static inline unsigned int queue_io_opt(struct request_queue *q)
{
	return q->limits.io_opt;
}

static inline int bdev_io_opt(struct block_device *bdev)
{
	return queue_io_opt(bdev_get_queue(bdev));
}

static inline int queue_alignment_offset(struct request_queue *q)
{
	if (q->limits.misaligned)
		return -1;

	return q->limits.alignment_offset;
}

static inline int queue_limit_alignment_offset(struct queue_limits *lim, sector_t sector)
{
	unsigned int granularity = max(lim->physical_block_size, lim->io_min);
	unsigned int alignment = sector_div(sector, granularity >> 9) << 9;

	return (granularity + lim->alignment_offset - alignment) % granularity;
}

static inline int bdev_alignment_offset(struct block_device *bdev)
{
	struct request_queue *q = bdev_get_queue(bdev);

	if (q->limits.misaligned)
		return -1;

	if (bdev != bdev->bd_contains)
		return bdev->bd_part->alignment_offset;

	return q->limits.alignment_offset;
}

static inline int queue_discard_alignment(struct request_queue *q)
{
	if (q->limits.discard_misaligned)
		return -1;

	return q->limits.discard_alignment;
}

static inline int queue_limit_discard_alignment(struct queue_limits *lim, sector_t sector)
{
	unsigned int alignment, granularity, offset;

	if (!lim->max_discard_sectors)
		return 0;

	/* Why are these in bytes, not sectors? */
	alignment = lim->discard_alignment >> 9;
	granularity = lim->discard_granularity >> 9;
	if (!granularity)
		return 0;

	/* Offset of the partition start in 'granularity' sectors */
	offset = sector_div(sector, granularity);

	/* And why do we do this modulus *again* in blkdev_issue_discard()? */
	offset = (granularity + alignment - offset) % granularity;

	/* Turn it back into bytes, gaah */
	return offset << 9;
}

static inline int bdev_discard_alignment(struct block_device *bdev)
{
	struct request_queue *q = bdev_get_queue(bdev);

	if (bdev != bdev->bd_contains)
		return bdev->bd_part->discard_alignment;

	return q->limits.discard_alignment;
}

static inline unsigned int bdev_write_same(struct block_device *bdev)
{
	struct request_queue *q = bdev_get_queue(bdev);

	if (q)
		return q->limits.max_write_same_sectors;

	return 0;
}

static inline unsigned int bdev_write_zeroes_sectors(struct block_device *bdev)
{
	struct request_queue *q = bdev_get_queue(bdev);

	if (q)
		return q->limits.max_write_zeroes_sectors;

	return 0;
}

static inline enum blk_zoned_model bdev_zoned_model(struct block_device *bdev)
{
	struct request_queue *q = bdev_get_queue(bdev);

	if (q)
		return blk_queue_zoned_model(q);

	return BLK_ZONED_NONE;
}

static inline bool bdev_is_zoned(struct block_device *bdev)
{
	struct request_queue *q = bdev_get_queue(bdev);

	if (q)
		return blk_queue_is_zoned(q);

	return false;
}

static inline unsigned int bdev_zone_sectors(struct block_device *bdev)
{
	struct request_queue *q = bdev_get_queue(bdev);

	if (q)
		return blk_queue_zone_sectors(q);

	return 0;
}

static inline int queue_dma_alignment(struct request_queue *q)
{
	return q ? q->dma_alignment : 511;
}

static inline int blk_rq_aligned(struct request_queue *q, unsigned long addr,
				 unsigned int len)
{
	unsigned int alignment = queue_dma_alignment(q) | q->dma_pad_mask;
	return !(addr & alignment) && !(len & alignment);
}

/* assumes size > 256 */
static inline unsigned int blksize_bits(unsigned int size)
{
	unsigned int bits = 8;
	do {
		bits++;
		size >>= 1;
	} while (size > 256);
	return bits;
}

static inline unsigned int block_size(struct block_device *bdev)
{
	return bdev->bd_block_size;
}

static inline bool queue_flush_queueable(struct request_queue *q)
{
	return !test_bit(QUEUE_FLAG_FLUSH_NQ, &q->queue_flags);
}

typedef struct {struct page *v;} Sector;

unsigned char *read_dev_sector(struct block_device *, sector_t, Sector *);

static inline void put_dev_sector(Sector p)
{
	put_page(p.v);
}

static inline bool __bvec_gap_to_prev(struct request_queue *q,
				struct bio_vec *bprv, unsigned int offset)
{
	return offset ||
		((bprv->bv_offset + bprv->bv_len) & queue_virt_boundary(q));
}

/*
 * Check if adding a bio_vec after bprv with offset would create a gap in
 * the SG list. Most drivers don't care about this, but some do.
 */
static inline bool bvec_gap_to_prev(struct request_queue *q,
				struct bio_vec *bprv, unsigned int offset)
{
	if (!queue_virt_boundary(q))
		return false;
	return __bvec_gap_to_prev(q, bprv, offset);
}

/*
 * Check if the two bvecs from two bios can be merged to one segment.
 * If yes, no need to check gap between the two bios since the 1st bio
 * and the 1st bvec in the 2nd bio can be handled in one segment.
 */
static inline bool bios_segs_mergeable(struct request_queue *q,
		struct bio *prev, struct bio_vec *prev_last_bv,
		struct bio_vec *next_first_bv)
{
	if (!BIOVEC_PHYS_MERGEABLE(prev_last_bv, next_first_bv))
		return false;
	if (!BIOVEC_SEG_BOUNDARY(q, prev_last_bv, next_first_bv))
		return false;
	if (prev->bi_seg_back_size + next_first_bv->bv_len >
			queue_max_segment_size(q))
		return false;
	return true;
}

static inline bool bio_will_gap(struct request_queue *q,
				struct request *prev_rq,
				struct bio *prev,
				struct bio *next)
{
	if (bio_has_data(prev) && queue_virt_boundary(q)) {
		struct bio_vec pb, nb;

		/*
		 * don't merge if the 1st bio starts with non-zero
		 * offset, otherwise it is quite difficult to respect
		 * sg gap limit. We work hard to merge a huge number of small
		 * single bios in case of mkfs.
		 */
		if (prev_rq)
			bio_get_first_bvec(prev_rq->bio, &pb);
		else
			bio_get_first_bvec(prev, &pb);
		if (pb.bv_offset)
			return true;

		/*
		 * We don't need to worry about the situation that the
		 * merged segment ends in unaligned virt boundary:
		 *
		 * - if 'pb' ends aligned, the merged segment ends aligned
		 * - if 'pb' ends unaligned, the next bio must include
		 *   one single bvec of 'nb', otherwise the 'nb' can't
		 *   merge with 'pb'
		 */
		bio_get_last_bvec(prev, &pb);
		bio_get_first_bvec(next, &nb);

		if (!bios_segs_mergeable(q, prev, &pb, &nb))
			return __bvec_gap_to_prev(q, &pb, nb.bv_offset);
	}

	return false;
}

static inline bool req_gap_back_merge(struct request *req, struct bio *bio)
{
	return bio_will_gap(req->q, req, req->biotail, bio);
}

static inline bool req_gap_front_merge(struct request *req, struct bio *bio)
{
	return bio_will_gap(req->q, NULL, bio, req->bio);
}

int kblockd_schedule_work(struct work_struct *work);
int kblockd_schedule_work_on(int cpu, struct work_struct *work);
int kblockd_schedule_delayed_work(struct delayed_work *dwork, unsigned long delay);
int kblockd_schedule_delayed_work_on(int cpu, struct delayed_work *dwork, unsigned long delay);
int kblockd_mod_delayed_work_on(int cpu, struct delayed_work *dwork, unsigned long delay);

#ifdef CONFIG_BLK_CGROUP
/*
 * This should not be using sched_clock(). A real patch is in progress
 * to fix this up, until that is in place we need to disable preemption
 * around sched_clock() in this function and set_io_start_time_ns().
 */
static inline void set_start_time_ns(struct request *req)
{
	preempt_disable();
	req->start_time_ns = sched_clock();
	preempt_enable();
}

static inline void set_io_start_time_ns(struct request *req)
{
	preempt_disable();
	req->io_start_time_ns = sched_clock();
	preempt_enable();
}

static inline uint64_t rq_start_time_ns(struct request *req)
{
        return req->start_time_ns;
}

static inline uint64_t rq_io_start_time_ns(struct request *req)
{
        return req->io_start_time_ns;
}
#else
static inline void set_start_time_ns(struct request *req) {}
static inline void set_io_start_time_ns(struct request *req) {}
static inline uint64_t rq_start_time_ns(struct request *req)
{
	return 0;
}
static inline uint64_t rq_io_start_time_ns(struct request *req)
{
	return 0;
}
#endif

#define MODULE_ALIAS_BLOCKDEV(major,minor) \
	MODULE_ALIAS("block-major-" __stringify(major) "-" __stringify(minor))
#define MODULE_ALIAS_BLOCKDEV_MAJOR(major) \
	MODULE_ALIAS("block-major-" __stringify(major) "-*")

#if defined(CONFIG_BLK_DEV_INTEGRITY)

enum blk_integrity_flags {
	BLK_INTEGRITY_VERIFY		= 1 << 0,
	BLK_INTEGRITY_GENERATE		= 1 << 1,
	BLK_INTEGRITY_DEVICE_CAPABLE	= 1 << 2,
	BLK_INTEGRITY_IP_CHECKSUM	= 1 << 3,
};

struct blk_integrity_iter {
	void			*prot_buf;
	void			*data_buf;
	sector_t		seed;
	unsigned int		data_size;
	unsigned short		interval;
	const char		*disk_name;
};

typedef blk_status_t (integrity_processing_fn) (struct blk_integrity_iter *);

struct blk_integrity_profile {
	integrity_processing_fn		*generate_fn;
	integrity_processing_fn		*verify_fn;
	const char			*name;
};

extern void blk_integrity_register(struct gendisk *, struct blk_integrity *);
extern void blk_integrity_unregister(struct gendisk *);
extern int blk_integrity_compare(struct gendisk *, struct gendisk *);
extern int blk_rq_map_integrity_sg(struct request_queue *, struct bio *,
				   struct scatterlist *);
extern int blk_rq_count_integrity_sg(struct request_queue *, struct bio *);
extern bool blk_integrity_merge_rq(struct request_queue *, struct request *,
				   struct request *);
extern bool blk_integrity_merge_bio(struct request_queue *, struct request *,
				    struct bio *);

static inline struct blk_integrity *blk_get_integrity(struct gendisk *disk)
{
	struct blk_integrity *bi = &disk->queue->integrity;

	if (!bi->profile)
		return NULL;

	return bi;
}

static inline
struct blk_integrity *bdev_get_integrity(struct block_device *bdev)
{
	return blk_get_integrity(bdev->bd_disk);
}

static inline bool blk_integrity_rq(struct request *rq)
{
	return rq->cmd_flags & REQ_INTEGRITY;
}

static inline void blk_queue_max_integrity_segments(struct request_queue *q,
						    unsigned int segs)
{
	q->limits.max_integrity_segments = segs;
}

static inline unsigned short
queue_max_integrity_segments(struct request_queue *q)
{
	return q->limits.max_integrity_segments;
}

static inline bool integrity_req_gap_back_merge(struct request *req,
						struct bio *next)
{
	struct bio_integrity_payload *bip = bio_integrity(req->bio);
	struct bio_integrity_payload *bip_next = bio_integrity(next);

	return bvec_gap_to_prev(req->q, &bip->bip_vec[bip->bip_vcnt - 1],
				bip_next->bip_vec[0].bv_offset);
}

static inline bool integrity_req_gap_front_merge(struct request *req,
						 struct bio *bio)
{
	struct bio_integrity_payload *bip = bio_integrity(bio);
	struct bio_integrity_payload *bip_next = bio_integrity(req->bio);

	return bvec_gap_to_prev(req->q, &bip->bip_vec[bip->bip_vcnt - 1],
				bip_next->bip_vec[0].bv_offset);
}

#else /* CONFIG_BLK_DEV_INTEGRITY */

struct bio;
struct block_device;
struct gendisk;
struct blk_integrity;

static inline int blk_integrity_rq(struct request *rq)
{
	return 0;
}
static inline int blk_rq_count_integrity_sg(struct request_queue *q,
					    struct bio *b)
{
	return 0;
}
static inline int blk_rq_map_integrity_sg(struct request_queue *q,
					  struct bio *b,
					  struct scatterlist *s)
{
	return 0;
}
static inline struct blk_integrity *bdev_get_integrity(struct block_device *b)
{
	return NULL;
}
static inline struct blk_integrity *blk_get_integrity(struct gendisk *disk)
{
	return NULL;
}
static inline int blk_integrity_compare(struct gendisk *a, struct gendisk *b)
{
	return 0;
}
static inline void blk_integrity_register(struct gendisk *d,
					 struct blk_integrity *b)
{
}
static inline void blk_integrity_unregister(struct gendisk *d)
{
}
static inline void blk_queue_max_integrity_segments(struct request_queue *q,
						    unsigned int segs)
{
}
static inline unsigned short queue_max_integrity_segments(struct request_queue *q)
{
	return 0;
}
static inline bool blk_integrity_merge_rq(struct request_queue *rq,
					  struct request *r1,
					  struct request *r2)
{
	return true;
}
static inline bool blk_integrity_merge_bio(struct request_queue *rq,
					   struct request *r,
					   struct bio *b)
{
	return true;
}

static inline bool integrity_req_gap_back_merge(struct request *req,
						struct bio *next)
{
	return false;
}
static inline bool integrity_req_gap_front_merge(struct request *req,
						 struct bio *bio)
{
	return false;
}

#endif /* CONFIG_BLK_DEV_INTEGRITY */

struct block_device_operations {
	int (*open) (struct block_device *, fmode_t);
	void (*release) (struct gendisk *, fmode_t);
	int (*rw_page)(struct block_device *, sector_t, struct page *, bool);
	int (*ioctl) (struct block_device *, fmode_t, unsigned, unsigned long);
	int (*compat_ioctl) (struct block_device *, fmode_t, unsigned, unsigned long);
	unsigned int (*check_events) (struct gendisk *disk,
				      unsigned int clearing);
	/* ->media_changed() is DEPRECATED, use ->check_events() instead */
	int (*media_changed) (struct gendisk *);
	void (*unlock_native_capacity) (struct gendisk *);
	int (*revalidate_disk) (struct gendisk *);
	int (*getgeo)(struct block_device *, struct hd_geometry *);
	/* this callback is with swap_lock and sometimes page table lock held */
	void (*swap_slot_free_notify) (struct block_device *, unsigned long);
	struct module *owner;
	const struct pr_ops *pr_ops;
};

extern int __blkdev_driver_ioctl(struct block_device *, fmode_t, unsigned int,
				 unsigned long);
extern int bdev_read_page(struct block_device *, sector_t, struct page *);
extern int bdev_write_page(struct block_device *, sector_t, struct page *,
						struct writeback_control *);
extern bool bdev_dax_capable(struct block_device *);
extern void print_bdev_access_info(void);

/*
 * X-axis for IO latency histogram support.
 */
static const u_int64_t latency_x_axis_us[] = {
	100,
	200,
	300,
	400,
	500,
	600,
	700,
	800,
	900,
	1000,
	1200,
	1400,
	1600,
	1800,
	2000,
	2500,
	3000,
	4000,
	5000,
	6000,
	7000,
	9000,
	10000
};

#define BLK_IO_LAT_HIST_DISABLE         0
#define BLK_IO_LAT_HIST_ENABLE          1
#define BLK_IO_LAT_HIST_ZERO            2
#define ASYNC_LIMIT_STAGE_ONE 0
#define ASYNC_LIMIT_STAGE_TWO 1

struct io_latency_state {
	u_int64_t	latency_y_axis[ARRAY_SIZE(latency_x_axis_us) + 1];
	u_int64_t	latency_elems;
	u_int64_t	latency_sum;
};
static inline void
blk_update_latency_hist(struct io_latency_state *s, u_int64_t delta_us)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(latency_x_axis_us); i++)
		if (delta_us < (u_int64_t)latency_x_axis_us[i])
			break;
	s->latency_y_axis[i]++;
	s->latency_elems++;
	s->latency_sum += delta_us;
}

ssize_t blk_latency_hist_show(char *name, struct io_latency_state *s,
		char *buf, int buf_size);

#if defined(CONFIG_MAS_BLK) && defined(CONFIG_MAS_UNISTORE_PRESERVE)
struct request *mas_blk_get_request_reset(
	struct request_queue *q, unsigned int op, gfp_t gfp_mask);
void mas_blk_queue_split_for_wop_write(
	struct request_queue *, struct bio **);
void mas_blk_fsync_barrier(struct block_device *bdev);
void mas_blk_dump_unistore(struct request_queue *q, unsigned char *prefix);
int mas_blk_data_move(struct block_device *bi_bdev,
	struct stor_dev_data_move_info *data_move_info);
void mas_blk_mq_tagset_data_move_register(
	struct blk_mq_tag_set *tag_set, lld_dev_data_move_fn func);
int mas_blk_slc_mode_configuration(
	struct block_device *bi_bdev, int *status);
void mas_blk_mq_tagset_slc_mode_configuration_register(
	struct blk_mq_tag_set *tag_set, lld_dev_slc_mode_configuration_fn func);
int mas_blk_sync_read_verify(struct block_device *bi_bdev,
	struct stor_dev_sync_read_verify_info *verify_info);
void mas_blk_mq_tagset_sync_read_verify_register(
	struct blk_mq_tag_set *tag_set, lld_dev_sync_read_verify_fn func);
int mas_blk_get_bad_block_info(struct block_device *bi_bdev,
	struct stor_dev_bad_block_info *bad_block_info);
void mas_blk_mq_tagset_get_bad_block_info_register(
	struct blk_mq_tag_set *tag_set, lld_dev_get_bad_block_info_fn func);
int mas_blk_device_pwron_info_sync(struct block_device *bi_bdev,
	struct stor_dev_pwron_info *stor_info, unsigned int rescue_seg_size);
void mas_blk_mq_tagset_pwron_info_sync_register(
	struct blk_mq_tag_set *tag_set, lld_dev_pwron_info_sync_fn func);
int mas_blk_stream_oob_info_fetch(struct block_device *bi_bdev,
	struct stor_dev_stream_info stream_info, unsigned int oob_entry_cnt,
	struct stor_dev_stream_oob_info *oob_info);
void mas_blk_mq_tagset_stream_oob_info_register(
	struct blk_mq_tag_set *tag_set, lld_dev_stream_oob_info_fetch_fn func);
int mas_blk_device_read_section(
	struct block_device *bi_bdev, unsigned int *section_size);
void mas_blk_mq_tagset_read_section_register(
	struct blk_mq_tag_set *tag_set, lld_dev_read_section_fn func);
void mas_blk_mq_tagset_read_lrb_in_use_register(
	struct blk_mq_tag_set *tag_set, lld_dev_read_lrb_in_use_fn func);
void mas_blk_mq_tagset_read_op_size_register(
	struct blk_mq_tag_set *tag_set, lld_dev_read_op_size_fn func);
int mas_blk_device_config_mapping_partition(
	struct block_device *bi_bdev,
	struct stor_dev_mapping_partition *mapping_info);
void mas_blk_mq_tagset_config_mapping_partition_register(
	struct blk_mq_tag_set *tag_set, lld_dev_config_mapping_partition_fn func);
int mas_blk_device_read_mapping_partition(
	struct block_device *bi_bdev,
	struct stor_dev_mapping_partition *mapping_info);
int mas_blk_device_read_op_size(struct block_device *bi_bdev,
	int *op_size);
void mas_blk_mq_tagset_read_mapping_partition_register(
	struct blk_mq_tag_set *tag_set, lld_dev_read_mapping_partition_fn func);
int mas_blk_fs_sync_done(struct block_device *bi_bdev);
void mas_blk_mq_tagset_fs_sync_done_register(
	struct blk_mq_tag_set *tag_set, lld_dev_fs_sync_done_fn func);
int mas_blk_get_program_size(struct block_device *bi_bdev,
	struct stor_dev_program_size *program_size);
void mas_blk_mq_tagset_get_program_size_register(
	struct blk_mq_tag_set *tag_set, lld_dev_get_program_size_fn func);
int mas_blk_device_close_section(struct block_device *bi_bdev,
	struct stor_dev_reset_ftl *reset_ftl_info);
void mas_blk_mq_tagset_reset_ftl_register(
	struct blk_mq_tag_set *tag_set, lld_dev_reset_ftl_fn func);
#if defined(CONFIG_MAS_DEBUG_FS) || defined(CONFIG_MAS_BLK_DEBUG)
int mas_blk_bad_block_error_inject(struct block_device *bi_bdev,
	unsigned char bad_slc_cnt, unsigned char bad_tlc_cnt);
int mas_blk_rescue_block_inject_data(
	struct block_device *bi_bdev, sector_t sect);
void mas_blk_mq_tagset_rescue_block_inject_data_register(
	struct blk_mq_tag_set *tag_set, lld_dev_rescue_block_inject_data_fn func);
void mas_blk_mq_tagset_bad_block_error_inject_register(
	struct blk_mq_tag_set *tag_set, lld_dev_bad_block_error_inject_fn func);
int mas_blk_unistore_debug_en(void);
int mas_blk_recovery_debug_on(void);
void mas_blk_recovery_debug_off(void);
#endif
void mas_blk_bad_block_notify_register(struct block_device *bi_bdev,
	blk_dev_bad_block_notify_fn func, void* param_data);
void mas_blk_mq_tagset_bad_block_notify_register(
	struct blk_mq_tag_set *tag_set, lld_dev_bad_block_notify_register_fn func);
int mas_bkops_work_query(struct block_device *bdev);
int mas_bkops_work_start(struct block_device *bdev);
void mas_bkops_work_stop(struct block_device *bdev);
void mas_blk_req_get_order_nr_unistore(struct request *req,
	unsigned char new_stream_type, unsigned char *order,
	unsigned char *pre_order_cnt, bool extern_protect);
void mas_blk_mq_tagset_set_bkops(
	struct request_queue *q, struct mas_bkops *ufs_bkops);
void mas_blk_queue_unistore_enable(
	struct request_queue *q, bool enable);
void mas_blk_set_up_unistore_env(struct request_queue *q,
	unsigned int mas_sec_size, unsigned int mas_pu_size, bool enable);
bool blk_queue_query_unistore_enable(struct request_queue *q);
int mas_blk_mq_update_unistore_tags(struct blk_mq_tag_set *set);
void mas_blk_insert_section_list(struct block_device *bdev,
	unsigned int start_blkaddr, int stream_type, int flash_mode);
unsigned int mas_blk_get_sec_size(struct request_queue *q);
unsigned int mas_blk_get_pu_size(struct request_queue *q);
int mas_blk_update_buf_bio_page(struct block_device *bdev,
	struct page *page, struct page *cached_page);
void mas_blk_set_recovery_flag(struct request_queue *q);
int mas_blk_get_recovery_pages(bool page_anon);
void mas_blk_recovery_pages_add(struct page *page);
void mas_blk_recovery_pages_sub(struct page *page);
void mas_blk_clear_section_list(struct block_device *bdev,
	unsigned char stream_type);
#else
static inline bool blk_queue_query_unistore_enable(struct request_queue *q)
{
	return false;
}
#endif

#ifdef CONFIG_SCSI_UFS_INLINE_CRYPTO
void blk_queue_set_inline_crypto_flag(
	struct request_queue *q, bool enable);
void mas_blk_inline_crypto_init_request_from_bio(
	struct request *req, const struct bio *bio);
bool mas_blk_inline_crypto_bio_merge_allow(
	const struct request *rq, const struct bio *bio);
void mas_blk_inline_crypto_bio_split_pre(
	struct bio *bio, struct bio *split);
void mas_blk_inline_crypto_bio_split_post(struct bio *bio);
void mas_blk_inline_crypto_req_init(struct request *rq);
#endif

#ifdef CONFIG_HP_CORE
void blk_dev_health_query_register(
	struct request_queue *q, lld_query_health_fn func);
int blk_dev_health_query(struct block_device *bi_bdev,
	u8 *pre_eol_info, u8 *life_time_est_a, u8 *life_time_est_b);
#endif
#ifdef CONFIG_MAS_BLK
void blk_queue_dump_register(
	struct request_queue *q, lld_dump_status_fn func);
void blk_mq_tagset_dump_register(
	struct blk_mq_tag_set *tag_set, lld_dump_status_fn func);
void blk_mq_tagset_tz_query_register(
	struct blk_mq_tag_set *tag_set, lld_tz_query_fn func);
int blk_lld_tz_query(
	const struct block_device *bi_bdev, u32 type, u8 *buf, u32 buf_len);
void blk_mq_tagset_tz_ctrl_register(
	struct blk_mq_tag_set *tag_set, lld_tz_ctrl_fn func);
int blk_lld_tz_ctrl(
	const struct block_device *bi_bdev, int desc_id, uint8_t index);
void blk_mq_tagset_latency_warning_set(
	struct blk_mq_tag_set *tag_set, unsigned int warning_threshold_ms);
void blk_queue_latency_warning_set(
	struct request_queue *q, unsigned int warning_threshold_ms);
int blk_busyidle_event_subscribe(const struct block_device *bi_bdev,
	const struct blk_busyidle_event_node *event_node);
int blk_queue_busyidle_event_subscribe(
	const struct request_queue *q,
	const struct blk_busyidle_event_node *event_node);
int blk_lld_busyidle_event_subscribe(
	const struct blk_dev_lld *lld,
	struct blk_busyidle_event_node *event_node);
int blk_busyidle_event_unsubscribe(
	const struct blk_busyidle_event_node *event_node);
int blk_queue_busyidle_event_unsubscribe(
	const struct blk_busyidle_event_node *event_node);
void blk_queue_busyidle_enable(
	const struct request_queue *q, int enable);
void blk_mq_tagset_busyidle_enable(
	struct blk_mq_tag_set *tag_set, int enable);
void blk_mq_tagset_hw_idle_notify_enable(
	struct blk_mq_tag_set *tag_set, int enable);
void blk_queue_direct_flush_register(
	struct request_queue *q, blk_direct_flush_fn func);
void blk_mq_tagset_direct_flush_register(
	struct blk_mq_tag_set *tag_set, blk_direct_flush_fn func);
void blk_queue_flush_reduce_config(
	struct request_queue *q, bool flush_reduce_enable);
void blk_mq_tagset_flush_reduce_config(
	struct blk_mq_tag_set *tag_set, bool flush_reduce_enable);
void blk_flush_set_async(struct bio *bio);
int blk_flush_async_support(const struct block_device *bi_bdev);
void blk_mq_tagset_ufs_mq_iosched_enable(
	struct blk_mq_tag_set *tag_set, int enable);
void blk_mq_tagset_mmc_mq_iosched_enable(
	struct blk_mq_tag_set *tag_set, int enable);
bool ufs_order_panic_wait_datasync_handle(struct blk_dev_lld *blk_lld);
void ufs_order_panic_datasync_handle(struct blk_dev_lld *blk_lld);
void blk_power_off_flush(int emergency);
void mas_blk_panic_flush(void);
void blk_write_throttle(struct request_queue *queue, int level);
void blk_generic_freeze(
	const void *freeze_obj, enum blk_freeze_obj_type type, bool freeze);
unsigned char req_get_streamid(struct request *req);
void blk_lld_idle_notify(const struct blk_dev_lld *lld);
bool blk_dev_write_order_preserved(struct block_device *bdev);
unsigned int blk_req_get_order_nr(struct request *req, bool extern_protect);
void blk_queue_order_enable(struct request_queue *q, bool enable);
bool blk_queue_query_order_enable(struct request_queue *q);
void blk_order_nr_reset(struct blk_mq_tag_set *tag_set);
int blk_mq_get_io_in_list_count(struct block_device *bdev);
#ifdef CONFIG_MAS_MQ_USING_CP
void blk_queue_cp_enable(struct request_queue *q, bool enable);
#endif
#ifdef CONFIG_MAS_QOS_MQ
void blk_mq_tagset_ufs_qos_mq_iosched_enable(
	struct blk_mq_tag_set *tag_set, int enable);
static __always_inline unsigned int qos_ufs_mq_get_send_cpu(struct request *rq)
{
	return rq->mas_req.slot_cpu;
}
#endif /* CONFIG_MAS_QOS_MQ */

void blk_dio_ck(struct gendisk *target_disk,
	ktime_t dio_start, int dio_op, int dio_page_count);
void blk_mq_tagset_vl_setup(
	struct blk_mq_tag_set *tag_set, u64 device_capacity);

#ifdef CONFIG_MMC_MQ_CQ_HCI
extern void mmc_mq_requeue_invalidate_reqs(struct request_queue *q);
#endif

#endif /* CONFIG_MAS_BLK */
#else /* CONFIG_BLOCK */

struct block_device;

/*
 * stubs for when the block layer is configured out
 */
#define buffer_heads_over_limit 0

static inline long nr_blockdev_pages(void)
{
	return 0;
}

struct blk_plug {
};

static inline void blk_start_plug(struct blk_plug *plug)
{
}

static inline void blk_finish_plug(struct blk_plug *plug)
{
}

static inline void blk_flush_plug(struct task_struct *task)
{
}

static inline void blk_schedule_flush_plug(struct task_struct *task)
{
}


static inline bool blk_needs_flush_plug(struct task_struct *tsk)
{
	return false;
}

static inline int blkdev_issue_flush(struct block_device *bdev, gfp_t gfp_mask,
				     sector_t *error_sector)
{
	return 0;
}

#endif /* CONFIG_BLOCK */

#endif
