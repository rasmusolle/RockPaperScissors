#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gccore.h>
#include <wiiuse/wpad.h>

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

static bool chosen = false;
static int item = 0;
static int AIitem = 0;
static int result = 0;

void Shutdown(void) {
	printf("Shutting Down...\n");
	exit(0);
}

int main(int argc, char **argv) {
	VIDEO_Init();
	WPAD_Init();
	rmode = VIDEO_GetPreferredMode(NULL);
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();

	VIDEO_WaitVSync();
	if (rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();

	printf("\x1b[1;0H");

	while(1) {
		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);
		VIDEO_WaitVSync();
		if (pressed & WPAD_BUTTON_HOME) Shutdown();

		printf("\n");
		printf("Left - Rock\n");
		printf("Up - Paper\n");
		printf("Right - Scissors\n\n");
		chosen = false;
		
		while (!chosen) {
			WPAD_ScanPads();
			u32 pressed = WPAD_ButtonsDown(0);
			VIDEO_WaitVSync();
			if (pressed & WPAD_BUTTON_HOME) Shutdown();
			
			// 1 - Rock
			// 2 - Paper
			// 3 - Scissors
			
			if (pressed & WPAD_BUTTON_LEFT) { 
				chosen = true;
				item = 1;
				printf("You chose: Rock\n");
			}
			if (pressed & WPAD_BUTTON_UP) { 
				chosen = true;
				item = 2;
				printf("You chose: Paper\n");
			}
			if (pressed & WPAD_BUTTON_RIGHT) { 
				chosen = true;
				item = 3;
				printf("You chose: Scissors\n");
			}
		}
		chosen = false;
		
		//AI
		printf("The AI is thinking...\n");
		sleep(3);
		REDO:
		AIitem = rand() % 4;
		if (AIitem == 0) goto REDO;
		if (AIitem == 1) printf("AI chose: Rock\n");
		if (AIitem == 2) printf("AI chose: Paper\n");
		if (AIitem == 3) printf("AI chose: Scissors\n");
		
		//Calculate results
		if (item == 1) {
			if (AIitem == 1) result = 3;
			if (AIitem == 2) result = 2;
			if (AIitem == 3) result = 1;
		}
		if (item == 2) {
			if (AIitem == 1) result = 1;
			if (AIitem == 2) result = 3;
			if (AIitem == 3) result = 2;
		}
		if (item == 3) {
			if (AIitem == 1) result = 2;
			if (AIitem == 2) result = 1;
			if (AIitem == 3) result = 3;
		}
		if (result == 1) printf("You WON!\n\n");
		if (result == 2) printf("You LOST!\n\n");
		if (result == 3) printf("It's a DRAW!\n\n");
		
		// Wanna try again?
		printf("Would you want to try again?\n");
		printf("A - Yes | B - No\n");
		while (!chosen) {
			WPAD_ScanPads();
			u32 pressed = WPAD_ButtonsDown(0);
			VIDEO_WaitVSync();
			if (pressed & WPAD_BUTTON_HOME) Shutdown();
			
			if (pressed & WPAD_BUTTON_A) {
				chosen = true;
				printf("Fun!\n\n");
			}
			if (pressed & WPAD_BUTTON_B) {
				printf("Bye!\n");
				exit(0);
			}
		}
		chosen = false;
	}
	return 0;
}
