#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <malloc.h>

#include "common.h"

struct menuview_s;

/*!
 *  \brief 
 *  menuview 控件的信号回调函数类型
 */
typedef void (*menuview_cb_t)(struct menuview_s *, char, void *);


/*!
 *  \brief 
 *  menuview 控件句柄结构定义
 */
typedef struct menuview_s
{
    /*! 列表显示类型 */
    char type;
    
    /*! 画布ID */
    int canvas_id;
    
    /*! 信号回调函数 */
    menuview_cb_t callback;
    /*! 回调函数参数 */
    void *cb_param;

}menuview_t;

char *pData = NULL;

static menuview_cb_t g_widget_menu_cg = NULL;

static void _widget_menu_cb( menuview_t * menuview, char signal, void* param)
{
    print_mcd("type = %d, id = %d", menuview->type, menuview->canvas_id);
}

bool setting_list_widget_menu_emit_signal(menuview_t *menuview, char sig, void* param)
{
    if ( g_widget_menu_cg != NULL )
    {
        (g_widget_menu_cg)(menuview, sig, param);
        return true;
    }
    else
    {
        return false;
    }
}

menuview_t* setting_list_widget_menu_create(int menuview_type, menuview_cb_t callback )
{
    menuview_t *menuview;

    menuview = calloc(1, sizeof(menuview_t));
    menuview->canvas_id = 100;
    menuview->type = 10;
    //create a menu viewer
    // print_mcd("get menuview_type = %d", menuview_type);
    // menuview = menuview_create( menuview_type, NULL, NULL );
    // if( menuview == NULL )
    // {
    //     print_err("menuview create failed!");
    //     return NULL;
    // }
    // else
    {
        //restore last menu
        // if ( g_menu_entry != NULL )
        // {
        //     (g_menu_entry)( menuview );
        // }

        //save callback
        g_widget_menu_cg = (menuview_cb_t)callback;
    }

    return menuview;
}

int creat_canvas(int w, int h)
{
    if(pData == NULL)
    {
        pData = (char *)memalign(32, w * h);
        print_mcd("memalign success pData = %p", pData);
    }
    else
    {
        char *ptr = (char *)realloc(pData,  w * h);
        if(ptr == NULL)
        {
            print_mcd("realloc failed!");
        #if 1
            if((w * h) > 0)
                free(pData);
            pData = NULL;
        #endif
        }
        else
        {
            pData = ptr;
            print_mcd("realloc success pData = %p", pData);
        }
    }
    // fread

    return 0;
}

int main(int argc, char const *argv[])
{
    int count = 0;
    char buf[10];
    char *test = "my test";

    strcpy((char *)buf, test);

    print_mcd("buf = %s, %d", buf, strlen(buf));

    menuview_t *menuview = setting_list_widget_menu_create(12, _widget_menu_cb);
    while (++count < 5)
    {
        print_mcd("count %d and ",count);
        setting_list_widget_menu_emit_signal(menuview, 1,  NULL);
        if(count == 1)
            creat_canvas(1024, 768);
        else if(count == 2)
            creat_canvas(0, 0);
        else
            creat_canvas(1024, 768);
        sleep(1);
        
    }
    
    if(pData != NULL)
    {
        free(pData);
        pData = NULL;
    }
    return 0;
}
