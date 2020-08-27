#include "gba.h"

volatile unsigned short *videoBuffer = (volatile unsigned short *) 0x6000000;
u32 vBlankCounter = 0;

void waitForVBlank(void) {
    // TODO: IMPLEMENT

    // (1)
    // Write a while loop that loops until we're NOT in vBlank anymore:
    // (This prevents counting one VBlank more than once if your app is too fast)

    // (2)
    // Write a while loop that keeps going until we're in vBlank:

    // (3)
    // Finally, increment the vBlank counter:

    while (*SCANLINECOUNTER > 160);
    while (*SCANLINECOUNTER < 160);
    vBlankCounter++;
}

static int __qran_seed= 42;
static int qran(void) {
    __qran_seed= 1664525*__qran_seed+1013904223;
    return (__qran_seed>>16) & 0x7FFF;
}

int randint(int min, int max) {
    return (qran()*(max-min)>>15)+min;
}

void setPixel(int x, int y, u16 color) {
    *(videoBuffer + OFFSET(y, x, WIDTH) ) = color;
}

void drawRectDMA(int x, int y, int width, int height, volatile u16 color) {
    // TODO: IMPLEMENT

    for (int r = 0; r < height; r++) {
        DMA[3].src = &color;
        DMA[3].dst = &videoBuffer[OFFSET(x, (y + r), WIDTH)];
        DMA[3].cnt = width | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT | DMA_ON;
    }

}

void drawFullScreenImageDMA(const u16 *image) {
    // TODO: IMPLEMENT

    DMA[3].src = image;
    DMA[3].dst = videoBuffer;
    DMA[3].cnt = (WIDTH * HEIGHT) | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
}

void drawImageDMA(int x, int y, int width, int height, const u16 *image) {
    // TODO: IMPLEMENT

    for (int i = 0; i < height; i++) {

        DMA[3].src = &image[OFFSET(0, i, width)];
        DMA[3].dst = &videoBuffer[OFFSET(x, y + i, WIDTH)];
        DMA[3].cnt = width | DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
   }
}

void fillScreenDMA(volatile u16 color) {
    // TODO: IMPLEMENT

    DMA[3].src = &color;
    DMA[3].dst = videoBuffer;
    DMA[3].cnt = (WIDTH * HEIGHT) | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT | DMA_ON;
}

void drawChar(int x, int y, char ch, u16 color) {
    for(int i = 0; i<6; i++) {
        for(int j=0; j<8; j++) {
            if(fontdata_6x8[OFFSET(i, j, 6) + ch*48]) {
                setPixel(x+j, y+i, color);
            }
        }
    }
}

void drawString(int x, int y, char *str, u16 color) {
    while(*str) {
        drawChar(x, y, *str++, color);
        y += 6;
    }
}

void drawCenteredString(int x, int y, int width, int height, char *str, u16 color) {
    u32 len = 0;
    char *strCpy = str;
    while (*strCpy) {
        len++;
        strCpy++;
    }

    u32 strWidth = 6 * len;
    u32 strHeight = 8;

    int col = x + ((width - strWidth) >> 1);
    int row = y + ((height - strHeight) >> 1);
    drawString(col, row, str, color);
}

/***************************** OWN METHODS *****************************/
int hit (u16 enemyX, u16 enemyY, u16 enemyWidth, u16 enemyHeight, u16 playerX, u16 playerY) {
    u16 playerX2 = playerX + 16;
    u16 playerY2 = playerY + 16;
    u16 enemyX2 = enemyX + enemyWidth;
    u16 enemyY2 = enemyY + enemyHeight;

    //checking bottom right corner of enemy
    if (playerX < enemyX2 && enemyX2 < playerX2
     && playerY < enemyY2 && enemyY2 < playerY2) {
        return 1;
    }
    //checking bottom left corner of enemy
    if (playerX < enemyX && enemyX < playerX2
     && playerY < enemyY2 && enemyY2 < playerY2) {
        return 1;
    }
    //checking top left corner of enemy
    if (playerX < enemyX && enemyX < playerX2
     && playerY < enemyY && enemyY < playerY2) {
        return 1;
    }
    //checking top right corner of enemy
    if (playerX < enemyX2 && enemyX2 < playerX2
     && playerY < enemyY && enemyY < playerY2) {
        return 1;
    }
    return 0;
}
