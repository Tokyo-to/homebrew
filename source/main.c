#include <3ds.h>
#include <stdio.h>
#include "img_bin.h"


int main(int argc, char **argv)
{
	//Matrix containing the name of each key. Useful for printing when a key is pressed
	char keysNames[32][32] = {
		"KEY_A", "KEY_B", "KEY_SELECT", "KEY_START",
		"KEY_DRIGHT", "KEY_DLEFT", "KEY_DUP", "KEY_DDOWN",
		"KEY_R", "KEY_L", "KEY_X", "KEY_Y",
		"", "", "KEY_ZL", "KEY_ZR",
		"", "", "", "",
		"KEY_TOUCH", "", "", "",
		"KEY_CSTICK_RIGHT", "KEY_CSTICK_LEFT", "KEY_CSTICK_UP", "KEY_CSTICK_DOWN",
		"KEY_CPAD_RIGHT", "KEY_CPAD_LEFT", "KEY_CPAD_UP", "KEY_CPAD_DOWN"
	};

	// Initialize services
	gfxInitDefault();

	//We assign a print console to each screen
	PrintConsole topScreen, bottomScreen;

	//Assign last key press


	//Initialize console on both screens
	consoleInit(GFX_TOP, &topScreen);
	consoleInit(GFX_BOTTOM, &bottomScreen);

	u8* fbTopLeft = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
    u8* fbTopRight = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL);
    u8* fbBottom = gfxGetFramebuffer(GFX_BOTTOM, 0, NULL, NULL);
    memset(fbTopLeft, 0, 240 * 400 * 3);
    memset(fbTopRight, 0, 240 * 400 * 3);
    memset(fbBottom, 0, 240 * 320 * 3);

	u32 kDownOld = 0, kHeldOld = 0, kUpOld = 0; //In these variables there will be information about keys detected in the previous frame

	//Move the cursor to row 15 and column 19 and then prints "Hello World!"
	//To move the cursor you have to print "\x1b[r;cH", where r and c are respectively
	//the row and column where you want your cursor to move
	//The top screen has 30 rows and 50 columns
	//The bottom screen has 30 rows and 40 columns

	//Always select what screen you will be printing to using consoleSelect or else it'll print to last used
	consoleSelect(&topScreen);
	printf("\x1b[8;19HHello World");
	printf("\x1b[9;8HJust a program to output button presses");
	printf("\x1b[15;5HThis is just a test. I should go to bed.");

	printf("\x1b[29;40H");
	printf("\x1b[31mV 1.0.1.\x1b[0m");
	printf("\x1b[29;15HPress Start to exit.");

	consoleSelect(&bottomScreen);
	printf("\x1b[20;0HThis is just a bottom screen test :)");


	// Main loop
	while (aptMainLoop())
	{
		//Scan all the inputs. This should be done once for each frame
		hidScanInput();

		//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
		u32 kDown = hidKeysDown();
		//hidKeysHeld returns information about which buttons have are held down in this frame
		u32 kHeld = hidKeysHeld();
		//hidKeysUp returns information about which buttons have been just released
		u32 kUp = hidKeysUp();


		if (kDown & KEY_START) break; // break in order to return to hbmenu

		if (kDown != kDownOld || kHeld != kHeldOld || kUp != kUpOld)
		{
			//Clears the console
			consoleClear();

			consoleSelect(&topScreen);
			printf("\x1b[8;19HHello World");
			printf("\x1b[9;8HJust a program to output button presses");
			printf("\x1b[15;5HThis is just a test. I should go to bed.");
			//Prints out last key pressed
			int j;
			for (j = 0; j < 32; j++)
			{
				if (kDown & BIT(j)) printf("\x1b[22;10H\x1b[37mThe last key pressed was \x1b[34m%s                       \n", keysNames[j]);
			}

			printf("\x1b[29;40H");
			printf("\x1b[31mV 1.0.1.\x1b[0m");
			printf("\x1b[29;15HPress Start to exit.");

			consoleSelect(&bottomScreen);
			drawImage(fbTopLeft, 100, 100, 384, 216, img_bin);
			printf("\x1b[20;0HThis is just a bottom screen test :)");
		}

		//Set keys old values for the next frame
		kDownOld = kDown;
		kHeldOld = kHeld;
		kUpOld = kUp;

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		//Wait for VBlank
		gspWaitForVBlank();
	}

	gfxExit();
	return 0;
}
