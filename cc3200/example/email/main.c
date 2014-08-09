//*****************************************************************************
// Copyright (C) 2014 Texas Instruments Incorporated
//
// All rights reserved. Property of Texas Instruments Incorporated.
// Restricted rights to use, duplicate or disclose this code are
// granted through contract.
// The program may not be used without the written permission of
// Texas Instruments Incorporated or against the terms and conditions
// stipulated in the agreement under which this program has been supplied,
// and under no circumstances can it be used with non-TI connectivity device.
//
//*****************************************************************************
//*****************************************************************************
//
// Application Name     -   Email
// Application Overview -   The email application on the CC3200 sends emails
//							using SMTP (Simple Mail Transfer Protocol). The email
//							application sends a preconfigured email at the push of
//							a button or a user-configured email through the
//							CLI (Command Line Interface)
//
// Application Details  -
// http://processors.wiki.ti.com/index.php/CC32xx_Email_Demo_Application
// or
// docs\examples\CC32xx_Email_Demo_Application.pdf
//
//*****************************************************************************
#include <stddef.h>

// simplelink includes
#include "simplelink.h"

// driverlib includes
#include "hw_ints.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "osi.h"
#include "rom.h"
#include "rom_map.h"
#include "utils.h"
#include "interrupt.h"
#include "prcm.h"
#include "timer.h"

// common interface includes
#include "network_if.h"
#include "timer_if.h"
#include "gpio_if.h"
#include "button_if.h"
#ifndef NOTERM
#include "uart_if.h"
#endif

#include "pinmux.h"
#include "email.h"
#include "demo_config.h"



#define SPAWN_TASK_PRIORITY             (9)
#define OSI_STACK_SIZE					(2048)
///////////////////////////////////////////////////////////////////////////////

#define UART_COMMAND_CC31xx_SIMPLE_CONFIG_START (0x31)
#define UART_COMMAND_CC31xx_EMAIL_SOURCE		(0x32)
#define UART_COMMAND_CC31xx_EMAIL_HEADER		(0x33)
#define UART_COMMAND_CC31xx_EMAIL_MESSAGE		(0x34)
#define UART_COMMAND_CC31xx_EMAIL_SEND			(0x35)
#define UART_COMMAND_CC31xx_SMART_CONFIG		(0x36)
#define CC31xx_APP_BUFFER_SIZE				    (5)
#define CC31xx_RX_BUFFER_OVERHEAD_SIZE			(20)
#define NETAPP_IPCONFIG_MAC_OFFSET			    (20)
#define PLATFORM_VERSION				        (1)
#define APPLICATION_VERSION				        (3)

#ifndef NOTERM
#define UART_PRINT                              Report
#define DispatcherUartSendPacket                Report
#else
#define UART_PRINT(x,...)                                    
#define DispatcherUartSendPacket(x,...)                          
#endif

#define APP_NAME					"Email"
#define TASK_PRIORITY					(1)

// Default SSID Settings
#define SSID_NAME               "cc3200demo"      /* AP SSID */
#define SECURITY_TYPE           SL_SEC_TYPE_OPEN  /* Security type (OPEN or WEP or WPA) */
#define SECURITY_KEY            ""                /* Password of the secured AP */

/////////////////// Define Random messages ////////////////////////////////////
#define RANDOM_MSG_0       "The CC32xx connects new devices to the internet!"
#define RANDOM_MSG_1       "The CC32xx brings Wi-Fi to battery operated devices!"
#define RANDOM_MSG_2       "The CC32xx enables creating IOT products FAST!"
#define RANDOM_MSG_3       "The CC32xx is enabling the Internet of Things!"
#define RANDOM_MSG_4       "The CC32xx brings Wi-Fi to microcontrollers!"

// Queue Structure 
typedef struct
{
    //Queue_Elem _elem;
    P_OSI_SPAWN_ENTRY pEntry;
    void* pValue;
}tPushButtonMsg;


//
// GLOBAL VARIABLES -- Start
//

///////////////////////////////////////////////////////////////////////////////
// *SMTP* UART Strings for Menu and acknowledge

const unsigned char pucUARTOKString[] = "\f\rOK\f\r";
const unsigned char pucUARTCommandDoneString[] = "\f\rDONE\f\r";
const unsigned char pucUARTNoDataString[] = "\f\rNo data received\f\r";
const unsigned char pucUARTIllegalCommandString[] = "\f\r Illegal command\f\r";
const unsigned char putUARTFinishString[] = "\f\rEmail Sent!\f\r";
const unsigned char pucUARTCommandSmartConfigDoneString[] = "\f\rSmart config DONE\f\r";
const unsigned char pucUARTErrorString[] = "\f\rERROR\f\r";
const unsigned char putUARTErrorInputString[] ="\f\rERROR in input\f\r";
const unsigned char pucUARTErrorSocketCreateString[] = "\f\rConnection with "
                "Email Server Failed, Check Internet Connection and Retry\f\r";
const unsigned char pucUARTErrorSocketOptionString[] = "\f\r ERROR in socket option\f\r";
const unsigned char pucUARTSmartConfigString[] = "\f\rConfigure AP from Smart "
                    "Config Application\f\r";

// Menu Strings
const unsigned char pucUARTExampleAppString[] = "f\rCC32xx Email App Version";
const unsigned char pucUARTIntroString[] = "\f\rCommand Usage\f\r";
const unsigned char pucUARTMenuString1[] ="\f\r01";
const unsigned char pucUARTIntroString1[] ="\t - Connect to Preconfigured AP";
const unsigned char pucUARTMenuString2[] = "\f\r02,<email>,<password>";
const unsigned char pucUARTIntroString2[] = "\f\r\t - Configure Source Email and Password";
const unsigned char pucUARTMenuString3[] = "\f\r03,<to>,<subject>";
const unsigned char pucUARTIntroString3[] = "\f\r\t - Write Recipient Email"
                                            "address(e.g abc@xyz.com) and Subject";
const unsigned char pucUARTMenuString4[] = "\f\r04,<message>";
const unsigned char pucUARTIntroString4[] = "\f\r\t - Write Content of the Email";
const unsigned char pucUARTMenuString5[] = "\f\r05";
const unsigned char pucUARTIntroString5[] = "\t - Send Email";
const unsigned char pucUARTMenuString6[] = "\f\r06";
const unsigned char pucUARTIntroString6[] = "\t - Connect to AP Using Smart Config\f\r";

// Parameters Strings
const unsigned char pucUARTParamString[] = "\f\rParameters\f\r";
const unsigned char pucUARTParamString1[] ="\f\rto - Recipient Email Address (e.g abc@xyz.com)";
const unsigned char pucUARTParamString2[] ="\f\rsubject - Subject of the Email";
const unsigned char pucUARTParamString3[] ="\f\rmessage - Content of Email Message \f\r";



///////////////////////////////////////////////////////////////////////////////
// *SMTP* UART-defined config variables for Email Application
char pcEmailto[25];
char *pcOfemailto;
char pcEmailsubject[25];
char *pcOfemailsubject;
char pcEmailmessage[64];
char *pcOfemailmessage;

//*****************************************************************************
// Variable used in Timer Interrupt Handler
//*****************************************************************************
unsigned short g_usTimerInts;
//*****************************************************************************
// global variables (UART Buffer, Email Task, Queues Parameters)
//*****************************************************************************
signed char g_cConnectStatus;
unsigned int uiUartCmd;
char ucUARTBuffer[200];

/* AP Security Parameters */
SlSecParams_t SecurityParams = {0};


OsiMsgQ_t g_PBQueue;
OsiTaskHandle g_PushButtonTask; 
unsigned int g_uiPBCount = 0;

//*****************************************************************************
//                      GLOBAL VARIABLES for VECTOR TABLE
//*****************************************************************************
#if defined(gcc)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif

//*****************************************************************************
// extern Functions definition
//*****************************************************************************
extern void SmartConfigConnect();




//*****************************************************************************
//
//! SetDefaultParameters();
//!
//!	@brief  This function Sets all variables required to send an 
//!     email
//!
//!     @param  None
//!
//!     @return None
//!  
//
//*****************************************************************************
void SetDefaultParameters(void)
{
    //Set Destination Email
    SlNetAppDestination_t destEmailAdd;
    memcpy(destEmailAdd.Email,RCPT_RFC,strlen(RCPT_RFC)+1);
    sl_NetAppEmailSet(SL_NET_APP_EMAIL_ID,NETAPP_DEST_EMAIL,strlen(RCPT_RFC)+1,
                      (unsigned char *)&destEmailAdd);

    //Subject Line
    SlNetAppEmailSubject_t emailSubject;
    memcpy(emailSubject.Value,EMAIL_SUB,strlen(EMAIL_SUB)+1);
    sl_NetAppEmailSet(SL_NET_APP_EMAIL_ID,NETAPP_SUBJECT,strlen(EMAIL_SUB)+1,
                      (unsigned char *)&emailSubject);

    SlNetAppSourceEmail_t sourceEmailId;
    memcpy(sourceEmailId.Username,USER,strlen(USER)+1);
    sl_NetAppEmailSet(SL_NET_APP_EMAIL_ID,NETAPP_SOURCE_EMAIL,strlen(USER)+1,
                      (unsigned char*)&sourceEmailId);

    SlNetAppSourcePassword_t sourceEmailPwd;
    memcpy(sourceEmailPwd.Password,PASS,strlen(PASS)+1);
    sl_NetAppEmailSet(SL_NET_APP_EMAIL_ID,NETAPP_PASSWORD,strlen(PASS)+1,
                      (unsigned char*)&sourceEmailPwd);

    SlNetAppEmailOpt_t eMailServerSetting;
    eMailServerSetting.Family = AF_INET;
    eMailServerSetting.Port = GMAIL_HOST_PORT;
    eMailServerSetting.Ip = SL_IPV4_VAL(74,125,129,108);
    eMailServerSetting.SecurityMethod = SL_SO_SEC_METHOD_SSLV3;
    eMailServerSetting.SecurityCypher = SL_SEC_MASK_SSL_RSA_WITH_RC4_128_MD5;

    sl_NetAppEmailSet(SL_NET_APP_EMAIL_ID,NETAPP_ADVANCED_OPT,
                      sizeof(SlNetAppEmailOpt_t),(unsigned char*)&eMailServerSetting);

    strncpy(pcEmailmessage,"The CC32xx is enabling the Internet of Things!",64);
   
}

//*****************************************************************************
//
//! Periodic Timer Interrupt Handler
//!
//! \param None
//!
//! \return None
//
//*****************************************************************************
void
TimerPeriodicIntHandler(void)
{
    unsigned long ulInts;
    
    //
    // Clear all pending interrupts from the timer we are
    // currently using.
    //
    ulInts = MAP_TimerIntStatus(TIMERA0_BASE, true);
    MAP_TimerIntClear(TIMERA0_BASE, ulInts);
    
    //
    // Increment our interrupt counter.
    //
    g_usTimerInts++;
    if(!(g_usTimerInts & 0x1))
    {
        //
        // Off Led
        //
        GPIO_IF_LedOff(MCU_RED_LED_GPIO);
    }
    else
    {
        //
        // On Led
        //
    	GPIO_IF_LedOn(MCU_RED_LED_GPIO);
    }
}

//****************************************************************************
//
//! Function to configure and start timer to blink the LED while device is
//! trying to connect to an AP
//!
//! \param none
//!
//! return none
//
//****************************************************************************
void LedTimerConfigNStart()
{
    //
    // Configure Timer for blinking the LED for IP acquisition
    //
    Timer_IF_Init(PRCM_TIMERA0,TIMERA0_BASE,TIMER_CFG_PERIODIC,TIMER_A,0);
    Timer_IF_IntSetup(TIMERA0_BASE,TIMER_A,TimerPeriodicIntHandler);
    Timer_IF_Start(TIMERA0_BASE,TIMER_A,PERIODIC_TEST_CYCLES / 10);
}

//****************************************************************************
//
//! Disable the LED blinking Timer as Device is connected to AP
//!
//! \param none
//!
//! return none
//
//****************************************************************************
void LedTimerDeinitStop()
{
    //
    // Disable the LED blinking Timer as Device is connected to AP
    //
    Timer_IF_Stop(TIMERA0_BASE,TIMER_A);
    Timer_IF_DeInit(TIMERA0_BASE,TIMER_A);

}

//*****************************************************************************
//
//! GenerateRandomMessage();
//!
//!	@brief  This function generates a random email message            
//!
//!     @param  None
//!
//!     @return None
//!  
//
//*****************************************************************************
void GenerateRandomMessage(void)
{
    if(g_uiPBCount==5)
    {
          g_uiPBCount=0;
    }

    // Email Message (cycles through on each button press)
    if(g_uiPBCount==0)
    {
        strncpy(pcEmailmessage, RANDOM_MSG_0, strlen(RANDOM_MSG_0));
    }
    if(g_uiPBCount==1)
    {
        strncpy(pcEmailmessage, RANDOM_MSG_1, strlen(RANDOM_MSG_1));
    }
    if(g_uiPBCount==2)
    {
        strncpy(pcEmailmessage, RANDOM_MSG_2, strlen(RANDOM_MSG_2));
    }
    if(g_uiPBCount==3)
    {
        strncpy(pcEmailmessage, RANDOM_MSG_3, strlen(RANDOM_MSG_3));
    }
    if(g_uiPBCount==4)
    {
    	strncpy(pcEmailmessage, RANDOM_MSG_4, strlen(RANDOM_MSG_4));
    }

    g_uiPBCount++;
}

//*****************************************************************************
//
//!	EmailHandleERROR  
//!
//! @brief  Performs Error Handling for SMTP State Machine
//!
//! @param  servermessage is the response buffer from the smtp server
//!
//! @return none
//
//*****************************************************************************
void EmailHandleERROR(long error, char * servermessage)
{
    // Errors are handled via flags set in the smtpStateMachine
    switch(error)
    { 
        case SL_EMAIL_ERROR_INIT:
                // Server connection could not be established
                Report((char*)"Server connection error.\r\n");
        break;
        case SL_EMAIL_ERROR_HELO:
                // Server did not accept the HELO command from server
                Report((char*)"Server did not accept HELO:\r\n");
                Report((char*)servermessage);
        break;
        case SL_EMAIL_ERROR_AUTH:
                // Server did not accept authorization credentials
                Report((char*)"Authorization unsuccessful, check username/"
                       "password.\r\n");
                Report((char*)servermessage);
        break;
        case SL_EMAIL_ERROR_FROM:
                // Server did not accept source email.
                Report((char*)"Email of sender not accepted by server.\r\n");
                Report((char*)servermessage);
        break;
        case SL_EMAIL_ERROR_RCPT:
                // Server did not accept destination email
                Report((char*)"Email of recipient not accepted by server.\r\n");
                Report((char*)servermessage);
        break;
        case SL_EMAIL_ERROR_DATA:
                // 'DATA' command to server was unsuccessful
                Report((char*)"smtp 'DATA' command not accepted by server.\r\n");
                Report((char*)servermessage);
        break;
        case SL_EMAIL_ERROR_MESSAGE:
                // Message body could not be sent to server
                Report((char*)"Email Message was not accepted by the server.\r\n");
                Report((char*)servermessage);
        break;
        case SL_EMAIL_ERROR_QUIT:
                // Message could not be finalized
                Report((char*)"Connection could not be properly closed. Message"
                       "not sent.\r\n");
                Report((char*)servermessage);
        break;
        default:
          break;
  }	
  Report("\r\n");
}

//*****************************************************************************
//
//! OutputMenu
//!
//!	@brief	This function will display the Application Menu and Command Usage
//!			via the UART terminal.
//!
//!	@param  None
//!
//!	@return None
//!
//
//*****************************************************************************
int OutputMenu(void)
{

    // Display Menu Options for Application
    Report((char*)pucUARTIntroString);
    Report("-------------\n\r");
    Report((char*)pucUARTMenuString1);
    Report((char*)pucUARTIntroString1); 

    Report((char*)pucUARTMenuString3);
    Report((char*)pucUARTIntroString3);

    Report((char*)pucUARTMenuString4);
    Report((char*)pucUARTIntroString4);

    Report((char*)pucUARTMenuString5);
    Report((char*)pucUARTIntroString5);

    Report((char*)pucUARTMenuString6);
    Report((char*)pucUARTIntroString6);

    Report((char*)pucUARTParamString);
    Report("----------\n\r");
    Report((char*)pucUARTParamString1);
    Report((char*)pucUARTParamString2);
    Report((char*)pucUARTParamString3);
    Report("------------------------------------------------------------------"
           "-------\n\r\n\r");

    return(0);
}

//*****************************************************************************
//
//! UARTCommandHandler
//!
//!     @brief  The function handles commands arrived from CLI
//!
//!     @param  usBuffer is the receive buffer from the UART interface to PC
//!
//!     @return none
//!
//
//*****************************************************************************
void UARTCommandHandler(char *usBuffer)
{
    int iIndex = 0;
    int iParamcount = 0;
    signed char cStatus1 = 0;
    signed char cStatus2 = 0;

    switch(usBuffer[1])
    {
        //**********************************************
        // Case 01: Connect to default AP
        //**********************************************
        case UART_COMMAND_CC31xx_SIMPLE_CONFIG_START:

            if(!IS_CONNECTED(Network_IF_CurrentMCUState()))
            {
            	LedTimerConfigNStart();
                Network_IF_ConnectAP(SSID_NAME,SecurityParams);
            }

            //
            // Disable the LED blinking Timer as Device is connected to AP
            //
            LedTimerDeinitStop();
            
            //
            // Switch ON RED LED to indicate that Device acquired an IP
            //
            GPIO_IF_LedOn(MCU_IP_ALLOC_IND);
            DispatcherUartSendPacket((char*)pucUARTOKString, 
                                     sizeof(pucUARTOKString));
            break;
                
	//**********************************************
	// Case 02: Configure sender (source) email
	//**********************************************
        // TODO Phase 2: Include
        /*
	case UART_COMMAND_CC31xx_EMAIL_SOURCE:
          
		memset(serveruser,0,sizeof(serveruser));
                memset(pcServerpass,0,sizeof(pcServerpass));
		pcOfserveruser = &serveruser[0];
                ofpcServerpass = &pcServerpass[0];
                // '<' To maintain RFC 2821 format
                *pcOfserveruser++= '<';
                
		iIndex = 2;
		while ((int)usBuffer[iIndex] != 0x0D)
		{

			//look for comma ',' for separation of params
			if((int)usBuffer[iIndex] == 44)
                        {
				iParamcount++;
			}
			else
                        {
				if(iParamcount==1)
                                {
                                    //Enter smtp server username (email address)
                                    *pcOfserveruser++ = usBuffer[iIndex];

				}
				if(iParamcount==2)
                                {
                                    //Enter username's password
                                    *ofpcServerpass++ = usBuffer[iIndex];
                                }
                        }
			iIndex++;
		}
                // '>' To maintain RFC 2821 format
                *pcOfserveruser++= '>';
		*pcOfserveruser++= '\0';
		*ofpcServerpass++= '\0';
		
		//Set variables in smtp.c
		cStatus1 = smtpSetVariable(serveruser, USERNAME_VAR);
		cStatus2 = smtpSetVariable(pcServerpass, PASSWORD_VAR);
                //If error in return
		if(cStatus1 == 0 && cStatus2 == 0)
		{
                    DispatcherUartSendPacket((char*)pucUARTOKString, sizeof(pucUARTOKString));
		}
                else
                {
                    DispatcherUartSendPacket((char*)putUARTErrorInputString, sizeof(putUARTErrorInputString));  
                }
		break;
                */
	//**********************************************
	// Case 03: Configure sender (source) email
	//**********************************************
        case UART_COMMAND_CC31xx_EMAIL_HEADER:
          
            memset(pcEmailto,0,sizeof(pcEmailto));
            pcOfemailto = &pcEmailto[0];
            pcOfemailsubject = &pcEmailsubject[0];
            // '<' To maintain RFC 2821 format
            *pcOfemailto++= '<';
            iIndex = 2;
            while ((int)usBuffer[iIndex] != 0x0D && usBuffer[iIndex] != '\0')
            {
                //look for comma ',' for separation of params
                if((int)usBuffer[iIndex] == 44)
                {
                    iParamcount++;
                }
                else
                {
                    if(iParamcount==1)
                    {
                        //Enter destination email address
                        *pcOfemailto++ = usBuffer[iIndex];

                    }
                    if(iParamcount==2)
                    {
                        //Enter email subject
                        *pcOfemailsubject++ = usBuffer[iIndex];
                    }
                }
                iIndex++;
            }
            
            // '>' To maintain RFC 2821 format
            *pcOfemailto++= '>';
            *pcOfemailto++= '\0';
            *pcOfemailsubject= '\0';

            SlNetAppDestination_t destEmailAdd;
            memcpy(destEmailAdd.Email,pcEmailto,strlen(pcEmailto)+1);
            cStatus1 = sl_NetAppEmailSet(SL_NET_APP_EMAIL_ID,NETAPP_DEST_EMAIL,
                                         strlen(pcEmailto)+1,
                                         (unsigned char *)&destEmailAdd);

            SlNetAppEmailSubject_t emailSubject;
            memcpy(emailSubject.Value,pcEmailsubject,strlen(pcEmailsubject)+1);
            cStatus2 = sl_NetAppEmailSet(SL_NET_APP_EMAIL_ID,NETAPP_SUBJECT,
                                         strlen(pcEmailsubject)+1,
                                         (unsigned char *)&emailSubject);
            
            // Check for Error in setting the variables
            if(cStatus1 == 0 && cStatus2 == 0)
            {
                DispatcherUartSendPacket((char*)pucUARTOKString, 
                                         sizeof(pucUARTOKString));
            }
            else
            {
                DispatcherUartSendPacket((char*)putUARTErrorInputString, 
                                         sizeof(putUARTErrorInputString));  
            }
            break;
                
        //**********************************************
        // Case 04: Record email message
        //**********************************************
        case UART_COMMAND_CC31xx_EMAIL_MESSAGE:

            pcOfemailmessage = &pcEmailmessage[0];

            //Enter "Message"
            iIndex =3;
            while ((int)usBuffer[iIndex] != 0x0D && usBuffer[iIndex] != '\0')
            {
                if((int)usBuffer[iIndex] == 62)
                {
                    iParamcount++;
                }
                else
                {
                    if(iParamcount==0)
                    {
                        *pcOfemailmessage++ = usBuffer[iIndex];
                    }
                }
                iIndex++;
            }
            *pcOfemailmessage= '\0';
            /* TODO here unsigned char is converting to char */
            DispatcherUartSendPacket((char*)pucUARTOKString, 
                                     sizeof(pucUARTOKString));

            break; 
        //**********************************************
        // Case 05: Send email message using configurations
        //**********************************************
        case UART_COMMAND_CC31xx_EMAIL_SEND:
        {

        	// reset Orange LED state
        	GPIO_IF_LedOff(MCU_SENDING_DATA_IND);
            // TODO: If no destination email given, default to hardcoded value
            SlNetAppEmailOpt_t eMailServerSetting;

            eMailServerSetting.Ip = Network_IF_GetHostIP(GMAIL_HOST_NAME);
            if(eMailServerSetting.Ip !=0)
            {
                eMailServerSetting.Family = AF_INET;
                eMailServerSetting.Port = GMAIL_HOST_PORT;
                eMailServerSetting.SecurityMethod = SL_SO_SEC_METHOD_SSLV3;
                eMailServerSetting.SecurityCypher = SL_SEC_MASK_SSL_RSA_WITH_RC4_128_MD5;
                sl_NetAppEmailSet(SL_NET_APP_EMAIL_ID,NETAPP_ADVANCED_OPT,
                                  sizeof(SlNetAppEmailOpt_t),
                                  (unsigned char*)&eMailServerSetting);
            }
            g_cConnectStatus = sl_NetAppEmailConnect();
            // If return -1, throw connect error
            if(g_cConnectStatus == -1)
            {
                DispatcherUartSendPacket((char*)pucUARTErrorSocketCreateString,
                                         sizeof(pucUARTErrorSocketCreateString));
            }
            // If return -2, throw socket option error
            if(g_cConnectStatus == -2)
            {
                DispatcherUartSendPacket((char*)pucUARTErrorSocketOptionString,
                                         sizeof(pucUARTErrorSocketOptionString));
            }

            if(g_cConnectStatus == 0)
            {
                SlNetAppServerError_t sEmailErrorInfo;
                long lRetCode = SL_EMAIL_ERROR_FAILED;
                if((lRetCode = sl_NetAppEmailSend(pcEmailto,pcEmailsubject,
                                      pcEmailmessage,
                                      &sEmailErrorInfo)) == SL_EMAIL_ERROR_NONE)
                {
                    // Blink LED7 to indicate email has been sent
                    for(iIndex=0 ;iIndex<5 ;iIndex++)
                    {
                          MAP_UtilsDelay(6000000);
                          GPIO_IF_LedOff(MCU_SENDING_DATA_IND);
                          MAP_UtilsDelay(6000000);
                          GPIO_IF_LedOn(MCU_SENDING_DATA_IND);

                    }
                    DispatcherUartSendPacket((char*)putUARTFinishString, 
                                             sizeof(putUARTFinishString));
                }
                else
                {
                    EmailHandleERROR(lRetCode,(char*)sEmailErrorInfo.Value);
                }
            }
        }
        break;

        case UART_COMMAND_CC31xx_SMART_CONFIG:
        	GPIO_IF_LedOff(MCU_IP_ALLOC_IND);
            DispatcherUartSendPacket((char*)pucUARTSmartConfigString, 
                                     sizeof(pucUARTSmartConfigString));
              
            // Start LED blinking Timer
            LedTimerConfigNStart();
            
            //Reset the Network Status before Entering Smart Config
            Network_IF_UnsetMCUMachineState(STATUS_BIT_CONNECTION);
            Network_IF_UnsetMCUMachineState(STATUS_BIT_IP_AQUIRED);
            
            // start smart config process
            SmartConfigConnect();
            while (!(IS_CONNECTED(Network_IF_CurrentMCUState())) || 
                   !(IS_IP_ACQUIRED(Network_IF_CurrentMCUState())))
            {
                MAP_UtilsDelay(100);
            }
            LedTimerDeinitStop();
            GPIO_IF_LedOn(MCU_IP_ALLOC_IND);
            break;
		              
        default:
            DispatcherUartSendPacket((char*)pucUARTErrorString, 
                                     sizeof(pucUARTErrorString));
            break;
  }


}


/*!
	\brief 	Generates Random Message and Sends Email to the preconfigured 
            Recipient Email Id

	\param	pValue		-	pointer to Input Data

	\return void
	\note
	\warning
*/

void PushButtonMailSend(void* pValue)
{
    int iIndex;
    // reset Orange LED
    GPIO_IF_LedOff(MCU_SENDING_DATA_IND);

    if(!IS_CONNECTED(Network_IF_CurrentMCUState()))
    {
    	LedTimerConfigNStart();
        Network_IF_ConnectAP(SSID_NAME,SecurityParams);
    }

    //
    // Disable the LED blinking Timer as Device is connected to AP
    //
    LedTimerDeinitStop();
    //
    // Switch ON RED LED to indicate that Device acquired an IP
    //
    GPIO_IF_LedOn(MCU_IP_ALLOC_IND);
    MAP_UtilsDelay(10000);

    GenerateRandomMessage();
    g_cConnectStatus = sl_NetAppEmailConnect();
    // If return -1, throw connect error
    if(g_cConnectStatus == -1)
    {
        DispatcherUartSendPacket((char*)pucUARTErrorSocketCreateString, 
                                 sizeof(pucUARTErrorSocketCreateString));
    }
    // If return -2, throw socket option error
    if(g_cConnectStatus == -2)
    {
        DispatcherUartSendPacket((char*)pucUARTErrorSocketOptionString, 
                                 sizeof(pucUARTErrorSocketOptionString));
    }
    if(g_cConnectStatus == 0)
    {
        SlNetAppServerError_t sEmailErrorInfo;
        long lRetCode = SL_EMAIL_ERROR_FAILED;
        if((lRetCode = sl_NetAppEmailSend(pcEmailto,pcEmailsubject,
                                          pcEmailmessage,
                                          &sEmailErrorInfo)) == SL_EMAIL_ERROR_NONE)
        {
            // Blink LED7 to indicate email has been sent
            for(iIndex=0 ;iIndex<5 ;iIndex++)
            { 
                MAP_UtilsDelay(6000000);
                GPIO_IF_LedOff(MCU_SENDING_DATA_IND);
                MAP_UtilsDelay(6000000);
                GPIO_IF_LedOn(MCU_SENDING_DATA_IND);
            }
            DispatcherUartSendPacket((char*)putUARTFinishString, 
                                     sizeof(putUARTFinishString));
        }
        else
        {
            EmailHandleERROR(lRetCode,(char*)sEmailErrorInfo.Value);
        }
        Report("Cmd#");                
    }      

    //Enable GPIO Interrupt
    Button_IF_EnableInterrupt(SW3);
     
}

/*!
	\brief 	Push Button Task to Handle Trigger from Push Button

	\param	pvParameters		-	pointer to the task parameter

	\return void
	\note
	\warning
*/
void PushButtonHandler(void *pvParameters)
{
    tPushButtonMsg Msg; 
    int ret=0;
	
    for(;;)
    {
        ret = osi_MsgQRead( &g_PBQueue, &Msg, OSI_WAIT_FOREVER );
        if(OSI_OK==ret)
        {
            Msg.pEntry(Msg.pValue);
        }
    }
}

/*!
	\brief 	Interrupt Handler to Send Email upon Press of Push Button (S3)

	\param	none

	\return void
	\note
	\warning
*/

void SendEmailInterruptHandler()
{
    tPushButtonMsg sMsg;
    sMsg.pEntry = &PushButtonMailSend;
    sMsg.pValue = NULL;
    osi_MsgQWrite(&g_PBQueue,&sMsg,OSI_NO_WAIT);
}

/*!
	\brief 	Starts Smart Configuration

	\param	none

	\return void
	\note
	\warning
*/

void SmartConfigTask(void* pValue)
{
    DispatcherUartSendPacket((char*)pucUARTSmartConfigString, 
                             sizeof(pucUARTSmartConfigString));
    
    //Turn off the Network Status LED
    GPIO_IF_LedOff(MCU_IP_ALLOC_IND);
    
    LedTimerConfigNStart();
    
    //Reset the Network Status before Entering Smart Config
    Network_IF_UnsetMCUMachineState(STATUS_BIT_CONNECTION);
    Network_IF_UnsetMCUMachineState(STATUS_BIT_IP_AQUIRED);
    
    SmartConfigConnect();

    //
    // Wait until IP is acquired
    //
    while (!(IS_CONNECTED(Network_IF_CurrentMCUState())) || 
           !(IS_IP_ACQUIRED(Network_IF_CurrentMCUState())));
    
    LedTimerDeinitStop();
    
    // Red LED on
    GPIO_IF_LedOn(MCU_IP_ALLOC_IND);
    
    //Enable GPIO Interrupt
    Button_IF_EnableInterrupt(SW2);
}

/*!
	\brief 	Interrupt Handler to Start Smart Config upon Press of Push Button (S2)

	\param	none

	\return void
	\note
	\warning
*/

void SmartConfigInterruptHandler()
{
    tPushButtonMsg sMsg;
    sMsg.pEntry = &SmartConfigTask;
    sMsg.pValue = NULL;
    osi_MsgQWrite(&g_PBQueue,&sMsg,OSI_NO_WAIT);
}


/*!
	\brief 	Email Application Main Task - Initializes SimpleLink Driver 
            and Handles UART Commands

	\param	pvParameters		-	pointer to the task parameter

	\return void
	\note
	\warning
*/

static void SimpleEmail(void *pvParameters)
{
    // Initialize Network Processor
    Network_IF_InitDriver(ROLE_STA);

    //Glow GreenLED to indicate successful initialization
    GPIO_IF_LedOn(MCU_ON_IND);
    
    //Set Default Parameters for Email
    SetDefaultParameters();

    //Initialize Push Botton Switch
    Button_IF_Init(SmartConfigInterruptHandler,SendEmailInterruptHandler);

    while(1)
    {
        Report("Cmd#");
        //
        // Get command from UART
        //
        memset(ucUARTBuffer,0,200);  
        uiUartCmd=GetCmd(&ucUARTBuffer[0], 200);
        if(uiUartCmd)
        {
            //
            // Parse the command
            //
            UARTCommandHandler(ucUARTBuffer);
        }

    }
}

//*****************************************************************************
//
//! Application startup display on UART
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
static void
DisplayBanner(char * AppName)
{
    UART_PRINT("\n\n\n\r");
    UART_PRINT("\t\t *************************************************\n\r");
    UART_PRINT("\t\t		CC3200 %s Application       \n\r", AppName);
    UART_PRINT("\t\t *************************************************\n\r");
    UART_PRINT("\n\n\n\r");
}

//*****************************************************************************
//
//! Board Initialization & Configuration
//!
//! \param  None
//!
//! \return None
//
//*****************************************************************************
static void
BoardInit(void)
{
/* In case of TI-RTOS vector table is initialize by OS itself */
#ifndef USE_TIRTOS
    //
    // Set vector table base
    //
#if defined(ccs)
    MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
#endif
#if defined(ewarm)
    MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif
#endif
    //
    // Enable Processor
    //
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);

    PRCMCC3200MCUInit();
}



//*****************************************************************************
//
//! Main function to start execution 
//!
//! \param  None
//!
//! \return None
//
//*****************************************************************************

void main()
{

  //
  // Initialize Board configurations
  //
  BoardInit();
  
  //
  // Pinmuxing for GPIO,UART
  //
  PinMuxConfig();
  
  //
  // configure LEDs
  //
  GPIO_IF_LedConfigure(LED1|LED2|LED3);

  GPIO_IF_LedOff(MCU_ALL_LED_IND);

#ifndef NOTERM  
  //
  // Configuring UART
  //
  InitTerm();
#endif
  //
  // Display Welcome Message
  //
  DisplayBanner(APP_NAME);

  // Generate Menu Output for Application
  OutputMenu();

  // Initialize AP security params
  SecurityParams.Key = SECURITY_KEY;
  SecurityParams.KeyLen = strlen(SECURITY_KEY);
  SecurityParams.Type = SECURITY_TYPE;
  uiUartCmd=0;
  
  //
  // Simplelinkspawntask
  //
  VStartSimpleLinkSpawnTask(SPAWN_TASK_PRIORITY);

  osi_MsgQCreate(&g_PBQueue,"PBQueue",sizeof(tPushButtonMsg),1);
  osi_TaskCreate(PushButtonHandler, (signed char*)"PushButtonHandler",
                 OSI_STACK_SIZE , NULL, TASK_PRIORITY+2, &g_PushButtonTask );

  osi_TaskCreate(SimpleEmail, (signed char*)"SimpleEmail",OSI_STACK_SIZE ,
                 NULL, TASK_PRIORITY+1, NULL );

  osi_start();

  while(1)
  {

  }

}
        
       
