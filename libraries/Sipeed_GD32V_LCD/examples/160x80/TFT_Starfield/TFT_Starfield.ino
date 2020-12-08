// Animates white pixels to simulate flying through a star field

#include <Sipeed_GD32V_LCD.h>

#define LCD_W 160
#define LCD_H 80

Sipeed_GD32V_LCD tft(LCD_W, LCD_H);

// With 1024 stars the update rate is ~65 frames per second
#define NSTARS 1024
uint8_t sx[NSTARS] = {};
uint8_t sy[NSTARS] = {};
uint8_t sz[NSTARS] = {};

uint8_t za, zb, zc, zx;

// Fast 0-255 random number generator from http://eternityforest.com/Projects/rng.php:
uint8_t __attribute__((always_inline)) rng() {
    zx++;
    za = (za ^ zc ^ zx);
    zb = (zb + za);
    zc = (zc + (zb >> 1)^za);
    return zc;
}

void setup() {
    za = random(256);
    zb = random(256);
    zc = random(256);
    zx = random(256);

    Serial.begin(115200);
    tft.begin();
    tft.setRotation(0);
    tft.fillScreen(COLOR_BLACK);

    // fastSetup() must be used immediately before fastPixel() to prepare screen
    // It must be called after any other graphics drawing function call if fastPixel()
    // is to be called again
    //tft.fastSetup(); // Prepare plot window range for fast pixel plotting
}

void loop() {
    unsigned long t0 = micros();
    uint8_t spawnDepthVariation = 255;

    for (int i = 0; i < NSTARS; ++i) {
        if (sz[i] <= 1) {
            sx[i] = (LCD_W/2) - (LCD_H/2) + rng();
            sy[i] = rng();
            sz[i] = spawnDepthVariation--;
        } else {
            int old_screen_x = ((int)sx[i] - (LCD_W/2)) * 256 / sz[i] + (LCD_W/2);
            int old_screen_y = ((int)sy[i] - (LCD_H/2)) * 256 / sz[i] + (LCD_H/2);

            // This is a faster pixel drawing function for occassions where many single pixels must be drawn
            tft.drawPixel(old_screen_x, old_screen_y, COLOR_BLACK);

            sz[i] -= 2;
            if (sz[i] > 1) {
                int screen_x = ((int)sx[i] - (LCD_W/2)) * 256 / sz[i] + (LCD_W/2);
                int screen_y = ((int)sy[i] - (LCD_H/2)) * 256 / sz[i] + (LCD_H/2);

                if (screen_x >= 0 && screen_y >= 0 && screen_x < LCD_W && screen_y < LCD_H) {
                    uint8_t r, g, b;
                    r = g = b = 255 - sz[i];
                    tft.drawPixel(screen_x, screen_y, tft.color565(r, g, b));
                } else {
                    sz[i] = 0;    // Out of screen, die.
                }
            }
        }
    }
    unsigned long t1 = micros();
    //static char timeMicros[8] = {};

    // Calcualte frames per second
    Serial.println(1.0 / ((t1 - t0) / 1000000.0));
}
