#include <stdio.h>
#include <math.h>
#include <unistd.h>  // For usleep()

typedef struct {
    double R, tau, V_th, V_reset, dt;
    int steps;
    char spike_char;
    char rest_char;

    // Input waveform
    int waveform_type;      // 0=const, 1=step, 2=sin, 3=pulse
    double I_const;         // Constant current
    double I_low, I_high;   // For step/pulse
    int step_time;          // Step onset or pulse duration (in steps)
    double A, freq;         // Sinusoidal amplitude and frequency
} LIFParams;

// Function to calculate input current at each step
double get_input_current(int step, LIFParams *p) {
    double t = step * p->dt;

    switch (p->waveform_type) {
        case 0: // Constant
            return p->I_const;
        case 1: // Step
            return (step >= p->step_time) ? p->I_high : p->I_low;
        case 2: // Sinusoidal
            return p->A * sin(2 * M_PI * p->freq * t);
        case 3: // Pulse train
            return ((step / p->step_time) % 2 == 0) ? p->I_high : p->I_low;
        default:
            return 0;
    }
}

// Get all parameters from user
int get_user_input(LIFParams *p) {
    printf("\n--- Enter Neuron Parameters ---\n");

    printf("Membrane resistance R (MOhm): ");
    if (scanf("%lf", &p->R) != 1) return 0;

    printf("Time constant tau (ms): ");
    if (scanf("%lf", &p->tau) != 1) return 0;

    printf("Spike threshold V_th (mV): ");
    if (scanf("%lf", &p->V_th) != 1) return 0;

    printf("Reset voltage V_reset (mV): ");
    if (scanf("%lf", &p->V_reset) != 1) return 0;

    printf("Time step dt (ms): ");
    if (scanf("%lf", &p->dt) != 1) return 0;

    printf("Number of time steps: ");
    if (scanf("%d", &p->steps) != 1) return 0;

    printf("Symbol for spike (e.g., |): ");
    scanf(" %c", &p->spike_char);  // space before %c eats leftover newline

    printf("Symbol for rest (e.g., -): ");
    scanf(" %c", &p->rest_char);

    printf("\nSelect input current waveform:\n");
    printf("0 = Constant\n1 = Step\n2 = Sinusoidal\n3 = Pulse Train\n");
    printf("Your choice: ");
    scanf("%d", &p->waveform_type);

    if (p->waveform_type == 0) {
        printf("Enter constant I_ext: ");
        scanf("%lf", &p->I_const);
    } else if (p->waveform_type == 1 || p->waveform_type == 3) {
        printf("Enter I_low: ");
        scanf("%lf", &p->I_low);
        printf("Enter I_high: ");
        scanf("%lf", &p->I_high);
        printf("Enter step/pulse duration (in steps): ");
        scanf("%d", &p->step_time);
    } else if (p->waveform_type == 2) {
        printf("Enter amplitude A: ");
        scanf("%lf", &p->A);
        printf("Enter frequency (Hz): ");
        scanf("%lf", &p->freq);
    }

    return 1;
}

// Run simulation
void run_simulation(LIFParams p) {
    double V = 0.0;

    printf("\nSignal: ");
    for (int i = 0; i < p.steps; ++i) {
        double I_ext = get_input_current(i, &p);
        V += p.dt * ((-V + p.R * I_ext) / p.tau);

        if (V >= p.V_th) {
            printf("%c", p.spike_char);
            V = p.V_reset;
        } else {
            printf("%c", p.rest_char);
        }

        usleep(8000);  // Optional delay for animation (~8 ms)
        fflush(stdout);
    }

    printf("\n");
}

// Main loop
int main() {
    char again;
    LIFParams params;

    do {
        if (!get_user_input(&params)) {
            printf("Invalid input. Exiting.\n");
            return 1;
        }

        run_simulation(params);

        printf("\nRun again? (y/n): ");
        scanf(" %c", &again);
    } while (again == 'y' || again == 'Y');

    printf("Goodbye!\n");
    return 0;
}
