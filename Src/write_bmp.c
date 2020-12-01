#include "write_bmp.h"
#include "ff.h"
#include "lcd_tft.h"

#define RGB24TORGB16(R, G, B) ((unsigned short int)((((R) >> 3) << 11) | (((G) >> 2) << 5) | ((B) >> 3)))

BYTE pColorData[960];
FIL bmpfsrc, bmpfdst;
FRESULT bmpres;

#define BMP_DEBUG_PRINTF(FORMAT, ...) printf(FORMAT, ##__VA_ARGS__)

int screen_shot(uint16_t x, uint16_t y, uint16_t Width, uint16_t Height, char *filename)
{
    unsigned char header[54] =
        {
            0x42, 0x4d, 0, 0, 0, 0,
            0, 0, 0, 0, 54, 0,
            0, 0, 40, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
            0, 0, 1, 0, 24, 0,
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0};

    int i;
    int j;
    long file_size;
    long width;
    long height;
    unsigned char r, g, b;
    unsigned int mybw;
    unsigned int read_data;
    char kk[4] = {0, 0, 0, 0};

    uint8_t ucAlign; 

    file_size = (long)Width * (long)Height * 3 + Height * (Width % 4) + 54;

    header[2] = (unsigned char)(file_size & 0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;

    width = Width;
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8) & 0x000000ff;
    header[20] = (width >> 16) & 0x000000ff;
    header[21] = (width >> 24) & 0x000000ff;

    height = Height;
    header[22] = height & 0x000000ff;
    header[23] = (height >> 8) & 0x000000ff;
    header[24] = (height >> 16) & 0x000000ff;
    header[25] = (height >> 24) & 0x000000ff;

    bmpres = f_open(&bmpfsrc, (char *)filename, FA_CREATE_ALWAYS | FA_WRITE);

    f_close(&bmpfsrc);

    bmpres = f_open(&bmpfsrc, (char *)filename, FA_OPEN_EXISTING | FA_WRITE);

    if (bmpres == FR_OK)
    {
        bmpres = f_write(&bmpfsrc, header, sizeof(unsigned char) * 54, &mybw);

        ucAlign = Width % 4;

        for (i = 0; i < Height; i++)
        {
            for (j = 0; j < Width; j++)
            {
                read_data = LCD_GetPointPixel(x + j, y + Height - 1 - i);

                r = GETR_FROM_RGB16(read_data);
                g = GETG_FROM_RGB16(read_data);
                b = GETB_FROM_RGB16(read_data);

                bmpres = f_write(&bmpfsrc, &b, sizeof(unsigned char), &mybw);
                bmpres = f_write(&bmpfsrc, &g, sizeof(unsigned char), &mybw);
                bmpres = f_write(&bmpfsrc, &r, sizeof(unsigned char), &mybw);
            }

            if (ucAlign)
                bmpres = f_write(&bmpfsrc, kk, sizeof(unsigned char) * (ucAlign), &mybw);
        }

        f_close(&bmpfsrc);

        return 0;
    }
    else
    {
        return -1;
    }
}