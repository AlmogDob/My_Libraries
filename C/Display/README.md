1. Befor including the `display.c` file, you need to define the following SETUP, UPDATE and RENDER and any macro you want to overwrite like so:
    ``` C
    #define SETUP
    #define UPDATE
    #define RENDER
    #include "display.c"
    ```
2. There is no need for a main function!!!
you just need to define the `setup`, `update` and `render` function like so:
    ``` C
    void setup(void) 
    {    
    }

    void update(void)
    {
    }

    void render(void)
    {
    }

    ```
3. After that, you are basically good to go (given that you have SDL installed on your machine - https://wiki.libsdl.org/SDL2/Installation).
Don't forget to linke with `-lSDL2` and `-lSDL2_ttf`