#include <stdio.h>
#include <malloc.h>
#include "color16.h"
#include "lodepng.h"


#define LOG_POS(x) fprintf(stderr, "POS 0x%x\n", ftell(x))

typedef struct header_metadata
{
    char desc[17];
    char ldesc[33];
    char creator[17];

    uint16_t icon_count;
    uint16_t anim_speed;
    uint16_t eyecatch_type;
    uint16_t crc;
    uint32_t file_after;
} header_metadata;

void init_metadata (header_metadata* metadata, FILE* input, int isData);
void save_icons(FILE* input, const int icon_count);

int main(int argc, char** argv)
{
    //TODO: Options
    //TODO: Error checking
    if (argc <= 1)
    {
        printf("You have not specified a file.\n");
        printf("USAGE: vms-icon [filename]\n");
        // No such file or directory
        return 2;

    }

    FILE* input;
    input = fopen(argv[1], "rb");
    if (input == NULL)
    {
        fprintf(stderr, "Error opening %s", argv[1]);
    }

    header_metadata* meta = (header_metadata*) malloc(sizeof(header_metadata));

    init_metadata(meta, input, 0);
    save_icons(input, meta->icon_count);

    fclose(input);
    free(meta);
    return 0;
}

void init_metadata(header_metadata* metadata, FILE* input, int isData)
{
    // Data files' header starts at 0x200 rather than the begining
    if (isData)
    {
        fseek(input, 0x200, SEEK_SET);
    }
    // Read text separately because the lack of \0 in VMS files
    fread(&(metadata->desc[0]), 1, 0x10, input);
    fread(&(metadata->ldesc[0]), 1, 0x20, input);
    fread(&(metadata->creator[0]), 1, 0x10, input);
    metadata->desc[16] = 0;
    metadata->ldesc[32] = 0;
    metadata->creator[16] = 0;

    fread(&(metadata->icon_count), 1, 2, input);
    fread(&(metadata->anim_speed), 1, 2, input);
    fread(&(metadata->eyecatch_type), 1, 2, input);
    fread(&(metadata->crc), 1, 2, input);
    fread(&(metadata->file_after), 1, 4, input);

    // Passing over the 0x14 reserved empty bytes
    fseek(input, 0x14, SEEK_CUR);



}

void save_icons(FILE*  input, const int icon_count)
{

    color16 palette[16];
    fread(&palette[0], sizeof(color16), 16, input);

    LodePNGState state;
    lodepng_state_init(&state);
    // Note: get_red() and the like do not return true black
    //       but #0F0F0F
    for (int i = 0; i < 16; i++)
    {
        // printf("#%04x A: %02i R: %02i G: %02i, B: %02i\n", palette[i].color, get_alpha(palette[i]), get_red(palette[i]), get_green(palette[i]), get_blue(palette[i]));
        lodepng_palette_add(&state.info_png.color,  get_red(palette[i]), get_green(palette[i]), get_blue(palette[i]),get_alpha(palette[i]));
        lodepng_palette_add(&state.info_raw,  get_red(palette[i]), get_green(palette[i]), get_blue(palette[i]),get_alpha(palette[i]));
    }

    state.info_png.color.colortype = LCT_PALETTE;
    state.info_png.color.bitdepth = 4;
    state.info_raw.colortype = LCT_PALETTE;
    state.info_raw.bitdepth = 4;
    state.encoder.auto_convert = 0;

    for (int i =0; i < icon_count; i++)
    {
        // Turns out you can encode palette indexes the same
        // way in PNG as in the VMS files
        uint8_t img[32][32];
        fread(&img[0], 1, 0x200, input);

        unsigned char* buf;
        size_t size;
        lodepng_encode(&buf, &size, &img[0][0], 32, 32, &state);

        char filename[16];
        snprintf(&filename[0], 16, "icon%02i.png", i);
        lodepng_save_file(buf, size, filename);
    }
}




