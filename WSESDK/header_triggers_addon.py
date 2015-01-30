ti_on_agent_hit = -28.0 #can only be used in module_mission_templates triggers
# Trigger Param 1: receiver agent no
# Trigger Param 2: dealer agent no
# Trigger Param 3: inflicted damage
# Trigger Param 4: raw damage (before being soaked by armor)
# Trigger Param 5: hit bone
# Trigger Param 6: item kind no
# Trigger Param 7: item modifier
# Trigger Param 8: missile item kind no
# Trigger Param 9: missile item modifier
# Trigger Param 10: damage type
# Position Register 0: position of the blow
#                      rotation gives the direction of the blow
# Trigger Result: if set, damage dealt to agent

ti_on_scene_prop_hit = -42.0 #can only be used in module_scene_props triggers
# Trigger Param 1: prop instance no
# Trigger Param 2: inflicted damage (server only)
# Trigger Param 3: dealer agent no (server only)
# Trigger Param 4: item kind no (server only)
# Trigger Param 5: item modifier (server only)
# Trigger Param 6: missile item kind no (server only)
# Trigger Param 7: missile item modifier (server only)
# Position Register 1: Hit Position
# Position Register 2: x holds dealer agent no

ti_on_scene_prop_stepped_on = -100.0 #can only be used in module_scene_props triggers
# Trigger Param 1: agent no
# Trigger Param 2: prop instance no

ti_on_init_missile = -101.0 #can only be used in module_items triggers
# Trigger Param 1: shooter agent no
# Trigger Param 2: launcher item kind no
# Trigger Param 3: launcher item modifier
# Trigger Param 4: missile item kind no
# Trigger Param 5: missile item modifier

ti_on_shield_hit = -103.0 #can only be used in module_items triggers
# Trigger Param 1: receiver agent no
# Trigger Param 2: dealer agent no
# Trigger Param 3: inflicted damage
# Trigger Param 4: raw damage (before shield skill and skill resistance bonuses)
# Trigger Param 5: item kind no
# Trigger Param 6: item modifier
# Trigger Param 7: missile item kind no
# Trigger Param 8: missile item modifier
# Trigger Result: if set, damage dealt to shield

ti_on_missile_dive = -104.0 #can only be used in module_items triggers
# Trigger Param 1: shooter agent no
# Trigger Param 2: launcher item kind no
# Trigger Param 3: launcher item modifier
# Trigger Param 4: missile item kind no
# Trigger Param 5: missile item modifier
# Position Register 0: water impact position and rotation

ti_on_agent_start_reloading = -105.0 #can only be used in module_mission_templates triggers
# Trigger Param 1: agent no

ti_on_agent_end_reloading = -106.0 #can only be used in module_mission_templates triggers
# Trigger Param 1: agent no
