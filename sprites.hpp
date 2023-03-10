#pragma once

// bricks
#define bricksWidth 64
#define bricksHeight 8
#define bricksFlags BLIT_2BPP
const uint8_t bricks[128] = { 
	0x55,0x55,0xaa,0xaa,0xaa,0xaa,0xaa,0xab,  0xaa,0xaa,0xaa,0xaa,0xa5,0x5a,0x57,0xd5,
	0x6a,0xa9,0xaa,0xaa,0xae,0xae,0xab,0xea,  0xaa,0xaa,0xaa,0xaa,0x9f,0xf6,0x57,0xd5,
	0x6a,0xa9,0xaa,0xaa,0xba,0xba,0xaf,0xae,  0xaf,0xfa,0xff,0xff,0x77,0xdd,0x5f,0xf5,
	0x6b,0xe9,0xaa,0xaa,0xaa,0xba,0xbb,0xaa,  0xad,0x7a,0x55,0x55,0x7f,0xfd,0xff,0xff,
	0x6b,0xe9,0xaa,0xaa,0xaa,0xaa,0xbb,0xbe,  0xad,0x7a,0x55,0x55,0x77,0xdd,0x7f,0xfd,
	0x6a,0xa9,0xaa,0xaa,0xab,0xaa,0xea,0xeb,  0xaf,0xfa,0xff,0xff,0x7d,0x7d,0x5f,0xf5,
	0x6a,0xa9,0xaa,0xaa,0xae,0xaa,0xaa,0xaa,  0xaa,0xaa,0xaa,0xaa,0x9f,0xf6,0x7d,0x7d,
	0x55,0x55,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,  0xaa,0xaa,0xaa,0xaa,0xa5,0x5a,0xd5,0x57,
};

// heart
const uint8_t heart[8] = {
	0b01101100,
	0b11111110,
	0b11111110,
	0b11111110,
	0b01111100,
	0b00111000,
	0b00010000,
	0b00000000,
};

// splash2
#define splash2Width 144
#define splash2Height 33
#define splash2Flags BLIT_1BPP
const uint8_t splash2[594] = { 
	0xff,0xff,0xff,0xf8,0x0f,0xff,0xff,0xff,  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xf0,0x03,0xff,0xe3,0xe1,0xff,  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xe7,0xf8,0x7f,0xcf,  0xfc,0x3f,0x00,0x3f,0xfc,0x3f,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xcf,0xff,  0x3f,0x9f,0xff,0xde,0x7f,0x8f,0xf9,0xcf,
	0xfc,0x1f,0xff,0xff,0x1f,0xff,0xff,0xff,  0x9f,0xff,0xcf,0x3f,0xff,0xdc,0xff,0xe7,
	0xf3,0xe7,0xf9,0xec,0x3f,0xfc,0x47,0xf0,  0x01,0xff,0x3f,0xff,0xe7,0x7f,0xff,0xd9,
	0xff,0xf3,0xe7,0xf7,0xfb,0xe1,0xdf,0xfd,  0xf3,0xc7,0xfc,0x7f,0x7f,0xff,0xf2,0x7f,
	0xff,0xe3,0xff,0xfb,0xef,0xfb,0xf3,0xe7,  0xcf,0xf9,0xf9,0x9f,0xff,0x3f,0x7f,0xff,
	0xfa,0xff,0xff,0xe7,0xff,0xfb,0xdf,0xfb,  0xf7,0xe7,0xef,0xfb,0xfc,0x3f,0xff,0xbf,
	0x7f,0xff,0xfa,0xff,0xff,0xe7,0xff,0xfb,  0x9f,0xf9,0xf7,0xf7,0xef,0xfb,0xfe,0x7f,
	0xff,0xdf,0x7f,0xff,0xfa,0xff,0xff,0xe7,  0xff,0xf3,0xbf,0xfd,0xe7,0xf7,0xef,0xf9,
	0xfc,0x7f,0xff,0xdf,0x7f,0xff,0xfa,0xff,  0xff,0xe7,0xff,0xe7,0xbf,0xfe,0xef,0xe7,
	0xef,0xfc,0xf1,0x7f,0xff,0xdf,0x7f,0xff,  0xfa,0xff,0xff,0xcf,0xff,0x07,0x3f,0xfe,
	0xef,0xef,0xdf,0xfe,0x03,0x7f,0xff,0xdf,  0x7f,0xff,0xfa,0xff,0xff,0xcf,0xf0,0x73,
	0x7f,0xfe,0x6f,0xef,0x9f,0xfc,0xfb,0x7f,  0xff,0xbf,0x7f,0xff,0xf2,0xff,0xff,0x8f,
	0xff,0xfa,0x7f,0xff,0x4f,0xef,0xb0,0x19,  0xf9,0x3f,0xff,0x3f,0x7f,0xff,0xf6,0xff,
	0xff,0x2f,0xff,0xfa,0xff,0xff,0x5f,0xff,  0x27,0xcb,0xfd,0x9f,0xfe,0x7f,0x7f,0xff,
	0xee,0xff,0xff,0x6f,0xff,0xfa,0xff,0xff,  0x1f,0xfe,0x4f,0xe3,0xfd,0xc1,0xfd,0xff,
	0x7f,0xff,0x8e,0xfc,0x7c,0xef,0xff,0xfa,  0xff,0xff,0x9f,0xf8,0xdf,0xe3,0xfd,0xfd,
	0xfd,0xff,0x7f,0xe0,0x06,0xff,0x80,0x6f,  0xf0,0x02,0xff,0xff,0x9f,0xfe,0x5f,0x8b,
	0xfd,0xfd,0xfd,0xff,0x7f,0xf8,0xf2,0xff,  0xcf,0x6f,0xff,0x9c,0xff,0xff,0x9f,0xff,
	0x00,0x1b,0xfd,0xfd,0xfd,0xff,0x3f,0xff,  0xf8,0xff,0xef,0x6f,0xff,0xcd,0xff,0xff,
	0x9f,0xef,0x9f,0xfb,0xfd,0xfd,0xfd,0xff,  0xbf,0xff,0xfc,0xff,0xcf,0x2f,0xff,0xe5,
	0xff,0x7f,0x9f,0xef,0xdf,0xfb,0xfd,0xf9,  0xfd,0xff,0xbf,0xff,0xfc,0xff,0xdf,0x2f,
	0xff,0xf5,0xff,0x7f,0x9f,0xef,0xdf,0xfb,  0xfd,0xf9,0xfd,0xff,0x9f,0xff,0xfc,0xff,
	0xdf,0xaf,0xff,0xf5,0xff,0x7f,0x5f,0xef,  0xdf,0xfb,0xfb,0xf9,0xfd,0xff,0xdf,0xff,
	0xfc,0x7f,0xdf,0xaf,0xff,0xf5,0xff,0x7f,  0x5f,0xef,0xcf,0xfb,0xfb,0xf9,0xfd,0xff,
	0xcf,0xff,0xfd,0x7f,0xdf,0xaf,0xff,0xe5,  0xfe,0x7e,0x5f,0xcf,0xef,0xfb,0xfb,0xf9,
	0xfd,0xff,0xef,0xff,0xfd,0x3f,0x9f,0xa7,  0xff,0xcd,0xfe,0xfe,0xdf,0x8f,0xcf,0xfb,
	0xf3,0xfe,0xfb,0xff,0xe7,0xff,0xf9,0x9f,  0x1f,0xb3,0xff,0x9d,0xfc,0xfc,0xcf,0x97,
	0xdf,0xf9,0xe7,0xfe,0xc7,0xff,0xf7,0xff,  0xfb,0xdf,0x0f,0xb8,0xff,0x3c,0xf8,0x79,
	0xe7,0x30,0x1f,0xfc,0x0f,0xfe,0x3f,0xff,  0xf3,0xff,0xfb,0xce,0x60,0x3e,0x1e,0x7e,
	0x23,0x13,0xf0,0x7f,0xff,0xff,0xff,0xff,  0xff,0xff,0xf9,0xff,0xf7,0xe0,0xff,0xff,
	0xc0,0xff,0x8f,0xc7,0xff,0xff,0xff,0xff,  0xff,0xff,0xff,0xff,0xfc,0xff,0xe7,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,  0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0x3f,
	0x0f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0x80,0xff,0xff,0xff,0xff,0xff,0xff,  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff 
};
