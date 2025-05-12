#include <stdio.h>

int main() {
    // Declare all parameters
    double V = 0.0;
    double R, tau, V_th, V_reset, I_ext, dt;
    int steps;

    // Get user input
    printf("Enter membrane resistance R: ");
    scanf("%lf", &R);

    printf("Enter membrane time constant tau: ");
    scanf("%lf", &tau);

    printf("Enter spike threshold V_th: ");
    scanf("%lf", &V_th);

    printf("Enter reset voltage V_reset: ");
    scanf("%lf", &V_reset);

    printf("Enter external input current I_ext: ");
    scanf("%lf", &I_ext);

    printf("Enter time step dt: ");
    scanf("%lf", &dt);

    printf("Enter number of steps: ");
    scanf("%d", &steps);

    printf("\nSignal: ");

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
