#include "ui.h"
#include "cat_pic.h"

void ui_display_hello(void) {
  lv_obj_t *scr = lv_screen_active();
  lv_obj_set_style_bg_color(scr, lv_color_white(), 0);

  lv_obj_t *label = lv_label_create(scr);
  lv_label_set_text(label, "Hello World!");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}

void ui_display_cat(void) {
  LV_IMAGE_DECLARE(cat_img);
  lv_obj_t *img1 = lv_image_create(lv_screen_active());
  lv_image_set_src(img1, &cat_img);
  lv_obj_align(img1, LV_ALIGN_CENTER, 0, 0);
}