#ifndef _SD_STRINGPARSER_H
#define _SD_STRINGPARSER_H

/* Tags for callbacks */
#define SD_CB_MUSIC "smo"
#define SD_CB_SOUND "s"

typedef void* sd_stringparser_tag_list;
typedef void* sd_stringparser_frame_list;

typedef struct sd_stringparser_tag_info_t {
    const char *tag;
    int has_param;
    char *description;
} sd_stringparser_tag_info;

typedef struct sd_stringparser_cb_param_t {
    const sd_stringparser_tag_info *tag_info;
    
    /* is_done may be set to 1 to mark this frame as complete */ 
    int is_done;
    
    /* The current tick */
    const int tick;
    
    const int duration;
    const char frame;
    
    /* The userdata pointer that was passed to sd_stringparser_set_default_cb/sd_stringparser_set_cb */
    void *userdata;
    
    const int tag_value;
} sd_stringparser_cb_param;

typedef void(*sd_stringparser_cb_t)(sd_stringparser_cb_param *info);

enum {
    SD_BLEND_ADDITIVE = 0,
    SD_BLEND_ALPHA
};

typedef struct sd_stringparser_t {
    char *string;
    sd_stringparser_tag_list tag_list;
    sd_stringparser_frame_list frame_list;

    int blendmode;
    int flip_horizontal;
    int flip_vertical;
} sd_stringparser;

sd_stringparser* sd_stringparser_create();
void sd_stringparser_delete(sd_stringparser *parser);

/* Parses the string and construct an animation list internally, may return error */
int sd_stringparser_set_string(sd_stringparser *parser, const char *string);

/* Set a callback, the userdata pointer is passed to the callback */
void sd_stringparser_set_cb(sd_stringparser *parser, const char *tag, sd_stringparser_cb_t callback, void *userdata);

/* Set a default callback to handle every other tags */
void sd_stringparser_set_default_cb(sd_stringparser *parser, sd_stringparser_cb_t callback, void *userdata);

/* Reset the animation to the first frame */
void sd_stringparser_reset(sd_stringparser *parser);

/* Run the animation at "ticks", may return error */
int sd_stringparser_run(sd_stringparser *parser, unsigned int ticks);

int sd_stringparser_prettyprint_frame(sd_stringparser *parser, unsigned int frame);

int sd_stringparser_prettyprint(sd_stringparser *parser);


#endif // _SD_STRINGPARSER_H
