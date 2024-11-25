#include "Bitmaps.h"

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 400)
const int epd_bitmap_allArray_LEN = 3;
const unsigned char* epd_bitmap_allArray[3] = {
	epd_bitmap_Profile,
	epd_bitmap_Selector,
	epd_bitmap_Start
};

const unsigned char* getBitmap(int index) {
    if (index >= 0 && index < epd_bitmap_allArray_LEN) {
        return epd_bitmap_allArray[index];
    }
    return nullptr;
}