#include "utility.cpp";
#include <algorithm>

void render_background() {
    uint* pixel = (uint*)render_state.memory;
    for (int y = 0; y < render_state.height; y++) {
        for (int x = 0; x < render_state.width; x++) {
            *pixel++ = x * x + y;
        }
    }
}

void clear_screen(uint color) {
    uint* pixel = (uint*)render_state.memory;
    for (int y = 0; y < render_state.height; y++) {
        for (int x = 0; x < render_state.width; x++) {
            *pixel++ = color;
        }
    }
}

void draw_rect_pixels(int x0, int y0, int x1, int y1, uint color) {
    // Ensure x0 < x1 and y0 < y1
    /*if (x0 > x1) std::swap(x0, x1);
    if (y0 > y1) std::swap(y0, y1);*/

    x0 = clamp(x0, 0, render_state.width);
    x1 = clamp(x1, 0, render_state.width);
    y0 = clamp(y0, 0, render_state.height);
    y1 = clamp(y1, 0, render_state.height);

    for (int y = y0; y < y1; y++) {
        uint* pixel = (uint*)render_state.memory + x0 + y * render_state.width;
        for (int x = x0; x < x1; x++) {
            *pixel++ = color;
        }
    }
}

void draw_rect(float x, float y, float half_size_x, float half_size_y, uint color) {
    // Correct mapping: scale x by width, y by height
    x *= render_state.height;
	y *= render_state.height;
	half_size_x *= render_state.height;
	half_size_y *= render_state.height;
	x += render_state.width / 2.0f;
	y += render_state.height / 2.0f;

    int x0 = static_cast<int>(x - half_size_x);
    int y0 = static_cast<int>(y - half_size_y);
    int x1 = static_cast<int>(x + half_size_x);
    int y1 = static_cast<int>(y + half_size_y);
    draw_rect_pixels(x0, y0, x1, y1, color);
}



void draw_numbers(int number, float x, float y, float size, uint color) { 
    float half_size = size * 0.5f;

    bool zero_done = false;
    while (number||!zero_done) {
        zero_done = true;
        int digit = number % 10;
        number /= 10;
        switch (digit) {
        case 0: {
            draw_rect(x - size, y, half_size, size * 2 + half_size, color);
            draw_rect(x + size, y, half_size, size * 2 + half_size, color);
            draw_rect(x, y + size * 2.f, size + half_size, half_size, color);
            draw_rect(x, y - size * 2.f, size + half_size, half_size, color);
            x -= size * 4.f;
        } break;

        case 1: {
            draw_rect(x, y, half_size, size * 2 + half_size, color);
            x -= size * 4.f;
        } break;

        case 2: {
            draw_rect(x, y + size * 2.f, size + half_size, half_size, color);
            draw_rect(x, y, size + half_size, half_size, color);
            draw_rect(x, y - size * 2.f, size + half_size, half_size, color);
            draw_rect(x + size, y + (size * 2 + half_size) / 2, half_size, (size * 2 + half_size) / 2, color);
            draw_rect(x - size, y - (size * 2 + half_size) / 2, half_size, (size * 2 + half_size) / 2, color);
            x -= size * 4.f;
        }break;

        case 3: {
            draw_rect(x, y + size * 2.f, size + half_size, half_size, color);
            draw_rect(x, y, size + half_size, half_size, color);
            draw_rect(x, y - size * 2.f, size + half_size, half_size, color);
            draw_rect(x + size, y, half_size, size * 2 + half_size, color);
            x -= size * 4.f;
        }break;

        case 4: {
            draw_rect(x, y, size + half_size, half_size, color);
            draw_rect(x - size, y + (size * 2 + half_size) / 2, half_size, (size * 2 + half_size) / 2, color);
            draw_rect(x + size, y, half_size, size * 2 + half_size, color);
            x -= size * 4.f;
        }break;

        case 5: {
            draw_rect(x, y + size * 2.f, size + half_size, half_size, color);
            draw_rect(x, y, size + half_size, half_size, color);
            draw_rect(x, y - size * 2.f, size + half_size, half_size, color);
            draw_rect(x - size, y + (size * 2 + half_size) / 2, half_size, (size * 2 + half_size) / 2, color);
            draw_rect(x + size, y - (size * 2 + half_size) / 2, half_size, (size * 2 + half_size) / 2, color);
            x -= size * 4.f;
        }break;

        case 6: {
            draw_rect(x, y + size * 2.f, size + half_size, half_size, color);
            draw_rect(x, y, size + half_size, half_size, color);
            draw_rect(x, y - size * 2.f, size + half_size, half_size, color);
            draw_rect(x - size, y, half_size, size * 2 + half_size, color);
            draw_rect(x + size, y - (size * 2 + half_size) / 2, half_size, (size * 2 + half_size) / 2, color);
            x -= size * 4.f;
        }break;

        case 7: {
            draw_rect(x + size, y, half_size, size * 2 + half_size, color);
            draw_rect(x, y + size * 2.f, size + half_size, half_size, color);
            x -= size * 4.f;
        } break;

        case 8: {
            draw_rect(x, y + size * 2.f, size + half_size, half_size, color);
            draw_rect(x, y, size + half_size, half_size, color);
            draw_rect(x, y - size * 2.f, size + half_size, half_size, color);
            draw_rect(x + size, y, half_size, size * 2 + half_size, color);
            draw_rect(x - size, y, half_size, size * 2 + half_size, color);
            x -= size * 4.f;
        } break;

        case 9: {
            draw_rect(x, y + size * 2.f, size + half_size, half_size, color);
            draw_rect(x, y, size + half_size, half_size, color);
            draw_rect(x + size, y, half_size, size * 2 + half_size, color);
            draw_rect(x - size, y + (size * 2 + half_size) / 2, half_size, (size * 2 + half_size) / 2, color);
            x -= size * 4.f;
        } break;
        }
    }
}


const char* alphabets[][7] = {
    {
        " 000 ",
        "0   0",
        "0   0",
        "00000",
        "0   0",
        "0   0",
        "0   0"
    },
    {
        "0000 ",
        "0   0",
        "0   0",
        "0000 ",
        "0   0",
        "0   0",
        "0000 "
    },
    {
        " 000 ",
        "0   0",
        "0    ",
        "0    ",
        "0    ",
        "0   0",
        " 000 "
    },
    {
        "0000 ",
        "0   0",
        "0   0",
        "0   0",
        "0   0",
        "0   0",
        "0000 "
    },
    {
        "00000",
        "0    ",
        "0    ",
        "0000 ",
        "0    ",
        "0    ",
        "00000"
    },
    {
        "00000",
        "0    ",
        "0    ",
        "0000 ",
        "0    ",
        "0    ",
        "0    "
    },
    {
        " 0000",
        "0    ",
        "0    ",
        "0 000",
        "0   0",
        "0   0",
        " 0000"
    },
    {
        "0   0",
        "0   0",
        "0   0",
        "00000",
        "0   0",
        "0   0",
        "0   0"
    },
    {
        " 000 ",
        "  0  ",
        "  0  ",
        "  0  ",
        "  0  ",
        "  0  ",
        " 000 "
    },
    {
        "  000",
        "   0 ",
        "   0 ",
        "   0 ",
        "   0 ",
        "0  0 ",
        " 00  "
    },
    {
        "0   0",
        "0  0 ",
        "0 0  ",
        "00   ",
        "0 0  ",
        "0  0 ",
        "0   0"
    },
    {
        "0    ",
        "0    ",
        "0    ",
        "0    ",
        "0    ",
        "0    ",
        "00000"
    },
    {
        "0   0",
        "00 00",
        "0 0 0",
        "0   0",
        "0   0",
        "0   0",
        "0   0"
    },
    {
        "0   0",
        "00  0",
        "0 0 0",
        "0  00",
        "0   0",
        "0   0",
        "0   0"
    },
    {
        " 000 ",
        "0   0",
        "0   0",
        "0   0",
        "0   0",
        "0   0",
        " 000 "
    },
    {
        "0000 ",
        "0   0",
        "0   0",
        "0000 ",
        "0    ",
        "0    ",
        "0    "
    },
    {
        " 000 ",
        "0   0",
        "0   0",
        "0   0",
        "0 0 0",
        "0  0 ",
        " 00 0"
    },
    {
        "0000 ",
        "0   0",
        "0   0",
        "0000 ",
        "0  0 ",
        "0   0",
        "0   0"
    },
    {
        " 0000",
        "0    ",
        "0    ",
        " 000 ",
        "    0",
        "    0",
        "0000 "
    },
    {
        "00000",
        "  0  ",
        "  0  ",
        "  0  ",
        "  0  ",
        "  0  ",
        "  0  "
    },
    {
        "0   0",
        "0   0",
        "0   0",
        "0   0",
        "0   0",
        "0   0",
        " 000 "
    },
    {
        "0   0",
        "0   0",
        "0   0",
        "0   0",
        " 0 0 ",
        " 0 0 ",
        "  0  "
    },
    {
        "0   0",
        "0   0",
        "0   0",
        "0   0",
        "0 0 0",
        "0 0 0",
        " 0 0 "
    },
    {
        "0   0",
        "0   0",
        " 0 0 ",
        "  0  ",
        " 0 0 ",
        "0   0",
        "0   0"
    },
    {
        "0   0",
        "0   0",
        " 0 0 ",
        "  0  ",
        "  0  ",
        "  0  ",
        "  0  "
    },
    {
        "00000",
        "    0",
        "   0 ",
        "  0  ",
        " 0   ",
        "0    ",
        "00000"
    }
};

void draw_char(char c, float x, float y, float size, uint color) {
    if (c < 'A' || c > 'Z') return;

    int index = c - 'A';

    float half_size = size * 0.5f;

    for (int row = 0; row < 7; ++row) {
        const char* row_data = alphabets[index][row];
        for (int col = 0; row_data[col] != '\0'; ++col) {
            if (row_data[col] == '0') {
                float dx = x + col * size;
                float dy = y - row * size;
                draw_rect(dx, dy, half_size, half_size, color);
            }
        }
    }
}

void draw_text(const char* text, float x, float y, float size, uint color){
    float start_x = x;
    for (const char* p = text; *p != '\0'; ++p) {
        if (*p == ' ') {
            x += size * 2.0f;
        } else if (*p >= 'A' && *p <= 'Z') {
            draw_char(*p, x, y, size, color);
            x += size * 6.0f;
        }
    }
}

