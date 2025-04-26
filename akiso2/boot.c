void wait(long delay) {
    for (volatile int i = 0; i < delay; i++);
}

void main() {
    char *video_memory = (char*) 0xB8000;
    char *message = "Hello World!";

    unsigned char color = 0x00;
    
    const int total_colors = 16*16; // 16 colors for foreground and 16 for background
    const int screen_width = 80;
    const int screen_height = 25;
    const int max_position = screen_width * screen_height;
    int position = 0;

    for (unsigned int c = 0; c < total_colors; c++) {

            // Write the message to video memory
            for (int i = 0; message[i] != '\0'; i++) {
                video_memory[position * 2] = message[i];       // Character
                video_memory[position * 2 + 1] = color;        // Color
                position++;
            }

            // Move to the next line after the message
            position += screen_width - (position % screen_width); // Start of the next line

            // Check if we reached the end of the screen and reset
            if (position >= max_position) {
                position = 0; // Reset to the top of the screen
            }

            color++;

            // Wait before displaying the next color combination
            wait(10000000);
    }

    while (1);
}
