# Changelog

## v4.11.7
- Added `missile_remove` operation.
- Added `ti_on_agent_footstep_sound_played` trigger.

## v4.11.6
- Fixed `agent_body_meta_mesh_deform` operations.
- Fixed lua `OnRglLogMsg` error handling.

## v4.11.5
- Bug Fix.
- Fixed `fmin`, `fmax`, `str_is_integer`, `get_time`, `edit_mode_set_highlighted_prop_instance`, `add_point_light`, `add_point_light_to_entity` operations.
- Fixed lua bugs.
- Refactored and fixed rcon and ogp.

## v4.11.4
- Fixed rcon bugs.

## v4.11.3
- Lua: Fix array bug, lsdir bug.

## v4.11.2
- Lua: Allow access to all modules in the storage dir, add lsdir, mkdir, rmdir.

## v4.11.1
- Lua update.
- Lua:
  - `game.addPsys`: allow integer mesh parameter
  - make error message for table check more useful
  - Some luaGuide updates
  - `game.getMeshId`, `game.getNumMeshes`
  - Add lua-std-regex lib
  - `lua_call`: allow underscore, allow table calls
  - Add `lua_test` operation
  - Make printTable nicer, detect inf loop

## v4.11.0
- Major Lua update.
- Fixed Warband bug where the combat log disappears from the screen if too much information is being written out too quickly (`fix_notification_messages` wse setting).
- Lua:
  - Update to LuaJIT 2.1
  - Update luaLanes to 3.17.0
  - Added lua debugger
  - Added lua sqlite library
  - Allow access to storage dir (WSE) in lua
  - Added `game.OnGameLoad`, `game.removeItemTrigger`, `game.getNumItemTriggers`, `game.removeScenePropTrigger`, `game.getNumScenePropTriggers`, `game.OnWorldTriggers`, `game.OnSave`, `game.OnLoadSave`.
  - Some more lua util funcs
  - Updated luaGuide

## v4.10.1
- Lua update - Support for `try_for_agents` `use_grid` parameter. `addTrigger` accepts mst index. Added `game.addPsys`, `game.removePsys`, `_G.make()`, `game.getCurTemplateNo`, `game.lGetNumTemplates`, `game.getTemplateId`, `game.getNumTriggers`. Updated luaGuide.
- `cur_item_add_mesh_with_material` operation correctly works with `use_case_insensitive_mesh_searches`.

## v4.10.0
- Lua update - Script hooking.

## v4.9.9
- Lua update - `header_operations`: Fix crash when reading very long lists, allow operation references (used for alias names).

## v4.9.8
- Lua update - global var access, scene prop triggers, few new features, more improvements to luaGuide.

## v4.9.7
- Lua update - fixed vector3, updated luaguide.

## v4.9.6
- Profiler update - before, you had to start/stop the entire game to make a recording. Now there are commands for this, and you can add markers with text. It uses a new binary format that allows viewing in timeline mode with many comfort features and also added a search bar for the list view.
- Added `profiler_start`, `profiler_stop`, `profiler_is_recording`, `profiler_mark`, `party_get_banner_icon`, `party_get_extra_icon` operations.
- Lua update - extend vector3, update missing documentation, more examples, better dark theme.

## v4.9.5
- Fixed and extended `try_for_agents` operation - additional parameter for mission grid iterator.
- Removed string parsing for `server_set_password_admin`, `server_set_password_private`, `server_set_password_rcon` operations.

## v4.9.4
- Added `ti_on_init_item` trigger for gloves.
- Extended `ti_on_init_item` trigger for boots and horse.
- `__int64` support for `store_trigger_param`, `return_values`, `store_return_value`, `register_get`, `register_set`, `str_to_num`, wse math operations.
- Updated muparser - fixed `feval` operation.

## v4.9.3
- Added `set_show_crosshair` operation.
- Added `show_crosshair`, `horse_friendly_fire` module.ini option.
- Fixed possible crash for `try_for_agents` operation.

## v4.9.2
- Fixed arguments order for `game_missile_dives_into_water` mapped script.
- Added `overlay_enable` operation.
- Extended `start_map_conversation` operation.
- Updated `dict_load_file`, `dict_save`, `dict_delete_file` operations (ini support).

## v4.9.1
- Added `local_variables_zero_initialization` module.ini option (default true).
- Fixed `overlay_button_set_type` operation.

## v4.9.0
- Added `party_stack_set_num_upgradeable` operation.
- Restored `edit_mode_in_edit_objects_mode` operation.

## v4.8.9
- Fixed lua bugs.
- Added `bypass_console_check` to `key_is_down`, `key_clicked`, `game_key_is_down`, `game_key_clicked`, `key_released`, `game_key_released` operations.

## v4.8.8
- Fixed memory leak for arrays operations.

## v4.8.7
- Added `get_campaign_time` and `set_campaign_time` operations.
- Added savegame slot to `wse_game_saved` and `wse_savegame_loaded` scripts.
- Added WSE version to xml server info.

## v4.8.6
- Updated `WSE_NETCODE_VERSION` for WSE2 compatibility.
- Fixed `wse_savegame_loaded` trigger - triggered only with savegame loaded.

## v4.8.5
- Fixed stack overflow for local variables.
- Fixed crash with enabled profiling when the recursion limit is reached.
- Max num nested loops reduced to default 128, which should reduce freezes in badly written scripts with lots of nested loops and recursion.

## v4.8.4
- Implemented SetCursor performance issues fix.

## v4.8.3
- Added `set_horse_friendly_fire` operation.

## v4.8.2
- Added `do_not_make_hands_parallel_to_ground` module.ini option.

## v4.8.1
- Fixed `try_for_players` operation.

## v4.8.0
- Added some Carribean operations - `set_random_seed`, `store_application_time`, `is_party_skill`, `agent_get_horse_rotation_velocity`, `agent_get_current_vertical_speed`, `agent_set_current_vertical_speed`, `prop_instance_remove_particle_system`, `prop_instance_remove_light`, `cur_item_mesh_set_color`, `cur_item_add_mesh_with_material`, `party_switch_stacks`, `position_get_length`, `get_dot_product_of_positions`, `get_cross_product_of_positions`, `str_store_skill_desc`, `overlay_button_set_type`, `overlay_get_scroll_pos`, `overlay_set_scroll_pos`.
- Extended `add_point_light` and `add_point_light_to_entity` operations - range parameter.
- Operations that are performed in lua now get context (trigger).
- Fixed `particle_system_remove` operation.

## v4.7.9
- Fixed buggy `face_keys_get_morph_key`, `face_keys_set_morph_key` operations - supports all morph keys, not only first 8.
- Fixed `fst` operation - supports big int.
- Removed currently unnecessary `allow_wse_execute_statement_blocks` module.ini option.

## v4.7.8
- Fixed `scene_set_*` operations.

## v4.7.7
- Added `agent_set_footstep_sound` operation.

## v4.7.6
- Restored female frame support for boots.
- Added `ti_on_init_item` trigger for boots.
- Added `ti_on_scene_prop_is_deforming` trigger.
- Added `itp_covers_hands` item flag.
- Added all scene prop triggers support for `add_point_light`, `particle_system_add_new`, `particle_system_remove`, `set_forced_lod` operations.

## v4.7.5
- Added `acf_lock_rotation` animation flag.

## v4.7.4
- Added `agent_body_meta_mesh_deform_in_range`, `agent_body_meta_mesh_deform_in_cycle_loop`, `agent_body_meta_mesh_get_current_deform_progress`, `agent_body_meta_mesh_get_current_deform_frame` operations.

## v4.7.3
- Extended `agent_set_animation_progress` operation.

## v4.7.2
- Added `dict_delete_file_json` operation.
- Array bugfixes.

## v4.7.1
- Restored `get_camera_position` operation.
- Extended `agent_accelerate` operation.
- Fixed `item_set_horse_blood_particles` operation.
- `agent_get_item_cur_ammo` and `agent_get_ammo_for_slot` operations takes any item slot.
- Removed admin privilege from `WSELoader.exe`
- Lua:
  - Added stack traces for error messages
  - Clear lua stack when it reaches 100 to prevent stack overflows
  - Bug fixes for hex constant parser, presentation triggers, `os.rename`
  - Return values in module triggers now optional
  - Added lua callback for chat messages (`game.OnChatMessageReceived`)
  - Few additions to vector3 and `game.pos`
  - Read luaGuide for details

## v4.7.0
- Ported to 1.174.

## v4.6.1
- Added operation `agent_add_stun`.
- Added `game.fail` which will trigger a ms op fail in `lua_call`.
- Added ini settings option `disable_game_const_warnings` that mutes warnings during msfiles folder scanning.

## v4.6.0
- Ported to 1.173.

## v4.5.6
- Fixed and restored operations `break_loop`, `continue_loop`, `try_for_dict_keys`. (requires `allow_wse_execute_statement_blocks = 1` in module.ini)
- Lua iterator `game.propInstI` now work as exactly like `try_for_prop_instances`.
- Response for http request for server info now sends correct headers and opens on any browsers. (`http://server_ip:server_port/`)
- Added script `wse_get_server_info`.
- Bug fixes.

## v4.5.5
- Added item trigger for lua.
- Skins use their own blood particles.
- Added operation `item_set_horse_blood_particles`.

## v4.5.4
- More skins support for multiplayer profile. Required `network_compatible = 0`.
- Limit for multiplayer gold increased to 1000000000. Required `network_compatible = 0`.
- Added operation `multiplayer_cur_profile_get_skin`.

## v4.5.3
- Restored function of profiler.
- Fixed precision loss for `agent_get_animation_progress`, `agent_get_scale`, `fst`, `get_water_level`, `party_stack_get_experience`, `position_get_vector_to_position`, `overlay_get_val` operations. And for triggers: max rotation speed for `ti_on_agent_turn` and raw damage for `ti_on_agent_hit`.

## v4.5.2
- Fixed precision loss for `agent_get_..._modifier` operations.
- Added operation `agent_get_ranged_damage_modifier`.

## v4.5.1
- Fixed lua `game.call_script` not working for some reason.
- Added operation `dict_erase`.

## v4.5.0
- Lua scripting support.
- Added operation `send_post_message_to_url_advanced`.
- Fixed operation `dict_from_url_encoded_json`.
- Operation `dict_to_url_encoded_json` now created json is writen in a single line.

## v4.4.4
- Added script `wse_savegame_loaded`.

## v4.4.3
- Added `dict_from_url_encoded_json`, `dict_to_url_encoded_json` and `str_decode_url` operations.

## v4.4.2
- Added `dict_load_file_json` and `dict_save_json` operations.

## v4.4.1
- Fixed crash with `AgentAttackCollidesWithPropHook`.

## v4.4.0
- Ported to 1.170.

## v4.3.7
- Repaired and enabled operation `set_prop_collision_threshold`.
- Implemented Signature Scanner. Technical tool to ease porting.

## v4.3.6
- Fixed bug: `ogp_server` crashes with long map filenames.
- Removed limit for `troop_set_attribute`.

## v4.3.5
- Added trigger: `ti_on_agent_turn`.

## v4.3.4
- Added `fadd` operation.
- Re-fixed dropped items with high ids not being displayed correctly on clients (bug [#3157](http://bugs.taleworlds.com/edit_bug.aspx?id=3157), fix available only in non network compatible mode)

## v4.3.3
- Fixed array bugs.
- Added operations: `array_eq`, `array_neq`, `array_gt`, `array_ge`, `array_lt`, `array_le`.

## v4.3.2
- New feature: magic dynamic n-dimensional typeless arrays.
- Added operations: `player_temp_ban`, `dict_get_pos`, `dict_set_pos`, `str_regex` operations, array operations.

## v4.3.1
- Agent's personal animation feature.
- Added operations: `agent_set_personal_animation`, `agent_get_personal_animation`, `agent_set_default_animations`, `agent_cancel_current_animation`.

## v4.3.0
- Enabled trigger `ti_on_agent_end_reloading`.
- Fixed trigger `ti_on_agent_start_reloading` for client.
- Fixed flag `itp_shield_no_parry`, left-handed item no more block couched lance.
- Fixed flag `itp_disable_agent_sounds`, horses no more play `snd_horse_snort`.

## v4.2.9
- Added trigger: `ti_on_agent_blocked`.
- Added item flag: `itp_shield_no_parry`.
- Added operation: `str_store_item_mesh_name`.

## v4.2.8
- Added new operation: `agent_body_meta_mesh_set_visibility`.

## v4.2.7
- Added scene prop flag `sokf_weapon_knock_back_collision`.
- Removed operations `camera_in_first_person` and `set_camera_in_first_person`, because already implemented ingame.

## v4.2.6
- Added module.ini option: `fix_bots_blocking_for_dedicated_server` ([fix bots blocking issue on dedicated manual block servers](http://bugs.taleworlds.com/edit_bug.aspx?id=3305)).
- Added new operation: `make_screenshot`.

## v4.2.5
- New feature: set difficulty requirement for item types (attributes and skills, check `module.ini_addon.ini`).

## v4.2.4
- Added new operation: `agent_body_meta_mesh_set_vertex_keys_time_point`.
- Enabled `item_set_` operations.

## v4.2.3
- Fixed `tactical_window` gamedata and operations `get_spectated_agent_no`, `order_flag_is_active`, `presentation_activate`.
- OGP server sends more info: BotCount, PlayerID (unique_id), PlayerClass (troop).

## v4.2.2
- Source RCON Protocol implementation for dedicated server.
- Added new operations: `server_set_password_rcon`, `str_store_server_password_rcon`, `execute_server_console_command`.

## v4.2.1
- Enabled network features: `hide_other_mod_servers`, `network_compatible`, more skins in multiplayer, composite network messages; `player_set_skin`, `player_stop_controlling_agent`, `horse_friendly_fire` and `show_crosshair` operations.
- Added new operation: `str_store_multiplayer_profile_face_keys`.
- Added more skins support for OGP Server.

## v4.2.0
- Ported to 1.168

## v4.1.0
- Added module.ini option: `module_download_url`.
- Added "missile no" parameter for `ti_on_init_missile` and `ti_on_missile_dive` triggers and `game_missile_dives_into_water` script.
- Added new operations: `missile_is_valid`, `missile_get_cur_position`.
- Operation `cur_missile_get_path_point_position` replaced with `missile_get_path_point_position`.

## v4.0.9
- Enabled operation `update_material`.
- Disabled `AgentAttackCollidesWithPropHook`, which caused a crash on some systems (and the associated operation `set_prop_collision_threshold`).
- Operations `agent_get_item_slot_ammo` and `get_camera_position` removed, because already implemented ingame (`agent_get_ammo_for_slot` and `mission_cam_get_position`).

## v4.0.8
- Repaired `play_bink_file` operation.
- Fixed party's gamedata (also wse party's operations).
- Added `set_main_party` and `get_main_party` operations. Important! `set_main_party` with dynamic spawned party (not listed in `module_parties.py`) will corrupt the savegame!

## v4.0.7
- Disabled unstable `OperationManagerExecuteHook`, that was causing server crashes. So operations `go_to`, `break_loop`, `continue_loop`, extended `try_for_agents` and `try_for_dict_keys` no longer work.
- For the replacement operation `try_for_dict_keys` added operation `dict_get_key_by_iterator`.
- Fixed operation `player_set_username`.
- Added WSELoader with Windows XP compatible.

## v4.0.6
- Operations `prop_instance_clear_attached_missiles` and `edit_mode_in_edit_objects_mode` removed, because already implemented ingame.
- Working module.ini option: `use_missile_damage_type`.
- Added new `wse_setting.ini` option to specify a port for listen server.
- Working `set_show_use_tooltip` operation.
- Enabled scripts: `wse_get_agent_scale`, extended `game_missile_dives_into_water`.
- Enabled triggers: `ti_on_scene_prop_stepped_on`, `ti_on_agent_start_reloading`, `ti_on_missile_dive`, extended `ti_on_agent_hit`.
- Enabled item flags: `itp_offset_mortschlag`, `itp_offset_flip`.
- Enabled operations: `set_ally_collision_threshold`, `set_prop_collision_threshold`.

## v4.0.5
- Fixed structure `mission_object`.
- Working module.ini option: `use_missile_damage_type`.
- Enabled operations: `send_message_to_url_advanced`, `process_advanced_url_messages`, `multiplayer_send_chat_message_to_player`, extended `prop_instance_receive_damage`, `add_point_light`, `particle_system_add_new`, `mission_cam_set_aperture`.
- Enabled triggers: `ti_on_init_missile`, `ti_on_init_item` for horses.

## v4.0.4
- Fixed range for StringOperations.
- Added scripts: `wse_console_command_received`, `wse_chat_message_received`, `wse_game_saved`, `wse_window_opened`.
- Working option: `max_corpses`.
- Working feature: `ogp_server`.
- Fixed bugs: `ogp_server` crashes with long data strings and not show maps filename.

## v4.0.3
- Structure of data is completed. Found most of offsets for hooks.
- Enabled operations: InputOperations, MenuOperations, PresentationOperations, some MultiplayerOperations, MissionOperations, CoreOperations and StringOperations.

## v4.0.2
- Fixed operation `try_for_prop_instances`.
- Added new operation: `shell_open_url`.

## v4.0.1
- Refixed `OperationExecuteHook` - repair CF operators.
- Working module.ini option: `ground_weapon_collision`.
- Enabled most operations: CoreOperations, AgentOperations, EditModeOperations, FloatingPointOperations, MathOperations, NetworkOperations, OptionsOperations, PartyOperations, PlayerOperations, PositionOperations, SiteOperations, StringOperations, TroopOperations.
- Added new operation: `player_set_username`.

## v4.0.0
- Start porting wse to 1166.
