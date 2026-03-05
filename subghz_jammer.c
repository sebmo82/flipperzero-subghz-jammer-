#include <furi.h>
#include <furi_hal.h>
#include <furi_hal_subghz.h>

uint32_t frequencies[] = {433920000, 315000000, 868350000, 434775000};

int32_t subghz_jammer_app(void* p) {
    UNUSED(p);

    // Initialisation douce
    furi_hal_subghz_reset();
    furi_hal_power_enable_otg(); // Active le 3.3V sur la pin 9
    furi_delay_ms(100); // Laisse le temps au CC1101 de s'allumer

    furi_hal_subghz_idle();

    // On lance la boucle
    while(1) {
        for(size_t i = 0; i < 4; i++) {
            furi_hal_subghz_set_frequency(frequencies[i]);
            furi_hal_subghz_tx(); 
            furi_delay_ms(50); 
            furi_hal_subghz_idle();
            furi_delay_ms(10); 
        }
        
        // Permet au système de respirer et d'éviter le "Watchdog Reset"
        furi_delay_ms(1); 
    }

    furi_hal_power_disable_otg();
    return 0;
}
