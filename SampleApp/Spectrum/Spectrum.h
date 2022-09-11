#pragma once
#include "KeyMapping.h"
#include "SpectrumTypes.h"
#include <vector>
#include <map>
#include <string>


class Spectrum
{
	public:
		Spectrum();
		void Init();
		uint8_t* AddEffect(uint8_t profile, const std::vector<uint16_t>& Key, const std::vector<RGB>& rgb, Effect Effect, uint8_t Speed, uint8_t Unknwon1, uint8_t Direction, ColorMode ColorMode, uint8_t Unknwon2);
		uint8_t* SetBacklight(uint8_t level);
		uint8_t* SetProfile_1(uint8_t profile);
		uint8_t* SetProfile_2(uint8_t profile);
	private:
		uint8_t EffectNo = 0;
		uint8_t data[960] = { 0 };
		uint8_t data_backLight[960] = { 0 };
		uint8_t data_Profile[960] = { 0 };
};
