#include "wasm4.h"
#include "etc.hpp"
#include "mstring.hpp"
#include <math.h>


uint32_t framecount = 0;
uint16_t score = 0;


struct MapPage {
	static const uint8_t WIDTH = 20, HEIGHT = 18, TSIZE = 8;
	int8_t data[WIDTH * HEIGHT] = {0};
	// int8_t collide(int x, int y, uint8_t w, uint8_t h) const {
	// 	int8_t c = 0;
	// 	if ((c = collidepoint(x, y))) return c;
	// 	if ((c = collidepoint(x + w - 1, y))) return c;
	// 	if ((c = collidepoint(x, y + h - 1))) return c;
	// 	if ((c = collidepoint(x + w - 1, y + h - 1))) return c;
	// 	return 0;
	// }

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
	float x = (160 - 8) / 2, y = 160 - 4*8;
	float vx = 0, vy = 0;
	uint8_t stuck = 1;
}
ball;

void resetball() {
	ball.x = pad.x + (pad.w - ball.w) / 2;
	ball.y = pad.y - ball.h;
	ball.vx = 0, ball.vy = 0, ball.stuck = 1;
}


void start() {
	pal.icecream();
	page = map1;
}

void update() {
	// manage state
	framecount++;
	gp.update();
	// auto& page = map;

	// move player
	if (gp.down(BUTTON_LEFT) && pad.x > 8) pad.x--;
	if (gp.down(BUTTON_RIGHT) && pad.x + pad.w < 160 - 8) pad.x++;
	if (gp.down(BUTTON_1) && ball.stuck) ball.vx = 1, ball.vy = -1, ball.stuck = 0;

	// sticky ball
	if (ball.stuck) {
		resetball();
	}
	// bouncing ball movement
	else {
		int8_t b = 0;
		// block strike next x position
		b = page.collideblock(int(ball.x + ball.vx), int(ball.y), ball.w, ball.h);
		if      (b == 0) ball.x += ball.vx;
		else if (b < 0) ball.vx *= -1;
		else    ball.vx *= -1, score += (int8_t)b;
		// block strike next y position
		b = page.collideblock(int(ball.x), int(ball.y + ball.vy), ball.w, ball.h);
		if      (b == 0) ball.y += ball.vy;
		else if (b < 0) ball.vy *= -1;
		else    ball.vy *= -1, score += (int8_t)b;
		// ball strikes paddle - bounce back up
		if (collideboxf(ball.x, ball.y, ball.w, ball.h, pad.x, pad.y, pad.w, pad.h)) {
			if (ball.vy > 0) ball.vy *= -1;  // rebound y
			if (ball.x + ball.w / 2 <  pad.x && ball.vx > 0) ball.vx *= -1;  // rebound x - paddle left edge
			if (ball.x + ball.w / 2 >= pad.x + pad.w && ball.vx < 0) ball.vx *= -1;  // recount x - paddle right edge
		}
		// ball off bottom of screen - reset
		// if (ball.y > 160) ball.x = (160 - 8) / 2, ball.y = 160 - 4*8, ball.vx = 0, ball.vy = 0;
		if (ball.y > 160) resetball();
	}

	// draw UI
	text(mstring::lit("score ").catint(score).str(), 8, 152);

	// draw map
	for (int y = 0; y < page.HEIGHT; y++)
	for (int x = 0; x < page.WIDTH; x++) {
		switch (abs(page.data[y * page.WIDTH + x])) {
			case 0:  continue;
			case 1:  *DRAW_COLORS = 2;  break;
			case 2:  *DRAW_COLORS = 3;  break;
		}
		rect(x * page.TSIZE, y * page.TSIZE, page.TSIZE, page.TSIZE);
	}

	// draw player
	*DRAW_COLORS = 4;
	rect((int)pad.x, (int)pad.y, pad.w, pad.h);
	rect((int)ball.x, (int)ball.y, ball.w, ball.h);
}