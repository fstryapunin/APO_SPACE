typedef enum ButtonState 
{
    HIGH = 0,
    LOW = 1
} ButtonState;

typedef struct
{
    double acceleration_input;
    double rotation_input;
    ButtonState pause_input;
    ButtonState map_input;
} Input;

Input init_input();
void get_input(Input *Input);
