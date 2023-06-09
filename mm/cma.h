/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __MM_CMA_H__
#define __MM_CMA_H__

struct cma {
	unsigned long   base_pfn;
	unsigned long   count;
	unsigned long   *bitmap;
	unsigned int order_per_bit; /* Order of pages represented by one bit */
	struct mutex    lock;
#ifdef CONFIG_CMA_DEBUGFS
	struct hlist_head mem_head;
	spinlock_t mem_head_lock;
#endif
	const char *name;
#ifdef CONFIG_HISI_CMA_DEBUG
	unsigned int flag;
#endif
};

#define HISI_CMA_ALLOC_RETRY_TIMES_MAX 3

extern struct cma cma_areas[MAX_CMA_AREAS];
extern unsigned cma_area_count;

static inline unsigned long cma_bitmap_maxno(struct cma *cma)
{
	return cma->count >> cma->order_per_bit;
}

#ifndef CONFIG_OPTIMIZE_MM_AQ
#ifdef CONFIG_HISI_CMA_DEBUG
struct mutex *get_cma_mutex(void);
#endif
#endif

#endif
