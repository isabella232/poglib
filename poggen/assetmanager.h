#pragma once
#include "../ds/list.h"
#include "../ds/hashtable.h"
#include "../poggen/asset.h"



/*===========================================================
                -- ASSET MANAGER --
===========================================================*/



typedef struct assetmanager_t assetmanager_t ;

assetmanager_t  assetmanager_init(void);

#define         assetmanager_add_shader(PASSETMANAGER, LABEL, VERTEXFILE, FRAGMENTFILE)    __impl_assetmanager_add_asset((PASSETMANAGER), str(LABEL), (VERTEXFILE), (FRAGMENTFILE), AT_GLSHADER) 
#define         assetmanager_add_texture2d(PASSETMANAGER, LABEL, FILEPATH)                 __impl_assetmanager_add_asset((PASSETMANAGER), str(LABEL), (FILEPATH), NULL, AT_GLTEXTURE2D) 
#define         assetmanager_add_freetypefont(PASSETMANAGER, LABEL, FILEPATH)                 __impl_assetmanager_add_asset((PASSETMANAGER), str(LABEL), (FILEPATH), NULL, AT_FONT_TTF) 

#define         assetmanager_get_shader(PASSETMANAGER, LABEL)       __impl_assetmanager_get_asset((PASSETMANAGER), str(LABEL), AT_GLSHADER)
#define         assetmanager_get_texture2d(PASSETMANAGER, LABEL)    __impl_assetmanager_get_asset((PASSETMANAGER), str(LABEL), AT_GLTEXTURE2D)
#define         assetmanager_get_freetypefont(PASSETMANAGER, LABEL)    __impl_assetmanager_get_asset((PASSETMANAGER), str(LABEL), AT_FONT_TTF)

void            assetmanager_destroy(assetmanager_t *self);





#ifndef IGNORE_ASSETMANAGER_IMPLEMENTATION


struct assetmanager_t {

    list_t assetlabels;
    list_t assets;

};

assetmanager_t assetmanager_init(void)
{
    assetmanager_t o = {
        .assetlabels = list_init(8, str_t ),
        .assets = list_init(AT_COUNT, hashtable_t )
    };

    for (u32 i = 0; i < AT_COUNT; i++)
    {
        hashtable_t *elem = (hashtable_t *)list_get_element_by_index(&o.assets, i);
        switch(i)
        {
            case AT_GLSHADER: {
                hashtable_t tmp = hashtable_init(10, glshader_t );
                memcpy(elem, &tmp, sizeof(tmp));
            } break;

            case AT_GLTEXTURE2D: {
                hashtable_t tmp = hashtable_init(10, gltexture2d_t );
                memcpy(elem, &tmp, sizeof(tmp));
            } break;

            case AT_SOUND_WAV:
            //TODO: sound
            break;

            case AT_FONT_FreeType: {
                hashtable_t tmp = hashtable_init(4, glfreetypefont_t );
                memcpy(elem, &tmp, sizeof(tmp));
            } break;

            default: eprint("type not accounted for");
        }
    }

    return o;
}

asset_t * __impl_assetmanager_add_asset(assetmanager_t *manager, str_t label, const char *filepath01, const char *filepath02, asset_type type) 
{
    assert(filepath01);
    assert(manager);

    asset_t output = {
        .label = label,
        .filepath01 = filepath01,
        .filepath02 = filepath02,
        .type = type
    };

    switch(type)
    {
        case AT_GLSHADER:
            assert(filepath02);
            output.shader = glshader_from_file_init(filepath01, filepath02);
        break;

        case AT_GLTEXTURE2D:
            output.texture2d = gltexture2d_init(filepath01);
        break;

        case AT_FONT_FreeType:
            output.font = glfreetypefont_init(filepath01, 45);
        break;

        case AT_SOUND_WAV:
            eprint("TODO : asset type");
        break;

        default: eprint("type not accounted for");
    }

    list_append(&manager->assetlabels, label);

    hashtable_t *table = (hashtable_t *)list_get_element_by_index(&manager->assets, type);
    assert(table);
    
    return (asset_t *)hashtable_insert(table, label.buf, output);

}


void assetmanager_destroy(assetmanager_t *self) 
{
    assert(self);

    for (u32 i = 0; i < AT_COUNT; i++)
    {
        hashtable_t *table = (hashtable_t *)list_get_element_by_index(&self->assets, i);
        assert(table);

        switch(i)
        {
            case AT_GLSHADER:
                for (u32 i = 0; i < table->len; i++)
                {
                    const char *value = (const char *)list_get_element_by_index(&self->assetlabels, i);
                    assert(value);

                    glshader_destroy((glshader_t *)hashtable_get_value_by_key(table, value));
                }
            break;

            case AT_GLTEXTURE2D:
                for (u32 i = 0; i < table->len; i++)
                {
                    const char *value = (const char *)list_get_element_by_index(&self->assetlabels, i);
                    assert(value);

                    gltexture2d_destroy((gltexture2d_t *)hashtable_get_value_by_key(table, value));
                }
            break;

            case AT_FONT_FreeType:
                for (u32 i = 0; i < table->len; i++)
                {
                    const char *value = (const char *)list_get_element_by_index(&self->assetlabels, i);
                    assert(value);

                    glfreetypefont_destroy((glfreetypefont_t *)hashtable_get_value_by_key(table, value));
                }
            break;

            case AT_SOUND_WAV:
                eprint("TODO : ");
            break;

            default: eprint("type not accounted for ");
        }


        hashtable_destroy(table);
        list_delete(&self->assets, i);
    }

    list_destroy(&self->assetlabels);
    list_destroy(&self->assets);
}


asset_t * __impl_assetmanager_get_asset(assetmanager_t *manager, str_t label, asset_type type)
{
    assert(manager);

    hashtable_t *table = (hashtable_t *)list_get_element_by_index(&manager->assets, type);
    assert(table);

    return (asset_t *)hashtable_get_value_by_key(table, label.buf);
}

#endif
