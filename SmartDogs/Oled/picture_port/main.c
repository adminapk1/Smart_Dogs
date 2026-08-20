#include "public.h"
#include "iic.h"
#include "ssd1306.h"
#include "picture.h"
#include "images.h"

u8 current_frame = 0; /* 当前播放帧索引 */

void main()
{
    picture_init();
    
    while(1)
    {
        picture_draw_image(animation_frames[current_frame]);
        delay_ms(200);
        current_frame++;
        if(current_frame >= ANIMATION_FRAMES)
        {
            current_frame = 0;
        }
    }
}
