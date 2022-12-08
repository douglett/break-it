// etc.
// various useful functions
#pragma once


// some of my favourite palettes
struct Palette {
	uint32_t* p = PALETTE;
	void icecream() { p[0]=0xfff6d3, p[1]=0xf9a875, p[2]=0xeb6b6f, p[3]=0x7c3f58; }  // ice-cream GB
	void wasm4()    { p[0]=0xe0f8cf, p[1]=0x86c06c, p[2]=0x306850, p[3]=0x071821; }  // classic wasm4 palette
}
pal;

// easy gamepad handling
struct GamePad {
	const uint8_t* gploc;
	uint8_t  cur = 0, prev = 0;
	GamePad(const uint8_t* loc) : gploc(loc) {}
	void    update  ()            { prev = cur, cur = *gploc; }
	uint8_t down    (uint8_t btn) { return btn & cur; }
	uint8_t pressed (uint8_t btn) { return btn & cur  & (cur ^ prev); }
	uint8_t released(uint8_t btn) { return btn & prev & (cur ^ prev); }
}
gp(GAMEPAD1);

// global game state
// struct State {
// 	uint32_t framecount = 0;
// }
// state;


// general helpers
uint8_t collidebox(int ax, int ay, uint8_t aw, uint8_t ah, int bx, int by, uint8_t bw, uint8_t bh) {
	return !(ax + aw <= bx || ay + ah <= by || ax >= bx + bw || ay >= by + bh);
}
uint8_t collideboxf(float ax, float ay, uint8_t aw, uint8_t ah, float bx, float by, uint8_t bw, uint8_t bh) {
	return !(ax + aw <= bx || ay + ah <= by || ax >= bx + bw || ay >= by + bh);
}