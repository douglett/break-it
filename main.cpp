#include "wasm4.h"
#include "etc.hpp"
#include "mstring.hpp"
#include "sprites.hpp"
#include <math.h>


uint32_t framecount = 0;
uint16_t score = 0;


struct MapPage {
	static const uint8_t WIDTH = 20, HEIGHT = 18, TSIZE = 8;
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
};
const MapPage map1 = {
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
MapPage page;


struct Paddle {
	uint8_t w = 32, h = 8;
	float x = (160 - 32)/2, y = 160 - 3*8;
}
pad;

struct Ball {
	uint8_t w = 8, h = 8;
	float x = 0, y = 0;
	float dx = 0, dy = 0, v = 2;
	uint8_t stuck = 1;
}
ball;

void resetball() {
	ball.x = pad.x + (pad.w - ball.w) / 2;
	ball.y = pad.y - ball.h;
	ball.dx = 0.5, ball.dy = -0.5; //, ball.v = 2;
	ball.stuck = 1;
}


void start() {
	// pal.icecream();
	page = map1;
}

void update() {
	// manage state
	framecount++;
	gp.update();

	// move player
	if (gp.down(BUTTON_LEFT) && pad.x > 8) pad.x--;
	if (gp.down(BUTTON_RIGHT) && pad.x + pad.w < 160 - 8) pad.x++;
	if (gp.down(BUTTON_1) && ball.stuck) ball.stuck = 0;

	// sticky ball
	if (ball.stuck) resetball();
	// bouncing ball movement
	else {
		int8_t b = 0;

		// block strike at next x position
		b = page.collideblock(int(ball.x + ball.dx * ball.v), int(ball.y), ball.w, ball.h);
		if      (b == 0) ball.x += ball.dx * ball.v;
		else if (b < 0) ball.dx *= -1;
		else    ball.dx *= -1, score += (int8_t)b;

		// block strike at next y position
		b = page.collideblock(int(ball.x), int(ball.y + ball.dy * ball.v), ball.w, ball.h);
		if      (b == 0) ball.y += ball.dy * ball.v;
		else if (b < 0) ball.dy *= -1;
		else    ball.dy *= -1, score += (int8_t)b;

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
		if (ball.y > 160) resetball();
	}

	// draw UI
	*DRAW_COLORS = 4;
	text(mstring::lit("score ").catint(score).str(), 8, 152);

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