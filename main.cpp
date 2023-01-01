#include "wasm4.h"
#include "etc.hpp"
#include "mstring.hpp"
#include "sprites.hpp"
#include <math.h>


enum GSTATE {
	GSTATE_BOOT = 0,
	GSTATE_MAINMENU,
	GSTATE_PLAY,
	GSTATE_PLAY_WAITNEXT,
	GSTATE_GAMEOVER,
};

const uint16_t LIVES_MAX = 5;

uint32_t framecount = 0;
uint16_t score = 0;
uint16_t lives = LIVES_MAX;
GSTATE   gamestate = GSTATE_BOOT;


struct MapPage {
	static const uint8_t WIDTH = 20, HEIGHT = 18, TSIZE = 8;
	int8_t level = 0;
	int8_t data[WIDTH * HEIGHT] = {0};

	// single point collision - returns a block id on collision
	int8_t collidepoint(int x, int y) const {
		return x >= 0 && x < WIDTH * TSIZE && y >= 0 && y < HEIGHT * TSIZE
			? data[y / TSIZE * WIDTH + x / TSIZE] : 0;
	}
	// block collision - returns a block id on collision, also sets map to zero if breakable
	int8_t collideblock(int x, int y, uint8_t w, uint8_t h) {
		int8_t c = 0;
		if ((c = collidepoint(x, y))) {
			if (c > 0) data[y / TSIZE * WIDTH + x / TSIZE] = 0;
		}
		else if ((c = collidepoint(x + w - 1, y))) {
			if (c > 0) data[y / TSIZE * WIDTH + (x + w - 1) / TSIZE] = 0;
		}
		else if ((c = collidepoint(x, y + h - 1))) {
			if (c > 0) data[(y + h - 1) / TSIZE * WIDTH + x / TSIZE] = 0;
		}
		else if ((c = collidepoint(x + w - 1, y + h - 1))) {
			if (c > 0) data[(y + h - 1) / TSIZE * WIDTH + (x + w - 1) / TSIZE] = 0;
		}
		return c;
	}
	// returns true if any blocks are remaning
	int8_t blocksremaining() {
		for (int i = 0; i < WIDTH * HEIGHT; i++)
			if (data[i] > 0) return 1;
		return 0;
	}
};
const MapPage map1 = {
	.level = 1,
	.data = {
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0, -1,
		-1,  0,  0,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  0,  0, -1,
		-1,  0,  0,  2,  5,  2,  5,  2,  5,  2,  5,  2,  5,  2,  5,  2,  2,  0,  0, -1,
		-1,  0,  0,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  0,  0, -1,
		-1,  0,  0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
	}
};
const MapPage map2 = {
	.level = 2,
	.data = {
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0, -1,
		-1,  0,  0,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  0,  0, -1,
		-1,  0,  0,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  0,  0, -1,
		-1,  0,  0,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  0,  0, -1,
		-1,  0,  0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
	}
};
const MapPage map3 = {
	.level = 3,
	.data = {
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  0,  0, -1,
		-1,  0,  0,  4,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  4,  0,  0, -1,
		-1,  0,  0,  4,  5,  6,  6,  6,  6,  7,  7,  6,  6,  6,  6,  5,  4,  0,  0, -1,
		-1,  0,  0,  4,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  4,  0,  0, -1,
		-1,  0,  0,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
	}
};
const MapPage map4 = {
	.level = 4,
	.data = {
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  5,  0,  5,  0,  7,  0,  5,  0,  7,  0,  5,  0,  5,  0,  0,  0, -1,
		-1,  0,  0,  0,  5,  0,  5,  0,  7,  0,  7,  0,  5,  0,  5,  0,  5,  0,  0, -1,
		-1,  0,  0,  5,  0,  5,  0,  5,  0,  8,  0,  5,  0,  5,  0,  5,  0,  0,  0, -1,
		-1,  0,  0,  0,  5,  0,  5,  0,  7,  0,  7,  0,  5,  0,  5,  0,  5,  0,  0, -1,
		-1,  0,  0,  5,  0,  5,  0,  7,  0,  5,  0,  7,  0,  5,  0,  5,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
	}
};
const MapPage map5 = {
	.level = 5,
	.data = {
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  7,  7,  7,  7,  7,  7,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  7,  0,  8,  0,  0,  8,  0,  7,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  7,  0,  8,  0,  0,  8,  0,  7,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  7,  0,  0,  8,  8,  0,  0,  7,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  7,  7,  7,  7,  7,  7,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
		-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,
	}
};
MapPage page;


struct Paddle {
	uint8_t w = 32, h = 8;
	float x = (160 - w)/2, y = 160 - 3*8;
}
pad;

struct Ball {
	uint8_t w = 8, h = 8;
	float x = 0, y = 0;
	float dx = 0, dy = 0, v = 2;
	uint8_t stuck = 1;
}
ball;


/*** game functions ***/

void resetpad() {
	pad.x = (160 - pad.w) / 2;
}

void resetball() {
	ball.x = pad.x + (pad.w - ball.w) / 2;
	ball.y = pad.y - ball.h;
	ball.dx = 0.5, ball.dy = -0.5;
	ball.stuck = 1;
}

void gamedraw() {
	// draw UI
	*DRAW_COLORS = 4;
	text(mstring::lit("score ").catint(score).str(), 1, 152);
	*DRAW_COLORS = 0x40;
	for (int i = 0; i < lives; i++)
		blit(heart, 160 - 8*LIVES_MAX + 8*i, 152, 8, 8, BLIT_1BPP);

	// draw map
	*DRAW_COLORS = 0x1234;
	for (int y = 0; y < page.HEIGHT; y++)
	for (int x = 0; x < page.WIDTH; x++) {
		int32_t t = abs(page.data[y * page.WIDTH + x]);  // tile number
		if (t > 0) {
			uint32_t sx = uint32_t(t - 1) * page.TSIZE;  // spritesheet source x
			blitSub(bricks, x * page.TSIZE, y * page.TSIZE, page.TSIZE, page.TSIZE, sx, 0, bricksWidth, bricksFlags);
		}
	}

	// draw player
	*DRAW_COLORS = 3;
	rect((int)pad.x, (int)pad.y, pad.w, pad.h);
	rect((int)ball.x, (int)ball.y, ball.w, ball.h);
}

void gameloop() {
	// move player
	if (gp.down(BUTTON_LEFT) && pad.x > 8) pad.x--;
	if (gp.down(BUTTON_RIGHT) && pad.x + pad.w < 160 - 8) pad.x++;
	if (gp.down(BUTTON_1) && ball.stuck) ball.stuck = 0;

	// sticky ball
	if (ball.stuck) resetball();
	// bouncing ball movement
	else {
		int8_t b = 0, hit = 0;

		// block strike at next x position
		b = page.collideblock(int(ball.x + ball.dx * ball.v), int(ball.y), ball.w, ball.h);
		if      (b == 0) ball.x += ball.dx * ball.v;
		else if (b < 0) ball.dx *= -1;
		else    ball.dx *= -1, score += (int8_t)b, hit = 1;

		// block strike at next y position
		b = page.collideblock(int(ball.x), int(ball.y + ball.dy * ball.v), ball.w, ball.h);
		if      (b == 0) ball.y += ball.dy * ball.v;
		else if (b < 0) ball.dy *= -1;
		else    ball.dy *= -1, score += (int8_t)b, hit = 1;

		// ball strikes paddle - bounce back up
		if (collideboxf(ball.x, ball.y, ball.w, ball.h, pad.x, pad.y, pad.w, pad.h)) {
			float ballmid = ball.x + ball.w / 2.0f;
			if      (ballmid <  pad.x - 2) ball.dx = -0.8f, ball.dy = -0.2f;  // paddle left edge
			else if (ballmid >= pad.x + pad.w + 2) ball.dx = 0.8f, ball.dy = -0.2f;  // paddle right edge
			else if (ballmid <  pad.x + 8) ball.dx = -0.65f, ball.dy = -0.35f;  // middle left reverse
			else if (ballmid >= pad.x + pad.w - 8) ball.dx = 0.65f, ball.dy = -0.35f;  // middle right reverse
			else    ball.dx = (ball.dx < 0 ? -0.5f : 0.5f), ball.dy = -0.5f;  // middle continue
		}

		// ball off bottom of screen - reset
		if (ball.y > 160) {
			if   (--lives == 0) gamestate = GSTATE_GAMEOVER;
			else resetball();
		}

		// no bricks left - move to next level
		if (hit && page.blocksremaining() == 0) gamestate = GSTATE_PLAY_WAITNEXT;
	}

	// repaint screen
	gamedraw();
}

void gameloop_waitnext() {
	// press a button to move to next map, or loop maps
	if (gp.released(BUTTON_1)) {
		switch (page.level) {
			case 1:  page = map2;  break;
			case 2:  page = map3;  break;
			case 3:  page = map4;  break;
			case 4:  page = map5;  break;
			case 5:  page = map1;  lives = LIVES_MAX;  ball.v += 1;  break;
		}
		resetpad(), resetball(), gamestate = GSTATE_PLAY;
	}

	// repaint screen
	gamedraw();
	*DRAW_COLORS = 4;
	text("good job!", (160 - 9 * 8) / 2, 80);
	text("\x80 to continue", (160 - 13 * 8) / 2, 90);
}

void mainmenu() {
	// next stage button
	if (gp.released(BUTTON_1)) gamestate = GSTATE_PLAY;
	// repaint screen
	*DRAW_COLORS = 3;
	blit(splash2, 10, 40, splash2Width, splash2Height, splash2Flags);
	*DRAW_COLORS = 4;
	text("press \x80 to start!", 12, 100);
}

void gameover() {
	static const char* msg[4] = {
		"your hearts gone",
		" your spirits down",
		"  pick yourself up",
		"   one more round!",
	};
	// main menu button
	if (gp.released(BUTTON_1)) gamestate = GSTATE_BOOT;
	// repaint screen
	*DRAW_COLORS = 4;
	for (int i = 0; i < 4; i++)
		text(msg[i], 8, 60 + i * 8);
}


/*** wasm4 functions ***/

void update() {
	// manage state
	framecount++;
	gp.update();

	// gamestate action
	switch (gamestate) {
		case GSTATE_BOOT:           start();  gamestate = GSTATE_MAINMENU;  break;
		case GSTATE_MAINMENU:       mainmenu();  break;
		case GSTATE_PLAY:           gameloop();  break;
		case GSTATE_PLAY_WAITNEXT:  gameloop_waitnext();  break;
		case GSTATE_GAMEOVER:       gameover();  break;
	}
}

void start() {
	// pal.icecream();
	score = 0;
	lives = LIVES_MAX;
	page = map1;
	ball.v = 2;
	resetpad();
	resetball();
}