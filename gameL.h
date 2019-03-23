extern int16_t _width, _height;

extern "C" void _pre_start();

extern "C" void _game_loop();

extern "C" void drawBit(int16_t x, int16_t y, byte w, byte h, int16_t xo, int16_t yo, byte wo, byte ho, uint8_t arr[]);

extern "C" void drawBitL(int16_t x, int16_t y, byte w, byte h, int16_t xo, int16_t yo, byte wo, byte ho, uint8_t arr[]);

extern "C" void clscr();

extern "C" uint8_t _rand(uint8_t n);