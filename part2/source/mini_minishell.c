#include "mini_minishell.h"


static char *available_commands[] = {
    "help", "exit", "clear", "echo", "color", "print_kernel_stack", "reboot", "halt"
};

void print_helpers(void) {
    // This function will print the available commands to the screen.
    set_input_mode(INPUT_MODE_NORMAL); // switch to normal input mode for not displaying >  
    print_f("Available commands:\n");
    print_f("  help - Show this help message\n");
    print_f("  exit - Exit the mini minishell\n");
    print_f("  clear - Clear the screen\n");
    print_f("  echo <message> - Print a message to the screen\n");
    print_f("  color <color> - Change the text color (e.g., red, green, blue)\n");
    print_f("  print_kernel_stack(<x>) - Print <x> line of the kernel stack (starting at the beginning)\n");
    print_f("  reboot - Reboot the system\n"); // not implemented yet
    print_f("  halt - Halt the system\n"); // not implemented yet
    print_f("Available colors:\n");
    for (size_t i = 0; i < COLOR_COUNT; ++i) {
        print_f("  %s\n", available_colors[i].name);
    }
    set_input_mode(INPUT_MODE_MINISHELL); // Switch back to mini minishell mode
    print_f("\n");
}
void launch_mini_minishell(void) {
    // This function will launch the mini minishell.
    // For now, we will just print a message to the screen.
    print_f("\nWelcome to the Mini Minishell!\n");
	print_f("Type 'help' for a list of commands.\n");
}

void handle_input(char *input) {
    // This function will handle the input from the user.
    // For now, we will just print the input to the screen.
    print_serial("Input received: %s\n", input);
    
    if (strcmp(input, "help") == 0) {
        print_helpers();
    } else if (strcmp(input, "exit") == 0) {
        print_f("Exiting Mini Minishell...");
        print_f("\n");
    }
    else if (strcmp(input, "clear") == 0) {
        clear_screen(&screen_context);
        print_f("\n");
    } else if (strncmp(input, "echo ", 5) == 0) {        
        print_f("\n%s", input + 5); // Print the message after "echo "
        print_f("\n");
    } else if (strncmp(input, "color ", 6) == 0) {
        char *color = input + 6;
        set_color(&screen_context, get_color_value(color));
        print_f("\nColor changed to %s\n", color);
    } else if (strncmp(input, "print_kernel_stack(", 18) == 0) {
        int lines = atoi(input + 18);
        print_kernel_stack(lines);
    } else if (strcmp(input, "reboot") == 0) {
        print_f("Rebooting...\n");
    }
    else {
        print_f("\n");
    }
}
