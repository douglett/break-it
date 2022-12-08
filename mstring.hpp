// mstring
// very small string class
#pragma once


struct mstring {
public: 
	static const uint8_t MSTRING_MAX = 127;
private: 
	char c[MSTRING_MAX+1] = {0};
	uint8_t len = 0;

public:
	const char* str   () { c[len] = 0; return c; }
	uint8_t     length() { return len; }

	mstring& cat(const mstring& s) {
		int i = 0;
		for (; i < s.len; i++)
			if   (len + i >= MSTRING_MAX) break;
			else c[len + i] = s.c[i];
		len += i;
		c[len+1] = 0;
		return *this;
	}

	mstring& catlit(const char* cstr) { cat(mstring::lit(cstr)); return *this; }
	mstring& catint(uint16_t v)       { cat(mstring::itos(v));   return *this; }

	static mstring itos(uint16_t v) {
		mstring  rval;
		uint16_t pow = 10000;  // power - goes up to 65535
		uint8_t  pos = 0, digit = 0, show = 0;
		while (pow > 0) {
			digit = (v / pow) % 10;  // get digit
			if (digit) show = 1;  // hide leading zeroes
			if (show) rval.c[pos++] = '0' + (char)digit;
			pow /= 10;
		}
		if (pos == 0) rval.c[0] = '0', pos++;
		rval.c[pos] = 0;
		rval.len = pos;
		return rval;
	}

	static mstring lit(const char* cstr) {
		mstring rval;
		uint8_t i = 0;
		for (; i < MSTRING_MAX && cstr[i] != 0; i++)
			rval.c[i] = cstr[i];
		rval.c[i] = 0;
		rval.len = i;
		return rval;
	}
};