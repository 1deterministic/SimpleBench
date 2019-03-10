#include "simplebench.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef __linux__
    #include <unistd.h>
    
#elif __MINGW64__ || __MINGW32__ || _WIN32
#endif

// normalizes the score
float handicap;
// size of the matrices being used
int alu_matrix_size;
int fpu_matrix_size;
int mem_matrix_size;
// size of the tasks
int alu_job_size;
int fpu_job_size;
int mem_job_size;

int main(int argc, char** argv) {
    MsgCode code = SUCCESS;

    float singlethread_score = 0.0;
    float multithread_score = 0.0;

    // default configuration
    #ifdef __linux__
        int threads = sysconf(_SC_NPROCESSORS_ONLN);

    #elif __MINGW64__ || __MINGW32__ || _WIN32
        SYSTEM_INFO info;
        GetSystemInfo(&info);
        int threads = info.dwNumberOfProcessors;

    #endif
    
    int hardware_level = DEFAULT_CONFIG_HARDWARE;
    bool show_gui = true;
    bool st_test = true;
    bool mt_test = true;

    code = get_cli_options(argc, argv, &show_gui, &st_test, &mt_test, &hardware_level, &threads);
    if (code) {printf("%s\n", get_string(code)); return code;}

    code = load_test_config(hardware_level);
    if (code) {printf("%s\n", get_string(code)); return code;}
    
    // gets the scores fot the single and multithreaded tests
    if (st_test) {
        code = test_system(&singlethread_score, 1, handicap, show_gui);
        if (code) {printf("%s\n", get_string(code)); return code;}
    }
    
    if (mt_test) {
        // skip the multithread test if the system only has 1
        if (threads > 1) {
            code = test_system(&multithread_score, threads, handicap, show_gui);
            if (code) {printf("%s\n", get_string(code)); return code;}
        } else {
            multithread_score = singlethread_score;
        }
    }

    show_score(singlethread_score, multithread_score, threads);
    return code;
}

// reads the options received via CLI
MsgCode get_cli_options(int argc, char** argv, bool* show_gui, bool* st_test, bool* mt_test, int* hardware_level, int* threads) {
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
void show_score(float singlethread_score, float multithread_score, int threads) {
    printf("%-20s: %20s\n", get_string(MSG_MAIN_SHOW_SCORE_SIMPLEBENCH_VERSION), BENCHMARK_VERSION);
    printf("%-20s: %20s\n", get_string(MSG_MAIN_SHOW_SCORE_SIMPLEBENCH_BUILD), BUILD_VERSION);
    printf("%-20s: %20.2f\n", get_string(MSG_MAIN_SHOW_SCORE_SINGLETHREAD_SCORE), singlethread_score);
    printf("%-20s: %20.2f\n", get_string(MSG_MAIN_SHOW_SCORE_MULTITHREAD_SCORE), multithread_score);
    printf("%-20s: %20.2f\n", get_string(MSG_MAIN_SHOW_SCORE_MULTIPLIER), (singlethread_score > 0.0) ? multithread_score / singlethread_score : 0.0);
}

// loads the test config
MsgCode load_test_config(int config) {
    switch (config) {
        // This configuration will use about 515MB of RAM
        case 2: {
            handicap = 1.0;
            alu_matrix_size = 256; // 256KB
            fpu_matrix_size = 256; // 256KB
            mem_matrix_size = 8192; // 256MB
            alu_job_size = 64 * alu_matrix_size; // 64 times * 256 rows * 256 columns * (8 sums + 8 subtractions + 8 multiplies + 2 division) = 109,051,904 integer ops
            fpu_job_size = 32 * fpu_matrix_size; // 32 times * 256 rows * 256 columns * (8 sums + 8 subtractions + 8 multiplies + 2 division) = 54,525,952 floating point ops
            mem_job_size = 32 * mem_matrix_size; // 32 times * 8192 rows * 8192 columns * 4 lines each time * 4 bytes per element = 32GB of data
            return SUCCESS;
        }

        // This configuration will use about 35MB of RAM
        case 1: {
            handicap = 0.0625;
            alu_matrix_size = 256; // 256KB
            fpu_matrix_size = 256; // 256KB
            mem_matrix_size = 2048; // 16MB
            alu_job_size = 4 * alu_matrix_size; // 4 times * 256 rows * 256 columns * (8 sums + 8 subtractions + 8 multiplies + 2 division) = 6,815,744 integer ops
            fpu_job_size = 2 * fpu_matrix_size; // 2 times * 256 rows * 256 columns * (8 sums + 8 subtractions + 8 multiplies + 2 division) = 3,407,872 floating point ops
            mem_job_size = 32 * mem_matrix_size; // 32 times * 2048 rows * 2048 columns * 4 lines each time * 4 bytes per element = 2GB of data
            return SUCCESS;
        }            
    };

    return MSG_GET_CLI_OPTIONS_INVALID_INT;
}