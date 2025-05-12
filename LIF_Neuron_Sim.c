#include <stdio.h>

int main() {
    // Neuron parameters
    double V = 0.0;
    double R = 1.0;
    double tau = 10.0;
    double V_th = 1.0;
    double V_reset = 0.0;
    double I_ext = 2.5;
    double dt = 1;
    int steps = 100;

    printf("Signal: ");

    for (int i = 0; i < steps; ++i) {
        V += dt * ((-V + R * I_ext) / tau);

        if (V >= V_th) {
            printf("|");
            V = V_reset;
        } else {
            printf("-");
        }
    }

    printf("\n");
    return 0;
}
