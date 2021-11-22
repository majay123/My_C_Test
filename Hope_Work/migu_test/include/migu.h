#ifndef __MIGU_H
#define __MIGU_H

/*****************************************************************
******************************结构体说明**************************
******************************************************************/
/*音响搜歌请求参数结构体*/
typedef struct boxSearchPostData{
	char * text;
	char * searchType;
	char * sort;
	char * issemantic;
	char * isCorrect;
	char * isCopyright;
	char * expire;
	char * sortType;
	char * contentFilter;
	char * matchType;
	char * searchRange;
	char * pageIndex;
	char * pageSize;
}boxSearchPostData_t;

/*试听行为结构体*/
typedef struct listeningBehavior{
	char * contentId;		//歌曲ID
	char * toneQuality;		//音质。"1"：标清；"2"：高清；"3"：无损
	char * runningTime;		//播放时长(毫秒)
	char * startTime;		//播放开始时间	格式yyyymmdd HHMMSS.mmmmmm
	char * stopTime;		//播放停止时间	格式yyyymmdd HHMMSS.mmmmmm
	char * userId;			//用户ID（合作伙伴自定义，不校验）
	char * deviceType;		//设备类型(1:安卓；2：IOS；3：linux；4：其他)
}listeningBehavior_t;

//=================================================================
//==========================通用接口===============================
//=================================================================
//初始化sdk
int migusdk_init( char * device_info);
//注销SDK
void migusdk_cleanup(void);
//获取SDK版本信息
char * getSdkVersion(char * version);

//=================================================================
//==========================curl控制接口=======================
//=================================================================
/*默认 CURLOPT_CONNECTTIMEOUT_MS = 5000ms， CURLOPT_TIMEOUT_MS = 10000ms*/
//获取curl连接超时时间（单位:ms）（CURLOPT_CONNECTTIMEOUT_MS）
int get_curl_connect_timeout();
//设置curl连接超时时间（单位:ms）（CURLOPT_CONNECTTIMEOUT_MS）
void set_curl_connect_timeout(long connect_timeout);
//获取curl访问超时时间（单位:ms）（CURLOPT_TIMEOUT_MS）
int get_curl_timeout();
//设置curl访问超时时间（单位:ms）（CURLOPT_TIMEOUT_MS）
void set_curl_timeout(long timeout);
//停止接口curl访问
void stop_curl_access();
//设置证书路径
void set_CAfile(char * CAfile);

/*
下面是咪咕后台相关接口。 “result” 为接收数据指针，请用户自行分配空间并置0
*/
//=================================================================
//==================获取歌曲、歌单专辑相关操作接口=================
//=================================================================
/*歌曲id查询歌曲信息接口*/
char * get_musicInfo(char * musicId,char *picSize, char *result);
/*歌曲id批量查询歌曲信息接口*/
char * get_musicInfos(char * musicIds,char *picSize, char *result);
/*查询音乐歌单下歌曲*/
char * get_musicInfo_musicSheetId(char * musicSheetId, int startNum , int endNum ,char *picSize,char *result);
/*查询音乐歌单*/
char * get_musicSheet(char * typeId, char *result);
/*获取专辑下歌曲*/
char * get_albumInfo(char * albumId,  char * pageNumber, char * numberPerPage,char *picSize, char *result);
/*关键字搜索*/
char * get_musicInfo_newKey(char * text, char * type, int  pageIndex, int  pageSize, int searchType, int issemantic, int isCorrect, char *searchRange, char *result);
/*音响搜歌*/
char * get_BoxSearch( boxSearchPostData_t sPostData, char *result);
/*获取电台信息*/
int get_radio_info(char *result);
/*智能硬件分群推荐*/
char * get_recommendByGroup(char *spallId, char *ageType, char *sex,char *timeRange,char *tep,char *weather,char *result);
/*FM播放列表*/
char * get_fm_list (char *result);

//=========================================================
//==================自定义歌单相关操作接口=================
//=========================================================
/*自定义歌单相关操作*/
char * get_collection_creatSheet(char * musicIds ,char * sheetName  ,char *result);
/*查询自定义收藏歌单*/
char * get_collection_sheet(int startNum ,int endNum ,char *result);
/*删除自定义收藏歌单*/
char * get_collection_delSheet(char * sheetId,char *result);
/*修改自定义收藏歌单*/
char * get_collection_modifySheet(char * sheetId,char * sheetName,char *result);
/*添加音乐收藏*/
char * get_collection_addMusicSheet(char * sheetId,char * musicIds,char *result);
/*获取音乐收藏（分页查询）*/
char * get_collection_musicSheet(char * sheetId,int startNum,int endNum,char *picSize,char *result);
/*取消音乐收藏*/
char * get_collection_delMusicSheet(char * sheetId,char * musicIds,char *result);

//=========================================================
//==================排行榜相关操作接口=====================
//=========================================================
/*获取榜单信息*/
char * get_music_rank(char *result);
/*获取新歌速递*/
char * get_music_newsong(char *result);
/*获取新碟速递*/
char * get_music_newalbum(char *result);
/*获取咪咕音乐客户端歌单信息*/
char * get_music_playlist(char *result);
/*查询咪咕音乐歌单下内容*/
char * get_music_playlistId(char * playListId, char *result);

//=========================================================
//=======================智能推荐接口======================
//=========================================================
/*智能终端个性化推荐，获取推荐的歌曲列表*/
char * get_GatewayRecommend(char * spallId, char *result);
/*试听行为上传*/
char * get_uploadBehavior(listeningBehavior_t sListeningBehavior,char *result);
/*获取推荐的相似歌曲*/
int get_alike_music(char * musicId,char *result);

//=========================================================
//=======================设备信息查询接口==================
//=========================================================
/*根据智能设备唯一识别号查询智能设备信息*/
char * get_device_info(char *key , char *result);
/*获取白金会员办理信息*/
char * get_member(char *result);


//=========================================================
//========================其他接口=========================
//=========================================================
/*获取歌曲的业务策略*/
char * get_policy(char * musicId,char * resource,char *result);
/*获取歌曲下载地址,目前只能下载策略中标示免费的歌曲*/
char * get_download(char * musicId,char * resource,char * bizCode ,char * bizType ,char *result);

#endif
