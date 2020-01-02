#include "simplebench.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#if __linux__ || __APPLE__
    #include <unistd.h>
#elif _WIN32
#endif

int main(int argc, char** argv) {
    // seed for the random number generator
    srand((unsigned) time(NULL));

    // default return code is success
    MsgCode code = SUCCESS;

    // scores start at 0
    float singlethread_score = 0.0;
    float multithread_score = 0.0;

    // default configuration
    int hardware_level = DEFAULT_CONFIG_HARDWARE;
    bool show_gui = DEFAULT_SHOW_GUI;
    bool st_test = DEFAULT_ST_TEST;
    bool mt_test = DEFAULT_MT_TEST;
    bool pin_threads = DEFAULT_PIN_THREADS;
    int threads = DEFAULT_THREADS;

    // auto detect number of threads
    #if __linux__ || __APPLE__
        threads = sysconf(_SC_NPROCESSORS_ONLN);
    #elif _WIN32
        SYSTEM_INFO info;
        GetSystemInfo(&info);
        threads = info.dwNumberOfProcessors;
    #endif

    // read command line options
    if (!code) {
        code = get_cli_options(argc, argv, &show_gui, &st_test, &mt_test, &hardware_level, &threads, &pin_threads);
    }
        

    // the matrix size changes with the hardware level used
    // every step up on hardware level doubles it horizontally and vertically
    // this increases the amount of work by 4 times, so the handicap is then increased by 4
    float handicap = 0.00390625 * pow(4, hardware_level - 1);
    int alu_matrix_size = 16 * pow(2, hardware_level - 1); // 1K; 4K; 16K; 64K; [256K]; 1M...
    int fpu_matrix_size = 16 * pow(2, hardware_level - 1); // 1K; 4K; 16K; 64K; [256K]; 1M...
    int mem_matrix_size = 1024 * pow(2, hardware_level - 1); // 4M; 16M; 64M; 256M; [1024M]; 4096M...
    
    // runs the singlethreaded test
    if (st_test) {
        if (!code) {
            code = test_system(&singlethread_score, 1, pin_threads, handicap, show_gui, alu_matrix_size, fpu_matrix_size, mem_matrix_size);
        }
    }
    
    // runs the multithreaded test
    if (mt_test) {
        // but only if set to run with more than 1 thread
        if (threads > 1) {
            if (!code) {
                code = test_system(&multithread_score, threads, pin_threads, handicap, show_gui, alu_matrix_size, fpu_matrix_size, mem_matrix_size); 
            }
        // else just replicates the singlethreaded score as the multithreaded score
        } else {
            multithread_score = singlethread_score;
        }
        
    }

    // show score if no errors occurred
    if (!code) {
        show_score(singlethread_score, multithread_score, threads, hardware_level); 
    } else {
        printf("%s\n", get_string(code));
    }

    // also return any errors to the terminal emulator
    return code;
}

// reads the options received via CLI
MsgCode get_cli_options(int argc, char** argv, bool* show_gui, bool* st_test, bool* mt_test, int* hardware_level, int* threads, bool* pin_threads) {
    for (int i = 1; i < argc; i++) {
        // --show-gui
        if (strcmp(argv[i], CLI_SHOW_GUI) == 0) {
            // check if the next argument exists before trying to access
            if (i + 1 < argc) {
                i++;
                // if the option is "on"
                if (strcmp(argv[i], CLI_ON) == 0) {
                    *show_gui = true;
                }
                // if the option is "off"
                else if (strcmp(argv[i], CLI_OFF) == 0) {
                    *show_gui = false;
                }
                // next argument is not "on"/"off"
                else {
                    return MSG_GET_CLI_OPTIONS_INVALID_ONOFF;
                }
                // no need to test the other if's, goes to the next argument
                continue;
            } else {
                // did not find "on"/"off", reports the error
                return MSG_GET_CLI_OPTIONS_MISSING_ONOFF;
            }
        }

        // --st-test
        if (strcmp(argv[i], CLI_ST_TEST) == 0) {
            // check if the next argument exists before trying to access
            if (i + 1 < argc) {
                i++;
                // if the option is "on"
                if (strcmp(argv[i], CLI_ON) == 0) {
                    *st_test = true;
                }
                // if the option is "off"
                else if (strcmp(argv[i], CLI_OFF) == 0) {
                    *st_test = false;
                }
                // next argument is not "on"/"off"
                else {
                    return MSG_GET_CLI_OPTIONS_INVALID_ONOFF;
                }
                // no need to test the other if's, goes to the next argument
                continue;
            } else {
                // did not find "on"/"off", reports the error
                return MSG_GET_CLI_OPTIONS_MISSING_ONOFF;
            }
        }

        // --mt-test
        if (strcmp(argv[i], CLI_MT_TEST) == 0) {
            // check if the next argument exists before trying to access
            if (i + 1 < argc) {
                i++;
                // if the option is "on"
                if (strcmp(argv[i], CLI_ON) == 0) {
                    *mt_test = true;
                }
                // if the option is "off"
                else if (strcmp(argv[i], CLI_OFF) == 0) {
                    *mt_test = false;
                }
                // next argument is not "on"/"off"
                else {
                    return MSG_GET_CLI_OPTIONS_INVALID_ONOFF;
                }
                // no need to test the other if's, goes to the next argument
                continue;
            } else {
                // did not find "on"/"off", reports the error
                return MSG_GET_CLI_OPTIONS_MISSING_ONOFF;
            }
        }

        // --threads
        else if (strcmp(argv[i], CLI_THREADS) == 0) {
            // check if the next argument exists before trying to access
            if (i + 1 < argc) {
                i++;
                // converts the text to an integer value
                *threads = atoi(argv[i]);
                // only values greater than zero are allowed
                // also, atoi will return 0 if the text is not an integer
                if (*threads <= 0) {
                    return MSG_GET_CLI_OPTIONS_INVALID_INT;
                }   
                // no need to test the other if's, goes to the next argument 
                continue;
            } else {
                // did not find a valid value, reports the error
                return MSG_GET_CLI_OPTIONS_MISSING_INT;
            }
        }

        // --pin-threads
        if (strcmp(argv[i], CLI_PIN_THREADS) == 0) {
            // check if the next argument exists before trying to access
            if (i + 1 < argc) {
                i++;
                // if the option is "on"
                if (strcmp(argv[i], CLI_ON) == 0) {
                    // macOS does not offer an interface to pin threads to cores
                    #if __APPLE__
                        return MSG_GET_CLI_OPTIONS_PINTHREADS_NOT_SUPPORTED;
                    // otherwise should work fine
                    #else
                        *pin_threads = true;

                    #endif
                }
                // if the option is "off"
                else if (strcmp(argv[i], CLI_OFF) == 0) {
                    *pin_threads = false;
                }
                // next argument is not "on"/"off"
                else {
                    return MSG_GET_CLI_OPTIONS_INVALID_ONOFF;
                }
                // no need to test the other if's, goes to the next argument
                continue;
            } else {
                // did not find "on"/"off", reports the error
                return MSG_GET_CLI_OPTIONS_MISSING_ONOFF;
            }
        }

        // --hardware-level
        if (strcmp(argv[i], CLI_HARDWARE_LEVEL) == 0) {
            // check if the next argument exists before trying to access
            if (i + 1 < argc) {
                i++;
                // converts the text to an integer value
                *hardware_level = atoi(argv[i]);
                // only values greater than zero are allowed
                // also, atoi will return 0 if the text is not an integer
                if (*hardware_level <= 0) {
                    return MSG_GET_CLI_OPTIONS_INVALID_INT;
                }
                // no need to test the other if's, goes to the next argument 
                continue;
            } else {
                // did not find a valid value, reports the error
                return MSG_GET_CLI_OPTIONS_MISSING_INT;
            }
        }

        // --version
        else if (strcmp(argv[i], CLI_VERSION) == 0) {
            return MSG_GET_CLI_OPTIONS_VERSION;
        }

        // --build
        else if (strcmp(argv[i], CLI_BUILD) == 0) {
            return MSG_GET_CLI_OPTIONS_BUILD;
        }

        // --help
        else if (strcmp(argv[i], CLI_HELP) == 0) {            
            return MSG_HELP_TEXT;
        }

        // not recognized option
        else {
            return MSG_GET_CLI_OPTIONS_UNKNOWN_OPTION;
        }
    }

    return SUCCESS;
}

// shows the system score
void show_score(float singlethread_score, float multithread_score, int threads, int hardware_level) {
    printf("%-20s:\n", get_string(MSG_MAIN_SHOW_SCORE_SIMPLEBENCH_HEADER));
    printf("%-20s: %-40s\n", get_string(MSG_MAIN_SHOW_SCORE_SIMPLEBENCH_VERSION), BENCHMARK_VERSION);
    printf("%-20s: %-40s\n", get_string(MSG_MAIN_SHOW_SCORE_SIMPLEBENCH_BUILD), BUILD_VERSION" ("BUILD_CODENAME")");
    printf("%-20s: %-40d\n", get_string(MSG_MAIN_SHOW_SCORE_HARDWARE_LEVEL), hardware_level);
    printf("%-20s: %-40.2f\n", get_string(MSG_MAIN_SHOW_SCORE_SINGLETHREAD_SCORE), singlethread_score);
    printf("%-20s: %-40.2f\n", get_string(MSG_MAIN_SHOW_SCORE_MULTITHREAD_SCORE), multithread_score);
    printf("%-20s: %-40.2f\n", get_string(MSG_MAIN_SHOW_SCORE_MULTIPLIER), (singlethread_score > 0.0) ? multithread_score / singlethread_score : 0.0);
}