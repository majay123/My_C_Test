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
 * ************Copyright 2022 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2022-03-03 09:38:03
 * @LastEditors  : MCD
 * @LastEditTime : 2022-11-15 11:31:18
 * @FilePath     : /My_C_Test/serial_test/serial_common.c
 * @Description  : 
 * 
 * ******************************************
 */
#include "serial_common.h"

static int _serial_set_parity(int fd, int speed, int databits, int stopbits, int parity)
{
    struct termios tio, oldtio;
    int i;
    int status;

    int speed_arr[] = {B115200, B19200, B9600, B4800, B2400, B1200, B300};
    int name_arr[] = {115200, 19200, 9600, 4800, 2400, 1200, 300};

    /*保存测试现有串口参数设置，在这里如果串口号等出错，会有相关的出错信息*/
    if (tcgetattr(fd, &oldtio) != 0) {
        printf("SetupSerial 1\n");
        return -1;
    }

    bzero(&tio, sizeof(tio));
    cfmakeraw(&tio); /*see man page */
                     /* 设置字符大小 */
    tio.c_cflag |= CLOCAL | CREAD;

    tio.c_iflag |= IGNPAR; /*ignore parity on input */
    tio.c_oflag &= ~(OPOST | ONLCR | OLCUC | OCRNL | ONOCR | ONLRET | OFILL);

    /* set 波特率 */
    for (i = 0; i < sizeof(speed_arr) / sizeof(int); i++) {
        if (speed == name_arr[i]) {
            cfsetispeed(&tio, speed_arr[i]);
            cfsetospeed(&tio, speed_arr[i]);
        }
    }

    switch (databits) {
        case 7:
            tio.c_cflag |= CS7;
            break;
        case 8:
            tio.c_cflag |= CS8;
            break;
        default:
            printf("unsupported databits: %d\n", databits);
            return -1;
    }

    switch (parity) {
        case 'n':
        case 'N':
            tio.c_cflag &= ~PARENB; /* Clear parity enable */
            tio.c_iflag &= ~INPCK;  /* Enable parity checking */
            break;
        case 'o':
        case 'O':
            tio.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/
            tio.c_iflag |= INPCK;             /* Disnable parity checking */
            break;
        case 'e':
        case 'E':
            tio.c_cflag |= PARENB;  /* Enable parity */
            tio.c_cflag &= ~PARODD; /* 转换为偶效验*/
            tio.c_iflag |= INPCK;   /* Disnable parity checking */
            break;
        case 'S':
        case 's': /*as no parity*/
            tio.c_cflag &= ~PARENB;
            tio.c_cflag &= ~CSTOPB;
            break;
        default:
            printf("Unsupported parity\n");
            return -1;
    }

    /* 设置停止位*/
    switch (stopbits) {
        case 1:
            tio.c_cflag &= ~CSTOPB;
            break;
        case 2:
            tio.c_cflag |= CSTOPB;
            break;
        default:
            printf("Unsupported stop bits\n");
            return -1;
    }

    tio.c_cc[VMIN] = 1;  /* block until 1 char received */
    tio.c_cc[VTIME] = 0; /*no inter-character timer */

    tcflush(fd, TCIFLUSH);
    if (tcsetattr(fd, TCSANOW, &tio) != 0) {
        printf("SetupSerial 3\n");
        return -1;
    }

    return 0;
}

int serial_init_open(char *dev_path, int speed, int databits, int stopbits, int parity)
{
#if 0
    int fd = 0;

    fd = open(dev_path, O_RDWR | O_NOCTTY); // block read/write
    if (fd < 0) {
        printf("%s device open failed", dev_path);
        return -1;
    }

    if (_serial_set_parity(fd, speed, databits, stopbits, parity) < 0) {
        printf("serial set parity failed");
        return -1;
    }
#else
    struct termios tio, oldtio;
    int fd = 0;

    /* open the device */
    fd = open(dev_path, O_RDWR | O_NOCTTY);  //block read/write
    if (fd < 0) {
        printf("%s device open failed\n", dev_path);
        return (-1);
    }

    /* c-iflags
     B115200 : set board rate to 115200
     CRTSCTS : HW flow control
     CS8     : 8n1 (8bit,no parity,1 stopbit)
     CLOCAL  : local connection, no modem contol
     CREAD   : enable receiving characters*/

    /* configure new values */
    cfmakeraw(&tio); /*see man page */

    tio.c_iflag |= IGNPAR; /*ignore parity on input */
    tio.c_iflag &= ~INPCK;
    tio.c_oflag &= ~(OPOST | ONLCR | OLCUC | OCRNL | ONOCR | ONLRET | OFILL);
    tio.c_cflag = CS8 | CLOCAL | CREAD;
    tio.c_cflag &= ~(PARENB | CSTOPB);
    tio.c_cc[VMIN] = 1;  /* block until 1 char received */
    tio.c_cc[VTIME] = 0; /*no inter-character timer */

    cfsetospeed(&tio, B9600);
    cfsetispeed(&tio, B9600);

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &tio);

#endif
    return fd;
}

int serial_read_data(int fd, char *rcv_buf, int data_len)
{
    int value = 0;
    int readBytes = 0;
    fd_set fs;
    int timeout = 1000;
    struct timeval tv;
    int notRead = data_len;
    int bytes_read = 0;

    if (fd <= 0) {
        return -1;
    }

    while (notRead > 0) {
        FD_ZERO(&fs);
        FD_SET(fd, &fs);

        //reinitialized time
        tv.tv_sec = (timeout / 1000);
        tv.tv_usec = (timeout % 1000) * 1000L;

        value = select(fd + 1, &fs, NULL, NULL, &tv);
        if (-1 == value) {
            printf("value = -1\n");
            return -1;
        }
        else if (value) {
            if (FD_ISSET(fd, &fs)) {
                readBytes = read(fd, rcv_buf + bytes_read, notRead);
                if (readBytes > 0) {
                    // printf("readBytes = %d\n", readBytes);
                    bytes_read += readBytes;
                    notRead -= readBytes;
                }
                else if (readBytes == 0) {
                    printf("readBytes = 0\n");
                    return -1;
                }
                else if (readBytes == -1) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
                        // no data, wait for read
                        continue;
                    }
                    else {
                        printf("read bytes error\n");
                        return -1;
                    }
                }
            }
            else {
                // printf("FD_ISSET error\n");
                return -1;
            }
        }
        else if (value == 0) {
            // printf("select time out\n");
            return 0;  //select time out
        }
    }
    // printf("bytes read = %d\n",bytes_read);
    return bytes_read;
}

int serial_write_data(int fd, char *send_buf, int data_len)
{
    int ret = 0;
    int remain = data_len;
    int offset = 0;
    int sub = 0, writeBytes = 0;

    if (fd < 0) {
        return -1;
    }
    while (remain > 0) {
        sub = (remain >= 8 ? 8 : remain);
        writeBytes = write(fd, send_buf + offset, sub);
        /*cflush(fd, TCOFLUSH);*/
        if (writeBytes > 0) {
            remain -= writeBytes;
            offset += writeBytes;
        }
        else if (writeBytes == -1) {
            ret = -1;
            break;
        }
    }

    return ret;
}

/* 3 steps are used to parse the query */
typedef enum {
    _STEP_FUNCTION,
    _STEP_CTRL,
    _STEP_DEV_NUM,
    _STEP_DATA
} _step_t;

/**
 * @brief 
 * 
 * @param data 
 * @param len 
 * @return uint8_t 
 */
uint8_t sum_check(const uint8_t *data, int len)
{
    int i;
    uint8_t sum = 0;

    for (i = 0; i < len; i++) {
        sum += data[i];
    }

    return sum;
}

static const char *maisi_rs485_strerror(int errnum)
{
    // switch (errnum) {
    //     case EMBXILFUN:
    //         return "Illegal function";
    //     case EMBXILADD:
    //         return "Illegal data address";
    //     case EMBXILVAL:
    //         return "Illegal data value";
    //     case EMBXSFAIL:
    //         return "Slave device or server failure";
    //     case EMBXACK:
    //         return "Acknowledge";
    //     case EMBXSBUSY:
    //         return "Slave device or server is busy";
    //     case EMBXNACK:
    //         return "Negative acknowledge";
    //     case EMBXMEMPAR:
    //         return "Memory parity error";
    //     case EMBXGPATH:
    //         return "Gateway path unavailable";
    //     case EMBXGTAR:
    //         return "Target device failed to respond";
    //     case EMBBADCRC:
    //         return "Invalid CRC";
    //     case EMBBADDATA:
    //         return "Invalid data";
    //     case EMBBADEXC:
    //         return "Invalid exception code";
    //     case EMBMDATA:
    //         return "Too many data";
    //     case EMBBADSLAVE:
    //         return "Response not from requested slave";
    //     default:
    //         return strerror(errnum);
    // }

    return "something error";
}

static void _maisi_rs485_error_print(const char *context)
{
    if (1) {
        fprintf(stderr, "ERROR %s", maisi_rs485_strerror(errno));
        if (context != NULL) {
            fprintf(stderr, ": %s\n", context);
        }
        else {
            fprintf(stderr, "\n");
        }
    }
}

static int _maisi_rs485_check_integrity(uint8_t *msg, const int msg_length)
{
    uint8_t crc_calculated;
    uint8_t crc_received;

    crc_calculated = sum_check(msg, msg_length - 1);
    crc_received = msg[msg_length - 1];

    printf("ERROR CRC received 0x%0X != CRC calculated 0x%0X\n",
           crc_received, crc_calculated);
    /* Check CRC of msg */
    if (crc_calculated == crc_received) {
        return msg_length;
    }
    else {
        // printf("ERROR CRC received 0x%0X != CRC calculated 0x%0X\n",
        //    crc_received, crc_calculated);
        return -1;
    }
}

int maisi_rs485_dev_open(ms_rs485_t *rs485)
{
    struct termios tios;
    speed_t speed;
    int flags;
    int fd;

    printf("Opening %s at %d bauds (%c, %d, %d)\n",
           rs485->device, rs485->baud, rs485->parity,
           rs485->data_bit, rs485->stop_bit);

    /* The O_NOCTTY flag tells UNIX that this program doesn't want
       to be the "controlling terminal" for that port. If you
       don't specify this then any input (such as keyboard abort
       signals and so forth) will affect your process

       Timeouts are ignored in canonical input mode or when the
       NDELAY option is set on the file via open or fcntl */
    flags = O_RDWR | O_NOCTTY | O_NDELAY /* | O_EXCL */;
#ifdef O_CLOEXEC
    flags |= O_CLOEXEC;
#endif

    fd = open(rs485->device, flags);
    if (fd < 0) {
        fprintf(stderr, "ERROR Can't open the device %s (%s)\n",
                rs485->device, strerror(errno));
        return -1;
    }

    /* Save */
    tcgetattr(fd, &rs485->old_tios);

    memset(&tios, 0, sizeof(struct termios));

    /* C_ISPEED     Input baud (new interface)
       C_OSPEED     Output baud (new interface)
    */
    switch (rs485->baud) {
        case 110:
            speed = B110;
            break;
        case 300:
            speed = B300;
            break;
        case 600:
            speed = B600;
            break;
        case 1200:
            speed = B1200;
            break;
        case 2400:
            speed = B2400;
            break;
        case 4800:
            speed = B4800;
            break;
        case 9600:
            speed = B9600;
            break;
        case 19200:
            speed = B19200;
            break;
        case 38400:
            speed = B38400;
            break;
#ifdef B57600
        case 57600:
            speed = B57600;
            break;
#endif
#ifdef B115200
        case 115200:
            speed = B115200;
            break;
#endif
#ifdef B230400
        case 230400:
            speed = B230400;
            break;
#endif
#ifdef B460800
        case 460800:
            speed = B460800;
            break;
#endif
#ifdef B500000
        case 500000:
            speed = B500000;
            break;
#endif
#ifdef B576000
        case 576000:
            speed = B576000;
            break;
#endif
#ifdef B921600
        case 921600:
            speed = B921600;
            break;
#endif
#ifdef B1000000
        case 1000000:
            speed = B1000000;
            break;
#endif
#ifdef B1152000
        case 1152000:
            speed = B1152000;
            break;
#endif
#ifdef B1500000
        case 1500000:
            speed = B1500000;
            break;
#endif
#ifdef B2500000
        case 2500000:
            speed = B2500000;
            break;
#endif
#ifdef B3000000
        case 3000000:
            speed = B3000000;
            break;
#endif
#ifdef B3500000
        case 3500000:
            speed = B3500000;
            break;
#endif
#ifdef B4000000
        case 4000000:
            speed = B4000000;
            break;
#endif
        default:
            speed = B9600;
            fprintf(stderr,
                    "WARNING Unknown baud rate %d for %s (B9600 used)\n",
                    rs485->baud, rs485->device);
    }

    /* Set the baud rate */
    if ((cfsetispeed(&tios, speed) < 0) ||
        (cfsetospeed(&tios, speed) < 0)) {
        close(fd);
        fd = -1;
        return -1;
    }

    /* C_CFLAG      Control options
       CLOCAL       Local line - do not change "owner" of port
       CREAD        Enable receiver
    */
    tios.c_cflag |= (CREAD | CLOCAL);
    /* CSIZE, HUPCL, CRTSCTS (hardware flow control) */

    /* Set data bits (5, 6, 7, 8 bits)
       CSIZE        Bit mask for data bits
    */
    tios.c_cflag &= ~CSIZE;
    switch (rs485->data_bit) {
        case 5:
            tios.c_cflag |= CS5;
            break;
        case 6:
            tios.c_cflag |= CS6;
            break;
        case 7:
            tios.c_cflag |= CS7;
            break;
        case 8:
        default:
            tios.c_cflag |= CS8;
            break;
    }

    /* Stop bit (1 or 2) */
    if (rs485->stop_bit == 1)
        tios.c_cflag &= ~CSTOPB;
    else /* 2 */
        tios.c_cflag |= CSTOPB;

    /* PARENB       Enable parity bit
       PARODD       Use odd parity instead of even */
    if (rs485->parity == 'N') {
        /* None */
        tios.c_cflag &= ~PARENB;
    }
    else if (rs485->parity == 'E') {
        /* Even */
        tios.c_cflag |= PARENB;
        tios.c_cflag &= ~PARODD;
    }
    else {
        /* Odd */
        tios.c_cflag |= PARENB;
        tios.c_cflag |= PARODD;
    }

    /* Read the man page of termios if you need more information. */

    /* This field isn't used on POSIX systems
       tios.c_line = 0;
    */

    /* C_LFLAG      Line options

       ISIG Enable SIGINTR, SIGSUSP, SIGDSUSP, and SIGQUIT signals
       ICANON       Enable canonical input (else raw)
       XCASE        Map uppercase \lowercase (obsolete)
       ECHO Enable echoing of input characters
       ECHOE        Echo erase character as BS-SP-BS
       ECHOK        Echo NL after kill character
       ECHONL       Echo NL
       NOFLSH       Disable flushing of input buffers after
       interrupt or quit characters
       IEXTEN       Enable extended functions
       ECHOCTL      Echo control characters as ^char and delete as ~?
       ECHOPRT      Echo erased character as character erased
       ECHOKE       BS-SP-BS entire line on line kill
       FLUSHO       Output being flushed
       PENDIN       Retype pending input at next read or input char
       TOSTOP       Send SIGTTOU for background output

       Canonical input is line-oriented. Input characters are put
       into a buffer which can be edited interactively by the user
       until a CR (carriage return) or LF (line feed) character is
       received.

       Raw input is unprocessed. Input characters are passed
       through exactly as they are received, when they are
       received. Generally you'll deselect the ICANON, ECHO,
       ECHOE, and ISIG options when using raw input
    */

    /* Raw input */
    tios.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    /* C_IFLAG      Input options

       Constant     Description
       INPCK        Enable parity check
       IGNPAR       Ignore parity errors
       PARMRK       Mark parity errors
       ISTRIP       Strip parity bits
       IXON Enable software flow control (outgoing)
       IXOFF        Enable software flow control (incoming)
       IXANY        Allow any character to start flow again
       IGNBRK       Ignore break condition
       BRKINT       Send a SIGINT when a break condition is detected
       INLCR        Map NL to CR
       IGNCR        Ignore CR
       ICRNL        Map CR to NL
       IUCLC        Map uppercase to lowercase
       IMAXBEL      Echo BEL on input line too long
    */
    if (rs485->parity == 'N') {
        /* None */
        tios.c_iflag &= ~INPCK;
    }
    else {
        tios.c_iflag |= INPCK;
    }

    /* Software flow control is disabled */
    tios.c_iflag &= ~(IXON | IXOFF | IXANY);

    /* C_OFLAG      Output options
       OPOST        Postprocess output (not set = raw output)
       ONLCR        Map NL to CR-NL

       ONCLR ant others needs OPOST to be enabled
    */

    /* Raw output */
    tios.c_oflag &= ~OPOST;

    /* C_CC         Control characters
       VMIN         Minimum number of characters to read
       VTIME        Time to wait for data (tenths of seconds)

       UNIX serial interface drivers provide the ability to
       specify character and packet timeouts. Two elements of the
       c_cc array are used for timeouts: VMIN and VTIME. Timeouts
       are ignored in canonical input mode or when the NDELAY
       option is set on the file via open or fcntl.

       VMIN specifies the minimum number of characters to read. If
       it is set to 0, then the VTIME value specifies the time to
       wait for every character read. Note that this does not mean
       that a read call for N bytes will wait for N characters to
       come in. Rather, the timeout will apply to the first
       character and the read call will return the number of
       characters immediately available (up to the number you
       request).

       If VMIN is non-zero, VTIME specifies the time to wait for
       the first character read. If a character is read within the
       time given, any read will block (wait) until all VMIN
       characters are read. That is, once the first character is
       read, the serial interface driver expects to receive an
       entire packet of characters (VMIN bytes total). If no
       character is read within the time allowed, then the call to
       read returns 0. This method allows you to tell the serial
       driver you need exactly N bytes and any read call will
       return 0 or N bytes. However, the timeout only applies to
       the first character read, so if for some reason the driver
       misses one character inside the N byte packet then the read
       call could block forever waiting for additional input
       characters.

       VTIME specifies the amount of time to wait for incoming
       characters in tenths of seconds. If VTIME is set to 0 (the
       default), reads will block (wait) indefinitely unless the
       NDELAY option is set on the port with open or fcntl.
    */
    /* Unused because we use open with the NDELAY option */
    tios.c_cc[VMIN] = 0;
    tios.c_cc[VTIME] = 0;

    if (tcsetattr(fd, TCSANOW, &tios) < 0) {
        close(fd);
        fd = -1;
        return -1;
    }

    return fd;
}

/**
 * @brief 
 * 
 * @param fd 
 */
void maisi_rs485_dev_close(int fd)
{
    if (fd >= 0) {
        // tcsetattr(fd, TCSANOW, &ctx_rtu->old_tios);
        close(fd);
        fd = -1;
    }
}

/**
 * @brief 
 * 
 * @param fd 
 * @return int 
 */
int maisi_rs485_flush(int fd)
{
    return tcflush(fd, TCIOFLUSH);
}

/**
 * @brief 
 * 
 * @param fd 
 * @param rsp 
 * @param rsp_length 
 * @return ssize_t 
 */
ssize_t maisi_rs485_recv(int fd, uint8_t *rsp, int rsp_length)
{
    return read(fd, rsp, rsp_length);
}

/**
 * @brief 
 * 
 * @param fd 
 * @param rset 
 * @param tv 
 * @param length_to_read 
 * @return int 
 */
int maisi_select(int fd, fd_set *rset, struct timeval *tv, int length_to_read)
{
    int s_rc;

    while ((s_rc = select(fd + 1, rset, NULL, NULL, tv)) == -1) {
        if (errno != EINTR) {
            return -1;
        }
        else {
            // DEBUG_ERROR("A non blocked signal was caught\n");
            // /* Necessary after an error */
            // FD_ZERO(rset);
            // FD_SET(fd, rset);
        }
    }

    if (s_rc == 0) {
        /* Timeout */
        errno = ETIMEDOUT;
        return -1;
    }

    return s_rc;
}

/**
 * @brief 
 * 
 * @param function 
 * @param msg_type 
 * @return uint8_t 
 */
static uint8_t compute_ctrl_length_after_function(uint8_t function, msg_type_t msg_type)
{
    int length = 0;

    switch (function) {
        case MAISI_CMD_CTRL_AIR_CONDITIONER_ONOFF:
        case MAISI_CMD_CTRL_AIR_CONDITIONER_TEMPERATURE:
        case MAISI_CMD_CTRL_AIR_CONDITIONER_MODE:
        case MAISI_CMD_CTRL_AIR_CONDITIONER_WIND_SPEED:
        case MAISI_CMD_CTRL_AIR_CONDITIONER_WIND_DIRECTION:
        case MAISI_CMD_CTRL_AIR_CONDITIONER_HUMIDITY:
        case MAISI_CMD_CTRL_AIR_CONDITIONER_CLEANLINESS:
        case MAISI_CMD_CTRL_VENTILATION_SYSTEM_ONOFF:
        case MAISI_CMD_CTRL_VENTILATION_SYSTEM_TEMPERATURE:
        case MAISI_CMD_CTRL_VENTILATION_SYSTEM_MODE:
        case MAISI_CMD_CTRL_VENTILATION_SYSTEM_WIND_SPEED:
        case MAISI_CMD_CTRL_FLOOR_HEATING_ONOFF:
        case MAISI_CMD_CTRL_FLOOR_HEATING_TEMPERATURE:
        case MAISI_CMD_CTRL_FLOOR_HEATING_MODE:
        case MAISI_CMD_CTRL_FLOOR_HEATING_ANTIFREEZE_PROTECTION:
        case MAISI_CMD_QUERY_AIR_CONDITIONING_STATUS:
        case MAISI_CMD_QUERY_VENTILATION_DEV_STATUS:
        case MAISI_CMD_QUERY_FLOOR_HEATING_DEV_STATUS:
            length = 1;
            break;
        default:
            length = 0;
    }

    return length;
}

/**
 * @brief 
 * 
 * @param msg 
 * @param msg_type 
 * @return int 
 */
static int compute_dnum_length_after_ctrl(uint8_t *msg, msg_type_t msg_type)
{
    int length;
    uint8_t func = msg[MAISI_HEADER_LENGTH];
    uint8_t ctrl = msg[MAISI_CTRL_CODE_INDEX];

    switch (func) {
        case MAISI_CMD_CTRL_AIR_CONDITIONER_ONOFF:
        case MAISI_CMD_CTRL_VENTILATION_SYSTEM_ONOFF:
        case MAISI_CMD_CTRL_FLOOR_HEATING_ONOFF:
        case MAISI_CMD_CTRL_FLOOR_HEATING_ANTIFREEZE_PROTECTION:
            if (ctrl == MAISI_DEV_OFF || ctrl == MAISI_DEV_ON)
                length = 1;
            else
                length = 0;
            break;
        case MAISI_CMD_CTRL_AIR_CONDITIONER_TEMPERATURE:
            if (ctrl >= MAISI_AC_CTRL_TEMPERATURE_MIN && ctrl <= MAISI_AC_CTRL_TEMPERATURE_MAX)
                length = 1;
            else
                length = 0;
            break;
        case MAISI_CMD_CTRL_AIR_CONDITIONER_MODE:
            if (ctrl >= MAISI_AC_MODE_COOL && ctrl <= MAISI_AC_MODE_HOT_HEAT)
                length = 1;
            else
                length = 0;
            break;
        case MAISI_CMD_CTRL_AIR_CONDITIONER_WIND_SPEED:
            if (ctrl >= MAISI_WIND_SPEED_HIGH && ctrl <= MAISI_WIND_SPEED_MID_LOW)
                length = 1;
            else
                length = 0;
            break;
        case MAISI_CMD_CTRL_AIR_CONDITIONER_WIND_DIRECTION: {
            uint8_t front_back_dir = (ctrl & 0xF0) >> 4;
            uint8_t left_right_dir = ctrl & 0x0F;
            if ((ctrl == 0) ||
                (front_back_dir >= 0x01 && front_back_dir <= 0x06) ||
                (left_right_dir >= 0x01 && left_right_dir <= 0x06))
                length = 1;
            else
                length = 0;
        } break;
        case MAISI_CMD_CTRL_VENTILATION_SYSTEM_MODE:
            if (ctrl >= MAISI_VEN_MODE_AUTO && ctrl <= MAISI_VEN_MODE_CLOSED)
                length = 1;
            else
                length = 0;
            break;
        case MAISI_CMD_CTRL_VENTILATION_SYSTEM_WIND_SPEED:
            if (ctrl >= MAISI_WIND_SPEED_AUTO && ctrl <= MAISI_WIND_SPEED_CLOSED)
                length = 1;
            else
                length = 0;
            break;
        case MAISI_CMD_CTRL_FLOOR_HEATING_TEMPERATURE:
            if (ctrl >= MAISI_FH_CTRL_TEMPERATURE_MIN && ctrl <= MAISI_FH_CTRL_TEMPERATURE_MAX)
                length = 1;
            else
                length = 0;
            // case MAISI_CMD_CTRL_AIR_CONDITIONER_HUMIDITY:
            // case MAISI_CMD_CTRL_AIR_CONDITIONER_CLEANLINESS:
            // case MAISI_CMD_CTRL_VENTILATION_SYSTEM_TEMPERATURE:
            // case MAISI_CMD_CTRL_FLOOR_HEATING_MODE:
        case MAISI_CMD_QUERY_AIR_CONDITIONING_STATUS:
        case MAISI_CMD_QUERY_VENTILATION_DEV_STATUS:
        case MAISI_CMD_QUERY_FLOOR_HEATING_DEV_STATUS:
            if (ctrl == MAISI_QUERY_DEV_STATUS_ONE ||
                ctrl == MAISI_QUERY_DEV_STATUS_MANY ||
                ctrl == MAISI_QUERY_DEV_STATUS_ALL)
                length = 1;
            else
                length = 0;
            break;
        default:
            length = 0;
            break;
    }

    return length;
}

/**
 * @brief 
 * 
 * @param msg 
 * @param msg_type 
 * @return int 
 */
static int compute_data_length_after_dnum(uint8_t *msg, msg_type_t msg_type)
{
    uint8_t func = msg[MAISI_HEADER_LENGTH];
    // uint8_t ctrl = msg[MAISI_CTRL_CODE_INDEX];
    uint8_t dev_num = msg[MAISI_DEV_NUMBER_INDEX];
    int length;

    switch (func) {
        case MAISI_CMD_CTRL_AIR_CONDITIONER_ONOFF:
        case MAISI_CMD_CTRL_AIR_CONDITIONER_TEMPERATURE:
        case MAISI_CMD_CTRL_AIR_CONDITIONER_MODE:
        case MAISI_CMD_CTRL_AIR_CONDITIONER_WIND_SPEED:
        case MAISI_CMD_CTRL_AIR_CONDITIONER_WIND_DIRECTION:
        case MAISI_CMD_CTRL_AIR_CONDITIONER_HUMIDITY:
        case MAISI_CMD_CTRL_AIR_CONDITIONER_CLEANLINESS:
        case MAISI_CMD_CTRL_VENTILATION_SYSTEM_ONOFF:
        case MAISI_CMD_CTRL_VENTILATION_SYSTEM_TEMPERATURE:
        case MAISI_CMD_CTRL_VENTILATION_SYSTEM_MODE:
        case MAISI_CMD_CTRL_VENTILATION_SYSTEM_WIND_SPEED:
        case MAISI_CMD_CTRL_FLOOR_HEATING_ONOFF:
        case MAISI_CMD_CTRL_FLOOR_HEATING_TEMPERATURE:
        case MAISI_CMD_CTRL_FLOOR_HEATING_MODE:
        case MAISI_CMD_CTRL_FLOOR_HEATING_ANTIFREEZE_PROTECTION:
            length = 3;
            break;
        case MAISI_CMD_QUERY_AIR_CONDITIONING_STATUS:
        case MAISI_CMD_QUERY_VENTILATION_DEV_STATUS:
        case MAISI_CMD_QUERY_FLOOR_HEATING_DEV_STATUS:
            if (dev_num > 0)
                length = dev_num * MAISI_DEV_INFO_DATA_LENGTH + 1;
            else
                length = 0;
            break;
        default:
            length = 0;
    }

    return length;
}

/**
 * @brief 
 * 
 * @param fd
 * @param msg 
 * @param msg_type 
 * @return int 
 */
int maisi_rs485_receive_msg(int fd, uint8_t *msg, msg_type_t msg_type)
{
    int rc;
    fd_set rset;
    struct timeval tv;
    struct timeval *p_tv;
    int length_to_read;
    int msg_length = 0;
    _step_t step;

    if (msg_type == MSG_INDICATION) {
        printf("Waiting for an indication...\n");
    }
    else {
        printf("Waiting for a confirmation...\n");
    }

    if (fd < 0) {
        printf("ERROR The connection is not established.\n");
        return -1;
    }

    /* Add a file descriptor to the set */
    FD_ZERO(&rset);
    FD_SET(fd, &rset);

    /* We need to analyse the message step by step.  At the first step, we want
     * to reach the function code because all packets contain this
     * information. */
    step = _STEP_FUNCTION;
    length_to_read = MAISI_HEADER_LENGTH + 1;

    if (msg_type == MSG_INDICATION) {
        /* Wait for a message, we don't know when the message will be received */
        /* By default, the indication timeout isn't set */
        // tv.tv_sec = (_RESPONSE_1S_TIMEOUT / 1000);
        // tv.tv_usec = (_RESPONSE_1S_TIMEOUT % 1000) * 1000L;
        // p_tv = &tv;
        p_tv = NULL;
    }
    else {
        tv.tv_sec = 0;
        tv.tv_usec = _RESPONSE_TIMEOUT;
        p_tv = &tv;
    }
    printf("length to read = %d\n", length_to_read);
#if 1
    while (length_to_read != 0) {
        rc = maisi_select(fd, &rset, p_tv, length_to_read);
        if (rc == -1) {
            _maisi_rs485_error_print("select");
            return -1;
        }

        rc = maisi_rs485_recv(fd, msg + msg_length, length_to_read);
        if (rc == 0) {
            errno = ECONNRESET;
            rc = -1;
        }

        if (rc == -1) {
            _maisi_rs485_error_print("read");
            return -1;
        }
        /* Display the hex code of each character received */
        if (1) {
            int i;
            for (i = 0; i < rc; i++)
                printf("<%.2X>", msg[msg_length + i]);
        }

        /* Sums bytes received */
        msg_length += rc;
        /* Computes remaining bytes */
        length_to_read -= rc;

        if (length_to_read == 0) {
            switch (step) {
                case _STEP_FUNCTION:
                    /* Function code position */
                    length_to_read = compute_ctrl_length_after_function(
                        msg[MAISI_HEADER_LENGTH],
                        msg_type);
                    if (length_to_read != 0) {
                        step = _STEP_CTRL;
                        break;
                    } /* else switches straight to the next step */
                case _STEP_CTRL:
                    length_to_read = compute_dnum_length_after_ctrl(msg, msg_type);
                    // if ((msg_length + length_to_read) > MAISI_MAX_ADU_LENGTH) {
                    //     errno = EMBBADDATA;
                    //     _maisi_rs485_error_print("too many data");
                    //     return -1;
                    // }
                    step = _STEP_DEV_NUM;
                    break;
                case _STEP_DEV_NUM:
                    length_to_read = compute_data_length_after_dnum(msg, msg_type);
                    if ((msg_length + length_to_read) > MAISI_MAX_ADU_LENGTH) {
                        _maisi_rs485_error_print("too many data");
                        return -1;
                    }
                    step = _STEP_DATA;
                    break;
                default:
                    break;
            }
        }
        if (length_to_read > 0) {
            /* If there is no character in the buffer, the allowed timeout
               interval between two consecutive bytes is defined by
               byte_timeout */
            tv.tv_sec = 0;
            tv.tv_usec = _BYTE_TIMEOUT;
            p_tv = &tv;
        }
        /* else timeout isn't set again, the full response must be read before
           expiration of response timeout (for CONFIRMATION only) */
    }

    if (1)
        printf("\n");

    return _maisi_rs485_check_integrity(msg, msg_length);
#endif
}

/**
 * @brief 
 * 
 * @param fd 
 * @param req 
 * @param req_length 
 * @return ssize_t 
 */
ssize_t maisi_rs485_send(int fd, const uint8_t *req, int req_length)
{
    return write(fd, req, req_length);
}