#define NUM_PIXELS 25 // Número total de LEDs na matriz

bool apagado[NUM_PIXELS] = {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0};

bool desenho0[NUM_PIXELS] = {
    1, 0, 0, 0, 0,
    0, 1, 0, 0, 0,
    0, 1, 0, 0, 0,
    0, 1, 0, 0, 0,
    1, 0, 0, 0, 0};

bool desenho1[NUM_PIXELS] = {
    0, 1, 0, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 1, 0, 0, 0};

bool desenho2[NUM_PIXELS] = {
    0, 0, 1, 0, 0,
    0, 0, 0, 1, 0,
    0, 0, 0, 1, 0,
    0, 0, 0, 1, 0,
    0, 0, 1, 0, 0};

bool desenho3[NUM_PIXELS] = {
    0, 0, 0, 1, 0,
    0, 0, 0, 0, 1,
    0, 0, 0, 0, 1,
    0, 0, 0, 0, 1,
    0, 0, 0, 1, 0};

bool desenho4[NUM_PIXELS] = {
    0, 1, 0, 1, 0,
    0, 0, 0, 0, 0,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    0, 1, 1, 1, 0};

bool desenho5[NUM_PIXELS] = {
    0, 1, 0, 1, 0,
    0, 0, 0, 0, 0,
    1, 1, 1, 1, 1,
    1, 0, 0, 0, 1,
    0, 1, 1, 1, 0};

bool desenho6[NUM_PIXELS] = {
    0, 1, 0, 1, 0,
    0, 0, 0, 0, 0,
    1, 0, 0, 0, 1,
    1, 1, 1, 1, 1,
    0, 0, 0, 0, 0};

