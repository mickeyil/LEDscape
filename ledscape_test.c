#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include "ledscape.h"

// for usleep
#define _BSD_SOURCE

const char * pru0_bin = "/home/debian/dev/LEDscape/pru/bin/ws281x-rgb-123-v3-pru0.bin";
const char * pru1_bin = "/home/debian/dev/LEDscape/pru/bin/ws281x-rgb-123-v3-pru1.bin";

const unsigned int MAX_PIXELS_PER_STRIP = 144;

int main()
{
  unsigned int nstrips = LEDSCAPE_NUM_STRIPS;
  unsigned int framebuffer_idx = 0;

  unsigned int frame_index = 0;
  
  // bufsize = nstrips * MAX_PIXELS_PER_STRIP * 3;
  // uint8_t * buffer = (uint8_t*) malloc(bufsize);
  // bzero(buffer, bufsize);

  printf("initializing prus.\n");
  ledscape_t * leds = ledscape_init_with_programs(
    MAX_PIXELS_PER_STRIP, pru0_bin, pru1_bin
  );
  
  
  for (size_t fidx = 0; fidx < 1000; fidx++) {

    ledscape_frame_t * frame = ledscape_frame(leds, framebuffer_idx);
    framebuffer_idx = (framebuffer_idx + 1) % 2;

    uint8_t value = fidx % 256;
    for (size_t strip_index = 0; strip_index < nstrips; strip_index++) {
      for (size_t led_index = 0; led_index < MAX_PIXELS_PER_STRIP; led_index++) {
        ledscape_pixel_t* pixel_out = & frame[led_index].strip[strip_index];

        pixel_out->a = value;   // R
        pixel_out->b = 0;       // G
        pixel_out->c = 0;       // B
      }
    }

		ledscape_wait(leds);

		// Send the frame to the PRU
		ledscape_draw(leds, framebuffer_idx);
    
    if (fidx % 10 == 0) {
      printf("\rframe: %zu           ", fidx);
      fflush(stdout);
    }

    usleep(20000);
  
  }

  printf("\nclosing ledscape.\n");
	ledscape_close(leds);
  return 0;
}
