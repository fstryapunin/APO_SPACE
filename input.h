enum ButtonState 
{
    HIGH = 0,
    LOW = 1
};

typedef struct
{
    int acceleration_input;
    int rotation_input;
    ButtonState pause_input;
    ButtonState map_input;
} Input;

void init_input(Input *Input);
void get_input(Input *Input);
