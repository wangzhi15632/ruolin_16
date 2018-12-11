#ifndef LIB_STORAGE_H
#define LIB_STORAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#define STORAGE_EXPORT
#define STORAGE_PARSE_VERSION "V1.1.0"
/*�ļ�����ö��*/
typedef enum
{
    RECORD_TYPE_NORMAL = 0,     /*��ͨ¼��*/
    RECORD_TYPE_EVENT,          /*�¼�¼��*/
    RECORD_TYPE_PICTURE,        /*ͼƬ*/
    RECORD_TYPE_AUDIO,          /*¼��*/
    RECORD_TYPE_SMART_PICTURE,  /*����ץͼ*/
    RECORD_TYPE_NUM
}RECORD_TYPE_E;

/*[����-ͨ��]ӳ��(40 bytes)*/
typedef struct
{
        char sTypeName[32];     /*��������*/
        unsigned int StartChan; /*��ʼͨ��*/
        unsigned int EndChan;   /*����ͨ��*/
}TYPE_CHAN_MAP_T;

/*�ļ���������(28 bytes)*/
typedef struct
{
        unsigned int uOrder;     /*����˳��(0����,��0����)*/
        unsigned int uMaxNum;    /*������������(Ĭ������ȫ��)*/
        int iChan;               /*����ͨ��*/
        int iType;               /*��������*/
        int iSubType;            /*����������*/
        unsigned int uStartTime; /*������ʼʱ��*/
        unsigned int uEndTime;   /*��������ʱ��*/
}RECORD_SEARCH_PARAM_T;

/*�ļ���Ϣ(88 bytes)*/
typedef struct
{
    char sName[64];          /*�ļ�����*/
    int iType;               /*�ļ�����*/
    int iChan;               /*�ļ�ͨ��*/
    unsigned int uFileSize;  /*�ļ���С*/
    unsigned int uStartTime; /*�ļ���ʼʱ��*/
    unsigned int uEndTime;   /*�ļ�����ʱ��*/
    unsigned int bLocked;    /*�ļ�״̬*/
}RECORD_INFO_T;

/*�豸��Ϣ(320 bytes)*/
typedef struct
{
     char sCameraModel[32];        /*����ͺ�*/
     char sFirmwareVersion[32];    /*�̼��汾*/
     char sFirmwareDate[32];       /*�̼�����*/
     char sSerialNumber[32];       /*���к�*/
     char sVerificationCode[32];   /*��֤��*/
     char sInfoString1[32];        /*�ַ�����Ϣ1*/
     char sInfoString2[32];        /*�ַ�����Ϣ2*/
     char sTrainNum[32];           /*�г���*/
     char sCarriageNum[32];        /*�����*/
     char sDriverNum[32];          /*˾����*/
}DEV_INFO_T;

/*GPS��Ϣ(28 bytes)*/
typedef struct
{
    unsigned int uTime;           /* ����ʱ��*/
    unsigned int uLongitude;      /* ���� = ʵ�ʶ�*3600*100+ʵ�ʷ�*60*100+ʵ����*100 */
    unsigned int uLatitude;       /* γ�� = ʵ�ʶ�*3600*100+ʵ�ʷ�*60*100+ʵ����*100 */
    unsigned int uSpeed;          /* �ٶ� = ʵ���ٶ�*1000*100 */
    unsigned int uAngle;          /* �ٶȷ���� = ʵ�ʷ���*100������Ϊ0�ȣ�˳ʱ�� */
    int iHeight;                  /* ���θ߶�(����) */
    unsigned char uValid;         /* GPS��Ϣ�Ƿ���Ч*/
    unsigned char uLongitudeType; /* E/W(����/����);*/
    unsigned char uLatitudeType;  /* S/N(��γ/��γ)*/
        unsigned char uRes;
}GPS_INFO_T;

/* @brief   ��ȡ�汾
 * @param   pBuf      �汾����ָ��
 * @param   uBufSize  �汾�����С
 * @return  void
 */
STORAGE_EXPORT void storage_version(char *pBuf, unsigned int uBufSize);

/* @brief   ��ʼ��
 * @param   pPath  �洢��·��
 * @return  �ɹ�����0,ʧ�ܷ���-1
 */
STORAGE_EXPORT int storage_init(const char *pPath);

/* @brief   ��ȡ����ͨ��ӳ��
 * @param   pMap         ����ͨ��ӳ��ָ��
 * @param   uMaxTypeNum  �������֧������
 * @return  �ɹ���������ʵ������,ʧ�ܷ���-1
 */
STORAGE_EXPORT int storage_type_chan_map_get(TYPE_CHAN_MAP_T *pMap, unsigned int uMaxTypeNum);

/* @brief   �����ļ���������
 * @param   pSearchParam  �ļ���������ָ��
 * @return  �ɹ������ļ�����������,ʧ�ܷ���NULL
 */
STORAGE_EXPORT void *storage_record_list_create(RECORD_SEARCH_PARAM_T *pSearchParam);

/* @brief   �ͷ��ļ���������
 * @param   pHandle �ļ�����������
 * @return  void
 */
STORAGE_EXPORT void storage_record_list_release(void *pHandle);

/* @brief   �ļ������������
 * @param   pHandle        �ļ�����������
 * @param   pRecInfo[out]  �ļ����������Ϣ
 * @return  �ɹ�����0,ʧ�ܻ������Ϸ���-1
 */
STORAGE_EXPORT int storage_record_list_dequeue(void *pHandle, RECORD_INFO_T *pRecInfo);

/* @brief   �ļ���(ֻ��)
 * @param   pName  �ļ�����
 * @return  �ɹ������ļ����,ʧ�ܷ���NULL
 */
STORAGE_EXPORT void *storage_record_open(const char *pName);

/* @brief   �ļ�ƫ��
 * @param   pHandle  �ļ����
 * @param   uOffset  ƫ����
 * @return  �ɹ�����0,ʧ�ܷ���-1
 */
STORAGE_EXPORT int storage_record_seek(void *pHandle, unsigned int uOffset);

/* @brief   �ļ�ƫ��(��ʱ��)
 * @param   pHandle       �ļ����
 * @param   uSec          ƫ������
 * @param   pOffset[out]  ƫ����
 * @return  �ɹ�����0,ʧ�ܷ���-1
 */
STORAGE_EXPORT int storage_record_seek_by_time(void *pHandle, unsigned int uSec, unsigned int *pOffset);

/* @brief   �ļ���ȡ
 * @param   pHandle   �ļ����
 * @param   pBuf      ���ݻ���ָ��
 * @param   uBufSize  ���ݻ����С
 * @return  �ɹ����ض�ȡ���ֽ���(����0��ʾ�ļ�����),ʧ�ܷ���-1,
 */
STORAGE_EXPORT int storage_record_read(void *pHandle, unsigned char *pBuf, unsigned int uBufSize);

/* @brief   �ļ��ر�
 * @param   pHandle  �ļ����
 * @return  void
 */
STORAGE_EXPORT void storage_record_close(void *pHandle);

/* @brief   �ļ���С��ȡ
 * @param   pName           �ļ�����
 * @param   pFileSize[out]  �ļ���С
 * @return  �ɹ�����0,ʧ�ܷ���-1
 */
STORAGE_EXPORT int storage_record_size_get(const char *pName, unsigned int *pFileSize);

/* @brief   �ļ��豸��Ϣ��ȡ
 * @param   pName          �ļ�����
 * @param   pDevInfo       �豸��Ϣ����ָ��
 * @return  �ɹ�����0,ʧ�ܷ���-1
 */
STORAGE_EXPORT int storage_record_dev_info_get(const char *pName, DEV_INFO_T *pDevInfo);

/* @brief   �ļ�GPS��Ϣ��ȡ
 * @param   pName          �ļ�����
 * @param   pGpsInfo       GPS��Ϣ����ָ��
 * @param   uMaxNum        ����֧�ֵ�GPS��Ϣ������
 * @param   pRealNum[out]  ʵ�ʵ�GPS��Ϣ����
 * @return  �ɹ�����0,ʧ�ܷ���-1
 */
STORAGE_EXPORT int storage_record_gps_info_get(const char *pName, GPS_INFO_T *pGpsInfo, unsigned int uMaxNum, unsigned int *pRealNum);

#ifdef __cplusplus
}
#endif
#endif
