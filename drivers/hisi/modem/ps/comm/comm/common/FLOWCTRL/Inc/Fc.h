/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and 
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may 
 * *    be used to endorse or promote products derived from this software 
 * *    without specific prior written permission.
 * 
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef __FC_H__
#define __FC_H__


/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "vos.h"
#include "PsLib.h"
#include "FcInterface.h"
#include "FcIntraMsg.h"
#if (OSA_CPU_ACPU == VOS_OSA_CPU)
#include "acore_nv_stru_gucttf.h"
#else
#include "ccore_nv_stru_gucttf.h"
#endif
#include "TTFComm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(4)

/*****************************************************************************
  2 宏定义
*****************************************************************************/
#if (OSA_CPU_CCPU == VOS_OSA_CPU)
#define FC_MAX_POINT_NUM                (22)    /* C核CPU和温保会初始化最多2*FC_UL_RATE_MAX_LEV个 */
#define UEPS_FID_FLOWCTRL               (UEPS_FID_FLOWCTRL_C)
#define UEPS_PID_FLOWCTRL               (UEPS_PID_FLOWCTRL_C)

typedef FC_CCORE_CFG_CPU_STRU FC_CFG_CPU_STRU;
typedef FC_CCORE_CFG_MEM_THRESHOLD_STRU FC_CFG_MEM_THRESHOLD_STRU;
#else
#define FC_MAX_POINT_NUM                (10)    /* A核暂定值 */
#define UEPS_FID_FLOWCTRL               (UEPS_FID_FLOWCTRL_A)
#define UEPS_PID_FLOWCTRL               (UEPS_PID_FLOWCTRL_A)

typedef FC_ACORE_CFG_CPU_STRU FC_CFG_CPU_STRU;
typedef FC_ACORE_CFG_MEM_THRESHOLD_STRU FC_CFG_MEM_THRESHOLD_STRU;
#endif

#define FC_MEM_THRESHOLD_MAX_NUM        (4)

#define FC_PRI_LOWEST                   (FC_PRI_1)
#define FC_PRI_HIGHEST                  (FC_PRI_9)

#define FC_MAX_NUM                      (10)
#define FC_RAB_MASK_ALL                 (0x0000FFFE)                            /* RAB范围 1~15 */

#define FC_MAX_BRIDGE_BYTE_CNT          (0x80000)  /* 2^32 >> 10 >> 3 */

#define FC_GET_MSG_NAME16(pRcvMsg)      (*((VOS_UINT16 *)((VOS_UINT8 *)(pRcvMsg) + VOS_MSG_HEAD_LENGTH)))
#define FC_GET_MSG_NAME32(pRcvMsg)      (*((VOS_UINT32 *)((VOS_UINT8 *)(pRcvMsg) + VOS_MSG_HEAD_LENGTH)))

#define FC_POLICY_MASK(enPolicyId)      (((VOS_UINT32)1) << (enPolicyId))

#define FC_POLICY_GetEnableMask(enPolicyId) ((FC_POLICY_MASK(enPolicyId) & g_ulFcEnableMask))
#define FC_POLICY_Get( enPolicyId )      (&g_astFcPolicy[enPolicyId])

#define FC_DOWN_RATE_LIMIT_MASK         (0xFFFF)


#define FC_UE_MIN_RAB_ID                (5)
#define FC_UE_MAX_RAB_ID                (15)


#define FC_ACPU_DRV_ASSEM_UE_ON_MASK    (1)
#define FC_ACPU_DRV_ASSEM_PC_ON_MASK    (1 << 1)
#define FC_ACPU_CDS_GU_ASSEM_ON_MASK    (1 << 2)


#define CPU_MAX_SET_DRV_FAIL_SMOOTH_CNT (50)                   /* 配置驱动失败平滑系数 */
#define CPU_DRV_ASSEM_PARA_MAX_NV_LEV   (4)


#define FC_ADS_DL_RATE_UP_LEV           (200)                 /* 200 * 20 * 1500 * 8 */
#define FC_ADS_DL_RATE_DOWN_LEV         (20)
#define FC_ADS_TIMER_LEN                (50)
#define FC_MODEM_ID_NUM                 (2)

#define FC_CCPU_TRACE_CPULOAD_TIMELEN   (1000)              /* CCPU负载上报周期，1s */
#define TIMER_FC_CCPU_TRACE_CPULOAD     (0x1001)

#define FC_CCPU_PTR_OCTET_OCCUPIED      (1)       /* CCPU上指针占用1个U32 */
#define FC_ACPU_PTR_OCTET_OCCUPIED      (2)       /* ACPU上指针占用1个U32 */
#define FC_PTR_MAX_OCTET_OCCUPIED       (2)

#define FC_LOG(ulLevel, pcString)                   \
    TTF_LOG(UEPS_PID_FLOWCTRL, DIAG_MODE_COMM, ulLevel, pcString)
#define FC_LOG1(ulLevel, pcString, lPara1)          \
    TTF_LOG1(UEPS_PID_FLOWCTRL, DIAG_MODE_COMM, ulLevel, pcString, lPara1)
#define FC_LOG2(ulLevel, pcString, lPara1, lPara12)     \
    TTF_LOG2(UEPS_PID_FLOWCTRL, DIAG_MODE_COMM, ulLevel, pcString, lPara1, lPara12)
#define FC_LOG3(ulLevel, pcString, lPara1, lPara2, lPara3)     \
    TTF_LOG3(UEPS_PID_FLOWCTRL, DIAG_MODE_COMM, ulLevel, pcString, lPara1, lPara2, lPara3)
#define FC_LOG4(ulLevel, pcString, lPara1, lPara2, lPara3, lPara4)      \
    TTF_LOG4(UEPS_PID_FLOWCTRL, DIAG_MODE_COMM, ulLevel, pcString, lPara1, lPara2, lPara3, lPara4)




/*****************************************************************************
  3 枚举定义
*****************************************************************************/
enum FC_PRI_OPER_ENUM
{
    FC_PRI_CHANGE_AND_CONTINUE          = 0,                    /* 1.Lev改变，继续下一级流控 */
    FC_PRI_CHANGE_AND_BREAK,                                    /* 2.Lev改变，退出当前操作 */
    FC_PRI_KEEP_AND_BREAK,                                      /* 3.维持Lev不变，退出当前操作 */
    FC_PRI_OPER_BUTT
};
typedef VOS_UINT32 FC_PRI_OPER_ENUM_UINT32;


enum FC_MNTN_EVENT_TYPE_ENUM
{
    ID_FC_MNTN_POINT_SET_FC             = 0x8001,                   /* _H2ASN_MsgChoice FC_MNTN_POINT_FC_STRU */
    ID_FC_MNTN_POINT_CLR_FC             = 0x8002,                   /* _H2ASN_MsgChoice FC_MNTN_POINT_FC_STRU */
    ID_FC_MNTN_POLICY_UP                = 0x8003,                   /* _H2ASN_MsgChoice FC_MNTN_POLICY_STRU */
    ID_FC_MNTN_POLICY_DOWN              = 0x8004,                   /* _H2ASN_MsgChoice FC_MNTN_POLICY_STRU */
    ID_FC_MNTN_POLICY_ADD_POINT         = 0x8005,                   /* _H2ASN_MsgChoice FC_MNTN_POLICY_STRU */
    ID_FC_MNTN_POLICY_DEL_POINT         = 0x8006,                   /* _H2ASN_MsgChoice FC_MNTN_POLICY_STRU */
    ID_FC_MNTN_POLICY_CHANGE_POINT      = 0x8007,                   /* _H2ASN_MsgChoice FC_MNTN_POLICY_STRU */
    ID_FC_MNTN_CPU_A_CPULOAD            = 0x8008,                   /* _H2ASN_MsgChoice FC_MNTN_CPULOAD_STRU */
    ID_FC_MNTN_CPU_C_CPULOAD            = 0x8009,                   /* _H2ASN_MsgChoice FC_MNTN_CPULOAD_STRU */
    ID_FC_MNTN_DRV_ASSEM_PARA           = 0x800A,                   /* _H2ASN_MsgChoice FC_MNTN_DRV_ASSEM_PARA_STRU */
    ID_FC_MNTN_ACORE_CRESET_START_FC    = 0x800B,                   /* _H2ASN_MsgChoice FC_MNTN_POINT_FC_STRU */
    ID_FC_MNTN_ACORE_CRESET_END_FC      = 0x800C,                   /* _H2ASN_MsgChoice FC_MNTN_POINT_FC_STRU */

    ID_FC_MNTN_EVENT_TYPE_BUTT          = 0xFFFF
};
typedef VOS_UINT16 FC_MNTN_EVENT_TYPE_ENUM_UINT16;


enum FC_TIMER_NAME_ENUM
{
    FC_TIMER_STOP_CPU_ATTEMPT   = 0,
    FC_TIMER_NAME_BUTT
} ;
typedef VOS_UINT32 FC_TIMER_NAME_ENUM_UINT32;


/*====================================*//* FC内部流控策略ID */
enum FC_PRIVATE_POLICY_ID_ENUM
{
    FC_PRIVATE_POLICY_ID_MEM_MODEM_0                    = 0,
    FC_PRIVATE_POLICY_ID_CPU_A_MODEM_0,
    FC_PRIVATE_POLICY_ID_CDS_MODEM_0,
    FC_PRIVATE_POLICY_ID_CST_MODEM_0,
    FC_PRIVATE_POLICY_ID_GPRS_MODEM_0,
    FC_PRIVATE_POLICY_ID_TMP_MODEM_0,
    FC_PRIVATE_POLICY_ID_CPU_C_MODEM_0,
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
    FC_PRIVATE_POLICY_ID_CDMA_MODEM_0,
#endif    
    FC_PRIVATE_POLICY_ID_MEM_MODEM_1,
    FC_PRIVATE_POLICY_ID_CPU_A_MODEM_1,
    FC_PRIVATE_POLICY_ID_CDS_MODEM_1,
    FC_PRIVATE_POLICY_ID_CST_MODEM_1,
    FC_PRIVATE_POLICY_ID_GPRS_MODEM_1,
    FC_PRIVATE_POLICY_ID_TMP_MODEM_1,
    FC_PRIVATE_POLICY_ID_CPU_C_MODEM_1,
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)    
    FC_PRIVATE_POLICY_ID_CDMA_MODEM_1,
#endif    
    FC_PRIVATE_POLICY_ID_BUTT
};
typedef VOS_UINT8 FC_PRIVATE_POLICY_ID_ENUM_UINT8;

/*****************************************************************************
  5 消息头定义
*****************************************************************************/


/*****************************************************************************
  6 消息定义
*****************************************************************************/


/*****************************************************************************
  7 STRUCT定义
*****************************************************************************/
/*****************************************************************************
结构名    : FC_POLICY_STRU
协议表格  : 无
ASN.1描述 : 无
结构说明  : 流控优先级结构
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucValid;                   /*PS_TRUE:有效, PS_FALSE:无效 */
    VOS_UINT8                           ucFcIdCnt;                 /*当前优先级对应的流控Id个数 */
    VOS_UINT8                           ucFcIdIndex;
    FC_PRI_ENUM_UINT8                   enPri;                     /*当前优先级*/
    FC_ID_ENUM_UINT8                    aenFcId[FC_MAX_POINT_NUM]; /*当前优先级对应的流控Id */
    VOS_UINT8                           aucRsv[6];
} FC_PRI_STRU;


/*****************************************************************************
 结构名    : FC_POLICY_STRU
 协议表格  : 无
 ASN.1描述 : 无
 结构说明  : 流控通用策略结构，进行
*****************************************************************************/
typedef VOS_UINT32 (*FC_POLICY_POSTPROCESS)(VOS_UINT32 ulParam1, VOS_UINT32 ulParam2);
typedef VOS_UINT32 (*FC_POLICY_ADJUST_FOR_UP_FUNC)(FC_PRI_ENUM_UINT8 enPointPri, FC_ID_ENUM_UINT8 enFcId);
typedef VOS_UINT32 (*FC_POLICY_ADJUST_FOR_DOWN_FUNC)(FC_PRI_ENUM_UINT8 enPointPri, FC_ID_ENUM_UINT8 enFcId);


typedef struct
{
    FC_PRIVATE_POLICY_ID_ENUM_UINT8     enPolicyId;             /* 策略ID */
    VOS_UINT8                           ucPriCnt;               /* 当前策略的优先级总数 */
    FC_PRI_ENUM_UINT8                   enHighestPri;           /* 当前流控策略的最高优先级别 */
    FC_PRI_ENUM_UINT8                   enDonePri;              /* 当前处理过的优先级 */
    FC_PRI_ENUM_UINT8                   enToPri;
    VOS_UINT8                           aucRsv[3];
    FC_PRI_STRU                         astFcPri[FC_PRI_BUTT];  /* 某一个优先级流控对应的处理 */
    FC_POLICY_ADJUST_FOR_UP_FUNC        pAdjustForUpFunc;       /* _H2ASN_Replace VOS_UINT32  pAdjustForUpFunc; */
    FC_POLICY_ADJUST_FOR_DOWN_FUNC      pAdjustForDownFunc;     /* _H2ASN_Replace VOS_UINT32  pAdjustForDownFunc; */
    FC_POLICY_POSTPROCESS               pPostFunc;              /* 该流控策略执行到最后一级的回调函数 *//* _H2ASN_Replace VOS_UINT32  pPostFunc; */
} FC_POLICY_STRU;

typedef struct
{
    FC_ID_ENUM_UINT8                    enFcId;                 /* 流控ID */
    VOS_UINT8                           aucRsv[1];
    MODEM_ID_ENUM_UINT16                enModemId;              /* ModemId */   /* _H2ASN_Replace VOS_UINT16  enModemId; */
    VOS_UINT32                          ulPolicyMask;           /* 该流控点关联的流控策略，便于查找核删除 */
    VOS_UINT32                          ulFcMask;               /* 流控投票管理 */
    VOS_UINT32                          ulParam1;               /* 该流控点注册时，记录的参数值 */
    VOS_UINT32                          ulParam2;               /* 该流控点注册时，记录的参数值 */
    FC_SET_FUNC                         pSetFunc;               /* 流控执行函数 */  /* _H2ASN_Replace VOS_UINT32  pSetFunc; */
    FC_CLR_FUNC                         pClrFunc;               /* 流控解除函数 */  /* _H2ASN_Replace VOS_UINT32  pClrFunc; */
    FC_RST_FUNC                         pRstFunc;               /* 新增的复位处理与恢复接口 */  /* _H2ASN_Replace VOS_UINT32  pRstFunc; */
} FC_POINT_STRU;


/*****************************************************************************
 结构名    : FC_POINT_MGR_STRU
 协议表格  : 无
 ASN.1描述 : 无
 结构说明  : 流控点管理结构
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulPointNum;
    VOS_UINT8                           aucRsv[4];
    FC_POINT_STRU                       astFcPoint[FC_MAX_POINT_NUM];
} FC_POINT_MGR_STRU;


typedef struct
{
    VOS_UINT32                          ulLastByteCnt;                          /* 上次统计时的值 */
    VOS_UINT32                          ulRate;                                 /* 网桥速率，单位:bps */
} FC_BRIDGE_RATE_STRU;


/*====================================*//*一个FC上FC 和RAB之间的映射关系*/
typedef struct
{
    FC_ID_ENUM_UINT8                    enFcId;
    VOS_UINT8                           aucRsv[7];
    VOS_UINT32                          ulIncludeRabMask;                       /* 该Fc Id对应的所有RAB，用掩码表示 */
    VOS_UINT32                          ulNoFcRabMask;                          /* 该Fc Id上不要求流控的RAB，没有RAB流控时等于ulIncludeRabMask */
} FC_RAB_MAPPING_INFO_STRU;


typedef struct
{
    FC_ID_ENUM_UINT8                    enFcIdCnt;                              /* FC Id流控实体个数 */
    VOS_UINT8                           aucRsv[3];
    FC_RAB_MAPPING_INFO_STRU            astFcRabMappingInfo[FC_MAX_NUM];        /* FC 和RAB之间的映射关系集合 */
} FC_RAB_MAPPING_INFO_SET_STRU;


/*====================================*//* 可维可测信息 */
/* 流控点钩包结构 */
typedef struct
{
    FC_ID_ENUM_UINT8                    enFcId;                 /* 流控ID */
    VOS_UINT8                           aucRsv[5];
    MODEM_ID_ENUM_UINT16                enModemId;              /* ModemId */   /* _H2ASN_Replace VOS_UINT16  enModemId; */
    VOS_UINT32                          ulPolicyMask;           /* 该流控点关联的流控策略，便于查找核删除 */
    VOS_UINT32                          ulFcMask;               /* 流控投票管理 */
    VOS_UINT32                          ulParam1;               /* 该流控点注册时，记录的参数值 */
    VOS_UINT32                          ulParam2;               /* 该流控点注册时，记录的参数值 */
    VOS_UINT32                          aulPointSetAddr[FC_PTR_MAX_OCTET_OCCUPIED];    /* 记录流控执行函数的地址 */
    VOS_UINT32                          aulPointClrAddr[FC_PTR_MAX_OCTET_OCCUPIED];    /* 记录流控清除函数的地址 */
    VOS_UINT32                          aulPointRstAddr[FC_PTR_MAX_OCTET_OCCUPIED];    /* 记录流控复位处理与恢复函数的地址 */
} FC_MNTN_POINT_INFO_STRU;

/* 流控点钩包TRACE结构 */
typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    FC_MNTN_EVENT_TYPE_ENUM_UINT16      enMsgName;                              /* _H2ASN_Skip */
    VOS_UINT8                           aucRsv[2];
    FC_MNTN_POINT_INFO_STRU             stFcPoint;
    VOS_UINT32                          ulIsFuncInvoked;
    VOS_UINT32                          ulResult;
} FC_MNTN_POINT_FC_STRU;

/* 流控策略钩包结构 */
typedef struct
{
    FC_PRIVATE_POLICY_ID_ENUM_UINT8     enPolicyId;             /* 策略ID */
    VOS_UINT8                           ucPriCnt;               /* 当前策略的优先级总数 */
    FC_PRI_ENUM_UINT8                   enHighestPri;           /* 当前流控策略的最高优先级别 */
    FC_PRI_ENUM_UINT8                   enDonePri;              /* 当前处理过的优先级 */
    FC_PRI_ENUM_UINT8                   enToPri;
    VOS_UINT8                           aucRsv[3];
    FC_PRI_STRU                         astFcPri[FC_PRI_BUTT];  /* 某一个优先级流控对应的处理 */
    VOS_UINT32                          aulPolicyUpAddr[FC_PTR_MAX_OCTET_OCCUPIED];    /* 记录流控策略升执行函数的地址 */
    VOS_UINT32                          aulPolicyDownAddr[FC_PTR_MAX_OCTET_OCCUPIED];    /* 记录流控策略降执行函数的地址 */
    VOS_UINT32                          aulPolicyPostAddr[FC_PTR_MAX_OCTET_OCCUPIED];    /* 流控策略执行到最后一级的回调函数的地址 */
} FC_MNTN_POLICY_INFO_STRU;

/* 流控策略钩包TRACE结构 */
typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    FC_MNTN_EVENT_TYPE_ENUM_UINT16      enMsgName;                              /* _H2ASN_Skip */
    VOS_UINT8                           aucRsv[2];
    FC_MNTN_POLICY_INFO_STRU            stPolicy;
} FC_MNTN_POLICY_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    FC_MNTN_EVENT_TYPE_ENUM_UINT16      enMsgName;                              /* _H2ASN_Skip */
    VOS_UINT8                           aucRsv[2];
    VOS_UINT32                          ulCpuLoad;
}FC_MNTN_CPULOAD_STRU;

typedef struct
{
    VOS_UINT32                          ulSmoothTimerLen;       /* CPU流控的平滑次数，连续若干次CPU超标，才进行CPU流控 */
    VOS_UINT8                           aucRsv[4];
    HTIMER                              pstStopAttemptTHandle;  /* CPU流控解除优化定时器 */
} FC_CPU_CTRL_STRU;

typedef struct
{
    const VOS_UINT32                    ulTraceCpuLoadTimerLen; /* 上报CCPU负载定时器时长 */ /* _H2ASN_Skip */
    VOS_UINT32                          ulCpuLoadRecord;        /* 记录底软上报的CPU负载 */
    HTIMER                              pstTraceCpuLoadTHandle; /* 上报CCPU负载定时器 */
} FC_TRACE_CPULOAD_STRU;

/*****************************************************************************
  4 全局变量声明
*****************************************************************************/
extern FC_POLICY_STRU                          g_astFcPolicy[]; /* 流控策略实体 */
extern FC_POINT_MGR_STRU                       g_stFcPointMgr;  /* 流控点管理实体 */
extern VOS_UINT32                              g_ulFcEnableMask;
/* 流控内部策略表 */
extern FC_PRIVATE_POLICY_ID_ENUM_UINT8         g_aenPrivatePolicyTbl[FC_MODEM_ID_NUM][FC_POLICY_ID_BUTT];

/*****************************************************************************
  5 OTHERS定义
*****************************************************************************/
/*****************************************************************************
  H2ASN顶级消息结构定义
*****************************************************************************/
typedef struct
{
    FC_MNTN_EVENT_TYPE_ENUM_UINT16      enMsgID;    /*_H2ASN_MsgChoice_Export FC_MNTN_EVENT_TYPE_ENUM_UINT16*/

    VOS_UINT8                           aucMsgBlock[2];
    /***************************************************************************
        _H2ASN_MsgChoice_When_Comment          FC_MNTN_EVENT_TYPE_ENUM_UINT16
    ****************************************************************************/
}FC_MSG_DATA;
/*_H2ASN_Length UINT32*/

typedef struct
{
    VOS_MSG_HEADER
    FC_MSG_DATA                   stMsgData;
}Fc_MSG;


/*****************************************************************************
  8 UNION定义
*****************************************************************************/


/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/



/*****************************************************************************
  10 函数声明
*****************************************************************************/

VOS_VOID  FC_SetFcEnableMask( VOS_UINT32 ulEnableMask );

extern VOS_UINT32 FC_SndTemperatureMsg
(
    FC_MSG_TYPE_ENUM_UINT16 usMsgName
);

VOS_UINT32  FC_CFG_CheckCpuParam( FC_CFG_CPU_STRU *pstFcCfgCpu );

extern VOS_UINT32 FC_SndCpuMsg
(
    FC_MSG_TYPE_ENUM_UINT16 usMsgName,
    VOS_UINT32              ulCpuLoad,
    VOS_UINT32              ulUlRate,
    VOS_UINT32              ulDlRate
);

extern VOS_UINT32 FC_POLICY_Up
(
    FC_POLICY_STRU *pPolicy
);

extern VOS_UINT32 FC_POLICY_Down
(
    FC_POLICY_STRU *pFcPolicy
);

extern VOS_UINT32  FC_POLICY_UpToTargetPri
(
    FC_POLICY_STRU                     *pFcPolicy,
    FC_PRI_ENUM_UINT8                   enTargetPri
);

extern VOS_UINT32  FC_POLICY_DownToTargetPri
(
    FC_POLICY_STRU                     *pFcPolicy,
    FC_PRI_ENUM_UINT8                   enTargetPri
);

extern FC_PRI_OPER_ENUM_UINT32 FC_POINT_SetFc
(
    VOS_UINT32                          ulPolicyMask,
    FC_ID_ENUM_UINT8                    enFcId
);

extern FC_PRI_OPER_ENUM_UINT32 FC_POINT_ClrFc
(
    VOS_UINT32                          ulPolicyMask,
    FC_ID_ENUM_UINT8                    enFcId
);

extern VOS_UINT32  FC_POINT_Reg
(
    FC_REG_POINT_STRU                  *pstFcRegPoint
);

extern VOS_UINT32  FC_POINT_DeReg(FC_ID_ENUM_UINT8 enFcId, MODEM_ID_ENUM_UINT16  enModemId);

extern VOS_UINT32  FC_POINT_Change
(
    FC_ID_ENUM_UINT8                    enFcId,
    FC_POLICY_ID_ENUM_UINT8             enPolicyId,
    FC_PRI_ENUM_UINT8                   enNewPri,
    MODEM_ID_ENUM_UINT16                enModemId
);



extern VOS_UINT32  FC_CommInit( VOS_VOID );

VOS_VOID FC_MNTN_TraceEvent(VOS_VOID *pMsg);
extern VOS_UINT32  FC_MNTN_TraceCpuLoad(FC_MNTN_EVENT_TYPE_ENUM_UINT16 enMsgName, VOS_UINT32  ulCpuLoad );
extern VOS_VOID FC_MNTN_TracePointFcEvent
(
    FC_MNTN_EVENT_TYPE_ENUM_UINT16      enMsgName,
    FC_POINT_STRU                      *pstFcPoint,
    VOS_UINT32                          ulIsFuncInvoked,
    VOS_UINT32                          ulResult
);
extern VOS_UINT32 FC_CPUA_UpJudge
(
    VOS_UINT32       ulCpuLoad,
    FC_CFG_CPU_STRU *pstFcCfgCpu,
    FC_POLICY_STRU  *pstFcPolicy
);

extern VOS_VOID FC_CPUA_RcvCpuLoad(VOS_UINT32  ulCpuLoad);
extern FC_PRI_ENUM_UINT8  FC_MEM_CalcUpTargetFcPri
(
    FC_POLICY_STRU                     *pPolicy,
    VOS_UINT32                          ulMemValue
);
extern FC_PRI_ENUM_UINT8  FC_MEM_CalcDownTargetFcPri
(
    FC_POLICY_STRU                     *pPolicy,
    VOS_UINT32                          ulMemValue
);

extern VOS_VOID    FC_MEM_UpProcess( VOS_UINT32 ulMemValue  );
extern VOS_VOID    FC_MEM_DownProcess( VOS_UINT32 ulMemValue );

extern VOS_UINT32  FC_CDS_DelFcId( FC_ID_ENUM_UINT8 enFcId, MODEM_ID_ENUM_UINT16 enModemId );
extern VOS_UINT32  FC_CDS_AddRab(FC_ID_ENUM_UINT8 enFcId, VOS_UINT8 ucRabId, MODEM_ID_ENUM_UINT16 enModemId );
extern VOS_UINT32  FC_CDS_DelRab( VOS_UINT8 ucRabId, MODEM_ID_ENUM_UINT16 enModemId );
extern VOS_UINT32  FC_CDS_UpProcess( VOS_UINT8 ucRabId, MODEM_ID_ENUM_UINT16 enModemId );
extern VOS_UINT32  FC_CDS_DownProcess( VOS_UINT8 ucRabId, MODEM_ID_ENUM_UINT16 enModemId );
extern VOS_UINT32  FC_ACORE_RcvIntraMsg( MsgBlock * pMsg );
extern VOS_VOID FC_ACORE_MsgProc(MsgBlock * pMsg);
extern VOS_UINT32  FC_ACORE_Init( VOS_VOID );

#pragma pack()


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of Fc.h */

