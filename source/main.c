#include <string.h>
#include <stdio.h>
#include <math.h>
#include <3ds.h>

#define FPS (24)

#define TICKS_PER_SEC    (268111856)
#define TICKS_PER_USEC   (268.111856)
#define TICKS_PER_MSEC   (268111.856)
#define TICKS_PER_FRAME  (TICKS_PER_SEC/FPS)


void gameLoop() {
	u64 nowTicks, deltaTicks, lastTicks= 0, frameCount = 0;

	nowTicks = svcGetSystemTick();
	deltaTicks = nowTicks - lastTicks;

	uint frame = 0;
	uint fps = 0;

	while (aptMainLoop()) {
		frame++;

		if (frameCount >= TICKS_PER_SEC) {
			fps = frame;
			frame = 0; frameCount = 0;
		}

		gspWaitForVBlank();

		printf("\x1b[2;0HFPS: %d", fps);
		printf("\x1b[3;0HDELTA TICKS: %lld", deltaTicks);

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		// Dormimos el ciclo principal
		// hasta alcanzar los ticks necesarios por frame
		while (1){

			nowTicks = svcGetSystemTick();
			deltaTicks = nowTicks - lastTicks;

			if (deltaTicks >= TICKS_PER_FRAME) {
				lastTicks = nowTicks;
				frameCount += deltaTicks;
				break;
			}
		}

	}

}


int main(int argc, char **argv) {

	gfxInitDefault();

	consoleInit(GFX_TOP, NULL);

	gameLoop();

	gfxExit();
	return 0;
}
