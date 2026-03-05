#include <furi.h>
#include <furi_hal.h>
#include <furi_hal_subghz.h>

uint32_t frequencies[] = {433920000, 315000000, 868350000};

int32_t subghz_jammer_app(void* p) {
    UNUSED(p);

    // Initialisation
    furi_hal_subghz_reset();
    furi_hal_power_enable_otg(); // Nécessaire pour ton module 3-en-1
    furi_delay_ms(150); // Pause un peu plus longue pour Momentum

    // On passe en mode Idle pour être prêt
    furi_hal_subghz_idle();

    FURI_LOG_I("JAMMER", "App Started on Momentum");

    // Boucle principale optimisée
    while(1) {
        for(size_t i = 0; i < 3; i++) {
            furi_hal_subghz_set_frequency(frequencies[i]);
            
            furi_hal_subghz_tx(); 
            furi_delay_ms(60); 
            
            furi_hal_subghz_idle();
            furi_delay_ms(15); 
        }

        // CRUCIAL : Relâcher le processeur pour éviter le sablier
        furi_delay_us(10); 
        if(furi_hal_gpio_read(&gpio_button_back) == false) {
             // Si on veut pouvoir quitter proprement avec le bouton retour
             // (On verra ça plus tard, pour l'instant on se concentre sur le freeze)
        }
    }

    return 0;
}
