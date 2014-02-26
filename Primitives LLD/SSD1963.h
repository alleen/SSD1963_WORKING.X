/*****************************************************************************
 *  Solomon Systech. SSD1963 LCD controller driver
 *****************************************************************************
 * FileName:        SSD1963.h
 * Dependencies:    plib.h, SSD1963_CMD.h, TimeDelay.h, TFT.h, & system.h
 * Processor:       Microchip PIC32MX360F512L
 * Compiler:       	MPLAB C32 version 1.10 or higher
 * Linker:          MPLAB LINK32
 * Company:			TechToys Company
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * John Leung		    17/06/2011
 *****************************************************************************/

#ifndef _SSD1963_H
#define _SSD1963_H

#include "../system.h"			//Define the hardware platform, system clock speed etc.
#include "SSD1963_CMD.h"		//include the command table for SSD1963
#include "TimeDelay.h"			//required for DelayMs(xx) software delay
#include "TFT.h"			//configuration for individual TFT panels

extern WORD	_color;

/******************************************************************************
* IO port definitions for Microchip PIC32MX series.
* Two development platforms available from us as the MCU host:
* (1) Multimedia Evaluation Kit Rev 1A (MMEVK R1A)
* (2) PIC24/32 EVK RD4 with PIC32MX360F512L as the MCU.
*
* Function	mcu pins			LCD pins
* ==========	========			========
* TE		AN4/RB4 or RG15			TE
* DATA LINES	PMD[15:0]			D[15:0] in 16-bit addressing
* RESET		RD1 or RA7			/RESET
* CHIP SELECT	AN15/RB15 or RD11		/CS
* COMMAND/DATA	RD3 or RA6			RS 
* WR STROBE	RD4/PMPWR			/WR
* RD STROBE	RD5/PMPRD			/RD
*
******************************************************************************/
#if defined (PIC32_STARTER_KIT)
	//Definitions for TE pin of SSD1963
	#define TE_TRIS_BIT         TRISBbits.TRISB4    //set digital under ResetDevice req.
	#define TE_PORT_BIT         PORTBbits.RB4       //it is AN4
		
	// Definitions for reset pin
	#define RST_TRIS_BIT        TRISDbits.TRISD1
	#define RST_LAT_BIT         LATDbits.LATD1
		
	//Definition for RS pin
	#define RS_TRIS_BIT         TRISDbits.TRISD3
	#define RS_LAT_BIT          LATDbits.LATD3
		
	// Definitions for CS pin
	#define CS_TRIS_BIT         TRISBbits.TRISB15   //set digital under ResetDevice req.
	#define CS_LAT_BIT          LATBbits.LATB15     //it is AN15
		
	// Definition for WR pin
	#define WR_TRIS_BIT         TRISDbits.TRISD4
	#define WR_LAT_BIT          LATDbits.LATD4
		
	// Definition for RD pin
	#define RD_TRIS_BIT         TRISDbits.TRISD5
	#define RD_LAT_BIT          LATDbits.LATD5

	//No IO port set for data bus because PMP module is used for the Microchip series
#elif defined (MIKROSHIT)
	// Definitions for reset pin
	#define RST_TRIS_BIT        TRISCbits.TRISC1
	#define RST_LAT_BIT         LATCbits.LATC1
		
	//Definition for RS pin
	#define RS_TRIS_BIT         TRISBbits.TRISB15
	#define RS_LAT_BIT          LATBbits.LATB15
		
	// Definitions for CS pin
	#define CS_TRIS_BIT         TRISFbits.TRISF12
	#define CS_LAT_BIT          LATFbits.LATF12
		
	// Definition for WR pin
	#define WR_TRIS_BIT         TRISDbits.TRISD4
	#define WR_LAT_BIT          LATDbits.LATD4
		
	// Definition for RD pin
	#define RD_TRIS_BIT         TRISDbits.TRISD5
	#define RD_LAT_BIT          LATDbits.LATD5
	//No IO port set for data bus because PMP module is used for the Microchip series
#endif

/*********************************************************************
* Macros: RGB565CONVERT(red, green, blue)
*
* Overview: Converts true color into 5:6:5 RGB format.
*
* PreCondition: none
*
* Input: Red, Green, Blue components.
*
* Output: 5 bits red, 6 bits green, 5 bits blue color.
*
* Side Effects: none
*
********************************************************************/
#define RGB565CONVERT(red, green, blue) (WORD) (((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3))

/*********************************************************************
* Overview: Some basic colors definitions.
*********************************************************************/
#define BLACK               RGB565CONVERT(0,    0,    0)
#define BRIGHTBLUE          RGB565CONVERT(0,    0,    255)
#define BRIGHTGREEN         RGB565CONVERT(0,    255,  0)
#define BRIGHTCYAN          RGB565CONVERT(0,    255,  255)
#define BRIGHTRED           RGB565CONVERT(255,  0,    0)
#define BRIGHTMAGENTA       RGB565CONVERT(255,  0,    255)
#define BRIGHTYELLOW        RGB565CONVERT(255,  255,  0)
#define BLUE                RGB565CONVERT(0,    0,    128)
#define GREEN               RGB565CONVERT(0,    128,  0)
#define CYAN                RGB565CONVERT(0,    128,  128)
#define RED                 RGB565CONVERT(128,  0,    0)
#define MAGENTA             RGB565CONVERT(128,  0,    128)
#define BROWN               RGB565CONVERT(255,  128,  0)
#define LIGHTGRAY           RGB565CONVERT(128,  128,  128)
#define DARKGRAY            RGB565CONVERT(64,   64,   64)
#define LIGHTBLUE           RGB565CONVERT(128,  128,  255)
#define LIGHTGREEN          RGB565CONVERT(128,  255,  128)
#define LIGHTCYAN           RGB565CONVERT(128,  255,  255)
#define LIGHTRED            RGB565CONVERT(255,  128,  128)
#define LIGHTMAGENTA        RGB565CONVERT(255,  128,  255)
#define YELLOW              RGB565CONVERT(255,  255,  128)
#define WHITE               RGB565CONVERT(255,  255,  255)
                            
#define GRAY0       	    RGB565CONVERT(224,  224,  224)
#define GRAY1         	    RGB565CONVERT(192,  192,  192)   
#define GRAY2               RGB565CONVERT(160,  160,  160)   
#define GRAY3               RGB565CONVERT(128,  128,  128)
#define GRAY4               RGB565CONVERT(96,   96,   96)
#define GRAY5               RGB565CONVERT(64,   64,   64)
#define GRAY6	            RGB565CONVERT(32,   32,   32)

/*********************************************************************
* Function:  void ResetDevice()
*
* Overview: Initializes LCD module.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void ResetDevice(void);

/*********************************************************************
* Function: void PutPixel(SHORT x, SHORT y)
*
* Overview: Puts pixel with the given x,y coordinate position.
*
* PreCondition: none
*
* Input: x - x position of the pixel.
*		 y - y position of the pixel.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void PutPixel(SHORT x, SHORT y);

/*********************************************************************
* Function: void ClearDevice(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: clears screen with current color 
*
* Note: none
*
********************************************************************/
void ClearDevice(void);

/*********************************************************************
* Function:  SetScrollArea(SHORT top, SHORT scroll, SHORT bottom)
*
* PreCondition: none
*
* Input: top - Top Fixed Area in number of lines from the top
*				of the frame buffer
*		 scroll - Vertical scrolling area in number of lines
*		 bottom - Bottom Fixed Area in number of lines
*
* Output: none
*
* Side Effects: none
*
* Overview:
*
* Note: Reference: section 9.22 Set Scroll Area, SSD1963 datasheet Rev0.20
*
********************************************************************/
void SetScrollArea(SHORT top, SHORT scroll, SHORT bottom);

/*********************************************************************
* Function:  void  SetScrollStart(SHORT line)
*
* Overview: First, we need to define the scrolling area by SetScrollArea()
*			before using this function. 
*
* PreCondition: SetScrollArea(SHORT top, SHORT scroll, SHORT bottom)
*
* Input: line - Vertical scrolling pointer (in number of lines) as 
*		 the first display line from the Top Fixed Area defined in SetScrollArea()
*
* Output: none
*
* Note: Example -
*
*		SHORT line=0;
*		SetScrollArea(0,272,272);
*		for(line=0;line<272;line++) {SetScrollStart(line);DelayMs(100);}
*		
*		Code above scrolls the whole page upwards in 100ms interval 
*		with page 2 replacing the first page in scrolling
********************************************************************/
void SetScrollStart(SHORT line);

/*********************************************************************
* Function:  void EnterSleepMode (void)
* PreCondition: none
* Input:  none
* Output: none
* Side Effects: none
* Overview: SSD1963 enters sleep mode
* Note: Host must wait 5mS after sending before sending next command
********************************************************************/
void EnterSleepMode (void);

/*********************************************************************
* Function:  void ExitSleepMode (void)
* PreCondition: none
* Input:  none
* Output: none
* Side Effects: none
* Overview: SSD1963 exits sleep mode
* Note:   cannot be called sooner than 15ms
********************************************************************/
void ExitSleepMode (void);

/*********************************************************************
* Function		: void DisplayOff(void)
* PreCondition	: none
* Input			: none
* Output		: none
* Side Effects	: none
* Overview		: SSD1963 changes the display state to OFF state
* Note			: none
********************************************************************/
void DisplayOff(void);

/*********************************************************************
* Function		: void DisplayOn(void)
* PreCondition	: none
* Input			: none
* Output		: none
* Side Effects	: none
* Overview		: SSD1963 changes the display state to ON state
* Note			: none
********************************************************************/
void DisplayOn(void);

/*********************************************************************
* Function		: void EnterDeepSleep(void)
* PreCondition	: none
* Input			: none
* Output		: none
* Side Effects	: none
* Overview		: SSD1963 enters deep sleep state with PLL stopped
* Note			: none
********************************************************************/
void EnterDeepSleep(void);

/*********************************************************************
* Function:  void  SetBacklight(BYTE intensity)
*
* Overview: This function makes use of PWM feature of ssd1963 to adjust
*			the backlight intensity. 
*
* PreCondition: Backlight circuit with shutdown pin connected to PWM output of ssd1963.
*
* Input: 	(BYTE) intensity from 
*			0x00 (total backlight shutdown, PWM pin pull-down to VSS)
*			0xff (99% pull-up, 255/256 pull-up to VDD)
*
* Output: none
*
* Note: The base frequency of PWM set to around 300Hz with PLL set to 120MHz.
*		This parameter is hardware dependent
********************************************************************/
void SetBacklight(BYTE intensity);

/*********************************************************************
* Function:  void  SetTearingCfg(BOOL state, BOOL mode)
*
* Overview: This function enable/disable tearing effect
*
* PreCondition: none
*
* Input: 	BOOL state -    1 to enable
*				0 to disable
*		BOOL mode -     0:  the tearing effect output line consists
*                                   of V-blanking information only
*				1:  the tearing effect output line consists
*                                   of both V-blanking and H-blanking info.
* Output: none
*
* Note:
********************************************************************/
void SetTearingCfg(BOOL state, BOOL mode);


#endif // _SSD1963_H
