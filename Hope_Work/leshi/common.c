/*
 * 
 * 　　┏┓　　　┏┓+ +
 * 　┏┛┻━━━┛┻┓ + +
 * 　┃　　　　　　　┃ 　
 * 　┃　　　━　　　┃ ++ + + +
 *  ████━████ ┃+
 * 　┃　　　　　　　┃ +
 * 　┃　　　┻　　　┃
 * 　┃　　　　　　　┃ + +
 * 　┗━┓　　　┏━┛
 * 　　　┃　　　┃　　　　　　　　　　　
 * 　　　┃　　　┃ + + + +
 * 　　　┃　　　┃
 * 　　　┃　　　┃ +  神兽保佑
 * 　　　┃　　　┃    代码无bug　　
 * 　　　┃　　　┃　　+　　　　　　　　　
 * 　　　┃　 　　┗━━━┓ + +
 * 　　　┃ 　　　　　　　┣┓
 * 　　　┃ 　　　　　　　┏┛
 * 　　　┗┓┓┏━┳┓┏┛ + + + +
 * 　　　　┃┫┫　┃┫┫
 * 　　　　┗┻┛　┗┻┛+ + + +
 * 
 * 
 * 
 * ************Copyright 2023 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2023-03-21 14:51:55
 * @LastEditors  : MCD
 * @LastEditTime : 2023-04-04 10:20:19
 * @FilePath     : /My_C_Test/Hope_Work/leshi/common.c
 * @Description  : 
 * 
 * ******************************************
 */

#include "common.h"
#include <ctype.h>
#include <errno.h>
#include <net/if.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <time.h>
// #include <uci.h>

/*********************************************************************
* MACROS
*/
#ifndef DEBUG_ERROR
#define DEBUG_ERROR printf
#endif

/*********************************************************************
* LOCAL VARIABLE
*/
#if 0
static leveldb_t *db_handle = NULL;
static leveldb_cache_t *db_cache = NULL;
static leveldb_env_t *db_env = NULL;
static leveldb_options_t *options = NULL;
static leveldb_readoptions_t *roptions = NULL;
static leveldb_writeoptions_t *woptions = NULL;
#endif

/*********************************************************************
* GLOBAL VARIABLES
*/

/*
*CRC 余式表
*/
static unsigned short crc16_ccitt_table[256] =
    {
        0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
        0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
        0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
        0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
        0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
        0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
        0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
        0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
        0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
        0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
        0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
        0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
        0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
        0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
        0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
        0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
        0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
        0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
        0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
        0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
        0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
        0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
        0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
        0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
        0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
        0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
        0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
        0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
        0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
        0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
        0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
        0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0};

/*********************************************************************
* LOCAL FUNCTIONS
*/

/*********************************************************************
 * @fn      setTimer

 * @brief
 *   This function is called to start a timer to expire in n mSecs then call callback func.
 *
 * @param   timeout_value - in milliseconds.
 * @param   repeat - true for repeat when timer expires,false for expiration once.
 * @param   func - callback function
 * @param   *param - passed callback function
 *
 * @return  timer handle, or NULL failure.
 */
timer_t setTimer(size_t timeout_value, unsigned int repeat, _function func, void *param)
{
    timer_t timer_id = 0;
    #if 0
    struct sigevent sevp;
    struct itimerspec it;
    time_t tv_sec;
    long tv_nsec;

    if (timeout_value == 0) {
        return NULL;
    }

    memset(&sevp, 0, sizeof(sevp));
    memset(&it, 0, sizeof(it));
    sevp.sigev_value.sival_ptr = param;  //param pass to func
    sevp.sigev_notify = SIGEV_THREAD;
    sevp.sigev_notify_function = func;
    if (timer_create(CLOCK_MONOTONIC, &sevp, &timer_id) < 0) {
        fprintf(stderr, "timer_create errno %d,%s\n", errno, strerror(errno));
        return NULL;
    }

    tv_sec = timeout_value / 1000;                   // seconds
    tv_nsec = (timeout_value % 1000) * 1000 * 1000;  // milliseconds to Nanoseconds

    if (repeat != 0) {
        /* interval */
        it.it_interval.tv_sec = tv_sec;
        it.it_interval.tv_nsec = tv_nsec;  //change milliseconds to Nanoseconds
    }

    /* first expiration */
    it.it_value.tv_sec = tv_sec;
    it.it_value.tv_nsec = tv_nsec;

    if (timer_settime(timer_id, 0, &it, NULL) < 0) {
        timer_delete(timer_id);
        fprintf(stderr, "timer_settime errno %d,%s\n", errno, strerror(errno));
        return NULL;
    }
    #endif
    return timer_id;
}

/*********************************************************************
 * @fn      killTimer

 * @brief
 *
 * @param   timer_id return by setTimer
 *
 * @return  0 success, or -1 failure.
 */
int killTimer(timer_t *timer_id)
{
    #if 0
    if (*timer_id == NULL) {
        return -1;
    }
    if (timer_delete(*timer_id) < 0) {
        fprintf(stderr, "timer_delete errno %d,%s\n", errno, strerror(errno));
        return -1;
    }

    *timer_id = NULL;
    #endif
    return 0;
}

/*********************************************************************
 * @fn      resetTimer

 * @brief   reset the timer interval to new timeout_value
 *
 * @param   timer_id return by setTimer
 *
 * @return  0 success, or -1 failure.
 */
int resetTimer(timer_t *timer_id, size_t timeout_value)
{
    int ret = 0;
    #if 0
    time_t tv_sec;
    long tv_nsec;
    struct itimerspec it;

    memset(&it, 0, sizeof(it));

    tv_sec = timeout_value / 1000;                   // seconds
    tv_nsec = (timeout_value % 1000) * 1000 * 1000;  // milliseconds to Nanoseconds

    if (timer_gettime(*timer_id, &it) < 0) {
        fprintf(stderr, "timer_gettime errno %d,%s\n", errno, strerror(errno));
        return -1;
    }

    if (it.it_interval.tv_sec > 0 ||
        it.it_interval.tv_nsec > 0) {
        /* reset interval */
        it.it_interval.tv_sec = tv_sec;
        it.it_interval.tv_nsec = tv_nsec;  //change milliseconds to Nanoseconds
        if (timer_settime(*timer_id, 0, &it, NULL) < 0) {
            timer_delete(*timer_id);
            ret = -1;
        }
    }
    else if (it.it_value.tv_sec > 0 ||
             it.it_value.tv_nsec > 0) {
        /* reset init */
        it.it_value.tv_sec = tv_sec;
        it.it_value.tv_nsec = tv_nsec;  //change milliseconds to Nanoseconds
        if (timer_settime(*timer_id, 0, &it, NULL) < 0) {
            timer_delete(*timer_id);
            ret = -1;
        }
    }
    else {
        fprintf(stderr, "timer interval/init invalid\n");
        ret = -1;
    }
    #endif
    return ret;
}

void select_sleep(int seconds)
{
    struct timeval timeout;
    timeout.tv_sec = seconds;
    timeout.tv_usec = 0;
    select(0, NULL, NULL, NULL, &timeout);
}

void select_usleep(int micro_seconds)
{
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = micro_seconds;
    select(0, NULL, NULL, NULL, &timeout);
}

#if 0
int uci_config_get(char *path, char *package, char *section, char *option, char *pdata, size_t plen)
{
    int ret = UCI_OK;
    struct uci_context *ctx = NULL;
    struct uci_package *pkg = NULL;

    if (package == NULL || section == NULL || option == NULL || pdata == NULL || plen == 0) {
        return UCI_ERR_INVAL;
    }

    ctx = uci_alloc_context();
    if (!ctx) {
        return UCI_ERR_MEM;
    }

    if (path) {
        uci_set_confdir(ctx, path);
    }

    ret = uci_load(ctx, package, &pkg);
    if (ret != UCI_OK) {
        ret = UCI_ERR_INVAL;
        goto Cleanup;
    }

    struct uci_section *s = uci_lookup_section(ctx, pkg, section);
    if (s != NULL) {
        const char *value = uci_lookup_option_string(ctx, s, option);
        if (value != NULL) {
            strncpy(pdata, value, plen);
        }
        else {
            ret = UCI_ERR_NOTFOUND;
            goto Cleanup;
        }
    }
    else {
        ret = UCI_ERR_NOTFOUND;
        goto Cleanup;
    }

Cleanup:
    if (pkg) {
        uci_unload(ctx, pkg);
    }
    uci_free_context(ctx);
    ctx = NULL;
    return ret;
}

int uci_config_set(char *path, char *package, char *section, char *option, char *pdata)
{
    int ret = UCI_OK;
    struct uci_context *ctx = NULL;

    ctx = uci_alloc_context();
    if (!ctx) {
        return UCI_ERR_MEM;
    }

    if (path) {
        uci_set_confdir(ctx, path);
    }

    struct uci_ptr ptr =
        {
            .package = package,
            .section = section,
            .option = option,
            .value = pdata,
        };

    ret = uci_set(ctx, &ptr);
    if (ret != UCI_OK) {
        ret = UCI_ERR_NOTFOUND;
        goto Cleanup;
    }

    ret = uci_save(ctx, ptr.p);
    if (ret != UCI_OK) {
        ret = UCI_ERR_INVAL;
        goto Cleanup;
    }

    ret = uci_commit(ctx, &ptr.p, false);
    if (ret != UCI_OK) {
        ret = UCI_ERR_IO;
        goto Cleanup;
    }

Cleanup:
    uci_free_context(ctx);
    return ret;
}

int kv_config_init(const char *dbname)
{
    int ret = 0;
    char *err = NULL;
    if (leveldb_major_version() < 1 || leveldb_minor_version() < 1) {
        DEBUG_ERROR("leveldb version not compatible\n");
        return -1;
    }
    db_env = leveldb_create_default_env();
    db_cache = leveldb_cache_create_lru(100000);

    options = leveldb_options_create();
    leveldb_options_set_error_if_exists(options, 0);
    leveldb_options_set_cache(options, db_cache);
    leveldb_options_set_env(options, db_env);
    leveldb_options_set_info_log(options, NULL);
    leveldb_options_set_write_buffer_size(options, 100000);
    leveldb_options_set_paranoid_checks(options, 1);
    leveldb_options_set_max_open_files(options, 10);
    leveldb_options_set_block_size(options, 1024);
    leveldb_options_set_block_restart_interval(options, 8);
    leveldb_options_set_max_file_size(options, 3 << 20);
    leveldb_options_set_compression(options, leveldb_no_compression);

    roptions = leveldb_readoptions_create();
    leveldb_readoptions_set_verify_checksums(roptions, 1);
    leveldb_readoptions_set_fill_cache(roptions, 0);

    woptions = leveldb_writeoptions_create();
    leveldb_writeoptions_set_sync(woptions, 1);

    /*  destroy previous database data */
    //leveldb_destroy_db(options, dbname, &err);
    //if(err)
    //leveldb_free(err);

    leveldb_options_set_create_if_missing(options, 1);
    db_handle = leveldb_open(options, dbname, &err);
    if (err != NULL) {
        DEBUG_ERROR("kv open failure:%s\n", err);
        leveldb_free(err);

        //attempt repair (Initialize 2022-01-04)
        leveldb_close(db_handle);
        leveldb_options_set_create_if_missing(options, 0);
        leveldb_repair_db(options, dbname, &err);
        if (err != NULL) {
            DEBUG_ERROR("kv repair failure:%s\n", err);
            leveldb_free(err);

            //give up repair,we have to destory
            leveldb_destroy_db(options, dbname, &err);
            if (err != NULL) {
                DEBUG_ERROR("kv destory failure:%s\n", err);
                leveldb_free(err);
                ret = -1;
            }
            else {
                leveldb_options_set_create_if_missing(options, 1);
                db_handle = leveldb_open(options, dbname, &err);
                if (err != NULL) {
                    DEBUG_ERROR("kv destory success,reopen failure:%s\n", err);
                    leveldb_free(err);
                    ret = -1;
                }
            }
        }
        else {
            leveldb_options_set_create_if_missing(options, 1);
            db_handle = leveldb_open(options, dbname, &err);
            if (err != NULL) {
                DEBUG_ERROR("kv repair success,reopen failure:%s\n", err);
                leveldb_free(err);
                ret = -1;
            }
        }
    }

    return ret;
}

int kv_config_set(const char *key, const char *v, size_t vlen)
{
    int ret = 0;
    if (key == NULL || v == NULL || vlen == 0)
        return -1;
    if (db_handle && woptions) {
        char *err = NULL;
        leveldb_put(db_handle, woptions, key, strlen(key), v, vlen, &err);
        if (err != NULL) {
            DEBUG_ERROR("%s\n", err);
            leveldb_free(err);
            ret = -1;
        }
    }
    else {
        DEBUG_ERROR("need call init_configEx firstly\n");
        ret = -1;
    }
    return ret;
}

int kv_config_get(const char *key, char *v, size_t vlen)
{
    int ret = -1;
    size_t val_len = 0;
    if (key == NULL || v == NULL || vlen == 0)
        return -1;

    if (db_handle && roptions) {
        char *err = NULL;
        char *val = leveldb_get(db_handle, roptions, key, strlen(key), &val_len, &err);
        if (err != NULL) {
            ret = -1;
            DEBUG_ERROR("%s\n", err);
            leveldb_free(err);
        }
        else {
            if (val && val_len > 0) {
                val_len = (val_len <= vlen) ? val_len : vlen;
                memcpy(v, val, val_len);
                ret = 0;
            }
        }
        if (val)
            leveldb_free(val);
    }
    else {
        DEBUG_ERROR("need call init_configEx firstly\n");
        ret = -1;
    }

    return ret;
}

int kv_config_reset(const char *key)
{
    int ret = 0;
    if (key == NULL)
        return -1;

    if (db_handle && woptions) {
        char *err = NULL;
        leveldb_delete(db_handle, woptions, key, strlen(key), &err);
        if (err != NULL) {
            ret = -1;
            DEBUG_ERROR("%s\n", err);
            leveldb_free(err);
        }
    }
    else {
        DEBUG_ERROR("need call init_configEx firstly\n");
        ret = -1;
    }
    return ret;
}

int kv_config_exit(void)
{
    if (db_handle) {
        leveldb_close(db_handle);
        db_handle = NULL;
    }
    if (options) {
        leveldb_options_destroy(options);
        options = NULL;
    }
    if (roptions) {
        leveldb_readoptions_destroy(roptions);
        roptions = NULL;
    }
    if (woptions) {
        leveldb_writeoptions_destroy(woptions);
        woptions = NULL;
    }
    if (db_cache) {
        leveldb_cache_destroy(db_cache);
        db_cache = NULL;
    }
    if (db_env) {
        leveldb_env_destroy(db_env);
        db_env = NULL;
    }
    return 0;
}
#endif

int get_devIntAttr_value(char *attrValue, int attrValueLen)
{
    int ret = 0;

    // we do hardcode for workaround
    switch (attrValueLen) {
        case 1:
            ret = attrValue[0];
            break;

        case 2:
            ret = (uint16_t)BUILD_UINT16(attrValue[1], attrValue[0]);
            break;

        case 4:
            ret = (int)BUILD_UINT32(attrValue[3], attrValue[2], attrValue[1], attrValue[0]);
            break;

        default:
            break;
    }

    return ret;
}

char *get_devStringAttr_value(char *attrValue, int attrValueLen)
{
    char *value = NULL;
    if (attrValueLen > 0) {
        value = (char *)calloc(attrValueLen + 1, sizeof(char));
        if (value) {
            memcpy(value, attrValue, attrValueLen);
        }
    }

    return value;
}

/* Derived from original code by CodesInChaos 
** convert binary array to HEX format string
** @param upper_case ,TRUE to upper case HEX string,FALSE to lower case HEX string
*/
char *
bin2hex_s(char *const hex, const size_t hex_maxlen, const unsigned char *const bin, const size_t bin_len, int upper_case)
{
    size_t i = (size_t)0U;
    unsigned int x;
    int b;
    int c;
    unsigned int base = 0;
    unsigned int mm = 0;

    if (upper_case) {
        base = 55U;
        mm = 262U;
    }
    else {
        base = 87U;
        mm = 38U;
    }

    if (bin_len >= SIZE_MAX / 2 || hex_maxlen <= bin_len * 2U) {
        return NULL;
    }
    while (i < bin_len) {
        c = bin[i] & 0xf;
        b = bin[i] >> 4;

        x = (unsigned char)(base + c + (((c - 10U) >> 8) & ~mm)) << 8 |
            (unsigned char)(base + b + (((b - 10U) >> 8) & ~mm));

        hex[i * 2U] = (char)x;
        x >>= 8;
        hex[i * 2U + 1U] = (char)x;
        i++;
    }
    hex[i * 2U] = 0U;

    return hex;
}

/*
** convert HEX format string to binary array
** @param
*/
int hex2bin_s(unsigned char *const bin, const size_t bin_maxlen, const char *const hex, const size_t hex_len, const char *const ignore, size_t *const bin_len, const char **const hex_end)
{
    size_t bin_pos = (size_t)0U;
    size_t hex_pos = (size_t)0U;
    int ret = 0;
    unsigned char c;
    unsigned char c_acc = 0U;
    unsigned char c_alpha0, c_alpha;
    unsigned char c_num0, c_num;
    unsigned char c_val;
    unsigned char state = 0U;

    while (hex_pos < hex_len) {
        c = (unsigned char)hex[hex_pos];
        c_num = c ^ 48U;
        c_num0 = (c_num - 10U) >> 8;
        c_alpha = (c & ~32U) - 55U;
        c_alpha0 = ((c_alpha - 10U) ^ (c_alpha - 16U)) >> 8;
        if ((c_num0 | c_alpha0) == 0U) {
            if (ignore != NULL && state == 0U && strchr(ignore, c) != NULL) {
                hex_pos++;
                continue;
            }
            break;
        }
        c_val = (c_num0 & c_num) | (c_alpha0 & c_alpha);
        if (bin_pos >= bin_maxlen) {
            ret = -1;
            errno = ERANGE;
            break;
        }
        if (state == 0U) {
            c_acc = c_val * 16U;
        }
        else {
            bin[bin_pos++] = c_acc | c_val;
        }
        state = ~state;
        hex_pos++;
    }
    if (state != 0U) {
        hex_pos--;
        errno = EINVAL;
        ret = -1;
    }
    if (ret != 0) {
        bin_pos = (size_t)0U;
    }
    if (hex_end != NULL) {
        *hex_end = &hex[hex_pos];
    }
    else if (hex_pos != hex_len) {
        errno = EINVAL;
        ret = -1;
    }
    if (bin_len != NULL) {
        *bin_len = bin_pos;
    }
    return ret;
}

char *strlwr(char *s)
{
    char *tmp = s;

    for (; *tmp; ++tmp) {
        *tmp = tolower((unsigned char)*tmp);
    }

    return s;
}

char *strupr(char *s)
{
    char *tmp = s;

    for (; *tmp; ++tmp) {
        *tmp = toupper((unsigned char)*tmp);
    }

    return s;
}

int hexStr2bytes(const uint8_t *hexStr, uint8_t *buf, int bufLen)
{
    int i;
    int len;

    if (NULL == hexStr) {
        len = 0;
    }
    else {
        len = (int)strlen((const char *)hexStr) / 2;

        if (bufLen < len) {
            len = bufLen;
        }
    }

    for (i = 0; i < len; i++) {
        char ch1, ch2;
        int val;

        ch1 = hexStr[i * 2];
        ch2 = hexStr[i * 2 + 1];
        if (ch1 >= '0' && ch1 <= '9') {
            val = (ch1 - '0') * 16;
        }
        else if (ch1 >= 'a' && ch1 <= 'f') {
            val = ((ch1 - 'a') + 10) * 16;
        }
        else if (ch1 >= 'A' && ch1 <= 'F') {
            val = ((ch1 - 'A') + 10) * 16;
        }
        else {
            return -1;
        }

        if (ch2 >= '0' && ch2 <= '9') {
            val += ch2 - '0';
        }
        else if (ch2 >= 'a' && ch2 <= 'f') {
            val += (ch2 - 'a') + 10;
        }
        else if (ch2 >= 'A' && ch2 <= 'F') {
            val += (ch2 - 'A') + 10;
        }
        else {
            return -1;
        }

        buf[i] = val & 0xff;
    }

    return 0;
}

int bytes2hexStr(const uint8_t *buf, char *hexStr, int bufLen)
{
    int i;

    for (i = 0; i < bufLen; i++) {
        snprintf(hexStr + 2 * i, 3, "%02x", buf[i]);
    }
    *(char *)(hexStr + 2 * i + 1) = '\0';

    return i;
}

/**
* @author  		MCD
* @date  		2021-08-24-11:07
* @details		CRC crc16_ccitt函数
*/
unsigned short crc16_ccitt_tab(unsigned char *pbuf, size_t len)
{
    unsigned short crc = CRC16_CCITT_SEED;

    while (len-- > 0) {
        crc = crc16_ccitt_table[(crc >> 8 ^ *pbuf++) & 0xff] ^ (crc << 8);
    }

    return crc;
}

unsigned short crc16_ccitt(unsigned char *puchMsg, unsigned int usDataLen)
{
    unsigned short wCRCin = CRC16_CCITT_SEED;
    unsigned short wCPoly = CRC16_CCITT_POLY16;
    unsigned char wChar = 0;
    int i;

    while (usDataLen--) {
        wChar = *(puchMsg++);
        wCRCin ^= (wChar << 8);
        for (i = 0; i < 8; i++) {
            if (wCRCin & 0x8000)
                wCRCin = (wCRCin << 1) ^ wCPoly;
            else
                wCRCin = wCRCin << 1;
        }
    }
    return (wCRCin);
}

int my_itoa(uint8_t *buf, uint32_t value, size_t size)
{
    uint8_t *p = buf;
    size_t i = 1;

    if (buf == NULL || size <= 0)
        return -1;
    for (i = 1; i <= size; i++) {
        *p = value >> (8 * (size - i));
        p++;
    }
    return 0;
}

uint32_t bytetol(uint8_t *buffer, size_t size)
{
    uint32_t data = 0;
    size_t i = 0;

    for (i = 0; i < size; i++) {
        data = data << 8;
        data = data | buffer[i];
        //
    }
    // printf("Running time: %x\n", data);
    return data;
}

void ltohex(uint32_t num, uint8_t *data, size_t len)
{
    int i = len - 1;
    for (; i >= 0; i--) {
        data[i] = (uint8_t)((num >> (i * 8)) & 0xFF);
        // printf("(%ld)%02x ",i,  data[i]);
    }
    // printf("\n");
}

void hextodecstring(uint8_t *src_data, size_t len, uint8_t *dest_data)
{
    int offset = 0;
    int i = 0;

    for (i = 0; i < len; i++) {
        offset += snprintf((char *)(dest_data + offset), sizeof(dest_data), "%02d", src_data[i]);  // 格式化的数据写入字符串
    }
}

void dump_rs485_data(uint8_t *data, size_t size)
{
    int i = 0;
    printf("dump data: ");
    if (data == NULL || size == 0)
        return;
    for (i = 0; i < size; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

int sys_get_mac(char *mac, size_t maclen)
{
    int ret = 0;
    struct ifreq ifr_mac;
    int sock;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    strcpy(ifr_mac.ifr_name, "eth0");  //get eth0
    if (ioctl(sock, SIOCGIFHWADDR, &ifr_mac) < 0) {
        strcpy(ifr_mac.ifr_name, "wlan0");  //get wlan0
        if (ioctl(sock, SIOCGIFHWADDR, &ifr_mac) < 0) {
            ret = -1;
            goto Cleanup;
        }
    }

    snprintf(mac, maclen, "%.02X%.02X%.02X%.02X%.02X%.02X%.02X%.02X",
             (unsigned char)ifr_mac.ifr_hwaddr.sa_data[0],
             (unsigned char)ifr_mac.ifr_hwaddr.sa_data[1],
             (unsigned char)ifr_mac.ifr_hwaddr.sa_data[2],
             (unsigned char)ifr_mac.ifr_hwaddr.sa_data[3],
             (unsigned char)ifr_mac.ifr_hwaddr.sa_data[4],
             (unsigned char)ifr_mac.ifr_hwaddr.sa_data[5],
             (unsigned char)ifr_mac.ifr_hwaddr.sa_data[6],
             (unsigned char)ifr_mac.ifr_hwaddr.sa_data[7]);
    // size_t len = sizeof(ifr_mac.ifr_hwaddr.sa_data) > maclen ? maclen : sizeof(ifr_mac.ifr_hwaddr.sa_data);
    // memcpy(mac, ifr_mac.ifr_hwaddr.sa_data, len);

Cleanup:
    close(sock);
    return ret;
}

/**
 * @brief need free
 * 
 * @param src 
 * @param ssize 
 * @return uint8_t* 
 */
uint8_t *dump_rs485_data1(uint8_t *src, size_t ssize)
{
    int i = 0;
    int offset = 0;
    uint8_t *dst;
    size_t dsize;

    if (src == NULL || ssize <= 0)
        return NULL;

    dst = (uint8_t *)calloc(ssize * 5 + 1, sizeof(uint8_t));
    if (!dst)
        return NULL;
    dsize = ssize * 5 + 1;

    for (i = 0; i < ssize; i++) {
        offset += snprintf((char *)(dst + offset), dsize, " 0x%02x", src[i]);  // 格式化的数据写入字符串
    }

    return dst;
}

uint8_t check_sum_ls(uint8_t *src, size_t ssize)
{
    uint32_t sum = 0;

    while (ssize--) {
        sum += *src++;
    }

    return sum % 256;
}


void hsv2rgb(float h, float s, float v, int* r, int* g, int* b) 
{
    float c = s * v;  // 饱和度 * 明度
    float x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));  // 计算中间变量
    float m = v - c;  // 计算中间变量

    if (h >= 0 && h < 60) {
        *r = round((c + m) * 255);  // 计算红色分量
        *g = round((x + m) * 255);  // 计算绿色分量
        *b = round((0 + m) * 255);  // 计算蓝色分量
    } else if (h >= 60 && h < 120) {
        *r = round((x + m) * 255);
        *g = round((c + m) * 255);
        *b = round((0 + m) * 255);
    } else if (h >= 120 && h < 180) {
        *r = round((0 + m) * 255);
        *g = round((c + m) * 255);
        *b = round((x + m) * 255);
    } else if (h >= 180 && h < 240) {
        *r = round((0 + m) * 255);
        *g = round((x + m) * 255);
        *b = round((c + m) * 255);
    } else if (h >= 240 && h < 300) {
        *r = round((x + m) * 255);
        *g = round((0 + m) * 255);
        *b = round((c + m) * 255);
    } else {
        *r = round((c + m) * 255);
        *g = round((0 + m) * 255);
        *b = round((x + m) * 255);
    }
}

void rgb2hsv(int r, int g, int b, float* h, float* s, float* v) 
{
    float red = (float)r / 255;
    float green = (float)g / 255;
    float blue = (float)b / 255;

    float max = red > green ? red : green;
    max = max > blue ? max : blue;

    float min = red < green ? red : green;
    min = min < blue ? min : blue;

    float delta = max - min;

    if (delta == 0) {  // 灰度图像
        *h = 0;
        *s = 0;
        *v = max;
    } else {
        if (max == red) {
            *h = (green - blue) / delta;
        } else if (max == green) {
            *h = 2 + (blue - red) / delta;
        } else {
            *h = 4 + (red - green) / delta;
        }

        *h *= 60;  // 转换为角度值

        if (*h < 0) {
            *h += 360;
        }

        *s = delta / max;
        *v = max;
    }
}