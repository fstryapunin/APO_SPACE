#include "render.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "mzapo_parlcd.h"
#include "font_types.h"

// void render_state(int fuel, double rotation, Vector position, bool is_stopped, bool is_crashed, Planet *planets);
// void render_map(Vector position, Planet *planets);
// void render_menu(MenuState state);

bool check_bounds(int x, int y){
    return x >= 0 && x < LCD_WIDTH && y >= 0 && y < LCD_HEIGHT;
}

void render_frame_buffer_to_lcd(unsigned short *buffer, void *mem_base){
    for(int pixel_index  = 0; pixel_index < LCD_WIDTH * LCD_HEIGHT; pixel_index++){
        parlcd_write_data(mem_base, buffer[pixel_index]);
    }
}

void draw_pixel(unsigned short *buffer, int x, int y, unsigned short color){
    if(check_bounds(x, y)){
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

void draw_planets(unsigned short *buffer, Planet *planets, unsigned short planet_count, Vector *player_position){
    for(int planet_index = 0; planet_index < planet_count; planet_index++){
        Planet planet = planets[planet_index];
        Vector planet_center_relative = subtract_vectors(planet.position, *player_position);
        Vector planet_center_relative_px = multiply_vector_by_scalar(planet_center_relative, PIXELS_PER_METER_IN_GAME);
        
        int offset_x = planet_center_relative_px.x + LCD_CENTER_X;
        int offset_y = planet_center_relative_px.y + LCD_CENTER_Y;

        draw_circle(buffer, offset_x, offset_y, planet.radius * PIXELS_PER_METER_IN_GAME, WHITE_COLOR);
    }
};

void draw_player(unsigned short *buffer, double rotation_radians){
    for (int player_y = - (PLAYER_HEIGHT_PX); player_y < PLAYER_HEIGHT_PX; player_y++) {
        uint8_t color = player_y > 0 ? PLAYER_COLOR: DARK_BLUE_COLOR;
        for (int player_x = -PLAYER_WIDTH_PX * PIXELS_PER_METER_IN_GAME; player_x < PLAYER_WIDTH_PX; player_x++) {
            Vector rotated_px = rotate_vector((Vector) {player_x, player_y}, rotation_radians + M_PI / 2);

            draw_pixel(buffer, (int)rotated_px.x + LCD_CENTER_X, (int)rotated_px.y + LCD_CENTER_Y, color);
        }
    }
}

void *loop_render(RenderArgs *args){
    void *parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    parlcd_hx8357_init(parlcd_mem_base);
    parlcd_write_cmd(parlcd_mem_base, 0x2c);

    while (!args->stop)
    {
        unsigned short frame_buffer[LCD_HEIGHT * LCD_WIDTH] = {0};
        draw_planets(frame_buffer, args->planets, *(args->planet_count), args->position);
        draw_player(frame_buffer, *(args->rotation));
        render_frame_buffer_to_lcd(frame_buffer, parlcd_mem_base);
        usleep(RENDER_DELAY_MS * 1000);
    }
};