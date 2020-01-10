/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <osal.h>
#include <oc_mqtt_al.h>
#include "gd32f30x.h"
#include "BearPi-IoT_gd32f303.h"
#include "lcd.h"
/* brief : the oceanconnect platform only support the ca_crt up tills now*/
/** the address product_id device_id password crt is only for the test  */

#define DEFAULT_LIFETIME            60
#define DEFAULT_SERVER_IPV4         "iot-acc.cn-north-4.myhuaweicloud.com"//"119.3.248.253"
#define DEFAULT_SERVER_PORT         "8883"
#define CN_MQTT_EP_NOTEID           "sinxxxxxvice"
#define CN_MQTT_EP_DEVICEID         "1c82xxxxxxxxxxxcaeea02b3"
#define CN_MQTT_EP_PASSWD           "c18f1xxxxxxxx8e6fef"

static char s_mqtt_ca_crt[] =
"-----BEGIN CERTIFICATE-----\r\n"
"MIID4DCCAsigAwIBAgIJAK97nNS67HRvMA0GCSqGSIb3DQEBCwUAMFMxCzAJBgNV\r\n"
"BAYTAkNOMQswCQYDVQQIEwJHRDELMAkGA1UEBxMCU1oxDzANBgNVBAoTBkh1YXdl\r\n"
"aTELMAkGA1UECxMCQ04xDDAKBgNVBAMTA0lPVDAeFw0xNjA1MDQxMjE3MjdaFw0y\r\n"
"NjA1MDIxMjE3MjdaMFMxCzAJBgNVBAYTAkNOMQswCQYDVQQIEwJHRDELMAkGA1UE\r\n"
"BxMCU1oxDzANBgNVBAoTBkh1YXdlaTELMAkGA1UECxMCQ04xDDAKBgNVBAMTA0lP\r\n"
"VDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAJxM9fwkwvxeILpkvoAM\r\n"
"Gdqq3x0G9o445F6Shg3I0xmmzu9Of8wYuW3c4jtQ/6zscuIGyWf06ke1z//AVZ/o\r\n"
"dp8LkuFbBbDXR5swjUJ6z15b6yaYH614Ty/d6DrCM+RaU+FWmxmOon9W/VELu2BB\r\n"
"NXDQHJBSbWrLNGnZA2erk4JSMp7RhHrZ0QaNtT4HhIczFYtQ2lYF+sQJpQMrjoRn\r\n"
"dSV9WB872Ja4DgcISU1+wuWLmS/NKjIvOWW1upS79yu2I4Rxos2mFy9xxz311rGC\r\n"
"Z3X65ejFNzCUrNgf6NEP1N7wB9hUu7u50aA+/56D7EgjeI0gpFytC+a4f6JCPVWI\r\n"
"Lr0CAwEAAaOBtjCBszAdBgNVHQ4EFgQUcGqy59oawLEgMl21//7F5RyABpwwgYMG\r\n"
"A1UdIwR8MHqAFHBqsufaGsCxIDJdtf/+xeUcgAacoVekVTBTMQswCQYDVQQGEwJD\r\n"
"TjELMAkGA1UECBMCR0QxCzAJBgNVBAcTAlNaMQ8wDQYDVQQKEwZIdWF3ZWkxCzAJ\r\n"
"BgNVBAsTAkNOMQwwCgYDVQQDEwNJT1SCCQCve5zUuux0bzAMBgNVHRMEBTADAQH/\r\n"
"MA0GCSqGSIb3DQEBCwUAA4IBAQBgv2PQn66gRMbGJMSYS48GIFqpCo783TUTePNS\r\n"
"tV8G1MIiQCpYNdk2wNw/iFjoLRkdx4va6jgceht5iX6SdjpoQF7y5qVDVrScQmsP\r\n"
"U95IFcOkZJCNtOpUXdT+a3N+NlpxiScyIOtSrQnDFixWMCJQwEfg8j74qO96UvDA\r\n"
"FuTCocOouER3ZZjQ8MEsMMquNEvMHJkMRX11L5Rxo1pc6J/EMWW5scK2rC0Hg91a\r\n"
"Lod6aezh2K7KleC0V5ZlIuEvFoBc7bCwcBSAKA3BnQveJ8nEu9pbuBsVAjHOroVb\r\n"
"8/bL5retJigmAN2GIyFv39TFXIySw+lW0wlp+iSPxO9s9J+t\r\n"
"-----END CERTIFICATE-----\r\n";

//if your command is very fast,please use a queue here--TODO
#define cn_app_rcv_buf_len 256
static char            s_rcv_buffer[cn_app_rcv_buf_len];
static int             s_rcv_datalen;
static osal_semp_t     s_rcv_sync;
static void           *s_mqtt_handle;

//X[2] and Y[7] devided the lcd into 7 rows and 2 cols
static int X[2] = {10,120};
static int Y[7] = {10,40,70,100,130,160,190};

//position of selected goods, (10,10) as default
static int goodsOptionX = 10;
static int goodsOptionY = 10;

//content to display on lcd
char* goodsView[10] = {"water 1","cola 3","tea 3","coffee 5","milk 4","juice 3","yogurt 4","bread 7","sandwich 7","sugar 2"};
char* Submit_View[2] = {"Submit","Cancel"};

//
int Goods_Price[10] = {1,3,3,5,4,3,4,7,7,2};
int goods_position[10] = {0,1,2,3,4,5,6,7,8,9};

//some basic info to report
int orderID = 10000001;
int userID = 123456;
int userAge = 23;
char* deviceID = "WZ_1-001";
char* area = "WZ";
char* Regions[] = {"School","Mall","Hospital","Community","Industry","Park","Station"};
int Region_Random = 0;
int longitude = 120.65;
int latitude = 28.02;
int orderTime = 9335848;
int orderTime1 = 157255;
char* Pay[] = {"Cash","WeChat","Alipay","UnionPay"};
int pay_Random = 0;
int Status = 0;
int Total_Num = 0;
int Total_Cost = 0;
int Goods_Num[10] = {0};

char* Goods_Num_String[10] = {"water_Num","cola_Num","tea_Num","coffee_Num","milk_Num","juice_Num","yogurt_Num","bread_Num","sanwiches_Num","sugar_Num"};
char* Goods_Price_String[10] = {"water_Price","cola_Price","tea_Price","coffee_Price","milk_Price","juice_Price","yogurt_Price","bread_Price","sanwiches_Price","sugar_Price"};
char orderID_string[9];
char userID_string[6];
char orderTime_string[13];

//use this function to push all the message to the buffer
static int app_msg_deal(void *handle,mqtt_al_msgrcv_t *msg)
{
    int ret = -1;
    printf("topic:%s qos:%d\n\r",msg->topic.data,msg->qos);

    if(msg->msg.len < cn_app_rcv_buf_len)
    {
        memcpy(s_rcv_buffer,msg->msg.data,msg->msg.len );
        s_rcv_buffer[msg->msg.len] = '\0'; ///< the json need it
        s_rcv_datalen = msg->msg.len;

        printf("msg:%s\n\r",s_rcv_buffer);

        osal_semp_post(s_rcv_sync);
        ret = 0;
    }
    return ret;
}

//function to handle oc cmd
static int oc_mqtt_cmd_entry( void *args)
{
    cJSON  *msg = NULL;
    cJSON  *mid = NULL;
    cJSON  *ioswitch = NULL;
    cJSON  *msgType = NULL;
    cJSON  *paras = NULL;
    cJSON  *serviceId = NULL;
    cJSON  *cmd = NULL;
    char   *buf = NULL;

    tag_oc_mqtt_response response;
    tag_key_value_list   list;

    int mid_int;
    int err_int;
    while(1)
    {
        if(osal_semp_pend(s_rcv_sync,cn_osal_timeout_forever))
        {
            err_int = 1;
            mid_int = 1;
            msg = cJSON_Parse(s_rcv_buffer);

            if(NULL != msg)
            {
                serviceId = cJSON_GetObjectItem(msg,"serviceId");
                if(NULL != serviceId)
                {
                    printf("serviceId:%s\n\r",serviceId->valuestring);
                }

                mid = cJSON_GetObjectItem(msg,"mid");
                if(NULL != mid)
                {
                    mid_int = mid->valueint;
                    printf("mid:%d\n\r",mid_int);
                }
                msgType = cJSON_GetObjectItem(msg,"msgType");
                if(NULL != msgType)
                {
                    printf("msgType:%s\n\r",msgType->valuestring);
                }
                cmd =  cJSON_GetObjectItem(msg,"cmd");
                if(NULL != cmd)
                {
                    printf("cmd:%s\n\r",cmd->valuestring);
                }

                paras = cJSON_GetObjectItem(msg,"paras");
                if(NULL != paras)
                {
                    ioswitch = cJSON_GetObjectItem(paras,"ioswitch");
                    if(NULL != ioswitch)
                    {
                        printf("ioswitch:%s\n\r",ioswitch->valuestring);
                        
                        char temp[20];
                        sprintf(temp,"%s",ioswitch->valuestring);
                        for(int i=0;i<10;i++){
                            goods_position[i] = temp[i]-'0';
                        }
                        LCD_Clear(BLACK);	  	
	                    POINT_COLOR = GREEN;
                        for(int i=0;i<10;i++){
                            if(i<5){
                                LCD_ShowString(X[0], Y[i], 240, 24, 24, goodsView[goods_position[i]]);
                            }else{
                                LCD_ShowString(X[1], Y[i-5], 240, 24, 24, goodsView[goods_position[i]]);
                            }
                        }
                        LCD_ShowString(X[0], Y[5], 240, 24, 24, Submit_View[0]);
                        LCD_ShowString(X[1], Y[5], 240, 24, 24, Submit_View[1]);

                        LCD_ShowString(X[0]-10, goodsOptionY, 10, 24, 24, "*");
                        LCD_ShowxNum(X[1], Y[6], 0, 3, 24, 1);
                        err_int = en_oc_mqtt_err_code_ok;
                    }
                    else
                    {
                        printf("handle the json data as your specific profile\r\n");
                        err_int = en_oc_mqtt_err_code_err;
                    }
                }
                cJSON_Delete(msg);

                list.item.name = "body_para";
                list.item.buf = "body_para";
                list.item.type = en_key_value_type_string;
                list.next = NULL;

                response.hasmore = 0;
                response.errcode = err_int;
                response.mid = mid_int;
                response.bodylst = &list;

                msg = oc_mqtt_json_fmt_response(&response);
                if(NULL != msg)
                {
                    buf = cJSON_Print(msg);
                    if(NULL != buf)
                    {
                        if(0 == oc_mqtt_report(s_mqtt_handle,buf,strlen(buf),en_mqtt_al_qos_1))
                        {
                           // printf("SNDMSG:%s\n\r",buf);
                        }
                        osal_free(buf);
                    }
                    cJSON_Delete(msg);
                }
            }
        }
    }

    return 0;
}

//function to report data to oc
 static int oc_mqtt_report_entry(void *args)
 {
    //json data
    tag_key_value_list orderID_List;
    tag_key_value_list userID_List;
    tag_key_value_list userAge_List;
    tag_key_value_list deviceID_List;
    tag_key_value_list area_List; 
    tag_key_value_list Region_List;
    tag_key_value_list Longitude_List;
    tag_key_value_list Latitude_List;
    tag_key_value_list orderTime_List;
    tag_key_value_list Pay_List;
    tag_key_value_list Goods_Num_List[10];
    tag_key_value_list Goods_Price_List[10];
    tag_key_value_list Status_List;
    tag_key_value_list Total_Cost_List;
 
    cJSON *root = NULL;
    char  *buf = NULL;
    void *handle = NULL;
    tag_oc_mqtt_report  report;

    tag_oc_mqtt_config config;
    
    config.boot_mode = en_oc_boot_strap_mode_factory;
    config.lifetime = DEFAULT_LIFETIME;
    config.server = DEFAULT_SERVER_IPV4;
    config.port = DEFAULT_SERVER_PORT;
    config.msgdealer = app_msg_deal;
    config.code_mode = en_oc_mqtt_code_mode_json;
    config.sign_type = en_mqtt_sign_type_hmacsha256_check_time_no;
    config.device_type = en_oc_mqtt_device_type_static;
    config.auth_type = en_mqtt_auth_type_nodeid;
    config.device_info.s_device.deviceid = CN_MQTT_EP_NOTEID;
    config.device_info.s_device.devicepasswd = CN_MQTT_EP_PASSWD;

    config.security.type = en_mqtt_al_security_cas;
    config.security.u.cas.ca_crt.data = s_mqtt_ca_crt;
    config.security.u.cas.ca_crt.len = sizeof(s_mqtt_ca_crt); ///< must including the end '\0'

    //connect to oc according to the settings
    handle = oc_mqtt_config(&config);

    if(NULL == handle)
    {
        printf("config err \r\n");
        return -1;
    }
    else
    {
        printf("config success\r\n");
    }
    s_mqtt_handle = handle;
    while(1)  //do the loop here
    {
        if(RESET == gd_eval_key_state_get(F2_KEY))//if key F2 is pressed
		{
        	//if cursor points to "Submit" or "Cancel"
            if(goodsOptionY==Y[5]){
            	//package json data
                sprintf(orderID_string,"%8d",orderID);
                orderID_List.item.name = "orderID";
                orderID_List.item.buf = (char *)orderID_string;
                orderID_List.item.len = sizeof(orderID_string);
                orderID_List.item.type = en_key_value_type_string;
                orderID_List.next = &userID_List;    

                pay_Random = rand()%3;
                userID = rand()%1000000;
                sprintf(userID_string,"%06d",userID);
                userID_List.item.name = "userID";
                userID_List.item.buf = (char *)userID_string;
                userID_List.item.len = sizeof(userID_string);
                userID_List.item.type = en_key_value_type_string;
                userID_List.next = &userAge_List;

                userAge_List.item.name = "userAge";
                userAge_List.item.buf = (char *)&userAge;
                userAge_List.item.len = sizeof(userAge);
                userAge_List.item.type = en_key_value_type_int;
                userAge_List.next = &deviceID_List;

                deviceID_List.item.name = "deviceID";
                deviceID_List.item.buf = (char *)deviceID;
                deviceID_List.item.len = sizeof(deviceID);
                deviceID_List.item.type = en_key_value_type_string;
                deviceID_List.next = &area_List;

                area_List.item.name = "area";
                area_List.item.buf = (char *)area;
                area_List.item.len = sizeof(area);
                area_List.item.type = en_key_value_type_string;
                area_List.next = &Region_List;

                Region_Random = rand()%6;
                Region_List.item.name = "region";
                Region_List.item.buf = (char *)Regions[Region_Random];
                Region_List.item.len = sizeof(Regions[Region_Random]);
                Region_List.item.type = en_key_value_type_string;
                Region_List.next = &Longitude_List;

                Longitude_List.item.name = "longitude";
                Longitude_List.item.buf = (char *)&longitude;
                Longitude_List.item.len = sizeof(longitude);
                Longitude_List.item.type = en_key_value_type_int;
                Longitude_List.next = &Latitude_List;

                Latitude_List.item.name = "latitude";
                Latitude_List.item.buf = (char *)&latitude;
                Latitude_List.item.len = sizeof(latitude);
                Latitude_List.item.type = en_key_value_type_int;
                Latitude_List.next = &orderTime_List;
                
                sprintf(orderTime_string,"%d%d",orderTime1,orderTime);
                orderTime_List.item.name = "orderTime";
                orderTime_List.item.buf = (char *)orderTime_string;
                orderTime_List.item.len = sizeof(orderTime_string);
                orderTime_List.item.type = en_key_value_type_string;
                orderTime_List.next = &Pay_List;                    
                
                pay_Random = rand()%3;
                Pay_List.item.name = "payment";
                Pay_List.item.buf = (char *)Pay[pay_Random];
                Pay_List.item.len = sizeof(Pay[pay_Random]);                   
                Pay_List.item.type = en_key_value_type_string;
                Pay_List.next = &Goods_Num_List[0];

                if(goodsOptionX==X[1]){ //"Cancel"
                    Status = 0;
                    for(int i=0;i<10;i++){
                        Goods_Num[i]=0;
                    }
                    Total_Cost = 0;
                }else{
                    Status = 1; //"Submit"
                }

                for(int i=0;i<10;i++){
                    Goods_Num_List[i].item.name = Goods_Num_String[i];
                    Goods_Num_List[i].item.buf = (char *)&Goods_Num[i];
                    Goods_Num_List[i].item.len = sizeof(Goods_Num[i]);                
                    Goods_Num_List[i].item.type = en_key_value_type_int;
                    Goods_Num_List[i].next = &Goods_Price_List[i];
                    Goods_Price_List[i].item.name = Goods_Price_String[i];
                    Goods_Price_List[i].item.buf = (char *)&Goods_Price[i];
                    Goods_Price_List[i].item.len = sizeof(Goods_Price[i]);                   
                    Goods_Price_List[i].item.type = en_key_value_type_int;
                    if(i<9){
                        Goods_Price_List[i].next = &Goods_Num_List[i+1];
                    }
                }
                Goods_Price_List[9].next = &Status_List;
                
                Status_List.item.name = "status";
                Status_List.item.buf = (char *)&Status;
                Status_List.item.len = sizeof(Status);                   
                Status_List.item.type = en_key_value_type_int;
                Status_List.next = &Total_Cost_List;

                Total_Cost_List.item.name = "totalCost";
                Total_Cost_List.item.buf = (char *)&Total_Cost;
                Total_Cost_List.item.len = sizeof(Total_Cost);                   
                Total_Cost_List.item.type = en_key_value_type_int;
                Total_Cost_List.next = NULL;

                report.hasmore = en_oc_mqtt_has_more_no;
                report.paralst= &orderID_List;
                report.serviceid = "order";
                report.eventtime = NULL;

                //package json data
                root = oc_mqtt_json_fmt_report(&report);
                if(NULL != root)
                {
                    buf = cJSON_Print(root);
                    if(NULL != buf)
                    {
                    	//data report
                        if(0 == oc_mqtt_report(handle,buf,strlen(buf),en_mqtt_al_qos_1))
                        {
                            printf("%s\r\n","My report success");

                            //clear data after report success
                            Total_Cost = 0;
                            Total_Num = 0;
                            orderID++;
                            orderTime += 1000;
                            for(int i=0;i<10;i++){
                                Goods_Num[i]=0;
                            }
                            LCD_ShowxNum(X[1], Y[6], Total_Num, 3, 24, 1);
                            LCD_ShowString(goodsOptionX-10, goodsOptionY, 10, 24, 24, " ");
                            goodsOptionX = X[0];
                            goodsOptionY = Y[0];
                            LCD_ShowString(goodsOptionX-10, goodsOptionY, 10, 24, 24, "*");
                        }
                        osal_free(buf);
                    }
                    cJSON_Delete(root);
                }
            }else {
            	//if cursor points to goods, add good to shopping cart
                for(int i=0;i<10;i++){
                    if(goodsOptionX==X[0]){
                        if(goodsOptionY==Y[i]){
                            Goods_Num[goods_position[i]]++;                                
                            Total_Num++;
                            Total_Cost+=Goods_Price[goods_position[i]];
                            LCD_ShowxNum(X[1], Y[6], Total_Num, 3, 24, 1);
                            break;
                        }
                    }else{
                        if(goodsOptionY==Y[i-5]){
                            Goods_Num[goods_position[i]]++;
                            Total_Num++;
                            Total_Cost+=Goods_Price[goods_position[i]];
                            LCD_ShowxNum(X[1], Y[6], Total_Num, 3, 24, 1);
                            break;
                        }
                    }                  
                }
            }
        }
        osal_task_sleep(120);
    }
    return 0;
}

/*!
    \brief      irq handler, change the location of cursor when F1 is pressed
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI1_IRQHandler(void)
{
    if(RESET != exti_interrupt_flag_get(EXTI_1)){
        for(int i=0;i<6;i++){
            if(goodsOptionY==Y[i]&&i<5){
                LCD_ShowString(goodsOptionX-10, goodsOptionY, 10, 24, 24, " ");
                goodsOptionY=Y[i+1];
                LCD_ShowString(goodsOptionX-10, goodsOptionY, 10, 24, 24, "*");
                break;
            }else if(goodsOptionY==Y[i]&&i==5){
                LCD_ShowString(goodsOptionX-10, goodsOptionY, 10, 24, 24, " ");
                goodsOptionY=Y[0];
                goodsOptionX=(goodsOptionX==X[0])?X[1]:X[0];
                LCD_ShowString(goodsOptionX-10, goodsOptionY, 10, 24, 24, "*");
            }                                       
        }    
        exti_interrupt_flag_clear(EXTI_1);
    }
}
//demo entry
int standard_app_demo_main()
{
    LCD_Init();
	LCD_Clear(BLACK);       //set lcd background color as black
	POINT_COLOR = GREEN;    //set lcd font color as green

	//display goods
    for(int i=0;i<10;i++){
        if(i<5){
            LCD_ShowString(X[0], Y[i], 240, 24, 24, goodsView[goods_position[i]]);
        }else{
            LCD_ShowString(X[1], Y[i-5], 240, 24, 24, goodsView[goods_position[i]]);
        }
    }

    //display "Submit" and "Cancel"
    LCD_ShowString(X[0], Y[5], 240, 24, 24, Submit_View[0]);
    LCD_ShowString(X[1], Y[5], 240, 24, 24, Submit_View[1]);

    LCD_ShowString(X[0]-10, goodsOptionY, 10, 24, 24, "*");//display original position of cursor
    LCD_ShowxNum(X[1], Y[6], 0, 3, 24, 1);//display current goods number in shopping cart

    gd_eval_key_init(F1_KEY, KEY_MODE_EXTI);//set key F1 as external interrupt mode
    gd_eval_key_init(F2_KEY, KEY_MODE_GPIO);//set key F2 as GPIO mode

    osal_int_connect(EXTI1_IRQn, 2,0,EXTI1_IRQHandler,NULL);//init F1 irq handler
    
    osal_semp_create(&s_rcv_sync,1,0);//create semaphore for oc cmd handler

    osal_task_create("ocmqtt_report",oc_mqtt_report_entry,NULL,0x1200,NULL,8); //create data report task

    osal_task_create("ocmqtt_cmd",oc_mqtt_cmd_entry,NULL,0x1000,NULL,8);//create oc cmd handle task

    return 0;
}
