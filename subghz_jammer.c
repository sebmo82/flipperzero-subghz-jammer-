#include <furi.h>
#include <furi_hal.h>
#include <furi_hal_subghz.h>
#include <gui/gui.h>

// Fonction de dessin pour voir que l'app tourne
static void render_callback(Canvas* canvas, void* ctx) {
    UNUSED(ctx);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 30, 30, "JAMMING...");
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 10, 50, "Ext. Module Active");
}

int32_t subghz_jammer_app(void* p) {
    UNUSED(p);

    // 1. Initialisation GUI
    Gui* gui = furi_record_open(RECORD_GUI);
    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, render_callback, NULL);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    // 2. Initialisation Radio
    furi_hal_subghz_reset();
    furi_hal_power_enable_otg(); 
    furi_delay_ms(150);
    furi_hal_subghz_idle();

    // 3. Boucle principale avec gestion d'événements
    // On utilise furi_delay_ms pour laisser l'affichage se faire
    while(1) {
        // Fréquence 1
        furi_hal_subghz_set_frequency(433920000);
        furi_hal_subghz_tx();
        furi_delay_ms(100);
        furi_hal_subghz_idle();

        // Fréquence 2
        furi_hal_subghz_set_frequency(868350000);
        furi_hal_subghz_tx();
        furi_delay_ms(100);
        furi_hal_subghz_idle();

        // Rafraîchir l'écran
        view_port_update(view_port);

        // Sortie : On vérifie si on doit quitter (Bouton retour maintenu)
        // Note : On ne vérifie pas le bouton à chaque ms pour éviter les crashs
        furi_delay_ms(10);
        
        // Sur Momentum, cette méthode est plus sûre pour sortir
        if(furi_hal_gpio_read(&gpio_button_back) == false) {
            break; 
        }
    }

    // Nettoyage
    furi_hal_subghz_idle();
    furi_hal_power_disable_otg();
    gui_remove_view_port(gui, view_port);
    view_port_free(view_port);
    furi_record_close(RECORD_GUI);

    return 0;
}
