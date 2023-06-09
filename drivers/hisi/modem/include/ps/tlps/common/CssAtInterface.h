/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
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

#ifndef  CSS_AT_INTERFACE_H
#define  CSS_AT_INTERFACE_H

/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include "vos.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 宏定义
*****************************************************************************/
#define MCC_INFO_VERSION_LEN                 (9)
#define AT_CSS_MAX_MCC_ID_NUM                (17)
/* Added for BlackCell Feature 2017-4-22 begin */
#define AT_CSS_BLACK_CELL_LIST_VERSION_LEN   (9)
/* Added for BlackCell Feature 2017-4-22 end */
/* add for CloudLine Feature 2017-6-28 begin */
#define AT_CSS_CLOUD_LINE_VERSION_LEN        (9)                 /* 云端预置高铁线路版本号字符串长度 */
#define AT_CSS_PLMN_MAX_LINE_NUM             (64)                /* 云端预置高铁线路一个PLMN下包含的最大线路个数 */
#define AT_CSS_TACLAC_MAX_LINE_NUM           (10)                /* 云端预置高铁线路一个TAC/LAC所对应的最大线路个数 */
/* add for CloudLine Feature 2017-6-28 end */

/* Added for EMC, 2018-1-9, begin */
#define AT_CSS_MRU_MAX_NUM                   (10)                /* MRU可以存储的最大个数 */
/* Added for EMC, 2018-1-9, end */

/*****************************************************************************
  3 枚举定义
*****************************************************************************/

enum CSS_AT_MSG_TYPE_ENUM
{
    /* AT->CSS */
    ID_AT_CSS_MCC_INFO_SET_REQ                = 0x0001,                           /* _H2ASN_MsgChoice AT_CSS_MCC_INFO_SET_REQ_STRU */
    ID_AT_CSS_MCC_VERSION_INFO_REQ            = 0x0002,                           /* _H2ASN_MsgChoice AT_CSS_MCC_VERSION_INFO_REQ_STRU */
    ID_AT_CSS_BLACK_CELL_LIST_SET_REQ         = 0x0003,                           /* _H2ASN_MsgChoice AT_CSS_BLACK_CELL_LIST_SET_REQ_STRU */
    ID_AT_CSS_BLACK_CELL_LIST_QUERY_REQ       = 0x0004,                           /* _H2ASN_MsgChoice AT_CSS_BLACK_CELL_LIST_QUERY_REQ_STRU */
    ID_AT_CSS_LINE_INDEX_LIST_SET_REQ         = 0x0005,                           /* _H2ASN_MsgChoice AT_CSS_LINE_INDEX_LIST_SET_REQ_STRU */
    ID_AT_CSS_LINE_DETAIL_SET_REQ             = 0x0006,                           /* _H2ASN_MsgChoice AT_CSS_LINE_DETAIL_SET_REQ_STRU */
    ID_AT_CSS_LINE_INDEX_LIST_QUERY_REQ       = 0x0007,                           /* _H2ASN_MsgChoice AT_CSS_LINE_INDEX_LIST_QUERY_REQ_STRU */
    /* Added for EMC, 2018-1-9, begin */
    ID_AT_CSS_VZWMRUC_SET_REQ                 = 0x0008,                           /* _H2ASN_MsgChoice AT_CSS_VZWMRUC_SET_REQ_STRU */
    ID_AT_CSS_VZWMRUE_SET_REQ                 = 0x0009,                           /* _H2ASN_MsgChoice AT_CSS_VZWMRUE_SET_REQ_STRU */
    ID_AT_CSS_VZWMRUE_QUERY_REQ               = 0x000a,                           /* _H2ASN_MsgChoice AT_CSS_VZWMRUE_QUERY_REQ_STRU */
    /* Added for EMC, 2018-1-9, end */

    /* CSS->AT */
    ID_CSS_AT_MCC_INFO_SET_CNF                = 0x1001,                           /* _H2ASN_MsgChoice CSS_AT_MCC_INFO_SET_CNF_STRU */
    ID_CSS_AT_MCC_VERSION_INFO_CNF            = 0x1002,                           /* _H2ASN_MsgChoice CSS_AT_MCC_VERSION_INFO_CNF_STRU */
    ID_CSS_AT_QUERY_MCC_INFO_NOTIFY           = 0x1003,                           /* _H2ASN_MsgChoice CSS_AT_QUERY_MCC_INFO_NOTIFY_STRU */
    ID_CSS_AT_BLACK_CELL_LIST_SET_CNF         = 0x1004,                           /* _H2ASN_MsgChoice CSS_AT_BLACK_CELL_LIST_SET_CNF_STRU */
    ID_CSS_AT_BLACK_CELL_LIST_QUERY_CNF       = 0x1005,                           /* _H2ASN_MsgChoice CSS_AT_BLACK_CELL_LIST_QUERY_CNF_STRU */
    ID_CSS_AT_BLACK_CELL_MCC_NOTIFY           = 0x1006,                           /* _H2ASN_MsgChoice CSS_AT_BLACK_CELL_MCC_NOTIFY_STRU */
    ID_CSS_AT_LINE_INDEX_LIST_SET_CNF         = 0x1007,                           /* _H2ASN_MsgChoice CSS_AT_LINE_INDEX_LIST_SET_CNF_STRU */
    ID_CSS_AT_LINE_DETAIL_SET_CNF             = 0x1008,                           /* _H2ASN_MsgChoice CSS_AT_LINE_DETAIL_SET_CNF_STRU */
    ID_CSS_AT_LINE_INDEX_LIST_QUERY_CNF       = 0x1009,                           /* _H2ASN_MsgChoice CSS_AT_LINE_INDEX_LIST_QUERY_CNF_STRU */
    ID_CSS_AT_LINE_PLMN_NOTIFY                = 0x100a,                           /* _H2ASN_MsgChoice CSS_AT_LINE_PLMN_NOTIFY_STRU */
    ID_CSS_AT_LINE_INDEX_NOTIFY               = 0x100b,                           /* _H2ASN_MsgChoice CSS_AT_LINE_INDEX_NOTIFY_STRU */
    /* Added for VZW EMC, 2018-1-9, begin */
    ID_CSS_AT_VZWMRUC_SET_CNF                 = 0x100c,                           /* _H2ASN_MsgChoice CSS_AT_VZWMRUC_SET_CNF_STRU */
    ID_CSS_AT_VZWMRUE_SET_CNF                 = 0x100d,                           /* _H2ASN_MsgChoice CSS_AT_VZWMRUE_SET_CNF_STRU */
    ID_CSS_AT_VZWMRUE_QUERY_CNF               = 0x100e,                           /* _H2ASN_MsgChoice CSS_AT_VZWMRUE_QUERY_CNF_STRU */
    /* Added for VZW EMC, 2018-1-9, end */

    ID_CSS_AT_MSG_BUTT
};
typedef  VOS_UINT32  CSS_AT_MSG_TYPE_ENUM_UINT32;


enum AT_CSS_RAT_ENUM
{
    AT_CSS_RAT_TYPE_GSM = 0,                           /* GSM接入技术 */
    AT_CSS_RAT_TYPE_WCDMA,                             /* WCDMA接入技术 */
    AT_CSS_RAT_TYPE_LTE,                               /* LTE接入技术 */

    AT_CSS_RAT_TYPE_BUTT
};
typedef  VOS_UINT8  AT_CSS_RAT_ENUM_UINT8;

enum AT_CSS_SET_MCC_OPERATE_TYPE_ENUM
{
    AT_CSS_SET_MCC_TYPE_ADD_MCC = 0,                   /* 新增MCC */
    AT_CSS_SET_MCC_TYPE_DELETE_ALL_MCC,                /* 删除所有MCC信息 */
    AT_CSS_SET_MCC_TYPE_DELETE_ONE_MCC,                /* 删除一个MCC信息 */

    AT_CSS_SET_MCC_TYPE_BUTT
};
typedef  VOS_UINT8  AT_CSS_SET_MCC_OPERATE_TYPE_ENUM_UINT8;

/* Added for BlackCell Feature 2017-4-22 begin */

enum AT_CSS_BLACK_CELL_LIST_OPERATE_TYPE_ENUM
{
    AT_CSS_BLACK_CELL_LIST_ADD_ONE      = 0,           /* 新增一条黑小区信息 */
    AT_CSS_BLACK_CELL_LIST_DELETE_ALL,                 /* 删除所有黑小区信息 */

    AT_CSS_BLACK_CELL_LIST_TYPE_BUTT
};
typedef  VOS_UINT8  AT_CSS_BLACK_CELL_LIST_OPERATE_TYPE_ENUM_UINT8;
/* Added for BlackCell Feature 2017-4-22 end */

/* add for CloudLine Feature 2017-6-29 begin */
/*****************************************************************************
枚举名    : AT_CSS_LINE_INDEX_LIST_OPERATE_TYPE_ENUM
结构说明  : 高铁索引表操作类型

*****************************************************************************/
enum AT_CSS_LINE_INDEX_LIST_OPERATE_TYPE_ENUM
{
    AT_CSS_LINE_INDEX_LIST_ADD          = 0,           /* 新增高铁索引信息 */
    AT_CSS_LINE_INDEX_LIST_DELETE_ALL,                 /* 删除高铁索引信息 */

    AT_CSS_LINE_INDEX_LIST_TYPE_BUTT
};
typedef  VOS_UINT8  AT_CSS_LINE_INDEX_LIST_OPERATE_TYPE_ENUM_UINT8;

/*****************************************************************************
枚举名    : AT_CSS_LINE_DETAIL_OPERATE_TYPE_ENUM
结构说明  : 高铁高铁详细信息表操作类型

*****************************************************************************/
enum AT_CSS_LINE_DETAIL_OPERATE_TYPE_ENUM
{
    AT_CSS_LINE_DETAIL_ADD              = 0,           /* 新增高铁线路详细信息 */
    AT_CSS_LINE_DETAIL_DELETE_ALL,                     /* 删除高铁线路详细信息 */

    AT_CSS_LINE_DETAIL_TYPE_BUTT
};
typedef  VOS_UINT8  AT_CSS_LINE_DETAIL_OPERATE_TYPE_ENUM_UINT8;

/*****************************************************************************
枚举名    : AT_CSS_LINE_DETAIL_INFO_TYPE_ENUM
结构说明  : 高铁高铁详细信息表类型

*****************************************************************************/
enum AT_CSS_LINE_DETAIL_INFO_TYPE_ENUM
{
    AT_CSS_LINE_DETAIL_INFO_TACLAC_PAIR     = 0,           /* TACLAC对 */
    AT_CSS_LINE_DETAIL_INFO_HO_PATH,                       /* HO预置路径 */
    AT_CSS_LINE_DETAIL_INFO_HO_BAR,                        /* HO预置Bar路径 */
    /* add for Cloudline Lte Feature 2018-4-16 begin */
    AT_CSS_LINE_DETAIL_INFO_LTE_HO_PATH,                   /* LTE预置路径 */
    /* add for Cloudline Lte Feature 2018-4-16 end */
    AT_CSS_LINE_DETAIL_INFO_NO_INFO         = 0xFF         /* 表示没有消息信息，停止解析 */
};
typedef  VOS_UINT8  AT_CSS_LINE_DETAIL_INFO_TYPE_ENUM_UINT8;
/* add for CloudLine Feature 2017-6-29 end */

/*****************************************************************************
  4 类型定义
*****************************************************************************/

typedef struct
{
    /*MCC在aucMcc[2]中的存放格式,mcc为460:
    ---------------------------------------------------------------------------
                 ||(BIT8)|(BIT7)|(BIT6)|(BIT5)|(BIT4)|(BIT3)|(BIT2)|(BIT1)
    ---------------------------------------------------------------------------
    aucMcc[0]    ||    MCC digit 2 = 6        |           MCC digit 1 = 4
    ---------------------------------------------------------------------------
    aucMcc[1]    ||    无效                   |           MCC digit 3 = 0
    ---------------------------------------------------------------------------*/
    VOS_UINT8                           aucMcc[2];                              /* MCC ID */
    VOS_UINT8                           aucRsv[2];
} AT_CSS_MCC_ID_STRU;


typedef struct
{
    VOS_UINT32                          ulMcc;
    VOS_UINT32                          ulMnc;
}CSS_AT_PLMN_ID_STRU;

typedef struct
{
    VOS_UINT32                          ulFreqHigh;
    VOS_UINT32                          ulFreqLow;
} AT_CSS_FREQ_RANGE_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                                /* _H2ASN_Skip */
    VOS_UINT16                          usClientId;
    VOS_UINT8                           aucRsv[2];
} AT_CSS_MCC_VERSION_INFO_REQ_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                                /* _H2ASN_Skip */
    VOS_UINT16                          usClientId;
    VOS_UINT8                           aucRsv[1];
    VOS_UINT8                           aucVersionId[MCC_INFO_VERSION_LEN];     /* 版本号，固定为xx.xx.xxx */
} CSS_AT_MCC_VERSION_INFO_CNF_STRU;



typedef struct
{
    VOS_MSG_HEADER                                                                        /* _H2ASN_Skip */
    VOS_UINT32                                    ulMsgId;                                /* _H2ASN_Skip */
    /* add for fill right clientId 2016-02-23, begin  */
    MODEM_ID_ENUM_UINT16                          usModemId;
    /* add for fill right clientId 2016-02-23, end  */
    VOS_UINT16                                    usClientId;
    VOS_UINT8                                     ucSeq;                                  /* 流水号 */
    AT_CSS_SET_MCC_OPERATE_TYPE_ENUM_UINT8        ucOperateType;                          /* 操作类型 */
    VOS_UINT8                                     aucVersionId[MCC_INFO_VERSION_LEN];     /* 版本号，固定为xx.xx.xxx */
    /* modified for fill right clientId 2016-02-23, begin  */
    VOS_UINT8                                     aucRsv[1];
    /* modified for fill right clientId 2016-02-23, end  */

    /*
        1)aucMccINfoBuff里存储的是MCC的信息，存储区的真实大小是ulMccInfoBuffLen里记录的字节数；
        2)aucMccINfoBuff里的格式为AT_CSS_MCC_INFO_STRU结构的格式，其中MNC个数,
          BAND个数，预置频段个数，预置频点的个数都是可变的。

        typedef struct
        {
            VOS_UINT8                           ucSupportFlag;    // 1:表示支持GSM 2:表示支持WCDMA 4:表示支持LTE，三者可以自由组合
            AT_CSS_FREQ_RANGE_STRU              stFreqRange;
        } AT_CSS_FREQ_RANGE_WITH_RAT_STRU;

        typedef struct
        {
            VOS_UINT8                                   ucBandInd;
            VOS_UINT8                                   ucFreqRangNum;
            //后面必须紧跟ucFreqRangNum个AT_CSS_FREQ_RANGE_WITH_RAT_STRU结构
              的FreqRange信息，如果没有FreqRange，则需要将ucFreqRangNum填为0
            AT_CSS_FREQ_RANGE_WITH_RAT_STRU             astFreqRangeArray[ucFreqRangNum];

            VOS_UINT8                                   ucPreConfigFreqNum;
            //后面必须紧跟ucPreConfigFreqNum个AT_CSS_FREQ_RANGE_WITH_RAT_STRU结构
              的PreConfigFreq信息，如果没有PreConfigFreq，则需要将ucPreConfigFreqNum填为0
            AT_CSS_FREQ_RANGE_WITH_RAT_STRU             astPreConfigFreqArray[ucPreConfigFreqNum];      //预置频点列表
        }AT_CSS_BAND_INFO_STRU;


        typedef struct
        {
            MNC在aucMnc[2]中的存放格式，mnc为01:
            ---------------------------------------------------------------------------
                         ||(BIT8)|(BIT7)|(BIT6)|(BIT5)|(BIT4)|(BIT3)|(BIT2)|(BIT1)
            ---------------------------------------------------------------------------
            aucMnc[0]    ||    MNC digit 3 = f        |           无效
            ---------------------------------------------------------------------------
            aucMnc[1]    ||    MNC digit 2 = 1        |           MNC digit 1 = 0
            ---------------------------------------------------------------------------
            VOS_UINT8                           aucMnc[2];
            VOS_UINT8                           ucBandCount;// BAND的个数
            //后面必须紧跟ucBandCount个band的信息，如果没有BAND，则需要将ucBandCount填为0
            AT_CSS_BAND_INFO_STRU               astBandArray[ucBandCount];
        }AT_CSS_MNC_INFO_STRU;


        typedef struct
        {
            MCC在aucMcc[2]中的存放格式,mcc为460:
            ---------------------------------------------------------------------------
                         ||(BIT8)|(BIT7)|(BIT6)|(BIT5)|(BIT4)|(BIT3)|(BIT2)|(BIT1)
            ---------------------------------------------------------------------------
            aucMcc[0]    ||    MCC digit 2 = 6        |           MCC digit 1 = 4
            ---------------------------------------------------------------------------
            aucMcc[1]    ||    无效                   |           MCC digit 3 = 0
            ---------------------------------------------------------------------------
            VOS_UINT8                           aucMcc[2];
            VOS_UINT8                           ucMncCount;// MNC的个数
            //后面必须紧跟ucMncCount个AT_CSS_MNC_INFO_STRU结构的mnc信息，如果没有mnc，则需要将ucMncCount填为0
            AT_CSS_MNC_INFO_STRU                astMncAarry[ucMncCount];
        }AT_CSS_MCC_INFO_STRU;


        3)aucMccINfoBuff中存储信息的格式AP与CSS直接对接，AT不会进行修改，AP先形成上述的格式，
          然后转成字符串格式发给AT，AT将字符串格式还原二进制码流，然后发给CSS；
          例如AP形成某一个字节为0x22，然后转化为字符串'22'，AT收到后再转成0x22;
        4)aucMccINfoBuff中的格式为小端；
        5)ulMccInfoBuffLen不能超过1.6K；
    */
    VOS_UINT32                          ulMccInfoBuffLen;
    VOS_UINT8                           aucMccInfoBuff[4];
} AT_CSS_MCC_INFO_SET_REQ_STRU;

/* Added for BlackCell Feature 2017-4-22 begin */

typedef struct
{
    VOS_MSG_HEADER                                                                                              /* _H2ASN_Skip */
    VOS_UINT32                                              ulMsgId;                                            /* _H2ASN_Skip */
    VOS_UINT16                                              usClientId;
    VOS_UINT8                                               ucSeq;                                              /* 流水号 */
    AT_CSS_BLACK_CELL_LIST_OPERATE_TYPE_ENUM_UINT8          enOperateType;                                      /* 操作类型 */
    VOS_UINT8                                               aucVersionId[AT_CSS_BLACK_CELL_LIST_VERSION_LEN];   /* 版本号，固定为xx.xx.xxx */
    AT_CSS_RAT_ENUM_UINT8                                   enRatType;                                          /* 系统制式 */
    VOS_UINT8                                               aucRsv[2];

    /*
        1)aucBlackCellListBuff里存储的是黑小区的信息，存储区的真实大小是ulBlackCellListBuffLen里记录的字节数；
        2)aucBlackCellListBuff里的格式为AT_CSS_GSM_CLOUD_BLACKCELL_STRU结构的格式

        码流参数BS1,BS2和BS3的编码格式相同，都按AT_CSS_GSM_CLOUD_BLACKCELL_STRU结构的格式编码：
        其中ucPairCount，astBlackListArray是可以变的
        typedef struct
        {
            CSS_PLMN_ID_STRU    stPlmn;
            VOS_UNIT8           ucPairCount;
            //后面必须紧跟ucPairCount个AT_CSS_GSM_CLOUD_BLACKCELL_PAIR_STRU结构的pair信息，
              如果没有如果该mcc没有黑小区信息，AP不需要下发该MCC的信息，Modem认为该MCC没有黑小区信息，
              后续不再要求上报^REPORTBLACKCELLMCC来获取黑小区
            AT_CSS_GSM_CLOUD_BLACKCELL_PAIR_STRU astBlackListPairArray[ucPairCount];
        }AT_CSS_GSM_CLOUD_BLACKCELL_STRU;

        typedef struct
        {
            VOS_UNIT32  ulMcc; //Modem内部plmn格式;比如460---0x00000604
            VOS_UNIT32  ulMnc; //Modem内部plmn格式;比如 01---0x000F0100
        }CSS_PLMN_ID_STRU;

        typedef struct
        {
            VOS_UNIT16  usSourceCellLai;
            VOS_UNIT16  usSourceCellId;
            VOS_UNIT16  usBlackCellArfcn; //包含了GSM的频段信息，比如0x1029表示GSM900频段,41频点
            VOS_UNIT8   ucBlackCellBsic;  //包含ncc,bcc;比如0x14表示<2,4>
            VOS_UNIT8   ucBlackCellReduceVal;//针对本黑小区惩罚时降低能量db值
        }AT_CSS_GSM_CLOUD_BLACKCELL_PAIR_STRU;
    */

    VOS_UINT32                                              ulBlackCellListBuffLen;
    VOS_UINT8                                               aucBlackCellListBuff[4];
} AT_CSS_BLACK_CELL_LIST_SET_REQ_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                                /* _H2ASN_Skip */
    VOS_UINT16                          usClientId;
    VOS_UINT8                           aucRsv[2];
} AT_CSS_BLACK_CELL_LIST_QUERY_REQ_STRU;
/* Added for BlackCell Feature 2017-4-22 end */

typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                                /* _H2ASN_Skip */
    VOS_UINT16                          usClientId;
    VOS_UINT8                           ucSeq;                                  /* 流水号 */
    VOS_UINT8                           aucRsv[1];

    VOS_UINT32                          ulResult;                               /*0表示成功，1表示失败*/
} CSS_AT_MCC_INFO_SET_CNF_STRU;

typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                                /* _H2ASN_Skip */
    VOS_UINT16                          usClientId;
    VOS_UINT8                           aucRsv[1];
    VOS_UINT8                           aucVersionId[MCC_INFO_VERSION_LEN];     /* 版本号，固定为xx.xx.xxx */
    VOS_UINT32                          ulMccNum;                               /* MCC 个数 */
    AT_CSS_MCC_ID_STRU                  astMccId[AT_CSS_MAX_MCC_ID_NUM];        /* MCC ID列表 */
} CSS_AT_QUERY_MCC_INFO_NOTIFY_STRU;


typedef struct
{
    VOS_UINT32                          ulMccNum;                                               /* MCC 个数 */
    VOS_UINT32                          aulMcc[AT_CSS_MAX_MCC_ID_NUM];                          /* MCC 列表,Modem内部plmn格式;比如460---0x00000604 */
} CSS_AT_BLACK_CELL_MCC_INFO_STRU;


/* Added for BlackCell Feature 2017-4-22 begin */

typedef struct
{
    VOS_MSG_HEADER                                                          /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                            /* _H2ASN_Skip */
    VOS_UINT16                          usClientId;
    VOS_UINT8                           ucSeq;                              /* 流水号 */
    VOS_UINT8                           ucReserved;
    VOS_UINT32                          ulResult;                           /* VOS_OK表示成功，VOS_ERR表示失败 */
} CSS_AT_BLACK_CELL_LIST_SET_CNF_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                                              /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                                                /* _H2ASN_Skip */
    VOS_UINT16                          usClientId;
    VOS_UINT8                           ucReserved;
    VOS_UINT8                           aucVersionId[AT_CSS_BLACK_CELL_LIST_VERSION_LEN];       /* 版本号，固定为xx.xx.xxx */
    CSS_AT_BLACK_CELL_MCC_INFO_STRU     stMccInfo;                                              /* 黑小区MCC信息 */
} CSS_AT_BLACK_CELL_LIST_QUERY_CNF_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                                              /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                                                /* _H2ASN_Skip */
    VOS_UINT16                          usClientId;
    VOS_UINT8                           ucReserved;
    VOS_UINT8                           aucVersionId[AT_CSS_BLACK_CELL_LIST_VERSION_LEN];       /* 版本号，固定为xx.xx.xxx */
    CSS_AT_BLACK_CELL_MCC_INFO_STRU     stMccInfo;                                              /* 黑小区MCC信息 */
} CSS_AT_BLACK_CELL_MCC_NOTIFY_STRU;
/* Added for BlackCell Feature 2017-4-22 end */

/* add for CloudLine Feature 2017-6-29 begin */


typedef struct
{
    VOS_MSG_HEADER                                                                                              /* _H2ASN_Skip */
    VOS_UINT32                                              ulMsgId;                                            /* _H2ASN_Skip */
    VOS_UINT16                                              usClientId;
    MODEM_ID_ENUM_UINT16                                    usModemId;
    VOS_UINT8                                               ucSeq;                                              /* 流水号 */
    AT_CSS_LINE_INDEX_LIST_OPERATE_TYPE_ENUM_UINT8          enOperateType;                                      /* 操作类型 */
    VOS_UINT8                                               aucVersionId[AT_CSS_CLOUD_LINE_VERSION_LEN];        /* 版本号，固定为xx.xx.xxx */
    VOS_UINT8                                               ucRsv;

    /*
        1)aucLineIndexListBuff里存储的是高铁线路索引的信息，存储区的真实大小是ulLineIndexListBuffLen里记录的字节数；
        2)aucLineIndexListBuff里的格式为AT_CSS_CLOUD_LINE_INDEX_LIST_INFO_STRU结构的格式
        当LineIndex为0xFFFF时表示该PLMN下没有高铁线路信息。

        typedef struct
        {
            CSS_PLMN_ID_STRU stPlmn;
            VOS_UNIT16          usLineIndex;
            VOS_UINT8           ucTacNum;
            VOS_UINT8           ucLacNum;
            //后面必须紧跟ucTacNum个VOS_UINT16的TAC列表信息。
            VOS_UINT16          ausTacList[ucTacNum];
            //后面必须紧跟ucLacNum个VOS_UINT16的LAC列表信息。
            VOS_UINT16          ausLacList[ucLacNum];
        }AT_CSS_CLOUD_LINE_INDEX_INFO_STRU;

    */
    VOS_UINT32                                              ulLineIndexListBuffLen;
    VOS_UINT8                                               aucLineIndexListBuff[4];
}AT_CSS_LINE_INDEX_LIST_SET_REQ_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                          /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                            /* _H2ASN_Skip */
    VOS_UINT16                          usClientId;
    VOS_UINT8                           ucSeq;                              /* 流水号 */
    VOS_UINT8                           ucRsv;
    VOS_UINT32                          ulResult;                           /* VOS_OK表示成功，VOS_ERR表示失败 */
} CSS_AT_LINE_INDEX_LIST_SET_CNF_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                                                              /* _H2ASN_Skip */
    VOS_UINT32                                              ulMsgId;                                            /* _H2ASN_Skip */
    VOS_UINT16                                              usClientId;
    MODEM_ID_ENUM_UINT16                                    usModemId;
    VOS_UINT8                                               ucSeq;                                              /* 流水号 */
    AT_CSS_LINE_DETAIL_OPERATE_TYPE_ENUM_UINT8              enOperateType;                                      /* 操作类型 */
    VOS_UINT8                                               aucVersionId[AT_CSS_CLOUD_LINE_VERSION_LEN];   /* 版本号，固定为xx.xx.xxx */
    VOS_UINT8                                               ucRsv;

    /*
        1)aucLineDetailBuff里存储的是高铁线路索引的信息，存储区的真实大小是ulLineDetailBuffLen里记录的字节数。

        2)aucLineDetailBuff里的格式为AT_CSS_LINE_DETAIL_INFO_STRU结构的格式:
        typedef struct
        {
            CSS_PLMN_ID_STRU                             stPlmn;
            VOS_UNIT16                                      usLineIndex;
            AT_CSS_LINE_DETAIL_INFO_TYPE_ENUM_UINT8         enInfoType;
            VOS_UINT8                                       aucLeftBuff[ulLineDetailBuffLen-11];
        }AT_CSS_LINE_DETAIL_INFO_STRU;

        3)AT_CSS_LINE_DETAIL_INFO_STRU结构中的aucLeftBuff根据enInfoType的指示进行编码：
          i.当enInfoType为AT_CSS_LINE_DETAIL_INFO_TACLAC_PAIR时aucLeftBuff里的格式为AT_CSS_CLOUD_LINE_TACLAC_MAP_INFO_STRU结构的格式：
            typedef struct
            {
                VOS_UITN16                                       usTac;
                VOS_UINT16                                       usLac;
            }AT_CSS_CLOUD_LINE_TACLAC_PAIR_INFO_STRU;
            typedef struct
            {
                VOS_UINT8                                        ucPairNum;
                //后面必须紧跟ucPairNum个AT_CSS_CLOUD_LINE_TACLAC_PAIR_INFO_STRU的信息。
                AT_CSS_CLOUD_LINE_TACLAC_PAIR_INFO_STRU          astCloudTacLacPairInfoList[ucPairNum];
            }AT_CSS_CLOUD_LINE_TACLAC_MAP_INFO_STRU;

          ii.当enInfoType为AT_CSS_LINE_DETAIL_INFO_HO_PATH时aucLeftBuff里的格式为AT_CSS_CLOUD_LINE_HO_PATH_INFO_STRU结构的格式：
            typedef struct
            {
                VOS_UINT32                                       usSrcCellId;
                VOS_UINT16                                       usSrcArfcn;
                VOS_UINT16                                       usSrcBsic;
                VOS_UINT16                                       usDstArfcn;
                VOS_UINT16                                       usDstBsic;
                VOS_UINT16                                       usRxlevA;
                VOS_UINT16                                       usRxlevB;
            }AT_CSS_CLOUD_LINE_HO_PATH_CELL_INFO_STRU;
            typedef struct
            {
                VOS_UITN16                                       usLac;
                VOS_UINT8                                        ucCellNum;
                //后面必须紧跟ucCellNum个AT_CSS_CLOUD_LINE_HO_PATH_CELL_INFO_STRU的信息。
                AT_CSS_CLOUD_LINE_HO_PATH_CELL_INFO_STRU         astCellInfoList[ucCellNum];
            }AT_CSS_CLOUD_LINE_HO_PATH_LAC_INFO_STRU;
            typedef struct
            {
                VOS_UINT8                                        ucRat;
                VOS_UINT8                                        ucDirection;
                VOS_UINT8                                        ucLacNum;
                //后面必须紧跟ucLacNum个AT_CSS_CLOUD_LINE_HO_PATH_LAC_INFO_STRU的信息。
                AT_CSS_CLOUD_LINE_HO_PATH_LAC_INFO_STRU          astLacInfoList[ucLacNum];
            }AT_CSS_CLOUD_LINE_HO_PATH_INFO_STRU;
          iii.当enInfoType为AT_CSS_LINE_DETAIL_INFO_HO_BAR时aucLeftBuff里的格式为AT_CSS_CLOUD_LINE_HO_BAR_INFO_STRU结构的格式：
            typedef struct
            {
                VOS_UINT16                                       usLac;
                VOS_UINT32                                       usSrcCellId;
                VOS_UINT16                                       usSrcArfcn;
                VOS_UINT16                                       usSrcBsic;
                VOS_UINT16                                       usDstArfcn;
                VOS_UINT16                                       usDstBsic;
            }AT_CSS_CLOUD_LINE_HO_BAR_CELL_INFO_STRU;
            typedef struct
            {
                VOS_UINT8                                        ucRat;
                VOS_UINT8                                        ucCellNum;
                //后面必须紧跟ucCellNum个AT_CSS_CLOUD_LINE_HO_BAR_CELL_INFO_STRU的信息。
                AT_CSS_CLOUD_LINE_HO_BAR_CELL_INFO_STRU          astCellInfoList[ucCellNum];
            }AT_CSS_CLOUD_LINE_HO_BAR_INFO_STRU;
          iiii.当enInfoType为AT_CSS_LINE_DETAIL_INFO_NO_INFO时表示该PLMN/线路下没有详细信息，停止解析，modem不再向ap查询该PLMN/线路详细信息。
          iiiii.当enInfoType为以上类型的其他无效类型时，停止解析，直接返回失败。
    */
    VOS_UINT32                                              ulLineDetailBuffLen;
    VOS_UINT8                                               aucLineDetailBuff[4];
} AT_CSS_LINE_DETAIL_SET_REQ_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                          /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                            /* _H2ASN_Skip */
    VOS_UINT16                          usClientId;
    VOS_UINT8                           ucSeq;                              /* 流水号 */
    VOS_UINT8                           ucRsv;
    VOS_UINT32                          ulResult;                           /* VOS_OK表示成功，VOS_ERR表示失败 */
}CSS_AT_LINE_DETAIL_SET_CNF_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                          /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                            /* _H2ASN_Skip */
    VOS_UINT16                          usClientId;
    MODEM_ID_ENUM_UINT16                usModemId;
}AT_CSS_LINE_INDEX_LIST_QUERY_REQ_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                          /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                            /* _H2ASN_Skip */
    VOS_UINT16                          usClientId;
    VOS_UINT8                           aucVersionId[AT_CSS_CLOUD_LINE_VERSION_LEN];   /* 版本号 */
    VOS_UINT8                           ucRsv;
    CSS_AT_PLMN_ID_STRU                 stRplmnInfo;                        /* rplmn信息 */
    VOS_UINT32                          ulResult;                           /* 查询结果是否有效 */
}CSS_AT_LINE_INDEX_LIST_QUERY_CNF_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                          /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                            /* _H2ASN_Skip */
    VOS_UINT16                          usClientId;
    VOS_UINT8                           aucRsv[2];
    CSS_AT_PLMN_ID_STRU                 stRplmnInfo;                        /* rplmn信息 */
} CSS_AT_LINE_PLMN_NOTIFY_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                          /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                            /* _H2ASN_Skip */
    VOS_UINT16                          usClientId;                         /* _H2ASN_Skip */
    VOS_UINT16                          usLineNum;                          /* 线路个数 */
    VOS_UINT16                          ausLineIndexList[AT_CSS_TACLAC_MAX_LINE_NUM]; /* 线路列表 */
    CSS_AT_PLMN_ID_STRU                 stPlmnId;                           /* plmn id */
} CSS_AT_LINE_INDEX_NOTIFY_STRU;

/* add for CloudLine Feature 2017-6-29 end */


/* Added for VZW EMC, 2018-1-9, begin */

typedef struct
{
    VOS_MSG_HEADER                                                          /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                            /* _H2ASN_Skip */
    VOS_UINT16                          usClientId;
    MODEM_ID_ENUM_UINT16                usModemId;
} AT_CSS_VZWMRUC_SET_REQ_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                                /* _H2ASN_Skip */
    VOS_UINT16                          usClientId;
    VOS_UINT8                           aucRsv[2];
    VOS_UINT32                          ulResult;                               /* VOS_OK表示成功，VOS_ERR表示失败 */
} CSS_AT_VZWMRUC_SET_CNF_STRU;


typedef struct
{
    VOS_UINT8                           ucEntry;
    AT_CSS_RAT_ENUM_UINT8               enRat;
    VOS_UINT16                          usBandId;
    CSS_AT_PLMN_ID_STRU                 stPlmnId;
}AT_CSS_MRU_STRU;



typedef struct
{
    VOS_MSG_HEADER                                                          /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                            /* _H2ASN_Skip */
    VOS_UINT16                          usClientId;
    MODEM_ID_ENUM_UINT16                usModemId;
    AT_CSS_MRU_STRU                     stMru;
} AT_CSS_VZWMRUE_SET_REQ_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                                /* _H2ASN_Skip */
    VOS_UINT16                          usClientId;
    VOS_UINT8                           aucRsv[2];
    VOS_UINT32                          ulResult;                               /* VOS_OK表示成功，VOS_ERR表示失败 */
} CSS_AT_VZWMRUE_SET_CNF_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                          /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                            /* _H2ASN_Skip */
    VOS_UINT16                          usClientId;
    MODEM_ID_ENUM_UINT16                usModemId;
} AT_CSS_VZWMRUE_QUERY_REQ_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                                /* _H2ASN_Skip */
    VOS_UINT32                          ulResult;                               /* VOS_OK表示成功，VOS_ERR表示失败 */
    VOS_UINT16                          usClientId;
    VOS_UINT8                           ucRsv;
    VOS_UINT8                           ucMruNum;
    AT_CSS_MRU_STRU                     astMru[AT_CSS_MRU_MAX_NUM];
} CSS_AT_VZWMRUE_QUERY_CNF_STRU;

/* Added for VZW EMC, 2018-1-9, end */

/*****************************************************************************
  5 全局变量声明
*****************************************************************************/


/*****************************************************************************
  6 接口函数声明
*****************************************************************************/

/*****************************************************************************
  7 OTHERS定义
*****************************************************************************/

/* ASN解析结构 */
typedef struct
{
    VOS_UINT32                          ulMsgId;                                /*_H2ASN_MsgChoice_Export CSS_AT_MSG_TYPE_ENUM_UINT32 */
    VOS_UINT8                           aucMsg[4];
    /***************************************************************************
        _H2ASN_MsgChoice_When_Comment          CSS_AT_MSG_TYPE_ENUM_UINT32
    ****************************************************************************/
}CSS_AT_INTERFACE_MSG_DATA;
/*_H2ASN_Length UINT32*/

typedef struct
{
    VOS_MSG_HEADER
    CSS_AT_INTERFACE_MSG_DATA           stMsgData;
} CssAtInterface_MSG;


#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


