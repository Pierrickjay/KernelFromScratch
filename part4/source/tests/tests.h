#ifndef TESTS_H
#define TESTS_H

void print_42(void);
void print_k_test();
void print_test();
void print_f_test();
void test_division_by_zero(void);
void main_tests();

// Tests pour le systeme de signal handler [B1]
void test_signal_handler_system(void);

// Tests pour le systeme de signal scheduler [B2]
void test_signal_scheduler_system(void);

#endif
