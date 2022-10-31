# Lenovo Spectrum Protocol

The new Legion RGB keyboard support both setting and getting effect..


# Writing

All communication is done sending a Hid Report over the USB Device (VID/PID: 0x048D/0xC977), the report has a size of 960 byte.


The report size is always 960 byte, so if the control structure is less than that, fill the remaining space with Zero.


All packet Contain a 4 byte Header

## Header

    struct Header
    {
        uint8_t Head = 0x7; //This is always 0x7
        uint8_t Operation;  //This Set the Operation to Execute
        uint8_t PacketSize; //0xC0 when Profile/Brightness or AuraraSync, otherwise the Payload Size if more than 256, put Size%256 here
        uint8_t Tail = 0x;  //This is always 0x3
    }

### Operation

The Field Operation Can be one of these

        enum Operation : uint8_t
        {
            Profile_Set_1 = 0xC8,       //Command to change Profile
            EffectChange = 0xCB,        //Command to change Effect
            Profile_Set_2 = 0xCC,       //Command  to change Profile
            Brightness = 0xCE           //Command to Change Brightness
            AuroraSendBitmap = 0xA1     //Command to send the aurora bitmap
        };

## Brightness Change

A brightness change packet look like the following

        struct BrightnessChange
        {
            struct Header header; //The main header with Operation set to Brightness (0xCE)
            uint8_t Brightness;  //Brigtness Value
            uint8_t Zero_Pad[955]; //Pad the remaining Byte With Zero
        }

## Profile Change

A Profile change packet look like the following

        struct ProfileChange
        {
            struct Header header; //The main header with Operation set to Profile_Set_2 (0xCC)
            uint8_t Profile;  //Profile Value
            uint8_t Zero_Pad[955]; //Pad the remaining Byte With Zero
        }

Followed by 

        struct ProfileChange
        {
            struct Header header; //The main header with Operation set to Profile_Set_1 (0xC8)
            uint8_t Profile;  //Profile Value
            uint8_t Zero_Pad[955]; //Pad the remaining Byte With Zero
        }

# Color and Effect

The Color Data is a Simple 8-8-8 RBG

    struct RGB
    {
        uint8_t R;
        uint8_t G;
        uint8_t B;
    };


A Profile is built by chaining multiple Effect together with the Header: 

        NO_EFFECT  is how many effect you want to send

        struct ProfileChange
        {
            struct Header header; //The main header with Operation set to EffectChange (0xCB)
            uint8_t Profile;  //Profile Value
            uint8_t unk1;     //Is always 1
            uint8_t unk2;     //Is always 1
            struct Effect[NO_EFFECT]
        }

the Effect Structure look Like the following

    struct Effect
    {
        uint8_t Effect_NO; //the first is 1 and the last is NO_EFFECT
        struct EffectHeader EffectHeader;
        struct RGBData[EffectHeader.NumRGBDataFollowing]; //RGB Data According to the Effect Header Request
        uint8_t No_of_key; //Number of Key to witch apply the effect
        uint16_t KeyCode;
    }


    struct EffectHeader
    {
        uint8_t Head = 0x6;
        uint8_t Param1_Header = 0x1;
        Effect Effect;
        uint8_t Param2_Header = 0x2;
        uint8_t Speed = 0x0;
        uint8_t Param3_Header = 0x3;
        uint8_t Unknwon2 = 0x0;
        uint8_t Param4_Header = 0x4;
        Direction Direction;
        uint8_t Param5_Header = 0x5;
        uint8_t ColorMode;
        uint8_t Param6_Header = 0x6;
        uint8_t Unknwon5 = 0x0;         //Always 0
        uint8_t NumRGBDataFollowing;
    };

# The Available Effect are

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

and color Mode can be

    enum ColorMode : uint8_t
    {
        None = 0x0, //For Effect that don't require a Color
        RandomColor = 0x1,
        ColorList = 0x2
    };

    when selecting ColorList, set the EffectHeader.NumRGBDataFollowing accordingly, otherwise is 0

# Aurora Sync Mode

When Working with Legion_Aurora_Sync, you need to periodically send the "bitmap" to display over the keyboard

    //TODO Add Aurora Sync structure



## Key Mapping

    0x01 -> Esc
    0x02 -> F1
    0x03 -> F2
    0x04 -> F3
    0x05 -> F4
    0x06 -> F5
    0x07 -> F6
    0x08 -> F7
    0x09 -> F8
    0x0A -> F9
    0x0B -> F10
    0x0C -> F11
    0x0D -> F12
    0x0E -> Insert
    0x0F -> Stamp
    0x10 -> Canc
    0x11 -> Home
    0x12 -> End
    0x13 -> PagUp
    0x14 -> PagDown
    0x16 -> Tilde (The one before 1, idk on other locale what is there)
    0x17 -> 1
    0x18 -> 2
    0x19 -> 3
    0x1A -> 4
    0x1B -> 5
    0x1C -> 6
    0x1D -> 7
    0x1E -> 8
    0x1F -> 9
    0x20 -> 0
    0x21 -> - (The one after 0, idk on other locale what is there)
    0x22 -> = (The one after previus, idk on other locale what is there)
    0x26 -> NumLock
    0x27 -> NumPad_Divide
    0x28 -> Numpad_Multiply
    0x29 -> Numpad_-
    0x38 -> BackSpace
    0x40 -> Tab
    0x42 -> Q
    0x43 -> W
    0x44 -> E
    0x45 -> R
    0x46 -> T
    0x47 -> Y
    0x48 -> U
    0x49 -> I
    0x4A -> O
    0x4B -> P
    0x4C -> { (The one after previus, idk on other locale what is there)
    0x4D -> } (The one after previus, idk on other locale what is there)
    0x4E -> \ (The one after previus, idk on other locale what is there)
    0x4F -> Numpad_7
    0x50 -> Numpad_8
    0x51 -> Numpad_9
    0x55 -> Caps_Lock
    0x58 -> D
    0x59 -> F
    0x5A -> G
    0x5B -> K
    0x5C -> L
    0x5D -> | (The one after previus, idk on other locale what is there)
    0x5F -> ' (The one after previus, idk on other locale what is there)
    0x68 -> Numpad_+
    0x6A -> Shift_L
    0x6D -> A
    0x6E -> S
    0x6F -> C
    0x70 -> V
    0x71 -> H
    0x72 -> J
    0x73 -> M
    0x74 -> < (The one after previus, idk on other locale what is there)
    0x75 -> > (The one after previus, idk on other locale what is there)
    0x76 -> / (The one after previus, idk on other locale what is there)
    0x77 -> Enter
    0x79 -> Numpad_4
    0x7B -> Numpad_5
    0x7C -> Numpad_6
    0x7F -> Ctrl_L
    0x80 -> Fn
    0x82 -> Z
    0x83 -> X
    0x87 -> B
    0x88 -> N
    0x8D -> Shift_R
    0x8E -> Numpad_1
    0x90 -> Numpad_2
    0x92 -> Numpad_3
    0x96 -> Windows
    0x97 -> Alt_L
    0x98 -> Space
    0x9A -> Alt_R
    0x9B -> Ctrl_R
    0x9C -> Left
    0x9D -> Up
    0x9F -> Down
    0xA1 -> Right
    0xA3 -> Numpad_0
    0xA5 -> Numpad_Dot
    0xA7 -> Numpad_Enter


# Reading

    WIP


# Reference implementation

    WIP: Sample C++ Code/class, for the protocol Implementation

# Credit

Huge thanks to GhostFella#4719 for his dump and his patience

Huge thanks to [Legion Series Discord](https://discord.gg/XVf3ZEFVJA)
