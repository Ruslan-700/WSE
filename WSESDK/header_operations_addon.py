#Add the following definitions to the end (!) of header_operations.py
go_to                        = 0 #(go_to, <statement_no>), #Jump to <statement_no>
break_loop                   = 8 #(break_loop), #Break out of a loop, no matter how deeply nested in try_begin blocks
continue_loop                = 9 #(continue_loop), #Continue to the next iteration of a loop, no matter how deeply nested in try_begin blocks
try_for_agents               = 12 #(try_for_agents, <cur_agent_no>, [<position_no>], [<radius_fixed_point>]), #Loops through agents in the scene. If [<position_no>] and [<radius_fixed_point>] are defined, it will only loop through agents in the chosen area
try_for_attached_parties     = 15 #(try_for_attached_parties, <cur_party_no>, <party_no>), #Loops through parties attached to <party_no>.
try_for_active_players       = 16 #(try_for_active_players, <cur_player_no>, [<skip_server>]), #Loops through all active players. If [<skip_server>] is set to non-zero, the server player (0) will be skipped
try_for_prop_instances       = 17 #(try_for_prop_instances, <cur_instance_no>, [<scene_prop_kind_no>]), #Loops through scene prop instances of [<scene_prop_kind_no>]. If [<scene_prop_kind_no>] is not set it will loop through all scene prop instances in the scene
try_for_dict_keys            = 18 #(try_for_dict_keys, <cur_key_string_register>, <dict>), #Loops through keys of <2>
server_set_max_num_players   = 491 #(server_set_max_num_players, <max_players>, [<max_private_players>]), #Sets maximum players to <max_players> and maximum private players to [<max_private_players>] (default = same as <max_players>). Both values must be in the range 2-250, [<max_private_players>] can't be lower than <max_players>
position_rotate_x            = 723 #(position_rotate_x, <position_register>, <angle>, [<use_global_axis>]), #Rotates <position_register> around the x-axis by <angle> degrees
position_rotate_y            = 724 #(position_rotate_y, <position_register>, <angle>, [<use_global_axis>]), #Rotates <position_register> around the y-axis by <angle> degrees
position_rotate_z            = 725 #(position_rotate_z, <position_register>, <angle>, [<use_global_axis>]), #Rotates <position_register> around the z-axis by <angle> degrees
position_rotate_x_floating   = 738 #(position_rotate_x_floating, <position_register>, <angle_fixed_point>, [<use_global_axis>]), #Rotates <position_register> around the x-axis by <angle_fixed_point> degrees
position_rotate_y_floating   = 739 #(position_rotate_y_floating, <position_register>, <angle_fixed_point>, [<use_global_axis>]), #Rotates <position_register> around the y-axis by <angle_fixed_point> degrees
is_vanilla_warband           = 1004 #(is_vanilla_warband), #Fails only when WSE is running
prop_instance_receive_damage = 1877 #(prop_instance_receive_damage, <prop_instance_no>, <agent_no>, <damage>, [<advanced>]), #<prop_instance_no> received <damage> damage from <agent_no>. If [<advanced>] is non-zero ti_on_scene_prop_hit will be called and the damage dealt will be sent to clients.
store_trigger_param          = 2070 #(store_trigger_param, <destination>, [<trigger_param_no>]), #Stores [<trigger_param_no>] into <destination>

str_equals                                = 2600 #(str_equals, <string_1>, <string_2>, [<case_insensitive>]), #Fails if <string_1> is not equal to <string_2>
str_contains                              = 2601 #(str_contains, <string_1>, <string_2>, [<case_insensitive>]), #Fails if <string_1> doesn't contain <string_2>
str_starts_with                           = 2602 #(str_starts_with, <string_1>, <string_2>, [<case_insensitive>]), #Fails if <string_1> doesn't start with <string_2>
str_ends_with                             = 2603 #(str_ends_with, <string_1>, <string_2>, [<case_insensitive>]), #Fails if <string_1> doesn't end with <string_2>
str_is_alpha                              = 2604 #(str_is_alpha, <string_1>, [<index>]), #Fails if character [<index>] of <string_1> isn't alphabetic. If [<index>] is not defined or is -1, the entire string is checked
str_is_digit                              = 2605 #(str_is_digit, <string_1>, [<index>]), #Fails if character [<index>] of <string_1> isn't a digit. If [<index>] is not defined or is -1, the entire string is checked
str_is_whitespace                         = 2606 #(str_is_whitespace, <string_1>, [<index>]), #Fails if character [<index>] of <string_1> isn't whitespace. If [<index>] is not defined or is -1, the entire string is checked
str_length                                = 2607 #(str_length, <destination>, <string_1>), #Stores the length of <string_1> into <destination>
str_index_of                              = 2608 #(str_index_of, <destination>, <string_1>, <string_2>, [<start>], [<end>]), #Stores the index of the first occurrence of <string_2> in <string_1> into <destination>. Search bounds can be specified with [<start>] and [<end>]
str_last_index_of                         = 2609 #(str_last_index_of, <destination>, <string_1>, <string_2>, [<start>], [<end>]), #Stores the index of the last occurrence of <string_2> in <string_1> into <destination>. Search bounds can be specified with [<start>] and [<end>]
str_get_char                              = 2610 #(str_get_char, <destination>, <string_1>, [<index>]), #Stores the numeric value of the [<index>]th character in <string_1> into <destination>
str_to_num                                = 2611 #(str_to_num, <destination_fixed_point>, <string_1>, [<use_fixed_point_multiplier>]), #Stores the numeric value of <string_1> into <destination_fixed_point>. Decimal values will be rounded to integers, for more precision set [<use_fixed_point_multiplier>] to non-zero
str_compare                               = 2612 #(str_compare, <destination>, <string_1>, <string_2>, [<case_insensitive>]), #Stores the relationship between <string_1> and <string_2> into <destination> (-1: s1 < s2, 0: s1 = s2, 1: s1 > s2)
str_split                                 = 2613 #(str_split, <destination>, <string_register>, <string_1>, <delimiter>, [<skip_empty>], [<max>]), #Splits <string_1> using <delimiter> into a range of string registers, starting from <string_register>, storing [<max>] substrings at most (default = unlimited), ignoring empty (zero length) substrings if [<skip_empty>] (default = false). Stores the amount of substrings split into <destination>
str_sort                                  = 2614 #(str_sort, <string_register>, [<count>], [<case_insensitive>], [<descending>]), #Sorts a range of [<count>] string registers starting from <string_register>
str_store_lower                           = 2615 #(str_store_lower, <string_register>, <string_1>), #Stores the lowercase version of <string_1> into <string_register>
str_store_upper                           = 2616 #(str_store_upper, <string_register>, <string_1>), #Stores the uppercase version of <string_1> into <string_register>
str_store_trim                            = 2617 #(str_store_trim, <string_register>, <string_1>, [<trim_mode>]), #Stores the whitespace trimmed version of <string_1> into <string_register>. [<trim_mode>]: 0 (default) = trim leading and trailing, 1 = trim leading, 2 = trim trailing
str_store_replace                         = 2618 #(str_store_replace, <string_register>, <string_1>, <string_2>, <string_3>), #Stores <string_1> into <string_register>, replacing occurrences of <string_2> with <string_3>
str_store_md5                             = 2619 #(str_store_md5, <string_register>, <string_1>), #MD5 encrypts <string_1> and stores it into <string_register>
str_store_substring                       = 2620 #(str_store_substring, <string_register>, <string_1>, [<start>], [<length>]), #Stores a substring of <string_1> into <string_register>, starting from [<start>]. If [<length>] is not specified, everything on the right of <start> will be used
str_store_reverse                         = 2621 #(str_store_reverse, <string_register>, <string_1>), #Stores the reverse of <string_register> into <string_1>
str_store_join                            = 2622 #(str_store_join, <string_register>, <start_string_register>, <count>, [<delimiter>]), #Joins <count> string registers starting from string register <start_string_register>, using [<delimiter>] (default = empty string) and stores them into <string_register>
str_store_replace_spaces_with_underscores = 2623 #(str_store_replace_spaces_with_underscores, <string_register>, <string_1>), #Stores <string_1> into <string_register>, replacing all spaces with underscores
str_store_replace_underscores_with_spaces = 2624 #(str_store_replace_underscores_with_spaces, <string_register>, <string_1>), #Stores <string_1> into <string_register>, replacing all underscores with spaces
str_store_multiplayer_profile_name        = 2625 #(str_store_multiplayer_profile_name, <string_register>, <profile_no>), #Stores <profile_no>'s name into <string_register>
str_store_face_keys                       = 2626 #(str_store_face_keys, <string_register>, <face_key_register>), #Stores the string representation of <face_key_register> into <string_register>
str_store_module_setting                  = 2627 #(str_store_module_setting, <string_register>, <setting>), #Stores the string value (empty if not found) of <setting> in module.ini into <string_register>
str_store_server_password_admin           = 2628 #(str_store_server_password_admin, <string_register>), #Stores the server administrator password into <string_register>
str_store_server_password_private         = 2629 #(str_store_server_password_private, <string_register>), #Stores the server private player password into <string_register>
str_store_overlay_text                    = 2630 #(str_store_overlay_text, <string_register>, <overlay_no>), #Stores <overlay_no>'s text into <string_register>
str_store_player_ip                       = 2631 #(str_store_player_ip, <string_register>, <player_no>), #Stores <player_no>'s IP address into <string_register> (works only on servers)
str_store_game_variable                   = 2632 #(str_store_game_variable, <string_register>, <variable>), #Stores the string value (empty if not found) of <variable> in game_variables.txt into <string_register>
str_store_skill_name                      = 2633 #(str_store_skill_name, <string_register>, <skill_no>), #Stores the name of <skill_no> into <string_register>
str_store_float                           = 2634 #(str_store_float, <string_register>, <fp_register>, [<precision>]), #Stores the string representation of <fp_register> into <string_register> showing [<precision>] decimal digits at most
str_sanitize                              = 2635 #(str_sanitize, <string_register>), #Removes invalid characters from <string_register>
str_store_item_id                         = 2636 #(str_store_item_id, <string_register>, <item_no>), #Stores the id of <item_no> into <string_register>
str_is_integer                            = 2637 #(str_is_integer, <string_1>), #Fails if <string_1> isn't a valid integer

face_keys_init             = 2700 #(face_keys_init, <face_key_register>), #Zeroes <face_key_register>
face_keys_copy             = 2701 #(face_keys_copy, <face_key_register_1>, <face_key_register_2>), #Copies <face_key_register_2> to <face_key_register_1>
face_keys_get_hair         = 2702 #(face_keys_get_hair, <destination>, <face_key_register>), #Stores <face_key_register>'s hair value into <destination>
face_keys_set_hair         = 2703 #(face_keys_set_hair, <face_key_register>, <hair_no>), #Sets <face_key_register>'s hair value
face_keys_get_beard        = 2704 #(face_keys_get_beard, <destination>, <face_key_register>), #Stores <face_key_register>'s beard value into <destination>
face_keys_set_beard        = 2705 #(face_keys_set_beard, <face_key_register>, <beard_no>), #Sets <face_key_register>'s beard value
face_keys_get_face_texture = 2706 #(face_keys_get_face_texture, <destination>, <face_key_register>), #Stores <face_key_register>'s face texture value into <destination>
face_keys_set_face_texture = 2707 #(face_keys_set_face_texture, <face_key_register>, <face_texture_no>), #Sets <face_key_register>'s face texture value
face_keys_get_hair_texture = 2708 #(face_keys_get_hair_texture, <destination>, <face_key_register>), #Stores <face_key_register>'s hair texture value into <destination>
face_keys_set_hair_texture = 2709 #(face_keys_set_hair_texture, <face_key_register>, <hair_texture_no>), #Sets <face_key_register>'s hair texture value
face_keys_get_hair_color   = 2710 #(face_keys_get_hair_color, <destination>, <face_key_register>), #Stores <face_key_register>'s hair color value (in %%) into <destination>
face_keys_set_hair_color   = 2711 #(face_keys_set_hair_color, <face_key_register>, <hair_color>), #Sets <face_key_register>'s hair color value (in %%)
face_keys_get_age          = 2712 #(face_keys_get_age, <destination>, <face_key_register>), #Stores <face_key_register>'s age value (in %%) into <destination>
face_keys_set_age          = 2713 #(face_keys_set_age, <face_key_register>, <age>), #Sets <face_key_register>'s age value (in %%)
face_keys_get_morph_key    = 2714 #(face_keys_get_morph_key, <destination>, <face_key_register>, <key_no>), #Stores <face_key_register>'s morph key value (0-7) into <destination>
face_keys_set_morph_key    = 2715 #(face_keys_set_morph_key, <face_key_register>, <key_no>, <value>), #Sets <face_key_register>'s morph key value (0-7) to <value>
face_keys_store_string     = 2716 #(face_keys_store_string, <face_key_register>, <string>), #Initializes <face_key_register> using <string>

val_shr    = 2800 #(val_shr, <value>, <shift>), #Performs an arithmetic right bit shift by <shift> on <value>
store_shr  = 2801 #(store_shr, <destination>, <value>, <shift>), #Performs an arithmetic right bit shift by <shift> on <value> and stores the result into <destination>
val_lshr   = 2802 #(val_lshr, <value>, <shift>), #Performs a logical right bit shift by <shift> on <value>
store_lshr = 2803 #(store_lshr, <destination>, <value>, <shift>), #Performs a logical right bit shift by <shift> on <value> and stores the result into <destination>
val_shl    = 2804 #(val_shl, <value>, <shift>), #Performs a left bit shift by <shift> on <value>
store_shl  = 2805 #(store_shl, <destination>, <value>, <shift>), #Performs a left bit shift by <shift> on <value> and stores the result into <destination>
val_xor    = 2806 #(val_xor, <value1>, <value2>), #Performs a bitwise exclusive or between <value1> and <value2>
store_xor  = 2807 #(store_xor, <destination>, <value1>, <value2>), #Performs a bitwise exclusive or between <value1> and <value2> and stores the result into <destination>
val_not    = 2808 #(val_not, <value>), #Performs a bitwise complement on <value>
store_not  = 2809 #(store_not, <destination>, <value>), #Performs a bitwise complement on <value> and stores the result into <destination>

player_get_face_keys          = 2900 #(player_get_face_keys, <face_key_register>, <player_no>), #Stores <player_no>'s face keys into <face_key_register>
player_set_face_keys          = 2901 #(player_set_face_keys, <player_no>, <face_key_register>), #Sets <player_no>'s face keys to <face_key_register>
player_set_skin               = 2902 #(player_set_skin, <player_no>, <skin_no>), #Sets <player_no>'s skin (gender) to <skin_no> (requires network_compatible = 0 in wse_settings.ini)
player_stop_controlling_agent = 2903 #(player_stop_controlling_agent, <player_no>), #Gives <player_no>'s agent back to AI control (requires network_compatible = 0 in wse_settings.ini)
player_set_banner_id          = 2904 #(player_set_banner_id, <player_no>, <banner_no>), #Sets <player_no>'s banner to <banner_no>

register_get                  = 3000 #(register_get, <destination>, <index>), #Stores the value of register <index> into <destination>
register_set                  = 3001 #(register_set, <index>, <value>), #Sets the value of register <index> to <value>
store_wse_version             = 3002 #(store_wse_version, <destination>, <component>), #Stores <component> of the WSE version (0: major, 1: minor, 2: build) version into <destination>
item_slot_gt                  = 3003 #(item_slot_gt, <item_kind_no>, <slot_no>, <value>), #Fails if <item_kind_no>'s <slot_no> is not greater than <value>
party_template_slot_gt        = 3004 #(party_template_slot_gt, <party_template_no>, <slot_no>, <value>), #Fails if <party_template_no>'s <slot_no> is not greater than <value>
troop_slot_gt                 = 3005 #(troop_slot_gt, <troop_no>, <slot_no>, <value>), #Fails if <troop_no>'s <slot_no> is not greater than <value>
faction_slot_gt               = 3006 #(faction_slot_gt, <faction_no>, <slot_no>, <value>), #Fails if <faction_no>'s <slot_no> is not greater than <value>
quest_slot_gt                 = 3007 #(quest_slot_gt, <quest_no>, <slot_no>, <value>), #Fails if <quest_no>'s <slot_no> is not greater than <value>
scene_slot_gt                 = 3008 #(scene_slot_gt, <site_no>, <slot_no>, <value>), #Fails if <site_no>'s <slot_no> is not greater than <value>
party_slot_gt                 = 3009 #(party_slot_gt, <party_no>, <slot_no>, <value>), #Fails if <party_no>'s <slot_no> is not greater than <value>
player_slot_gt                = 3010 #(player_slot_gt, <player_no>, <slot_no>, <value>), #Fails if <player_no>'s <slot_no> is not greater than <value>
team_slot_gt                  = 3011 #(team_slot_gt, <team_no>, <slot_no>, <value>), #Fails if <team_no>'s <slot_no> is not greater than <value>
agent_slot_gt                 = 3012 #(agent_slot_gt, <agent_no>, <slot_no>, <value>), #Fails if <agent_no>'s <slot_no> is not greater than <value>
scene_prop_slot_gt            = 3013 #(scene_prop_slot_gt, <prop_instance_no>, <slot_no>, <value>), #Fails if <prop_instance_no>'s <slot_no> is not greater than <value>
store_current_trigger         = 3014 #(store_current_trigger, <destination>), #Stores the current trigger into <destination> (0 if not in a trigger)
return_values                 = 3015 #(return_values, [<value_1>], [<value_2>], [<value_3>], [<value_4>], [<value_5>], [<value_6>], [<value_7>], [<value_8>], [<value_9>], [<value_10>], [<value_11>], [<value_12>], [<value_13>], [<value_14>], [<value_15>], [<value_16>]), #Stores up to 16 return values
store_num_return_values       = 3016 #(store_num_return_values, <destination>), #Stores the amount of return values available into <destination>
store_return_value            = 3017 #(store_return_value, <destination>, [<value>]), #Stores return value no. [<value>] into <destination>
set_forced_lod                = 3018 #(set_forced_lod, <lod_level>), #Forces the current trigger entity's LOD level to <lod_level> (0 = auto)
send_message_to_url_advanced  = 3019 #(send_message_to_url_advanced, <url_string>, <user_agent_string>, [<success_callback_script_no>], [<failure_callback_script_no>], [<skip_parsing>], [<timeout>]), #Sends a HTTP request to <url_string> with <user_agent_string>. If the request succeeds, [<success_callback_script_no>] will be called. The script will behave like game_receive_url_response, unless [<skip_parsing>] is non-zero, in which case the script will receive no arguments and s0 will contain the full response. If the request fails, [<failure_callback_script_no>] will be called.
mtsrand                       = 3020 #(mtsrand, <value>), #Seeds the MT19937 random generator with <value>
mtrand                        = 3021 #(mtrand, <destination>, <min>, <max>), #Stores a random value between <min> and <max> into <destination> using the MT19937 random generator
get_time                      = 3022 #(get_time, <destination>, [<local>]), #Stores the current UNIX time into <destination>. If [<local>] is non-zero, it will store local time instead of universal time.
close_order_menu              = 3023 #(close_order_menu), #Closes the order menu
order_flag_is_active          = 3024 #(order_flag_is_active), #Fails if the order flag is not being placed
play_bink_file                = 3025 #(play_bink_file, <path_from_module_directory>, [<duration_in_ms>]), #Plays a .bik file located at <path_from_module_directory>. If [<duration_in_ms>] is not set the full movie will be played
process_advanced_url_messages = 3026 #(process_advanced_url_messages), #Forces processing of URL messages sent with send_message_to_url_advanced
sleep_ms                      = 3027 #(sleep_ms, <time>), #Sleeps (blocking the game) for <time> ms
timer_reset                   = 3028 #(timer_reset, <timer_register_no>, [<use_game_time>]), #Resets <timer_register_no>. If [<use_game_time>] is non-zero the timer will count game time rather than mission time
timer_get_elapsed_time        = 3029 #(timer_get_elapsed_time, <destination>, <timer_register_no>), #Stores <timer_register_no>'s elapsed time into <destination>

game_key_get_key  = 3100 #(game_key_get_key, <destination>, <game_key_no>), #Stores the key mapped to <game_key_no> into <destination>
key_released      = 3101 #(key_released, <key>), #Fails if <key> wasn't released in the current frame
game_key_released = 3102 #(game_key_released, <game_key_no>), #Fails if <game_key_no> wasn't released in the current frame

dict_create      = 3200 #(dict_create, <destination>), #Creates an empty dictionary object and stores it into <destination>
dict_free        = 3201 #(dict_free, <dict>), #Frees the dictionary object <dict>. A dictionary can't be used after freeing it
dict_load_file   = 3202 #(dict_load_file, <dict>, <file>, [<mode>]), #Loads a dictionary file into <dict>. Setting [<mode>] to 0 (default) clears <dict> and then loads the file, setting [<mode>] to 1 doesn't clear <dict> but overrides any key that's already present, [<mode>] to 2 doesn't clear <dict> and doesn't overwrite keys that are already present
dict_load_dict   = 3203 #(dict_load_dict, <dict_1>, <dict_2>, [<mode>]), #Loads <dict_2> into <dict_1>. [<mode>]: see above
dict_save        = 3204 #(dict_save, <dict>, <file>), #Saves <dict> into a file. For security reasons, <file> is just a name, not a full path, and will be stored into a WSE managed directory
dict_clear       = 3205 #(dict_clear, <dict>), #Clears all key-value pairs from <dict>
dict_is_empty    = 3206 #(dict_is_empty, <dict>), #Fails if <dict> is not empty
dict_has_key     = 3207 #(dict_has_key, <dict>, <key>), #Fails if <key> is not present in <dict>
dict_get_size    = 3208 #(dict_get_size, <destination>, <dict>), #Stores the count of key-value pairs in <dict> into <destination>
dict_delete_file = 3209 #(dict_delete_file, <file>), #Deletes dictionary file <file> from disk
dict_get_str     = 3210 #(dict_get_str, <string_register>, <dict>, <key>, [<default>]), #Stores the string value paired to <key> into <string_register>. If the key is not found and [<default>] is set, [<default>] will be stored instead. If [<default>] is not set, an empty string will be stored
dict_get_int     = 3211 #(dict_get_int, <destination>, <dict>, <key>, [<default>]), #Stores the numeric value paired to <key> into <destination>. If the key is not found and [<default>] is set, [<default>] will be stored instead. If [<default>] is not set, 0 will be stored
dict_set_str     = 3212 #(dict_set_str, <dict>, <key>, <string_no>), #Adds (or changes) <string_no> as the string value paired to <key>
dict_set_int     = 3213 #(dict_set_int, <dict>, <key>, <value>), #Adds (or changes) <value> as the numeric value paired to <key>

agent_get_item_modifier           = 3300 #(agent_get_item_modifier, <destination>, <agent_no>), #Stores <agent_no>'s horse item modifier (-1 if agent is not a horse) into <destination>
agent_get_item_slot_modifier      = 3301 #(agent_get_item_slot_modifier, <destination>, <agent_no>, <item_slot_no>), #Stores <agent_no>'s <item_slot_no> modifier into <destination>
agent_get_animation_progress      = 3302 #(agent_get_animation_progress, <destination>, <agent_no>, [<channel_no>]), #Stores <agent_no>'s channel [<channel_no>] animation progress (in %%) into <destination>
agent_get_dna                     = 3303 #(agent_get_dna, <destination>, <agent_no>), #Stores <agent_no>'s dna into <destination>
agent_get_ground_scene_prop       = 3304 #(agent_get_ground_scene_prop, <destination>, <agent_no>), #Stores the prop instance on which <agent_no> is standing into <destination>
agent_get_item_slot_ammo          = 3305 #(agent_get_item_slot_ammo, <destination>, <agent_no>, <item_slot_no>), #Stores <agent_no>'s <item_slot_no> ammo count into <destination>
agent_set_item_slot_ammo          = 3306 #(agent_set_item_slot_ammo, <agent_no>, <item_slot_no>, <value>), #Sets <agent_no>'s <item_slot_no> ammo count to <value>
agent_get_item_slot_hit_points    = 3307 #(agent_get_item_slot_hit_points, <destination>, <agent_no>, <item_slot_no>), #Stores <agent_no>'s <item_slot_no> shield hitpoints into <destination>
agent_set_item_slot_hit_points    = 3308 #(agent_set_item_slot_hit_points, <agent_no>, <item_slot_no>, <value>), #Sets <agent_no>'s <item_slot_no> shield hitpoints to <value>
agent_get_wielded_item_slot_no    = 3309 #(agent_get_wielded_item_slot_no, <destination>, <agent_no>, [<hand_no>]), #Stores <agent_no>'s wielded item slot for [<hand_no>] into <destination>
agent_get_bone_position           = 3310 #(agent_get_bone_position, <position_register>, <agent_no>, <bone_no>), #Stores <agent_no>'s <bone_no> position into <position_register>
agent_get_scale                   = 3311 #(agent_get_scale, <destination_fixed_point>, <agent_no>), #Stores <agent_no>'s scale into <destination_fixed_point>
agent_set_forced_lod              = 3312 #(agent_set_forced_lod, <agent_no>, <lod_level>), #Forces <agent_no>'s LOD level to <lod_level> (0 = auto)
agent_get_item_slot_flags         = 3313 #(agent_get_item_slot_flags, <destination>, <agent_no>, <item_slot_no>), #Stores <agent_no>'s <item_slot_no> item slot flags into <destination>
agent_ai_get_move_target_position = 3314 #(agent_ai_get_move_target_position, <position_register>, <agent_no>), #Stores <agent_no>'s move target position agent into <position_register>
agent_set_horse                   = 3315 #(agent_set_horse, <agent_no>, <horse_agent_no>), #Sets <agent_no>'s horse to <horse_agent_no> (-1 for no horse)
agent_ai_set_simple_behavior      = 3316 #(agent_ai_set_simple_behavior, <agent_no>, <simple_behavior>, [<guaranteed_time>]), #Sets <agent_no>'s behavior to <simple_behavior> and guarantees it won't be changed for [<guaranteed_time>] seconds. If [<guaranteed_time>] is not specified or <= 0, it won't be changed until agent_force_rethink is called
agent_accelerate                  = 3317 #(agent_accelerate, <agent_no>, <position_register_no>), #Uses x, y, z components of <position_register_no> to apply acceleration to <agent_no>
agent_set_item_slot_modifier      = 3318 #(agent_set_item_slot_modifier, <agent_no>, <item_slot_no>, <item_modifier_no>), #Sets <agent_no>'s <item_slot_no> modifier to <item_modifier_no>

multiplayer_send_chat_message_to_player      = 3400 #(multiplayer_send_chat_message_to_player, <player_no>, <sender_player_no>, <text>, [<type>]), #Sends <text> to <player_no> as a (native compatible) chat message by <sender_player_no>. Works only on servers. [<type>]: 0 = chat, 1 = team chat
multiplayer_send_composite_message_to_player = 3401 #(multiplayer_send_composite_message_to_player, <player_no>, <message_type>, <message_register>), #Sends <message_register> with <message_type> to <player_no> (requires network_compatible = 0 in wse_settings.ini)
multiplayer_send_composite_message_to_server = 3402 #(multiplayer_send_composite_message_to_server, <message_type>, <message_register>), #Sends <message_register> with <message_type> to the server (requires network_compatible = 0 in wse_settings.ini)
multiplayer_get_cur_profile                  = 3403 #(multiplayer_get_cur_profile, <destination>), #Stores the current multiplayer profile into <destination>
multiplayer_get_num_profiles                 = 3404 #(multiplayer_get_num_profiles, <destination>), #Stores the number of multiplayer profiles into <destination>
multiplayer_profile_get_face_keys            = 3405 #(multiplayer_profile_get_face_keys, <face_key_register>, <profile_no>), #Stores <profile_no>'s face keys into <face_key_register>
multiplayer_message_init                     = 3406 #(multiplayer_message_init, <message_register>), #Initializes (empties) <message_register> (requires network_compatible = 0 in wse_settings.ini)
multiplayer_message_put_string               = 3407 #(multiplayer_message_put_string, <message_register>, <string>), #Puts <string> into <message_register> (requires network_compatible = 0 in wse_settings.ini)
multiplayer_message_put_int                  = 3408 #(multiplayer_message_put_int, <message_register>, <value>, [<num_bits>]), #Puts [<num_bits>] of <value> into <message_register> (requires network_compatible = 0 in wse_settings.ini)
multiplayer_message_put_position             = 3409 #(multiplayer_message_put_position, <message_register>, <position_register>, [<local>]), #Puts <position_register> into <9>. Set [<local>] to non-zero for small, relative positions (default: scene positions) (requires network_compatible = 0 in wse_settings.ini)
multiplayer_message_put_coordinate           = 3410 #(multiplayer_message_put_coordinate, <message_register>, <position_register>, [<local>]), #Puts x, y, z coordinates from <position_register> into <message_register>. Set [<local>] to non-zero for small, relative positions (default: scene positions) (requires network_compatible = 0 in wse_settings.ini)
multiplayer_message_put_face_keys            = 3411 #(multiplayer_message_put_face_keys, <message_register>, <face_keys_register>), #Puts <face_keys_register> into <message_register> (requires network_compatible = 0 in wse_settings.ini)
multiplayer_cur_message_get_string           = 3412 #(multiplayer_cur_message_get_string, <string_register>), #Stores a string from the current message register into <string_register> (requires network_compatible = 0 in wse_settings.ini)
multiplayer_cur_message_get_int              = 3413 #(multiplayer_cur_message_get_int, <destination>, [<num_bits>]), #Stores [<num_bits>] of an int from the current message register into <destination>. [<num_bits>] MUST match the number of bits sent (requires network_compatible = 0 in wse_settings.ini)
multiplayer_cur_message_get_position         = 3414 #(multiplayer_cur_message_get_position, <position_register>, [<local>]), #Stores a position from the current message register into <position_register>. [<local>] MUST match the type sent (requires network_compatible = 0 in wse_settings.ini)
multiplayer_cur_message_get_coordinate       = 3415 #(multiplayer_cur_message_get_coordinate, <position_register>, [<local>]), #Stores x, y, z coordinates from the current message register into <position_register>. [<local>] MUST match the type sent (requires network_compatible = 0 in wse_settings.ini)
multiplayer_cur_message_get_face_keys        = 3416 #(multiplayer_cur_message_get_face_keys, <face_keys_register>), #Stores face keys from the current message register into <face_keys_register> (requires network_compatible = 0 in wse_settings.ini)

server_set_password_admin      = 3500 #(server_set_password_admin, <password>), #Sets <password> as server administrator password
server_set_password_private    = 3501 #(server_set_password_private, <password>), #Sets <password> as server private player password
server_map_rotation_get_count  = 3502 #(server_map_rotation_get_count, <destination>), #Stores the number of maps in rotation into <destination>
server_map_rotation_get_index  = 3503 #(server_map_rotation_get_index, <destination>), #Stores the current map rotation index into <destination>
server_map_rotation_set_index  = 3504 #(server_map_rotation_set_index, <index>), #Sets the current rotation index to <index>
server_map_rotation_get_map    = 3505 #(server_map_rotation_get_map, <destination>, <index>), #Stores the map at <index> into <destination>
server_map_rotation_add_map    = 3506 #(server_map_rotation_add_map, <site_no>, [<index>]), #Adds <site_no> to the map rotation at [<index>]
server_map_rotation_remove_map = 3507 #(server_map_rotation_remove_map, [<index>]), #Removes the map at [<index>] from the map rotation (does not work when only one left)
server_get_horse_friendly_fire = 3508 #(server_get_horse_friendly_fire, <destination>), #Stores horse friendly fire status into <destination> (requires network_compatible = 0 in wse_settings.ini)
server_set_horse_friendly_fire = 3509 #(server_set_horse_friendly_fire, <value>), #Enables or disables horse friendly fire (requires network_compatible = 0 in wse_settings.ini)
server_get_show_crosshair      = 3510 #(server_get_show_crosshair, <destination>), #Stores crosshair visibility status into <destination> (requires network_compatible = 0 in wse_settings.ini)
server_set_show_crosshair      = 3511 #(server_set_show_crosshair, <value>), #Enables or disables the crosshair (requires network_compatible = 0 in wse_settings.ini)
get_server_option_at_connect   = 3512 #(get_server_option_at_connect, <destination>, [<index>]), #Stores option [<index>] into <destination>

store_cur_mission_template_no            = 3600 #(store_cur_mission_template_no, <destination>), #Stores the current mission template into <destination>
prop_instance_clear_attached_missiles    = 3601 #(prop_instance_clear_attached_missiles, <prop_instance_no>), #Removes all attached missiles from <prop_instance_no>. Works only with dynamic scene props (non-retrievable missiles)
camera_in_first_person                   = 3602 #(camera_in_first_person), #Fails if the camera is not in first person
set_camera_in_first_person               = 3603 #(set_camera_in_first_person, <value>), #Sets the camera to first or third person
set_show_use_tooltip                     = 3604 #(set_show_use_tooltip, <tooltip_type>, [<value>]), #Enables or disables use tooltips. See header_constants_addon.py for possible types
set_ally_collision_threshold             = 3605 #(set_ally_collision_threshold, <low_boundary>, <high_boundary>), #Changes the animation progress boundaries (in percents) that determine if attacks on allies will collide (default: 45% <= x <= 60%)
set_prop_collision_threshold             = 3606 #(set_prop_collision_threshold, <attack_direction>, <low_boundary>, <high_boundary>), #Changes the animation progress boundaries (in percents) that determine if swing attacks on props will collide (default: 40% <= x <= 80% (75% for overheads))
particle_system_remove                   = 3607 #(particle_system_remove, [<particle_system_no>]), #Removes [<particle_system_no>] (all particle systems if not set or -1) from the current entity (can be used in several in triggers)
get_camera_position                      = 3608 #(get_camera_position, <position_register_no>), #Stores camera position and rotation into <position_register_no>
get_spectated_agent_no                   = 3609 #(get_spectated_agent_no, <destination>), #Stores spectated agent no into <destination>
prop_instance_set_forced_lod             = 3610 #(prop_instance_set_forced_lod, <prop_instance_no>, <lod_level>), #Forces <prop_instance_no>'s LOD level to <lod_level> (0 = auto)
prop_instance_set_variation_id           = 3611 #(prop_instance_set_variation_id, <prop_instance_no>, <value>), #Sets <prop_instance_no>'s variation id to <value>
prop_instance_set_variation_id_2         = 3612 #(prop_instance_set_variation_id_2, <prop_instance_no>, <value>), #Sets <prop_instance_no>'s variation id 2 to <value>
stop_time                                = 3613 #(stop_time, <value>), #Stops/resumes the mission. Works only in singleplayer with cheat mode enabled.
cur_missile_get_path_point_position      = 3614 #(cur_missile_get_path_point_position, <position_register>, <path_point_no>), #Stores the position of the missile's <path_point_no> (0-499) into <position_register> (can be used in ti_on_init_missile)
get_water_level                          = 3615 #(get_water_level, <destination_fixed_point>), #Stores the water level into <destination_fixed_point>
cast_ray                                 = 3616 #(cast_ray, <destination>, <hit_position_register>, <ray_position_register>, [<ray_length_fixed_point>]), #Casts a ray of length [<ray_length_fixed_point>] starting from <ray_position_register> and stores the closest hit position into <hit_position_register> (fails if no hits). If the body hit is a prop instance, its id will be stored into <destination>
prop_instance_set_vertex_keys_time_point = 3617 #(prop_instance_set_vertex_keys_time_point, <prop_instance_no>, <time_point>), #Sets <prop_instance_no>'s mesh vertex keys time point to <time_point>
missile_remove_on_hit                    = 3618 #(missile_remove_on_hit), #Causes a missile item not to spawn on hit (can be only used inside ti_on_missile_hit)

troop_get_skill_points       = 3700 #(troop_get_skill_points, <destination>, <troop_no>), #Stores <troop_no>'s unused skill points into <destination>
troop_set_skill_points       = 3701 #(troop_set_skill_points, <troop_no>, <value>), #Sets <troop_no>'s unused skill points to <value>
troop_get_attribute_points   = 3702 #(troop_get_attribute_points, <destination>, <troop_no>), #Stores <troop_no>'s unused attribute points into <destination>
troop_set_attribute_points   = 3703 #(troop_set_attribute_points, <troop_no>, <value>), #Sets <troop_no>'s unused attribute points to <value>
troop_get_proficiency_points = 3704 #(troop_get_proficiency_points, <destination>, <troop_no>), #Stores <troop_no>'s unused proficiency points into <destination>
troop_set_proficiency_points = 3705 #(troop_set_proficiency_points, <troop_no>, <value>), #Sets <troop_no>'s unused proficiency points to <value>
troop_get_face_keys          = 3706 #(troop_get_face_keys, <face_key_register>, <troop_no>, [<alt>]), #Stores <troop_no>'s face keys into <face_key_register>. if [<alt>] is non-zero the second pair of face keys is stored
troop_set_face_keys          = 3707 #(troop_set_face_keys, <troop_no>, <face_key_register>, [<alt>]), #Sets <troop_no>'s face keys to <face_key_register>. if [<alt>] is non-zero the second pair of face keys is set
troop_has_flag               = 3708 #(troop_has_flag, <troop_no>, <flag>), #Fails if <troop_no> doesn't have <flag>
troop_set_skill              = 3709 #(troop_set_skill, <troop_no>, <skill_no>, <value>), #Sets <troop_no>'s <skill_no> to <value>
troop_set_attribute          = 3710 #(troop_set_attribute, <troop_no>, <attribute>, <value>), #Sets <troop_no>'s <attribute> to <value>
troop_set_proficiency        = 3711 #(troop_set_proficiency, <troop_no>, <proficiency>, <value>), #Sets <troop_no>'s <proficiency> to <value>

item_get_weight              = 3800 #(item_get_weight, <destination_fixed_point>, <item_kind_no>), #Stores <item_kind_no>'s weight into <destination_fixed_point>
item_get_value               = 3801 #(item_get_value, <destination>, <item_kind_no>), #Stores <item_kind_no>'s value into <destination>
item_get_difficulty          = 3802 #(item_get_difficulty, <destination>, <item_kind_no>), #Stores <item_kind_no>'s difficulty into <destination>
item_get_head_armor          = 3803 #(item_get_head_armor, <destination>, <item_kind_no>), #Stores <item_kind_no>'s head armor into <destination>
item_get_body_armor          = 3804 #(item_get_body_armor, <destination>, <item_kind_no>), #Stores <item_kind_no>'s body armor into <destination>
item_get_leg_armor           = 3805 #(item_get_leg_armor, <destination>, <item_kind_no>), #Stores <item_kind_no>'s leg armor into <destination>
item_get_hit_points          = 3806 #(item_get_hit_points, <destination>, <item_kind_no>), #Stores <item_kind_no>'s hit points into <destination>
item_get_weapon_length       = 3807 #(item_get_weapon_length, <destination>, <item_kind_no>), #Stores <item_kind_no>'s weapon length into <destination>
item_get_speed_rating        = 3808 #(item_get_speed_rating, <destination>, <item_kind_no>), #Stores <item_kind_no>'s speed rating into <destination>
item_get_missile_speed       = 3809 #(item_get_missile_speed, <destination>, <item_kind_no>), #Stores <item_kind_no>'s missile speed into <destination>
item_get_max_ammo            = 3810 #(item_get_max_ammo, <destination>, <item_kind_no>), #Stores <item_kind_no>'s max ammo into <destination>
item_get_accuracy            = 3811 #(item_get_accuracy, <destination>, <item_kind_no>), #Stores <item_kind_no>'s accuracy into <destination>
item_get_shield_width        = 3812 #(item_get_shield_width, <destination>, <item_kind_no>), #Stores <item_kind_no>'s shield width into <destination>
item_get_shield_height       = 3813 #(item_get_shield_height, <destination>, <item_kind_no>), #Stores <item_kind_no>'s shield height into <destination>
item_get_horse_scale         = 3814 #(item_get_horse_scale, <destination>, <item_kind_no>), #Stores <item_kind_no>'s horse scale into <destination>
item_get_horse_speed         = 3815 #(item_get_horse_speed, <destination>, <item_kind_no>), #Stores <item_kind_no>'s horse speed into <destination>
item_get_horse_maneuver      = 3816 #(item_get_horse_maneuver, <destination>, <item_kind_no>), #Stores <item_kind_no>'s horse maneuver into <destination>
item_get_food_quality        = 3817 #(item_get_food_quality, <destination>, <item_kind_no>), #Stores <item_kind_no>'s food quality into <destination>
item_get_abundance           = 3818 #(item_get_abundance, <destination>, <item_kind_no>), #Stores <item_kind_no>'s abundance into <destination>
item_get_thrust_damage       = 3819 #(item_get_thrust_damage, <destination>, <item_kind_no>), #Stores <item_kind_no>'s thrust damage into <destination>
item_get_thrust_damage_type  = 3820 #(item_get_thrust_damage_type, <destination>, <item_kind_no>), #Stores <item_kind_no>'s thrust damage type into <destination>
item_get_swing_damage        = 3821 #(item_get_swing_damage, <destination>, <item_kind_no>), #Stores <item_kind_no>'s thrust damage into <destination>
item_get_swing_damage_type   = 3822 #(item_get_swing_damage_type, <destination>, <item_kind_no>), #Stores <item_kind_no>'s thrust damage type into <destination>
item_get_horse_charge_damage = 3823 #(item_get_horse_charge_damage, <destination>, <item_kind_no>), #Stores <item_kind_no>'s thrust damage into <destination>
item_has_property            = 3824 #(item_has_property, <item_kind_no>, <property>), #Fails if <item_kind_no> doesn't have <property>
item_has_capability          = 3825 #(item_has_capability, <item_kind_no>, <capability>), #Fails if <item_kind_no> doesn't have <capability>
item_has_modifier            = 3826 #(item_has_modifier, <item_kind_no>, <item_modifier_no>), #Fails if <item_modifier_no> is not a valid modifier for <item_kind_no>
item_has_faction             = 3827 #(item_has_faction, <item_kind_no>, <faction_no>), #Fails if <item_kind_no> doesn't have <faction_no> in its faction list
item_set_thrust_damage       = 3828 #(item_set_thrust_damage, <item_kind_no>, <value>), #Sets <item_kind_no>'s thrust damage to <value>
item_set_thrust_damage_type  = 3829 #(item_set_thrust_damage_type, <item_kind_no>, <value>), #Sets <item_kind_no>'s thrust damage type to <value>
item_set_swing_damage        = 3830 #(item_set_swing_damage, <item_kind_no>, <value>), #Sets <item_kind_no>'s thrust damage to <value>
item_set_swing_damage_type   = 3831 #(item_set_swing_damage_type, <item_kind_no>, <value>), #Sets <item_kind_no>'s thrust damage type to <value>
item_set_head_armor          = 3832 #(item_set_head_armor, <item_kind_no>, <value>), #Sets <item_kind_no>'s head armor to <value>
item_set_body_armor          = 3833 #(item_set_body_armor, <item_kind_no>, <value>), #Sets <item_kind_no>'s body armor to <value>
item_set_leg_armor           = 3834 #(item_set_leg_armor, <item_kind_no>, <value>), #Sets <item_kind_no>'s leg armor to <value>
item_set_speed_rating        = 3835 #(item_set_speed_rating, <item_kind_no>, <value>), #Sets <item_kind_no>'s speed rating to <value>
item_set_missile_speed       = 3836 #(item_set_missile_speed, <item_kind_no>, <value>), #Sets <item_kind_no>'s missile speed to <value>

party_stack_get_experience      = 3900 #(party_stack_get_experience, <destination>, <party_no>, <party_stack_no>), #Stores the experience of <party_no>'s <party_stack_no> into <destination>
party_stack_get_num_upgradeable = 3901 #(party_stack_get_num_upgradeable, <destination>, <party_no>, <party_stack_no>), #Stores the amount of upgradeable troops in <party_no>'s <party_stack_no> into <destination>
party_has_flag                  = 3902 #(party_has_flag, <party_no>, <flag>), #Fails if <party_no> doesn't have <flag>
party_heal_members              = 3903 #(party_heal_members, <party_no>, <troop_no>, <number>), #Heals <number> <troop_no> of <party_no>

position_rotate_z_floating      = 4100 #(position_rotate_z_floating, <position_register>, <angle_fixed_point>, [<use_global_axis>]), #Rotates <position_register> around the z-axis by <angle_fixed_point> degrees
position_get_vector_to_position = 4101 #(position_get_vector_to_position, <destination_fixed_point>, <dest_position_register>, <position_register_1>, <position_register_2>), #Stores the vector from <position_register_1> to <position_register_2> into <dest_position_register> and its length into <destination_fixed_point>
position_align_to_ground        = 4102 #(position_align_to_ground, <position_register>, [<point_up>], [<set_z_to_ground_level>]), #Aligns <position_register> to the ground (or to the ground normal if [<point_up>] is set)

options_get_battle_size         = 4300 #(options_get_battle_size, <destination>), #Stores battle size into <destination>
options_set_battle_size         = 4301 #(options_set_battle_size, <value>), #Sets batte size to <value> (must be between 30 and 2000)
options_get_verbose_casualties  = 4302 #(options_get_verbose_casualties, <destination>), #Stores verbose casualties enabled/disabled into <destination>
options_set_verbose_casualties  = 4303 #(options_set_verbose_casualties, <value>), #Enables or disables verbose casualties
options_get_cheat_mode          = 4304 #(options_get_cheat_mode, <destination>), #Stores cheat mode enabled/disabled into <destination>
options_set_cheat_mode          = 4305 #(options_set_cheat_mode, <value>), #Enables or disables cheat mode
options_get_realistic_headshots = 4306 #(options_get_realistic_headshots, <destination>), #Stores "realistic" headshots enabled/disabled into <destination>
options_set_realistic_headshots = 4307 #(options_set_realistic_headshots, <value>), #Enables or disables "realistic" headshots

fld       = 4400 #(fld, <fp_register>, <value_fixed_point>), #Loads <value_fixed_point> into <fp_register>
fld_str   = 4401 #(fld_str, <fp_register>, <string>), #Parses <string> and loads it into <fp_register>
fld_pos_x = 4402 #(fld_pos_x, <fp_register>, <position_register_no>), #Loads the x component of <position_register_no> into <fp_register>
fld_pos_y = 4403 #(fld_pos_y, <fp_register>, <position_register_no>), #Loads the y component of <position_register_no> into <fp_register>
fld_pos_z = 4404 #(fld_pos_z, <fp_register>, <position_register_no>), #Loads the z component of <position_register_no> into <fp_register>
fst       = 4405 #(fst, <destination_fixed_point>, <fp_register>), #Stores <fp_register> into <destination_fixed_point>
fcpy      = 4406 #(fcpy, <fp_register_1>, <fp_register_2>), #Copies <fp_register_2> into <fp_register_1>
feq       = 4407 #(feq, <destination_fp_register>, <fp_register_1>, <fp_register_2>), #Fails if <fp_register_1> isn't approximately equal to <fp_register_2>
fgt       = 4408 #(fgt, <destination_fp_register>, <fp_register_1>, <fp_register_2>), #Fails if <fp_register_1> isn't greater than <fp_register_2>
flt       = 4409 #(flt, <destination_fp_register>, <fp_register_1>, <fp_register_2>), #Fails if <fp_register_1> isn't less than <fp_register_2>
fge       = 4410 #(fge, <destination_fp_register>, <fp_register_1>, <fp_register_2>), #Fails if <fp_register_1> isn't greater or approximately equal to <fp_register_2>
fle       = 4411 #(fle, <destination_fp_register>, <fp_register_1>, <fp_register_2>), #Fails if <fp_register_1> isn't less or approximately equal to <fp_register_2>
fsub      = 4412 #(fsub, <destination_fp_register>, <fp_register_1>, <fp_register_2>), #Subtracts <fp_register_2> from <fp_register_1> and stores the result into <destination_fp_register>
fmul      = 4413 #(fmul, <destination_fp_register>, <fp_register_1>, <fp_register_2>), #Multiplies <fp_register_1> by <fp_register_2> and stores the result into <destination_fp_register>
fdiv      = 4414 #(fdiv, <destination_fp_register>, <fp_register_1>, <fp_register_2>), #Divides <fp_register_1> by <fp_register_2> and stores the result into <destination_fp_register>
fmin      = 4415 #(fmin, <destination_fp_register>, <fp_register_1>, <fp_register_2>), #Stores the smaller value between <fp_register_1> and <fp_register_2> into <destination_fp_register>
fmax      = 4416 #(fmax, <destination_fp_register>, <fp_register_1>, <fp_register_2>), #Stores the larger value between <fp_register_1> and <fp_register_2> into <destination_fp_register>
fclamp    = 4417 #(fclamp, <destination_fp_register>, <fp_register_1>, <fp_register_2>, <fp_register_3>), #Clamps <fp_register_1> between <fp_register_2> and <fp_register_3> and stores the result into <destination_fp_register>
fsqrt     = 4418 #(fsqrt, <destination_fp_register>, <fp_register>), #Stores the square root of <fp_register> into <destination_fp_register>
fabs      = 4419 #(fabs, <destination_fp_register>, <fp_register>), #Stores the absolute value of <fp_register> into <destination_fp_register>
fceil     = 4420 #(fceil, <destination_fp_register>, <fp_register>), #Stores the ceiling of <fp_register> into <destination_fp_register>
ffloor    = 4421 #(ffloor, <destination_fp_register>, <fp_register>), #Stores the floor of <fp_register> into <destination_fp_register>
fexp      = 4422 #(fexp, <destination_fp_register>, <fp_register>), #Stores e raised to the power of <fp_register> into <destination_fp_register>
fpow      = 4423 #(fpow, <destination_fp_register>, <fp_register_1>, <fp_register_2>), #Stores <fp_register_1> raised to the power of <fp_register_2> into <destination_fp_register>
fln       = 4424 #(fln, <destination_fp_register>, <fp_register>), #Stores the natural logarithm of <fp_register> into <destination_fp_register>
flog      = 4425 #(flog, <destination_fp_register>, <fp_register>, [<base>]), #Stores the base-[<base>] (default: base-10) logarithm of <fp_register> into <destination_fp_register>
fmod      = 4426 #(fmod, <destination_fp_register>, <fp_register_1>, <fp_register_2>), #Stores the remainder of <fp_register_1>/<fp_register_2> into <destination_fp_register>
fsin      = 4427 #(fsin, <destination_fp_register>, <fp_register>), #Stores the sine of <fp_register> into <destination_fp_register>
fcos      = 4428 #(fcos, <destination_fp_register>, <fp_register>), #Stores the cosine of <fp_register> into <destination_fp_register>
ftan      = 4429 #(ftan, <destination_fp_register>, <fp_register>), #Stores the tangent of <fp_register> into <destination_fp_register>
fsinh     = 4430 #(fsinh, <destination_fp_register>, <fp_register>), #Stores the hyperbolic sine of <fp_register> into <destination_fp_register>
fcosh     = 4431 #(fcosh, <destination_fp_register>, <fp_register>), #Stores the hyperbolic cosine of <fp_register> into <destination_fp_register>
ftanh     = 4432 #(ftanh, <destination_fp_register>, <fp_register>), #Stores the hyperbolic tangent of <fp_register> into <destination_fp_register>
fasin     = 4433 #(fasin, <destination_fp_register>, <fp_register>), #Stores the arc sine of <fp_register> into <destination_fp_register>
facos     = 4434 #(facos, <destination_fp_register>, <fp_register>), #Stores the arc cosine of <fp_register> into <destination_fp_register>
fatan     = 4435 #(fatan, <destination_fp_register>, <fp_register>), #Stores the arc tangent of <fp_register> into <destination_fp_register>
fatan2    = 4436 #(fatan2, <destination_fp_register>, <fp_register_1>, <fp_register_2>), #Stores the arc cosine of <fp_register_1>/<fp_register_2> into <destination_fp_register>
feval     = 4437 #(feval, <expression_string>), #Evaluates <expression_string>. See EVAL_README.txt in WSESDK for more information

scene_set_flags         = 4500 #(scene_set_flags, <scene_no>, <flags>), #Sets <scene_no>'s flags to <flags>
scene_set_water_level   = 4501 #(scene_set_water_level, <scene_no>, <water_level_fixed_point>), #Sets <scene_no>'s water level to <water_level_fixed_point>
scene_set_bounds        = 4502 #(scene_set_bounds, <scene_no>, <min_x_fixed_point>, <min_y_fixed_point>, <max_x_fixed_point>, <max_y_fixed_point>), #Sets <scene_no>'s bounds to (<min_x_fixed_point>, <min_y_fixed_point>) and (<max_x_fixed_point>, <max_y_fixed_point>)
scene_set_outer_terrain = 4503 #(scene_set_outer_terrain, <scene_no>, <outer_terrain_mesh_name>), #Sets <scene_no>'s outer terrain to <outer_terrain_mesh_name>
scene_set_terrain_seed  = 4504 #(scene_set_terrain_seed, <scene_no>, <value>), #Sets <scene_no>'s terrain generator terrain seed to <value>
scene_set_river_seed    = 4505 #(scene_set_river_seed, <scene_no>, <value>), #Sets <scene_no>'s terrain generator river seed to <value>
scene_set_flora_seed    = 4506 #(scene_set_flora_seed, <scene_no>, <value>), #Sets <scene_no>'s terrain generator flora seed to <value>
scene_set_deep_water    = 4507 #(scene_set_deep_water, <scene_no>, <value>), #Enables or disables terrain generator deep water for <scene_no>
scene_set_place_river   = 4508 #(scene_set_place_river, <scene_no>, <value>), #Enables or disables terrain generator river placement for <scene_no>
scene_set_disable_grass = 4509 #(scene_set_disable_grass, <scene_no>, <value>), #Enables or disables terrain generator grass placement for <scene_no>
scene_set_valley_size   = 4510 #(scene_set_valley_size, <scene_no>, <value>), #Sets <scene_no>'s terrain generator valley size to <value> (0-127)
scene_set_hill_height   = 4511 #(scene_set_hill_height, <scene_no>, <value>), #Sets <scene_no>'s terrain generator hill height to <value> (0-127)
scene_set_ruggedness    = 4512 #(scene_set_ruggedness, <scene_no>, <value>), #Sets <scene_no>'s terrain generator ruggedness to <value> (0-127)
scene_set_vegetation    = 4513 #(scene_set_vegetation, <scene_no>, <value>), #Sets <scene_no>'s terrain generator vegetation to <value> (0-127)
scene_set_size          = 4514 #(scene_set_size, <scene_no>, <x>, <y>), #Sets <scene_no>'s terrain generator map size to (<x>, <y>) (0-1023)
scene_set_region_type   = 4515 #(scene_set_region_type, <scene_no>, <value>), #Sets <scene_no>'s terrain generator region type to <value> (0-15)
scene_set_region_detail = 4516 #(scene_set_region_detail, <scene_no>, <value>), #Sets <scene_no>'s terrain generator region detail to <value> (0-3)

edit_mode_in_edit_objects_mode            = 4600 #(edit_mode_in_edit_objects_mode), #Fails if the game is not in edit objects mode
edit_mode_get_num_selected_prop_instances = 4601 #(edit_mode_get_num_selected_prop_instances, <destination>), #Stores the number of selected prop instances into <destination>
edit_mode_get_selected_prop_instance      = 4602 #(edit_mode_get_selected_prop_instance, <destination>, <index>), #Stores the <index>th selected prop instance into instance no into <destination>
edit_mode_select_prop_instance            = 4603 #(edit_mode_select_prop_instance, <prop_instance_no>), #Stores the <1>th selected prop instance into instance no into <prop_instance_no>
edit_mode_deselect_prop_instance          = 4604 #(edit_mode_deselect_prop_instance, <prop_instance_no>), #Stores the <1>th selected prop instance into instance no into <prop_instance_no>
edit_mode_get_highlighted_prop_instance   = 4605 #(edit_mode_get_highlighted_prop_instance, <destination>), #Stores the highlighted prop instance into <destination>
edit_mode_set_highlighted_prop_instance   = 4606 #(edit_mode_set_highlighted_prop_instance, [<prop_instance_no>]), #Stores the <1>th selected prop instance into instance no into [<prop_instance_no>]
edit_mode_set_enabled                     = 4607 #(edit_mode_set_enabled, <value>), #Enables or disables edit mode

set_shader_param_int    = 4700 #(set_shader_param_int, <parameter_name>, <value>), #Sets the int shader parameter <parameter_name> to <value>
set_shader_param_float  = 4701 #(set_shader_param_float, <parameter_name>, <value_fixed_point>), #Sets the float shader parameter <parameter_name> to <value_fixed_point>
set_shader_param_vector = 4702 #(set_shader_param_vector, <parameter_name>, <position_register>), #Sets the float4 shader parameter <parameter_name> to <position_register>'s origin
set_shader_param_matrix = 4703 #(set_shader_param_matrix, <parameter_name>, <position_register>), #Sets the float4x4 shader parameter <parameter_name> to <position_register>
update_material         = 4704 #(update_material, <material_name>, <new_material_name>), #Updates <material_name> with <new_material_name>

menu_create_new      = 4800 #(menu_create_new, <destination>, <text>, [<mesh_name>], [<flags>], [<script_no>], [<script_param>]), #Creates a dynamic menu and stores its id into <destination>. [<script_no>] (-1 for no script) will be called with params 1 = menu_no, 2 = [<script_param>] when the operations block is executed
menu_add_item        = 4801 #(menu_add_item, <menu_no>, <text>, [<conditions_script_no>], [<consequences_script_no>], [<script_param>]), #Adds a new menu item to <menu_no>. [<conditions_script_no>] and [<consequences_script_no>] (-1 for no script) will be called with params 1 = <menu_no>, 2 = [<script_param>] when the conditions/consequences blocks are executed
menu_clear_items     = 4802 #(menu_clear_items, <menu_no>), #Removes all menu items from <menu_no>
menu_clear_generated = 4803 #(menu_clear_generated), #Removes all dynamic menus

overlay_get_val       = 4900 #(overlay_get_val, <destination>, <overlay_no>), #Stores <overlay_no>'s value into <destination>
presentation_activate = 4901 #(presentation_activate, <presentation_no>), #Activates <presentation_no>. Fails if <presentation_no> is not running

lhs_operations += [
	try_for_agents,
	try_for_attached_parties,
	try_for_active_players,
	try_for_prop_instances,
	store_trigger_param,
	str_length,
	str_index_of,
	str_last_index_of,
	str_get_char,
	str_to_num,
	str_compare,
	str_split,
	face_keys_get_hair,
	face_keys_get_beard,
	face_keys_get_face_texture,
	face_keys_get_hair_texture,
	face_keys_get_hair_color,
	face_keys_get_age,
	face_keys_get_morph_key,
	val_shr,
	store_shr,
	val_lshr,
	store_lshr,
	val_shl,
	store_shl,
	val_xor,
	store_xor,
	val_not,
	store_not,
	register_get,
	store_wse_version,
	store_current_trigger,
	store_num_return_values,
	store_return_value,
	mtrand,
	get_time,
	timer_get_elapsed_time,
	game_key_get_key,
	dict_create,
	dict_get_size,
	dict_get_int,
	agent_get_item_modifier,
	agent_get_item_slot_modifier,
	agent_get_animation_progress,
	agent_get_dna,
	agent_get_ground_scene_prop,
	agent_get_item_slot_ammo,
	agent_get_item_slot_hit_points,
	agent_get_wielded_item_slot_no,
	agent_get_scale,
	agent_get_item_slot_flags,
	multiplayer_get_cur_profile,
	multiplayer_get_num_profiles,
	multiplayer_cur_message_get_int,
	server_map_rotation_get_count,
	server_map_rotation_get_index,
	server_map_rotation_get_map,
	server_get_horse_friendly_fire,
	server_get_show_crosshair,
	get_server_option_at_connect,
	store_cur_mission_template_no,
	get_spectated_agent_no,
	get_water_level,
	cast_ray,
	troop_get_skill_points,
	troop_get_attribute_points,
	troop_get_proficiency_points,
	item_get_weight,
	item_get_value,
	item_get_difficulty,
	item_get_head_armor,
	item_get_body_armor,
	item_get_leg_armor,
	item_get_hit_points,
	item_get_weapon_length,
	item_get_speed_rating,
	item_get_missile_speed,
	item_get_max_ammo,
	item_get_accuracy,
	item_get_shield_width,
	item_get_shield_height,
	item_get_horse_scale,
	item_get_horse_speed,
	item_get_horse_maneuver,
	item_get_food_quality,
	item_get_abundance,
	item_get_thrust_damage,
	item_get_thrust_damage_type,
	item_get_swing_damage,
	item_get_swing_damage_type,
	item_get_horse_charge_damage,
	party_stack_get_experience,
	party_stack_get_num_upgradeable,
	position_get_vector_to_position,
	options_get_battle_size,
	options_get_verbose_casualties,
	options_get_cheat_mode,
	options_get_realistic_headshots,
	fst,
	edit_mode_get_num_selected_prop_instances,
	edit_mode_get_selected_prop_instance,
	edit_mode_get_highlighted_prop_instance,
	menu_create_new,
	overlay_get_val,
]

can_fail_operations += [
	is_vanilla_warband,
	str_equals,
	str_contains,
	str_starts_with,
	str_ends_with,
	str_is_alpha,
	str_is_digit,
	str_is_whitespace,
	str_is_integer,
	item_slot_gt,
	party_template_slot_gt,
	troop_slot_gt,
	faction_slot_gt,
	quest_slot_gt,
	scene_slot_gt,
	party_slot_gt,
	player_slot_gt,
	team_slot_gt,
	agent_slot_gt,
	scene_prop_slot_gt,
	order_flag_is_active,
	key_released,
	game_key_released,
	dict_is_empty,
	dict_has_key,
	camera_in_first_person,
	cast_ray,
	troop_has_flag,
	item_has_property,
	item_has_capability,
	item_has_modifier,
	item_has_faction,
	party_has_flag,
	feq,
	fgt,
	flt,
	fge,
	fle,
	edit_mode_in_edit_objects_mode,
	presentation_activate,
]
