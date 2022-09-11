#pragma once



enum Operation : uint8_t
{
	Profile_Set_1 = 0xC8,		//Command to change Profile
	EffectChange = 0xCB,		//Command to change Effect
	Profile_Set_2 = 0xCC,		//Command  to change Profile
	Brightness = 0xCE,			//Command to Change Brightness
	AuroraSendBitmap = 0xA1		//Command to send the aurora bitmap
};

struct Header
{
	uint8_t Head = 0x7;				//This is always 0x7
	Operation Operation;				//This Set the Operation to Execute
	uint8_t PacketSize = 0x4;		//The Payload Size; if more than 256, put Size%256 here
	uint8_t Tail = 0x3;				//This is always 0x3
};

struct BrightnessChange
{
	struct Header header;		//The main header with Operation set to Brightness (0xCE)
	uint8_t Brightness;			//Brigtness Value
};

struct ProfileChange
{
	struct Header header;	//The main header with Operation set to Profile_Set_2 (0xCC)
	uint8_t Profile;		//Profile Value
};

struct RGB
{
	uint8_t R;
	uint8_t G;
	uint8_t B;
};


enum Effect : uint8_t
{
	Screen_Rainbow = 1,
	Rainbow_Wave = 2,
	Color_Change = 3,
	Color_Pulse = 4,
	Color_Wave = 5,
	Smoth = 6,
	Rain = 7,
	Ripple = 8,                     //Apply to All key, send all 0x65 KeyCode
	Audio_bounce_Lighting = 9,      //Apply to All key, send all 0x65 KeyCode
	Audio_ripple_Lighting = 0xa,    //Apply to All key, send all 0x65 KeyCode
	Always = 0xb,                   //NumRGBDataFollowing need to be 1
	Type_Lighting = 0xc,            //Apply to All key, send all 0x65 KeyCode
	Legion_Aurora_Sync = 0xd,       //Apply to All key, send all 0x65 KeyCode  
};

enum ColorMode : uint8_t
{
	None = 0x0, //For Effect that don't require a Color
	RandomColor = 0x1,
	ColorList = 0x2
};


struct EffectHeader
{
	uint8_t Head = 0x6;
	uint8_t Param1_Header = 0x1;
	Effect Effect;
	uint8_t Param2_Header = 0x2;
	uint8_t Speed = 0x0;
	uint8_t Param3_Header = 0x3;
	uint8_t Unknwon = 0x0;
	uint8_t Param4_Header = 0x4;
	uint8_t Direction;
	uint8_t Param5_Header = 0x5;
	uint8_t ColorMode;
	uint8_t Param6_Header = 0x6;
	uint8_t Unknwon1 = 0x0;         //Always 0
	uint8_t NumRGBDataFollowing;
};


struct Effect_Data
{
	uint8_t Effect_NO; //the first is 1 and the last is NO_EFFECT
	struct EffectHeader EffectHeader;
};

struct ColorChange
{
	struct Header header; //The main header with Operation set to EffectChange (0xCB)
	uint8_t Profile;  //Profile Value
	uint8_t unk1 = 1;     //Is always 1
	uint8_t unk2 = 1;     //Is always 1
	struct Effect_Data Effect1;
};