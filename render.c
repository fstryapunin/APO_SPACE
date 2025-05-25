#include "render.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "mzapo_parlcd.h"
#include "font_types.h"

// void render_state(int fuel, double rotation, Vector position, bool is_stopped, bool is_crashed, Planet *planets);
// void render_map(Vector position, Planet *planets);
// void render_menu(MenuState state);

void render_frame_buffer_to_lcd(unsigned short *buffer, void *mem_base){
    for(int pixel_index  = 0; pixel_index < LCD_WIDTH * LCD_HEIGHT; pixel_index++){
        parlcd_write_data(mem_base, buffer[pixel_index]);
    }
}

void draw_pixel(unsigned short *buffer, int x, int y, unsigned short color){
    if(x >= 0 && x < LCD_WIDTH && y >= 0 && y < LCD_HEIGHT){
        printf("Drawind %d %d\n", x, y);
        buffer[y * LCD_WIDTH + x] = color;
    }
};

void draw_circle(unsigned short *buffer, int center_x, int center_y, int pixel_radius, unsigned short color){
    double radius_square = (double)pixel_radius * (double)pixel_radius;
    for(int y = center_y - pixel_radius; y <= center_y + pixel_radius; y++){
        double y_offset = fabs(y - center_y);
        double x_limit = sqrt(radius_square - y_offset * y_offset);

        for(int x = center_x - x_limit; x < center_x + x_limit; x++){
            draw_pixel(buffer, x, y, color);
        }
    }
}

void draw_planets(unsigned short *buffer, Planet *planets, int planet_count, Vector player_position){
    for(int planet_index = 0; planet_index < planet_count; planet_index++){
        Planet planet = planets[planet_index];
        if(
            fabs(player_position.x - planet.position.x - planet.radius) * PIXELS_PER_METER_IN_GAME <= LCD_WIDTH
            && fabs(player_position.y - planet.position.y - planet.radius) * PIXELS_PER_METER_IN_GAME <= LCD_HEIGHT
        ){
            Vector relative = multiply_vector_by_scalar((planet.position, player_position), PIXELS_PER_METER_IN_GAME);
            draw_circle(buffer, relative.x, relative.y, planet.radius * PIXELS_PER_METER_IN_GAME, WHITE_COLOR);
        }
    }
};

void *loop_render(RenderArgs *args){
    void *parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    unsigned short frame_buffer[LCD_HEIGHT * LCD_WIDTH] = {0};
    parlcd_hx8357_init(parlcd_mem_base);
    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    draw_planets(frame_buffer, args->planets, args->planet_count, *(args->position));
    while (!args->stop)
    {
        render_frame_buffer_to_lcd(frame_buffer, parlcd_mem_base);
        usleep(RENDER_DELAY_MS * 1000);
    }
};