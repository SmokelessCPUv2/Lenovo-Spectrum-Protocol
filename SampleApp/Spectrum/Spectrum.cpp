#include "Spectrum.h"

Spectrum::Spectrum()
{
	Init();
}

void Spectrum::Init()
{
	memset(data, 0, 960);
	memset(data_backLight, 0, 960);
	memset(data_Profile, 0, 960);
}


uint8_t* Spectrum::AddEffect(uint8_t profile, const std::vector<uint16_t>& Key, const std::vector<RGB>& rgb, Effect Effect, uint8_t Speed, uint8_t Unknwon1, uint8_t Direction, ColorMode ColorMode, uint8_t Unknwon2)
{
	if (EffectNo == 0)
	{
		ColorChange Color;
		Color.header.Operation = EffectChange;
		Color.Profile = profile;
		memcpy(data, &Color, sizeof(Color));
	}
	Header* header = (Header*)data;
	header->PacketSize = offsetof(ColorChange, Effect1);

	Effect_Data EffectData;
	EffectData.Effect_NO = EffectNo + 1;
	EffectData.EffectHeader.NumRGBDataFollowing = rgb.size();
	EffectData.EffectHeader.Effect = Effect;
	EffectData.EffectHeader.Speed = Speed;
	EffectData.EffectHeader.Direction = Direction;
	EffectData.EffectHeader.ColorMode = ColorMode;

	memcpy(data + header->PacketSize, &EffectData, sizeof(EffectData));
	header->PacketSize += sizeof(EffectData);
	
	for (int i = 0; i < rgb.size(); i++)
	{
		memcpy(data + header->PacketSize, &rgb.at(i) , sizeof(RGB));
		header->PacketSize += sizeof(RGB);
	}
	data[header->PacketSize] = Key.size();
	header->PacketSize += 1;
	for (int i = 0; i < Key.size(); i++)
	{
		memcpy(data + header->PacketSize, &Key.at(i), sizeof(uint16_t));
		header->PacketSize += sizeof(uint16_t);
	}

	EffectNo++;
	return data;
}






uint8_t* Spectrum::SetBacklight(uint8_t level)
{
	BrightnessChange brightness;
	brightness.header.Operation = Brightness;
	brightness.Brightness = level;
	brightness.header.PacketSize += sizeof(brightness.Brightness);
	memcpy(data_backLight, &brightness, sizeof(brightness));
	return data_backLight;
}

uint8_t* Spectrum::SetProfile_1(uint8_t profile)
{
	ProfileChange Profile;
	Profile.Profile = profile;
	Profile.header.Operation = Profile_Set_1;
	Profile.header.PacketSize += sizeof(Profile.Profile);
	memcpy(data_Profile, &Profile, sizeof(ProfileChange));
	return data_Profile;
}

uint8_t* Spectrum::SetProfile_2(uint8_t profile)
{
	ProfileChange Profile;
	Profile.Profile = profile;
	Profile.header.Operation = Profile_Set_2;
	Profile.header.PacketSize += sizeof(Profile.Profile);
	memcpy(data_Profile, &Profile, sizeof(ProfileChange));
	return data_Profile;
}