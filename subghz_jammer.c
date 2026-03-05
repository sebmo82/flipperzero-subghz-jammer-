#include <furi.h>
#include <furi_hal.h>
#include <furi_hal_subghz.h>
#include <gui/gui.h> // Ajout pour forcer l'affichage

int32_t subghz_jammer_app(void* p) {
    UNUSED(p);

    // 1. Créer une vue vide pour dire au système "Je suis là !"
    // C'est ce qui retire le sablier sur Momentum
    Gui* gui = furi_record_open(RECORD_GUI);
    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, NULL, NULL);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    // 2. Initialisation Radio avec sécurité
    if(furi_hal_subghz_is_tx_allowed(433920000)) {
        furi_hal_subghz_reset();
        furi_hal_power_enable_otg();
        furi_delay_ms(100);
    }

    FURI_LOG_I("JAMMER", "App started, UI loaded");

    // 3. Boucle de travail
    while(1) {
        // Fréquence 1
        furi_hal_subghz_set_frequency(433920000);
        furi_hal_subghz_tx();
        furi_delay_ms(50);
        furi_hal_subghz_idle();

        // Fréquence 2
        furi_hal_subghz_set_frequency(868350000);
        furi_hal_subghz_tx();
        furi_delay_ms(50);
        furi_hal_subghz_idle();

        // Crucial pour Momentum : laisser l'OS respirer
        furi_delay_ms(20);

        // Si tu appuies sur BACK, on quitte (évite de forcer le reboot)
        if(furi_hal_gpio_read(&gpio_button_back) == false) break;
    }

    // Nettoyage
    furi_hal_subghz_idle();
    furi_hal_power_disable_otg();
    gui_remove_view_port(gui, view_port);
    view_port_free(view_port);
    furi_record_close(RECORD_GUI);

    return 0;
}
