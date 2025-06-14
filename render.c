#include "render.h"
#include "font_types.h"
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

bool check_bounds(int x, int y)
{
    return x >= 0 && x < LCD_WIDTH && y >= 0 && y < LCD_HEIGHT;
}

void render_frame_buffer_to_lcd(unsigned short *buffer, void *mem_base)
{
    for (int pixel_index = 0; pixel_index < LCD_WIDTH * LCD_HEIGHT; pixel_index++)
    {
        parlcd_write_data(mem_base, buffer[pixel_index]);
    }
}

void render_player_state(PlayerState *state, void *mem_base)
{
    *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) = (uint8_t)15;

    int shift = 0;

    if (*state == LANDED)
        shift = 8;
    if (*state == CRASHED)
        shift = 16;

    *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) <<= shift;
}

void render_fuel_to_leds(double *remaining_fuel, void *mem_base)
{
    int active_led_count = ceil(*remaining_fuel / FUEL_PER_LED);

    if (active_led_count == 0)
    {
        *(volatile uint32_t *)(mem_base + SPILED_REG_LED_LINE_o) = 0;
        return;
    }

    *(volatile uint32_t *)(mem_base + SPILED_REG_LED_LINE_o) = 1;

    for (int i = 0; i < active_led_count - 1; i++)
    {
        *(volatile uint32_t *)(mem_base + SPILED_REG_LED_LINE_o) =
            *(volatile uint32_t *)(mem_base + SPILED_REG_LED_LINE_o) << 1;
        *(volatile uint32_t *)(mem_base + SPILED_REG_LED_LINE_o) += 1;
    }
}

void draw_pixel(unsigned short *buffer, int x, int y, unsigned short color)
{
    if (check_bounds(x, y))
    {
        buffer[y * LCD_WIDTH + x] = color;
    }
};

void draw_circle(unsigned short *buffer, int center_x, int center_y, int pixel_radius, unsigned short color)
{
    double radius_square = (double)pixel_radius * (double)pixel_radius;
    for (int y = center_y - pixel_radius; y <= center_y + pixel_radius; y++)
    {
        double y_offset = fabs(y - center_y);
        double x_limit = sqrt(radius_square - y_offset * y_offset);

        for (int x = center_x - x_limit; x < center_x + x_limit; x++)
        {
            draw_pixel(buffer, x, y, color);
        }
    }
}

void draw_planets(unsigned short *buffer, Planet *planets, unsigned short planet_count, Vector *player_position)
{
    for (int planet_index = 0; planet_index < planet_count; planet_index++)
    {
        Planet planet = planets[planet_index];
        Vector planet_center_relative = subtract_vectors(planet.position, *player_position);
        Vector planet_center_relative_px = multiply_vector_by_scalar(planet_center_relative, PIXELS_PER_METER_IN_GAME);

        int offset_x = planet_center_relative_px.x + LCD_CENTER_X;
        int offset_y = planet_center_relative_px.y + LCD_CENTER_Y;

        draw_circle(buffer, offset_x, offset_y, planet.radius * PIXELS_PER_METER_IN_GAME, WHITE_COLOR);
    }
};

void draw_player(unsigned short *buffer, double rotation_radians)
{
    for (int player_y = -(PLAYER_HEIGHT_PX); player_y < PLAYER_HEIGHT_PX; player_y++)
    {
        uint8_t color = player_y > 0 ? PLAYER_COLOR : DARK_BLUE_COLOR;
        for (int player_x = -PLAYER_WIDTH_PX * PIXELS_PER_METER_IN_GAME; player_x < PLAYER_WIDTH_PX; player_x++)
        {
            Vector rotated_px = rotate_vector((Vector){player_x, player_y}, rotation_radians + M_PI / 2);

            draw_pixel(buffer, (int)rotated_px.x + LCD_CENTER_X - PLAYER_WIDTH_PX, (int)rotated_px.y + LCD_CENTER_Y,
                       color);
        }
    }
}

void draw_indicators(unsigned short *buffer, Vector *player_position, Planet *planets, unsigned short *planet_count)
{
    for (int planet_index = 0; planet_index < *planet_count; planet_index++)
    {
        Planet planet = planets[planet_index];
        double distance = get_distance(*player_position, planet.position);

        if (distance > INDICATOR_TRESHOLD)
        {
            Vector direction = subtract_vectors(planet.position, *player_position);
            Vector normalized = divide_vector_by_scalar(direction, get_magnitude_from_vector(direction));
            Vector indicator = multiply_vector_by_scalar(normalized, PIXELS_PER_METER_IN_GAME * 30);

            draw_circle(buffer, indicator.x + LCD_CENTER_X, indicator.y + LCD_CENTER_Y, 5, WHITE_COLOR);
        }
    }
}

void draw_map(unsigned short *buffer, Planet *planets, Vector *position, unsigned short *planet_count)
{
    for (int planet_index = 0; planet_index < *planet_count; planet_index++)
    {
        Planet planet = planets[planet_index];
        Vector planet_center_relative = subtract_vectors(planet.position, *position);
        Vector planet_center_scaled = divide_vector_by_scalar(planet_center_relative, 200);

        int offset_x = planet_center_scaled.x + LCD_CENTER_X;
        int offset_y = planet_center_scaled.y + LCD_CENTER_Y;

        draw_circle(buffer, offset_x, offset_y, 10, WHITE_COLOR);
    }

    draw_circle(buffer, LCD_CENTER_X, LCD_CENTER_Y, 3, PLAYER_COLOR);
}

void *loop_render(RenderArgs *args)
{
    void *parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    void *spiled_mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);

    parlcd_hx8357_init(parlcd_mem_base);
    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    while (!*(args->stop))
    {
        unsigned short frame_buffer[LCD_HEIGHT * LCD_WIDTH] = {0};
        if (*(args->show_map))
        {
            draw_map(frame_buffer, args->planets, args->position, args->planet_count);
        }
        else
        {
            draw_planets(frame_buffer, args->planets, *(args->planet_count), args->position);
            draw_player(frame_buffer, *(args->rotation));
            draw_indicators(frame_buffer, args->position, args->planets, args->planet_count);
        }

        render_fuel_to_leds(args->remaining_fuel, spiled_mem_base);
        render_player_state(args->player_state, spiled_mem_base);
        render_frame_buffer_to_lcd(frame_buffer, parlcd_mem_base);

        usleep(RENDER_DELAY_MS * 1000);
    }
};