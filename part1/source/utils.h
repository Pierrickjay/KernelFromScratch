#ifndef UTILS_H
# define UTILS_H


struct arg_list {
    int *current_arg;
};

// Helper functions pour rendre le code plus clair
static void init_args(struct arg_list *args, char *first_param) {
    args->current_arg = (int *)(&first_param);
    args->current_arg++; // Skip le paramètre format
}

static char *get_string_arg(struct arg_list *args) {
    return *((char **)args->current_arg++);
}

static int get_int_arg(struct arg_list *args) {
    return *args->current_arg++;
}

static char get_char_arg(struct arg_list *args) {
    return (char)(*args->current_arg++);
}

#endif