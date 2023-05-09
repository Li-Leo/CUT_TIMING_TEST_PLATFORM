/************************************************
* DESCRIPTION:
*
************************************************/
#include "app_cmd.h"
#include "timer.h"
#include "version.h"
#include "string.h"
#include "stdlib.h"
#include "ssz_common.h"
#include "drv_key.h"
#include "com.h"
#include "common.h"
#include "drv_18b20.h"
#include "ssz_str_utility.h"
#include "ssz_time_utility.h"
#include "i2c.h"
#include "lcd.h"

/************************************************
* Declaration
************************************************/
#define CMD_MAX_SIZE_OF_RECEIVE_STR 60
#define CMD_ID kComPCUart
#define CMD_HANDLE_ORDER kComHandleFirst
#define CMD_ENABLE_PASSWORD "givemecmd"

typedef  int (*AppCmdHandler)(const char* cmd, char *params[], int param_size);

typedef struct  
{
    const char *command;
    AppCmdHandler handler;
    const char *description;
} AppCmdInfo;

static void app_cmd_parse_from_str(const char* cmd);

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TEST
int run_test_main(int argc, const char **argv);
#endif

#ifdef __cplusplus
}
#endif


/************************************************
* Variable 
************************************************/
static char g_app_cmd_receive_str[CMD_MAX_SIZE_OF_RECEIVE_STR];
static int g_app_cmd_receive_index = 0;
static char g_app_cmd_last_receive_str[CMD_MAX_SIZE_OF_RECEIVE_STR];
static bool g_app_cmd_is_enable_cmd = false;
static bool g_app_cmd_is_enable_show_input = false;
static const AppCmdInfo* g_app_cmd_info_ptr = NULL;
static const uint8_t g_app_cmd_cmd_return_id[] = {0x0};
static bool g_app_cmd_wait_line_end; //wait \n when receive \r

/************************************************
* Function 
************************************************/

/***********************************************
* Description:
*   send the flag to PC, so PC can know that the cmd is exec complete.
* Argument:
*   ret_code:
*
* Return:
*
************************************************/
static void app_cmd_send_return_flag(int ret_code)
{
#ifdef SSZ_TARGET_SIMULATOR
    fwrite(g_app_cmd_cmd_return_id, 1, sizeof(g_app_cmd_cmd_return_id), stdout);
#else
    ssz_fwrite(com_to_file(CMD_ID), g_app_cmd_cmd_return_id, sizeof(g_app_cmd_cmd_return_id));
#endif
}

void app_cmd_repeat_exe_cmd(void)
{
    char tmp_receive_str[CMD_MAX_SIZE_OF_RECEIVE_STR];

    strcpy(tmp_receive_str, g_app_cmd_last_receive_str);
    app_cmd_parse_from_str(tmp_receive_str);
}

// static void app_cmd_output_error_info(int error_code)
// {
// 	switch (error_code) {
// 	case kInvalidParam:
// 		printf("invalid param\n");
// 		break;
// 	case kNotExist:
// 		printf("not exist\n");
// 		break;
// 	default:
// 		printf("error: %d\n", error_code);
// 		break;
// 	}
// }

//all command function
static int app_cmd_help(const char* cmd, char* params[], int param_size)
{
    const AppCmdInfo* p;
    p = g_app_cmd_info_ptr;

    printf("all cmds:\n");
    while(p->command[0] != 0)
    {
        if (strlen(p->command) >= 9 ) {
            if(strcmp(p->command, "clear_data") == 0 || strcmp(p->command, "clear_log") == 0 )
                printf("%s\t\t", p->command);//%-24s
            else
                printf("%s\t", p->command);//%-24s

        } else {
            printf("%s\t\t", p->command);//%-24s
        }

        if (p->description != 0)
        {
            printf(" %s", p->description);
        }
        putchar('\n');
        p++;
    }
    
    return 0;
}

static int app_cmd_exit(const char* cmd, char* params[], int param_size)
{
    //do not receive data
    com_set_receive_handler(CMD_ID, CMD_HANDLE_ORDER, NULL);
    g_app_cmd_is_enable_cmd = false;
            
    return 0;
}

// static int app_cmd_disable(const char* cmd, char* params[], int param_size)
// {
//     g_app_cmd_is_enable_cmd = false;
            
//     return 0;
// }

static int app_cmd_repeat(const char* cmd, char* params[], int param_size)
{
    int time_ms = 0;

    if (param_size > 0) {
        time_ms = atoi(params[0]);
    }
    if (time_ms==0) {
        time_ms = 1000;
    }
    timer_start_periodic_every(kTimerRepeatCmd, time_ms);
    app_cmd_repeat_exe_cmd();
    return 0;
}

#ifdef TEST
static int app_cmd_test(const char* cmd, char* params[], int param_size)
{
    run_test_main(param_size, (const char **)params);
    return 0;
}
#endif
#if STOP_WATCH_MODULE_ENABLE
static int app_cmd_stop_watch_set(const char* cmd, char* params[], int param_size) {
    int is_on = 1;
    if (param_size > 0) {
        is_on = atoi(params[0]);
    }
    if (is_on==0) {
        printf("disable stopwatch\n");
        stop_watch_disable();
    }
    else {
        printf("enable stopwatch\n");
        stop_watch_enable();
    }
    return 0;
}
#endif
static void app_cmd_output_all_module_info(void)
{
    int i;

    printf("\nmodule output status list:\n");

    for (i = 0; i < kModuleIDMax; i++)
    {
        if (is_module_enable_output((ModuleID)i))
        {
            printf("%s [yes]\n", get_module_name((ModuleID)i));
        }
        else
        {
            printf("%s [no]\n", get_module_name((ModuleID)i));
        }
    }
}

static bool app_cmd_set_module_output_by_name(const char* module_name, bool is_enable)
{
    int i;
    bool ret = false;

    for (i = 0; i < kModuleIDMax; i++) {
        if (strcmp(module_name, get_module_name((ModuleID)i)) == 0 ) {
            set_module_output((ModuleID)i, is_enable);
            ret = true;
            break;
        } else if(strcmp(module_name,"all") == 0) {
            set_module_output((ModuleID)i, is_enable);
            ret = true;
        }
            
    }

    return ret;
}

static int app_cmd_module(const char* cmd, char* params[], int param_size)
{
    app_cmd_output_all_module_info();

    return 0;
}

static int app_cmd_enable_output(const char* cmd, char* params[], int param_size)
{
    int i;

    for(i=0; i<param_size; i++)
    {
        if(app_cmd_set_module_output_by_name(params[i], true))
        {
            printf("%s module set enabled success\n", params[i]);
        }
        else {
            printf("unknow module: %s\n", params[i]);
        }
    }

    app_cmd_output_all_module_info();

    return 0;
}

static int app_cmd_disable_output(const char* cmd, char* params[], int param_size)
{
    int i;

    for(i=0; i<param_size; i++)
    {
        if(app_cmd_set_module_output_by_name(params[i], false))
        {
            printf("%s module set disabled success\n", params[i]);
        }
        else {
            printf("unkonw module: %s\n", params[i]);
        }
    }
    app_cmd_output_all_module_info();

    return 0;
}


// static int app_cmd_test_watchdog(const char* cmd, char* params[], int param_size)
// {
//     ssz_check(0); //lint !e506
//     return 0;
// }

static int app_cmd_version(const char* cmd, char* params[], int param_size)
{
    // printf("%s\n", version_software_name());
    printf("software version: %d.%d.%d.%d\n", version_info()->major, version_info()->minor, version_info()->revision,
        version_info()->internal_version);

    printf("build time: %s\n", version_build_time());
    int days;
    int hour;
    int minute;
    int second;
    int ms;
    ms = ssz_milliseconds_to_run_time_of_days(ssz_tick_time_now(), &days, &hour, &minute, &second);
    printf("run time: %d day, %02d:%02d:%02d.%03d\n", days, hour, minute, second, ms);
    return 0;
}

#if CPU_USAGE_MODULE_ENABLE
static int app_cmd_cpu_usage(const char* cmd, char* params[], int param_size)
{
    cpu_usage_print_if_large_than(0);
    return 0;
}
#endif
// static int app_cmd_time(const char* cmd, char* params[], int param_size)
// {
// 	// printf("%s\n", ssz_time_now_str());
// 	return 0;
// }


static int app_cmd_read_temp(const char *cmd, char *params[], int param_size)
{
    uint16_t temp;
    uint32_t celsius;
    
    temp = read_channel_temp(kChannel1);
    celsius = temp * 625.0f / 1000.0f;
    printf("temp:%d.%d\n", celsius / 10, celsius % 10);

    printf("temp:%u\n", ds18b20_read_temp(&thermometer[0]));

    return 0;
}

static int app_cmd_test_us(const char *cmd, char *params[], int param_size)
{
    uint16_t temp = 0;
    
    for (int i = 0; i < 5; ++i){
        ssz_delay_us(1000000);
        printf("test_us:%d\n", temp++);
    }
    
    printf("\n");
    for (int i = 0, temp = 0; i < 5; ++i){
        ssz_delay_ms(1000);
        printf("test_ms:%d\n", temp++);
    }

    return 0;
}

// static int app_cmd_tick(const char *cmd, char *params[], int param_size)
// {
//     printf("tick_time:%s\n", ssz_tick_time_now_str());

//     return 0;
// }


static int app_cmd_e2prom_read(const char *cmd, char *params[], int param_size)
{
    int addr = 0;
    uint16_t size = 64;

    if (param_size == 2) {
        addr = ssz_str_hex_to_int(params[0]);
        size = atoi(params[1]);
        read_test(addr, size);

    } else {
        printf("error param\n");
        return 0;
    }

    return 0;
}

static int app_cmd_e2prom_write(const char *cmd, char *params[], int param_size)
{
    int addr = 0;
    uint16_t size = 64;

    if (param_size == 2) {
        addr = ssz_str_hex_to_int(params[0]);
        size = atoi(params[1]);
        write_test(addr, size);
        printf("write e2prom done\n");

    } else {
        printf("error param\n");
        return 0;
    }

    return 0;
}

extern int32_t g_default_time;
static int app_cmd_write_cutting_time(const char *cmd, char *params[], int param_size)
{
    g_default_time = atoi(params[0]) * 1000;
    e2prom_write(CUTTING_TIME_ADDR, &g_default_time, sizeof(g_default_time));
    printf("write cutting_time:%lds, %ldms\n", g_default_time/1000, g_default_time); 

    return 0;
}

static int app_cmd_read_cutting_time(const char *cmd, char *params[], int param_size)
{
    e2prom_read(CUTTING_TIME_ADDR, &g_default_time, sizeof(g_default_time));
    printf("read cutting_time:%lds, %ldms\n", g_default_time/1000, g_default_time); 

    return 0;
}

static int app_cmd_lcd_test(const char *cmd, char *params[], int param_size)
{

    uint8_t dis1[] = "abcd";
    uint8_t dis2[] = "12345";
    uint8_t dis3[] = "hello world!";
    uint8_t dis4[] = "0123456789";

	lcd_write_msg(1, 0, dis1);
	lcd_write_msg(2, 0, dis2);
	lcd_write_msg(3, 0, dis3);
	lcd_write_msg(4, 0, dis4);

    printf("send to lcd\n");

    return 0;
}

static int app_cmd_lcd_cmd(const char *cmd, char *params[], int param_size)
{
    uint8_t data;

    data = ssz_str_hex_to_int(params[0]);
    
    lcd_write_cmd(data);
    printf("cmd = 0x%02x\n", data);
    return 0;
}


static int app_cmd_lcd_str(const char *cmd, char *params[], int param_size)
{
    uint8_t r, c;
    char *str;

    r = atoi(params[0]);
    c = ssz_str_hex_to_int(params[1]);
    str = params[2];
    
    lcd_write_msg(r, c, (uint8_t *)str);

    return 0;
}


static int app_cmd_led1(const char *cmd, char *params[], int param_size)
{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
    printf("led high\n");

    return 0;
}

static int app_cmd_led0(const char *cmd, char *params[], int param_size)
{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
    printf("led low\n");
    
    return 0;
}

//all cmd handlers
const static AppCmdInfo g_app_cmd_info[] =
{
    {"help", app_cmd_help,"show cmd info"},
    {"exit", app_cmd_exit,"exit cmd mode"},
    //{"disable", app_cmd_disable,"disable cmd"},
    {"repeat", app_cmd_repeat, "repeat last cmd each time, e.g. repeat [time_ms], repeat 2000"},
    {"enable_output", app_cmd_enable_output, "enable module's output. e.g. enable_output [module_name], enable_output all"
     "\n\t\t module_name: common, infusion_monitor, motor\n\t\t motor_monitor, screen"},
    {"disable_output", app_cmd_disable_output, "disable module's output. e.g. disable_output [module_id], disable_output all"},
    {"module", app_cmd_module, "show all module output state"},
    {"temp", app_cmd_read_temp, "read temperature"},
    {"test", app_cmd_test_us, "test us delay"},
    // {"tick", app_cmd_tick, "print tick"},
    {"e2prom_read", app_cmd_e2prom_read, "e2prom read test [0xXX] [size]"},
    {"e2prom_write", app_cmd_e2prom_write, "e2prom write test [0xXX] [size]"},
    {"read_cut_time", app_cmd_read_cutting_time, "read_cutting_time"},
    {"write_cut_time", app_cmd_write_cutting_time, "write_cutting_time [s]"},
    {"lcd", app_cmd_lcd_test, "lcd test"},
    {"cmd", app_cmd_lcd_cmd, "lcd test"},
    {"display", app_cmd_lcd_str, "row col string"},
    {"led1", app_cmd_led1, "led"},
    {"led0", app_cmd_led0, "led"},
    // {"test_watchdog", app_cmd_test_watchdog},
    //output version
    {"ver", app_cmd_version, "show system version" },


    //below must at end
    {"", NULL}   
};

/***********************************************
* Description:
*   parse cmd str
* Argument:
*   cmd:
*
* Return:
*
************************************************/
static void app_cmd_parse_from_str(const char* cmd)
{      
   
    if (strcmp(cmd, CMD_ENABLE_PASSWORD)==0) {
        printf("cmd enable, input help to get avalid cmd\n");
        g_app_cmd_is_enable_cmd = true;
        g_app_cmd_is_enable_show_input = false;
        return;
    } else if (strcmp(cmd, CMD_ENABLE_PASSWORD "_for_AI") == 0) {
        printf("cmd with format enable, input help to get avalid cmd\n");
        g_app_cmd_is_enable_cmd = true;
        g_app_cmd_is_enable_show_input = false;
        app_cmd_send_return_flag(0);
        return;
    } else if(g_app_cmd_is_enable_cmd) {
        int ret = -1;
        char *params[10];
        int param_size = 0;
        const AppCmdInfo* p;
        char *work_str;

        //find the ' '
        work_str = (char*)strchr(cmd, ' ');
        if (work_str != NULL) {
            *work_str = 0;
            work_str++;
        }
        param_size = ssz_str_split(work_str, " ",params, 10);

        p = g_app_cmd_info_ptr;

        //find the cmd handler
        while (p->command[0] != 0) {
            if (strcmp(p->command, cmd) == 0) {
                ret = p->handler(cmd, params, param_size);
                break;
            }
            p++;
        }


        //if not find cmd handler, return -1
        if (p->command[0] == 0) {
            ret = -1;
            if(g_app_cmd_is_enable_cmd && g_app_cmd_is_enable_show_input)
            {
                puts("invalid cmd");
            }
        }

        if (!g_app_cmd_is_enable_show_input) {
            //if have format, send the end flag
            app_cmd_send_return_flag(ret);
        }
    }
}



/***********************************************
* Description:
*   handle special char like backspace,delete
* Argument:
*   ch:
*
* Return:
*   true:it is handled,  false:it is not handled
************************************************/
static bool app_cmd_handle_special_char(uint8_t ch)
{
    bool ret = false;

    if (ch == 0x8)//backspace key
    {        
        ret = true;
        if (g_app_cmd_receive_index>0)
        {
            g_app_cmd_receive_index--;
        }
        if(g_app_cmd_is_enable_cmd && g_app_cmd_is_enable_show_input)
        {
            putchar(0x8);
        }
    }
    else if(ch == 0x1B)//ESC key
    {
        ret = true;
        g_app_cmd_receive_index = 0;
        if(g_app_cmd_is_enable_cmd && g_app_cmd_is_enable_show_input)
        {
            putchar('\n');
        }
    }
    else if(ch == '\r')//return key
    {
        ret = true;
    }
    else if (ch == '\n')//return key
    {
        ret = true;
    }
    return ret;
}


/***********************************************
* Description:
*   handle char received from one com
* Argument:
*   ch:
*   is_handled_ok_at_pre_handler:
*
* Return:
*
************************************************/
static bool app_cmd_on_receive_byte(uint8_t ch, bool is_handled_ok_at_pre_handler)
{
    //if previous handler handle it OK, reset the index
    if (is_handled_ok_at_pre_handler) {
        g_app_cmd_receive_index = 0;
    } else {
        if (g_app_cmd_wait_line_end && ch == '\n') {
            g_app_cmd_wait_line_end = false;
            return true;
        }

        g_app_cmd_wait_line_end = false;
        //stop last cmd repeat run
        timer_stop(kTimerRepeatCmd);

        if (ch == '\n' || ch == '\r') {//cmd end char
            if (ch == '\r') {
                g_app_cmd_wait_line_end = true;
            }

            if (g_app_cmd_is_enable_cmd && g_app_cmd_is_enable_show_input) {
                printf("\n");
            }
            g_app_cmd_receive_str[g_app_cmd_receive_index] = 0;
            if (strncmp(g_app_cmd_receive_str, "repeat", 6) != 0) {
                strcpy(g_app_cmd_last_receive_str, g_app_cmd_receive_str);
            }
            //parse the cmd
            if (strlen(g_app_cmd_receive_str) > 0) {
                app_cmd_parse_from_str(g_app_cmd_receive_str);
            }
            //reset the index
            g_app_cmd_receive_index = 0;
        } else if (app_cmd_handle_special_char(ch)) {  //is special char
            //handle special char ok
        } else {
            if (g_app_cmd_is_enable_cmd && g_app_cmd_is_enable_show_input) {
                putchar(ch);
            }

            //receive cmd srt
            g_app_cmd_receive_str[g_app_cmd_receive_index] = ch;
            g_app_cmd_receive_index++;
            if (g_app_cmd_receive_index >= CMD_MAX_SIZE_OF_RECEIVE_STR - 1) {
                if (g_app_cmd_is_enable_cmd && g_app_cmd_is_enable_show_input) {
                    printf("cmd too long\n");
                }
                g_app_cmd_receive_index = 0;
            }
        }
    }
    return true;
}


//enable cmd receive
void app_cmd_enable_receive(void)
{
    //handle the receive char
    com_set_receive_handler(CMD_ID, CMD_HANDLE_ORDER, app_cmd_on_receive_byte);
}
//disable cmd receive
void app_cmd_disable_receive(void)
{
    com_set_receive_handler(CMD_ID, CMD_HANDLE_ORDER, NULL);	
}

//enable cmd respond
void app_cmd_enable_respond(void)
{
    g_app_cmd_is_enable_cmd = true;
}
//disable cmd respond except the password
void app_cmd_disable_respond(void)
{
    g_app_cmd_is_enable_cmd = false;
}


/***********************************************
* Description:
*
* Argument:
*
* Return:
*
************************************************/
void app_cmd_init(void)
{
    g_app_cmd_info_ptr = g_app_cmd_info;
    timer_set_handler(kTimerRepeatCmd, app_cmd_repeat_exe_cmd);

    app_cmd_enable_receive();

#ifdef DEBUG
    g_app_cmd_is_enable_cmd = true;
    g_app_cmd_is_enable_show_input = true;
#endif
}
