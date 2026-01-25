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

static void anim_x_cb(void *var, int32_t v) {
  lv_obj_set_x((lv_obj_t *)var, v);
}

static void anim_size_cb(void *var, int32_t v) {
  lv_obj_set_size((lv_obj_t *)var, v, v);
}

/**
 * Create a playback animation
 */
void ui_lv_example_anim_2(void) {

  lv_obj_t *obj = lv_obj_create(lv_screen_active());
  lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_RED), 0);
  lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, 0);

  lv_obj_align(obj, LV_ALIGN_LEFT_MID, 10, 0);

  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_var(&a, obj);
  lv_anim_set_values(&a, 10, 50);
  lv_anim_set_duration(&a, 1000);
  lv_anim_set_reverse_delay(&a, 100);
  lv_anim_set_reverse_duration(&a, 300);
  lv_anim_set_repeat_delay(&a, 500);
  lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
  lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);

  lv_anim_set_exec_cb(&a, anim_size_cb);
  lv_anim_start(&a);
  lv_anim_set_exec_cb(&a, anim_x_cb);
  lv_anim_set_values(&a, 10, 240);
  lv_anim_start(&a);
}

static void btn_event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *btn = lv_event_get_target_obj(e);
  if (code == LV_EVENT_CLICKED) {
    static uint8_t cnt = 0;
    cnt++;

    /*Get the first child of the button which is the label and change its text*/
    lv_obj_t *label = lv_obj_get_child(btn, 0);
    lv_label_set_text_fmt(label, "Start: %d", cnt);
  }
}

/**
 * Create a button with a label and react on click event.
 */
void ui_lv_example_get_started_2(void) {
  lv_obj_t *btn =
      lv_button_create(lv_screen_active()); /*Add a button the current screen*/
  lv_obj_set_pos(btn, 10, 10);              /*Set its position*/
  lv_obj_set_size(btn, 200, 50);            /*Set its size*/
  lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL,
                      NULL); /*Assign a callback to the button*/

  lv_obj_t *label = lv_label_create(btn); /*Add a label to the button*/
  lv_label_set_text(label, "Start");      /*Set the labels text*/
  lv_obj_center(label);
}

void spawn_random_dot(void) {
  uint32_t rand_color = lv_rand(LV_PALETTE_RED, LV_PALETTE_GREY);
  uint32_t rand_x = lv_rand(0, 240);
  uint32_t rand_y = lv_rand(0, 240);
  lv_obj_t *dot = lv_obj_create(lv_screen_active());
  lv_obj_set_style_bg_color(dot, lv_palette_main(rand_color), 0);
  lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
  lv_obj_set_pos(dot, rand_x, rand_y);
}