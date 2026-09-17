#include <stdio.h>
#include <stdbool.h>

#define APL_SETUP
#define APL_UPDATE
#define APL_RENDER
#define APL_INPUT
#define APL_DESTROY

#define AMD_MEMORY_DEBUG
#define ALMOG_MEMORY_DEBUG_IMPLEMENTATION
#include "../includes/Almog_Memory_Debug.h"

#define APL_ATR_SINGLE_PRECISION
#define APL_ATR_BRIDGE_IMPLEMENTATION
#include "../includes/APL_ATR_BRIDGE.h"
#define ALMOG_PLATFORM_LIBRARY_IMPLEMENTATION
#include "../includes/Almog_Platform_Library.h"
#define ALMOG_TEXT_RENDERING_IMPLEMENTATION
#include "../includes/Almog_Text_Rendering.h"


struct Atr_Offset_Zoom offzoom = {0};
struct Atr_Font font = {0};

enum Apl_Return_Types apl_setup(struct Apl_Window_State *ws)
{
    ws->to_limit_fps = false;
    offzoom = ATR_DEFAULT_OFFSET_ZOOM;

    // char font_file_name[] = "../src/fonts/nehama-webfont.ttf";

    char font_file_name[] = "../src/fonts/MPLUSU-VariableFont_wght.ttf";

    /* english */
    // char font_file_name[] = "../src/fonts/BLKCHCRY.ttf";
    // char font_file_name[] = "../src/fonts/Canterbury.ttf";
    // char font_file_name[] = "../src/fonts/Inconsolata-Regular.ttf";
    // char font_file_name[] = "../src/fonts/Symbola.ttf";
    // char font_file_name[] = "../src/fonts/waltographUI.ttf";

    if (ATR_FAIL == atr_font_load_from_file_name(&font, font_file_name)) {
        atr_dprintERROR("Failed to load font from file '%s'.", font_file_name);
        return APL_FAIL;
    }

    return APL_SUCCESS;
}

enum Apl_Return_Types apl_update(struct Apl_Window_State *ws)
{
    APL_UNUSED(ws);

    return APL_SUCCESS;
}

enum Apl_Return_Types apl_render(struct Apl_Window_State *ws)
{
    struct Atr_Pixel_Buffer font_pixels = apl_pixel_buffer_as_atr_pixel_buffer(ws->window_pixels_mat);

    // char str1[] = "the quick brown fox jumps over the lazy dog! @#$%^&*:\"{}[]?><\\/';.()_+-";
    // char str1[] = "אלמוג";
    char str1[] = "いろはにほへと ちりぬるを わかよたれそ つねならむ うゐのおくやま けふこえて あさきゆめみし ゑひもせす" 
                  "媛 怨 鬱 唄 淫 咽 茨 彙 椅 萎 畏 嵐 宛 顎 曖 挨 韓 鎌 葛 骸 蓋 崖 諧 潰 瓦 牙 苛 俺 臆 岡 旺 艶 稽 憬 詣 熊 窟 串 惧 錦 僅 巾 嗅 臼 畿 亀 伎 玩 挫 沙 痕 頃 駒 傲 乞 喉 梗 虎 股 舷 鍵 拳 桁 隙 呪 腫 嫉 𠮟/叱 鹿 餌 摯 恣 斬 拶 刹 柵 埼 塞 采 戚 脊 醒 凄 裾 須 腎 芯 尻 拭 憧 蹴 羞 袖 汰 遜 捉 踪 痩 曽 爽 遡 狙 膳 箋 詮 腺 煎 羨 鶴 爪 椎 捗 嘲 貼 酎 緻 綻 旦 誰 戴 堆 唾 鍋 謎 梨 奈 那 丼 貪 頓 栃 瞳 藤 賭 妬 塡[填] 溺 諦 阜 訃 肘 膝 眉 斑 阪 汎 氾 箸 剝[剥] 罵 捻 虹 匂 喩 闇 弥 冶 麺 冥 蜜 枕 昧 勃 頰[頬] 貌 蜂 蔑 璧 餅 蔽 脇 麓 籠 弄 呂 瑠 瞭 侶 慄 璃 藍 辣 拉 沃 瘍 妖 湧 柿 哺 楷 睦 釜 錮 賂 毀 勾";

    atr_real top_left_x = 10, top_left_y = 10, letter_hight = 100, spacing = 0;
    atr_text_line_draw(font_pixels, &font, (uint8_t *)str1, top_left_x, top_left_y, letter_hight, spacing, 0xFFFFFFFF, -1, (offzoom));

    // return APL_FAIL;

    return APL_SUCCESS;
}

enum Apl_Return_Types apl_input(struct Apl_Window_State *ws)
{
    size_t time_delay = 60;
    if (ws->buttons.e_is_pressed) {
        offzoom.zoom_multiplier *= 1.1f;
        ws->to_render = true;
        apl_sleep(time_delay);
    } else if (ws->buttons.q_is_pressed) {
        offzoom.zoom_multiplier /= 1.1f;
        ws->to_render = true;
        apl_sleep(time_delay);
    } else if (ws->buttons.r_is_pressed) {
        offzoom = ATR_DEFAULT_OFFSET_ZOOM;
        ws->to_render = true;
        apl_sleep(time_delay);
    } else if (ws->buttons.d_is_pressed) {
        offzoom.offset_x -= 1 / offzoom.zoom_multiplier * ws->window_pixels_mat.cols / 100;
        ws->to_render = true;
        apl_sleep(time_delay);
    } else if (ws->buttons.a_is_pressed) {
        offzoom.offset_x += 1 / offzoom.zoom_multiplier * ws->window_pixels_mat.cols / 100;
        ws->to_render = true;
        apl_sleep(time_delay);
    } else if (ws->buttons.s_is_pressed) {
        offzoom.offset_y -= 1 / offzoom.zoom_multiplier * ws->window_pixels_mat.rows / 100;
        ws->to_render = true;
        apl_sleep(time_delay);
    } else if (ws->buttons.w_is_pressed) {
        offzoom.offset_y += 1 / offzoom.zoom_multiplier * ws->window_pixels_mat.rows / 100;
        ws->to_render = true;
        apl_sleep(time_delay);
    } else if (ws->buttons.space_bar_is_pressed) {
        ws->to_update = !ws->to_update;
        apl_sleep(time_delay * 2000);
    }


    return APL_SUCCESS;
}

enum Apl_Return_Types apl_destroy(struct Apl_Window_State *ws)
{
    atr_font_free(&font);
    free(ws->window_pixels_mat.elements);
    free(ws->inv_z_buffer_mat.elements);

    if (AMD_FAIL == amd_debug_mem()) {
        amd_dprintERROR("%s", "Corrupted memory detected.");
        return APL_FAIL;
    }
    // amd_debug_mem_print(0);
    amd_debug_mem_reset();


    return APL_SUCCESS;
}
