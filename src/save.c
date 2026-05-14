#include "simple_logger.h"
#include "simple_json.h"

#include "gfc_text.h"

#include "save.h"

#define SAVE_FILE "save/save.json"

SaveData save_data_default()
{
	SaveData data = { 0 };
	data.rings = 0;
	data.current_level = 0;
	gfc_line_sprintf(data.current_level_path, "maps/testworld.json");
	return data;
}

void save_data_write(SaveData* data)
{
	SJson* root = sj_object_new();
	if (!root)
	{
		slog("save_dta_write: failed to create json object");
		return;
	}
	sj_object_insert(root, "rings", sj_new_int(data->rings));
	sj_object_insert(root, "current_level", sj_new_int(data->current_level));
	sj_object_insert(root, "current_level_path", sj_new_str(data->current_level_path));

	sj_save(root, SAVE_FILE);
	sj_free(root);
	slog("save_data_write: saved rings=%d level = %d", data->rings, data->current_level);
}

SaveData save_data_read()
{
	SaveData data = save_data_default();

	SJson* root = sj_load(SAVE_FILE);
	if (!root)
	{
		slog("save_data_read: no save file found, returning defaults");
		return data;
	}

	sj_get_integer_value(sj_object_get_value(root, "rings"), &data.rings);
	sj_get_integer_value(sj_object_get_value(root, "current_level"), &data.current_level);

	const char* path = sj_get_string_value(sj_object_get_value(root, "current_level_path"));
	if (path) gfc_line_sprintf(data.current_level_path, "%s", path);

	sj_free(root);
	return data;
}