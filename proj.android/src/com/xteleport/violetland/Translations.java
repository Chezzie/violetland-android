package com.xteleport.violetland;

import java.util.HashMap;

import android.content.Context;

final class Translations
{
	private final HashMap<String, Integer> mResMap = new HashMap<String, Integer>();
	
	public Translations() {
		mResMap.put("Accuracy deviation modifier: %i%%", R.string.accuracy_deviation_modifier___i__);
	    mResMap.put("Agility: %i", R.string.agility___i);
	    mResMap.put("All have been frozen around you.", R.string.all_have_been_frozen_around_you_);
	    mResMap.put("Attack waves", R.string.attack_waves);
	    mResMap.put("Back to main menu", R.string.back_to_main_menu);
	    mResMap.put("Big calibre", R.string.big_calibre);
	    mResMap.put("Big calibre: your bullets can wound a few monsters in a row.", R.string.big_calibre__your_bullets_can_wound_a_few_monsters_in_a_row_);
	    mResMap.put("Bind action %s to %s.\n", R.string.bind_action__s_to__s__n);
	    mResMap.put("Boom!", R.string.boom_);
	    mResMap.put("Chance of block: %i%%", R.string.chance_of_block___i__);
	    mResMap.put("Controls", R.string.controls);
	    mResMap.put("Current player level: %i", R.string.current_player_level___i);
	    mResMap.put("Drawing splash screen...\n", R.string.drawing_splash_screen____n);
	    mResMap.put("Edit Controls", R.string.edit_controls);
	    mResMap.put("Enter your name:", R.string.enter_your_name_);
	    mResMap.put("Exit", R.string.exit);
	    mResMap.put("FPS: %i", R.string.fps___i);
	    mResMap.put("Fire", R.string.fire);
	    mResMap.put("Friendly fire", R.string.friendly_fire);
	    mResMap.put("Fullscreen", R.string.fullscreen);
	    mResMap.put("Gameplay", R.string.gameplay);
	    mResMap.put("Get ready to new wave!", R.string.get_ready_to_new_wave_);
	    mResMap.put("Graphics", R.string.graphics);
	    mResMap.put("Grenades: %i", R.string.grenades___i);
	    mResMap.put("HP: %i / Max HP: %i", R.string.hp___i___max_hp___i);
	    mResMap.put("Health regeneration: %.2f/min", R.string.health_regeneration____2f_min);
	    mResMap.put("Help", R.string.help);
	    mResMap.put("High scores", R.string.high_scores);
	    mResMap.put("Highscores", R.string.highscores);
	    mResMap.put("Hot video mode changing is not supported on windows now. You should restart the game.", R.string.hot_video_mode_changing_is_not_supported_on_windows_now__you_should_restart_the_game_);
	    mResMap.put("Looting", R.string.looting);
	    mResMap.put("Looting: Monsters will drop more bonuses.", R.string.looting__monsters_will_drop_more_bonuses_);
	    mResMap.put("Melee damage: %i", R.string.melee_damage___i);
	    mResMap.put("Menu", R.string.menu);
	    mResMap.put("MenuClickA", R.string.menuclicka);
	    mResMap.put("MenuClickB", R.string.menuclickb);
	    mResMap.put("Move Down", R.string.move_down);
	    mResMap.put("Move Left", R.string.move_left);
	    mResMap.put("Move Right", R.string.move_right);
	    mResMap.put("Move Up", R.string.move_up);
	    mResMap.put("Move mouse over text to get explantation...", R.string.move_mouse_over_text_to_get_explantation___);
	    mResMap.put("Music volume", R.string.music_volume);
	    mResMap.put("Name", R.string.name);
	    mResMap.put("Name found : %s\n", R.string.name_found____s_n);
	    mResMap.put("Night vision", R.string.night_vision);
	    mResMap.put("Night vision: you can see in the dark.", R.string.night_vision__you_can_see_in_the_dark_);
	    mResMap.put("Options", R.string.options);
	    mResMap.put("PAUSE", R.string.pause);
	    mResMap.put("Pause", R.string.pause_);
	    mResMap.put("Perks:", R.string.perks_);
	    mResMap.put("Pick Up", R.string.pick_up);
	    mResMap.put("Please wait...", R.string.please_wait___);
	    mResMap.put("Poison bullets", R.string.poison_bullets);
	    mResMap.put("Poison bullets: after getting hit by your bullet, enemies slowly lose health until they die.", R.string.poison_bullets__after_getting_hit_by_your_bullet__enemies_slowly_lose_health_until_they_die_);
	    mResMap.put("Preparing sound systems...\n", R.string.preparing_sound_systems____n);
	    mResMap.put("Preparing window...\n", R.string.preparing_window____n);
	    mResMap.put("Press a key, please...", R.string.press_a_key__please___);
	    mResMap.put("Reload", R.string.reload);
	    mResMap.put("Reloading speed modifier: %i%%", R.string.reloading_speed_modifier___i__);
	    mResMap.put("Reset Controls", R.string.reset_controls);
	    mResMap.put("Reset list", R.string.reset_list);
	    mResMap.put("Resolution", R.string.resolution);
	    mResMap.put("Restart", R.string.restart);
	    mResMap.put("Resume", R.string.resume);
	    mResMap.put("Save and return", R.string.save_and_return);
	    mResMap.put("Shoot monsters to receive experience and other bonuses.", R.string.shoot_monsters_to_receive_experience_and_other_bonuses_);
	    mResMap.put("Show Char", R.string.show_char);
	    mResMap.put("Sound", R.string.sound);
	    mResMap.put("Sound volume", R.string.sound_volume);
	    mResMap.put("Start", R.string.start);
	    mResMap.put("Str/Agil/Vital", R.string.str_agil_vital);
	    mResMap.put("Strength: %i", R.string.strength___i);
	    mResMap.put("Telekinesis", R.string.telekinesis);
	    mResMap.put("Telekinesis: useful things slowly move towards you.", R.string.telekinesis__useful_things_slowly_move_towards_you_);
	    mResMap.put("Teleport", R.string.teleport);
	    mResMap.put("Teleports: %i", R.string.teleports___i);
	    mResMap.put("They have overcome...", R.string.they_have_overcome___);
	    mResMap.put("Throw Grenade", R.string.throw_grenade);
	    mResMap.put("Time", R.string.time);
	    mResMap.put("Toggle Laser", R.string.toggle_laser);
	    mResMap.put("Toggle Light", R.string.toggle_light);
	    mResMap.put("Try to survive as long as you can.", R.string.try_to_survive_as_long_as_you_can_);
	    mResMap.put("Unknown", R.string.unknown);
	    mResMap.put("Unknown event", R.string.unknown_event);
	    mResMap.put("Unstoppable", R.string.unstoppable);
	    mResMap.put("Unstoppable: enemies can't block your movement any more, but they still can hurt you.", R.string.unstoppable__enemies_can_t_block_your_movement_any_more__but_they_still_can_hurt_you_);
	    mResMap.put("Violetland Survival", R.string.violetland_survival);
	    mResMap.put("Vitality: %i", R.string.vitality___i);
	    mResMap.put("Weapon autoreloading", R.string.weapon_autoreloading);
	    mResMap.put("Weapon autotaking", R.string.weapon_autotaking);
	    mResMap.put("Wide sight", R.string.wide_sight);
	    mResMap.put("Wide sight: accessible area for action is much more.", R.string.wide_sight__accessible_area_for_action_is_much_more_);
	    mResMap.put("XP", R.string.xp);
	    mResMap.put("You are gobbled up.", R.string.you_are_gobbled_up_);
	    mResMap.put("You got a agility boost.", R.string.you_got_a_agility_boost_);
	    mResMap.put("You got a strength boost.", R.string.you_got_a_strength_boost_);
	    mResMap.put("You got a vitality boost.", R.string.you_got_a_vitality_boost_);
	    mResMap.put("You got powerful penetration bullets.", R.string.you_got_powerful_penetration_bullets_);
	    mResMap.put("You have earned %i points.", R.string.you_have_earned__i_points_);
	    mResMap.put("You have reached new level.", R.string.you_have_reached_new_level_);
	    mResMap.put("You have taken a grenade.", R.string.you_have_taken_a_grenade_);
	    mResMap.put("You have taken a medical kit.", R.string.you_have_taken_a_medical_kit_);
	    mResMap.put("You have taken a teleport.", R.string.you_have_taken_a_teleport_);
	    mResMap.put("You have taken the %s.", R.string.you_have_taken_the__s_);
	    mResMap.put("a hand grenade", R.string.a_hand_grenade);
	    mResMap.put("a medikit", R.string.a_medikit);
	    mResMap.put("a nitrogen bomb", R.string.a_nitrogen_bomb);
	    mResMap.put("a teleport", R.string.a_teleport);
	    mResMap.put("agility boost", R.string.agility_boost);
	    mResMap.put("nuke!", R.string.nuke_);
	    mResMap.put("penetration bullets", R.string.penetration_bullets);
	    mResMap.put("strength boost", R.string.strength_boost);
	    mResMap.put("vitality boost", R.string.vitality_boost);
	
	    mResMap.put("Available perk points: %i", R.string.available_perk_points___i);
	    mResMap.put("Available perk points: %i - %i", R.string.available_perk_points___i___i);
	    mResMap.put("Highscore!!!", R.string.highscore___);
	    mResMap.put("Strong", R.string.strong_);
	    mResMap.put("Fast", R.string.fast_);
	    mResMap.put("Healthy", R.string.healthy_);
	    mResMap.put("Regular", R.string.regular_);
	    mResMap.put("About", R.string.about);
	    mResMap.put("Press Back key to continue.", R.string.press_back_key_to_continue__);
	    mResMap.put("Purchase perk points", R.string.purchase_perk_points);
	}
	
	public String getString(Context context, String key) {
		Integer resId = mResMap.get(key);
		if (resId == null) {
			return key;
		}
		
		String result = null;
		try {
			result = context.getString(resId);
		} catch(Exception ex) {};
		if (result == null) {
			return key;
		}
		
		return result;
	}
}
