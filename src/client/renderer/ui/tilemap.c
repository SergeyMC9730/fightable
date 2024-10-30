#include <fightable/tilemap.h>
#include <fightable/intvec_math.h>

void _fTilemapDraw(struct ftilemap *tilemap, IVector2 render_pos, IVector2 tile_pos, unsigned char fliped_x, unsigned char fliped_y, Color tint) {
    Rectangle r;

    r.width = tilemap->tile_size.x;
    r.height = tilemap->tile_size.y;
    r.x = tile_pos.x * r.width;
    r.y = tile_pos.y * r.height;

    if (fliped_x) {
        r.width *= -1.f;
    }
    if (fliped_y) {
        r.height *= -1.f;
    }

    DrawTextureRec(tilemap->texture, r, _fImathToVFloat(render_pos), tint);
}
void _fTilemapDrawMegatile(struct ftilemap *tilemap, IVector2 render_pos, IVector2 start_pos, IVector2 tiles, unsigned char fliped_x, unsigned char fliped_y, Color tint) {
    for (int x = 0; x < tiles.x; x++) {
        for (int y = 0; y < tiles.y; y++) {
            int _x = x;
            int _y = y;

            if (fliped_x) {
                _x = tiles.x - x;
            }
            if (fliped_y) {
                _y = tiles.y - y;
            }

            IVector2 rpos = {render_pos.x + (x * tilemap->tile_size.x), render_pos.y + (y * tilemap->tile_size.y)};
            IVector2 tpos = {start_pos.x + _x, start_pos.y + _y};

            _fTilemapDraw(tilemap, rpos, tpos, fliped_x, fliped_y, tint);
        }
    }
}

struct ftilemap _fTilemapCreate(const char *path, IVector2 tile_size) {
    struct ftilemap m = {};

    m.tile_size = tile_size;
    m.texture = LoadTexture(path);

    return m;
}
void _fTilemapUnload(struct ftilemap *tilemap) {
    UnloadTexture(tilemap->texture);
}

Texture2D _fTilemapExportTile(struct ftilemap *tilemap, IVector2 tile_pos) {
    Image tile = _fTilemapExportTileAsImage(tilemap, tile_pos);
    Texture2D tile_texture = LoadTextureFromImage(tile);

    UnloadImage(tile);

    return tile_texture;
}

Rectangle _fTilemapGetTileRect(struct ftilemap *tilemap, IVector2 tile_pos) {
    return (Rectangle){tile_pos.x * tilemap->tile_size.x, tile_pos.y * tilemap->tile_size.y, tilemap->tile_size.x, tilemap->tile_size.y};
}

Image _fTilemapExportTileAsImage(struct ftilemap *tilemap, IVector2 tile_pos) {
    Rectangle r = _fTilemapGetTileRect(tilemap, tile_pos);

    Image mapimg = LoadImageFromTexture(tilemap->texture);
    ImageCrop(&mapimg, r);
    mapimg.width = r.width; mapimg.height = r.height;
    
    return mapimg;
}

#include <cJSON.h>

struct cJSON *_fTilemapCreateJson(struct ftilemap *tilemap) {
    int v[2] = {tilemap->tile_size.x, tilemap->tile_size.y};

    cJSON *j = cJSON_CreateObject();
    cJSON *j1 = cJSON_AddArrayToObject(j, "tile_size");

    cJSON_InsertItemInArray(j1, 0, cJSON_CreateNumber(v[0]));
    cJSON_InsertItemInArray(j1, 1, cJSON_CreateNumber(v[1]));

    return j;
}
void _fTilemapFromJson(struct ftilemap *tilemap, struct cJSON *j) {
    if (!j || !cJSON_IsObject(j)) return;

    cJSON *array = cJSON_GetObjectItem(j, "tile_size");
    if (!array || !cJSON_IsArray(array)) return;

    int array_size = cJSON_GetArraySize(array);
    if (array_size < 2) return;

    cJSON *v = cJSON_GetArrayItem(array, 0);
    tilemap->tile_size.x = v->valueint;

    v = cJSON_GetArrayItem(array, 10);
    tilemap->tile_size.y = v->valueint;
}