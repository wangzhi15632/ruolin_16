#ifndef LIB_STORAGE_H
#define LIB_STORAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#define STORAGE_EXPORT
#define STORAGE_PARSE_VERSION "V1.1.0"
/*文件类型枚举*/
typedef enum
{
    RECORD_TYPE_NORMAL = 0,     /*普通录像*/
    RECORD_TYPE_EVENT,          /*事件录像*/
    RECORD_TYPE_PICTURE,        /*图片*/
    RECORD_TYPE_AUDIO,          /*录音*/
    RECORD_TYPE_SMART_PICTURE,  /*智能抓图*/
    RECORD_TYPE_NUM
}RECORD_TYPE_E;

/*[类型-通道]映射(40 bytes)*/
typedef struct
{
        char sTypeName[32];     /*类型名称*/
        unsigned int StartChan; /*起始通道*/
        unsigned int EndChan;   /*结束通道*/
}TYPE_CHAN_MAP_T;

/*文件搜索参数(28 bytes)*/
typedef struct
{
        unsigned int uOrder;     /*搜索顺序(0正序,非0倒序)*/
        unsigned int uMaxNum;    /*搜索数量上限(默认搜索全部)*/
        int iChan;               /*搜索通道*/
        int iType;               /*搜索类型*/
        int iSubType;            /*搜索子类型*/
        unsigned int uStartTime; /*搜索起始时间*/
        unsigned int uEndTime;   /*搜索结束时间*/
}RECORD_SEARCH_PARAM_T;

/*文件信息(88 bytes)*/
typedef struct
{
    char sName[64];          /*文件名称*/
    int iType;               /*文件类型*/
    int iChan;               /*文件通道*/
    unsigned int uFileSize;  /*文件大小*/
    unsigned int uStartTime; /*文件起始时间*/
    unsigned int uEndTime;   /*文件结束时间*/
    unsigned int bLocked;    /*文件状态*/
}RECORD_INFO_T;

/*设备信息(320 bytes)*/
typedef struct
{
     char sCameraModel[32];        /*相机型号*/
     char sFirmwareVersion[32];    /*固件版本*/
     char sFirmwareDate[32];       /*固件日期*/
     char sSerialNumber[32];       /*序列号*/
     char sVerificationCode[32];   /*验证码*/
     char sInfoString1[32];        /*字符串信息1*/
     char sInfoString2[32];        /*字符串信息2*/
     char sTrainNum[32];           /*列车号*/
     char sCarriageNum[32];        /*车厢号*/
     char sDriverNum[32];          /*司机号*/
}DEV_INFO_T;

/*GPS信息(28 bytes)*/
typedef struct
{
    unsigned int uTime;           /* 绝对时间*/
    unsigned int uLongitude;      /* 经度 = 实际度*3600*100+实际分*60*100+实际秒*100 */
    unsigned int uLatitude;       /* 纬度 = 实际度*3600*100+实际分*60*100+实际秒*100 */
    unsigned int uSpeed;          /* 速度 = 实际速度*1000*100 */
    unsigned int uAngle;          /* 速度方向角 = 实际方向*100，正北为0度，顺时针 */
    int iHeight;                  /* 海拔高度(厘米) */
    unsigned char uValid;         /* GPS信息是否有效*/
    unsigned char uLongitudeType; /* E/W(东经/西经);*/
    unsigned char uLatitudeType;  /* S/N(南纬/北纬)*/
        unsigned char uRes;
}GPS_INFO_T;

/* @brief   获取版本
 * @param   pBuf      版本缓存指针
 * @param   uBufSize  版本缓存大小
 * @return  void
 */
STORAGE_EXPORT void storage_version(char *pBuf, unsigned int uBufSize);

/* @brief   初始化
 * @param   pPath  存储器路径
 * @return  成功返回0,失败返回-1
 */
STORAGE_EXPORT int storage_init(const char *pPath);

/* @brief   获取类型通道映射
 * @param   pMap         类型通道映射指针
 * @param   uMaxTypeNum  类型最大支持数量
 * @return  成功返回类型实际数量,失败返回-1
 */
STORAGE_EXPORT int storage_type_chan_map_get(TYPE_CHAN_MAP_T *pMap, unsigned int uMaxTypeNum);

/* @brief   创建文件搜索链表
 * @param   pSearchParam  文件搜索参数指针
 * @return  成功返回文件搜索链表句柄,失败返回NULL
 */
STORAGE_EXPORT void *storage_record_list_create(RECORD_SEARCH_PARAM_T *pSearchParam);

/* @brief   释放文件搜索链表
 * @param   pHandle 文件搜索链表句柄
 * @return  void
 */
STORAGE_EXPORT void storage_record_list_release(void *pHandle);

/* @brief   文件搜索链表出队
 * @param   pHandle        文件搜索链表句柄
 * @param   pRecInfo[out]  文件搜索结果信息
 * @return  成功返回0,失败或出队完毕返回-1
 */
STORAGE_EXPORT int storage_record_list_dequeue(void *pHandle, RECORD_INFO_T *pRecInfo);

/* @brief   文件打开(只读)
 * @param   pName  文件名称
 * @return  成功返回文件句柄,失败返回NULL
 */
STORAGE_EXPORT void *storage_record_open(const char *pName);

/* @brief   文件偏移
 * @param   pHandle  文件句柄
 * @param   uOffset  偏移量
 * @return  成功返回0,失败返回-1
 */
STORAGE_EXPORT int storage_record_seek(void *pHandle, unsigned int uOffset);

/* @brief   文件偏移(按时间)
 * @param   pHandle       文件句柄
 * @param   uSec          偏移秒数
 * @param   pOffset[out]  偏移量
 * @return  成功返回0,失败返回-1
 */
STORAGE_EXPORT int storage_record_seek_by_time(void *pHandle, unsigned int uSec, unsigned int *pOffset);

/* @brief   文件读取
 * @param   pHandle   文件句柄
 * @param   pBuf      数据缓存指针
 * @param   uBufSize  数据缓存大小
 * @return  成功返回读取的字节数(返回0表示文件结束),失败返回-1,
 */
STORAGE_EXPORT int storage_record_read(void *pHandle, unsigned char *pBuf, unsigned int uBufSize);

/* @brief   文件关闭
 * @param   pHandle  文件句柄
 * @return  void
 */
STORAGE_EXPORT void storage_record_close(void *pHandle);

/* @brief   文件大小获取
 * @param   pName           文件名称
 * @param   pFileSize[out]  文件大小
 * @return  成功返回0,失败返回-1
 */
STORAGE_EXPORT int storage_record_size_get(const char *pName, unsigned int *pFileSize);

/* @brief   文件设备信息获取
 * @param   pName          文件名称
 * @param   pDevInfo       设备信息缓存指针
 * @return  成功返回0,失败返回-1
 */
STORAGE_EXPORT int storage_record_dev_info_get(const char *pName, DEV_INFO_T *pDevInfo);

/* @brief   文件GPS信息获取
 * @param   pName          文件名称
 * @param   pGpsInfo       GPS信息缓存指针
 * @param   uMaxNum        缓存支持的GPS信息最大个数
 * @param   pRealNum[out]  实际的GPS信息个数
 * @return  成功返回0,失败返回-1
 */
STORAGE_EXPORT int storage_record_gps_info_get(const char *pName, GPS_INFO_T *pGpsInfo, unsigned int uMaxNum, unsigned int *pRealNum);

#ifdef __cplusplus
}
#endif
#endif
