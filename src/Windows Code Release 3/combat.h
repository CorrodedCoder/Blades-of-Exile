void start_outdoor_combat(outdoor_creature_type encounter,unsigned char in_which_terrain,short num_walls);
Boolean pc_combat_move(location destination);
void char_parry();
void char_stand_ready();
void pc_attack(short who_att,short target);
short calc_spec_dam(short abil,short abil_str,creature_data_type *monst);
void place_target(location target);
void do_combat_cast(location target);
void handle_marked_damage();
void load_missile();
void fire_missile(location target);
Boolean combat_next_step();
Boolean pick_next_pc();
void combat_run_monst();
void do_monster_turn();
void monster_attack_pc(short who_att,short target);
void monster_attack_monster(short who_att,short attackee);
void monst_fire_missile(short m_num,short skill,short bless,short level,location source,short target);
Boolean monst_breathe(creature_data_type *caster,location targ_space,short dam_type);
Boolean monst_cast_mage(creature_data_type *caster,short targ);
Boolean monst_cast_priest(creature_data_type *caster,short targ);
//void damage_target(short target, short dam, damage_type type);
void damage_target(short target,short dam,short type);
location find_fireball_loc(location where,short radius,short mode,short *m);
location closest_pc_loc(location where);
short count_levels(location where,short radius);
Boolean pc_near(short pc_num,location where,short radius);
Boolean monst_near(short m_num,location where,short radius,short active);
void fireball_space(location loc,short dam);
void place_spell_pattern(effect_pat_type pat,location center,short type,Boolean prep_for_anim,short who_hit);
void handle_item_spell(location loc,short num);
void modify_pattern(effect_pat_type *pat,short type);
void do_shockwave(location target);
void radius_damage(location target,short radius, short dam, short type);
void hit_pcs_in_space(location target,short dam, short type,short report,short hit_all);
//void hit_space(location target,short dam, damage_type type,short report,short hit_all);
void hit_space(location target, short dam, short type, short report, short hit_all);
void do_poison();
void handle_disease();
void handle_acid();
Boolean no_pcs_left();
Boolean hit_end_c_button();
Boolean out_monst_all_dead();
void end_combat();
Boolean combat_cast_mage_spell();
Boolean combat_cast_priest_spell();
void start_spell_targeting(short num);
void start_fancy_spell_targeting(short num);
void spell_cast_hit_return();
void process_fields();
void scloud_space(short m,short n);
void web_space(short m,short n);
void sleep_cloud_space(short m,short n);
void take_m_ap(short num,creature_data_type *monst);
void add_new_action(short pc_num);
short get_monst_sound(creature_data_type *attacker,short which_att);
