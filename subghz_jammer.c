#include <furi.h>
#include <furi_hal.h>
#include <furi_hal_subghz.h>
#include <gui/gui.h>

int32_t subghz_jammer_app(void* p) {
    UNUSED(p);

    // --- INITIALISATION GUI (Pour enlever le sablier) ---
    Gui* gui = furi_record_open(RECORD_GUI);
    ViewPort* view_port = view_port_alloc();
    // On ne dessine rien, mais on enregistre la fenêtre
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    // --- INITIALISATION RADIO ---
    furi_hal_subghz_reset();
    furi_hal_power_enable_otg(); // Alim pour ton module 3-in-1
    furi_delay_ms(100);
    furi_hal_subghz_idle();

    FURI_LOG_I("JAMMER", "App Started");

    // --- BOUCLE PRINCIPALE ---
    // On tourne tant que l'utilisateur ne reste pas appuyé sur BACK
    while(1) {
        // Fréquence 1 : 433.92 MHz
        furi_hal_subghz_set_frequency(433920000);
        furi_hal_subghz_tx();
        furi_delay_ms(50);
        furi_hal_subghz_idle();

        // Fréquence 2 : 868.35 MHz
        furi_hal_subghz_set_frequency(868350000);
        furi_hal_subghz_tx();
        furi_delay_ms(50);
        furi_hal_subghz_idle();

        // Petite pause pour laisser le système respirer
        furi_delay_ms(10);

        // Sortie de secours si on appuie sur BACK
        if(furi_hal_gpio_read(&gpio_button_back) == false) {
            break;
        }
    }

    // --- NETTOYAGE ---
    furi_hal_subghz_idle();
    furi_hal_power_disable_otg();
    
    gui_remove_view_port(gui, view_port);
    view_port_free(view_port);
    furi_record_close(RECORD_GUI);

    return 0;
}
