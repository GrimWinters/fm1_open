/* SPDX-License-Identifier: GPL-3.0-only WITH GPL-3.0-linking-exception */
/* Brickwave Edition: the SDK functions that the normal firmware's pages,
 * projects and presets use. The edition leaves those out, so nothing would
 * call these; the linker would then drop them and parts of the instrument
 * they reach, and sdk/Build.ps1 rejects a firmware without them (it checks
 * the audio callbacks and runtime parts). Listing them here keeps exactly
 * what the normal firmware keeps. No headers on purpose: only addresses are
 * taken, the prototypes do not matter.
 *
 * Generated from the normal build's references into libfm1_engine.a. */
#ifdef FM1_BRICKWAVE
void drum808_nparams(void);
void drum808_param(void);
void fm1_ble_connect(void);
void fm1_ble_disconnect(void);
void fm1_ble_forget_bonds(void);
void fm1_ble_get_device(void);
void fm1_ble_get_status(void);
void fm1_ble_scan(void);
void fm1_ble_set_role(void);
void fm1_bluetooth_configure(void);
void fm1_bluetooth_get_status(void);
void fm1_control_request(void);
void fm1_engine_stats_read(void);
void fm1_flash_erase(void);
void fm1_flash_invalidate(void);
void fm1_flash_map(void);
void fm1_flash_read(void);
void fm1_flash_size(void);
void fm1_flash_write(void);
void fm1_midi_input_source(void);
void groove_adsr_edit(void);
void groove_adsr_enable(void);
void groove_arp_edit(void);
void groove_arp_set(void);
void groove_arp_setting(void);
void groove_arrangement_capture(void);
void groove_arrangement_tuning_apply(void);
void groove_arrangement_tuning_capture(void);
void groove_audio_stats(void);
void groove_clock_set(void);
void groove_control_edit(void);
void groove_delay_ticks(void);
void groove_drum_steps(void);
void groove_edit_checkpoint(void);
void groove_edit_clear(void);
void groove_edit_clipboard_count(void);
void groove_edit_copy(void);
void groove_edit_cursor(void);
void groove_edit_error(void);
void groove_edit_paste(void);
void groove_edit_record(void);
void groove_edit_recording(void);
void groove_edit_repeat(void);
void groove_edit_seek(void);
void groove_edit_undo(void);
void groove_effects_edit(void);
void groove_effects_get(void);
void groove_fm_copy(void);
void groove_fm_edit(void);
void groove_fm_import(void);
void groove_fm_name_edit(void);
void groove_get_view(void);
void groove_key_note(void);
void groove_lfo_copy(void);
void groove_lfo_edit(void);
void groove_lfo_rate(void);
void groove_lock_parameter_name(void);
void groove_master_state(void);
void groove_midi_activity(void);
void groove_midi_input_set(void);
void groove_mix_toggle(void);
void groove_mono_set(void);
void groove_operator_toggle(void);
void groove_pad_index(void);
void groove_pad_label(void);
void groove_pad_name(void);
void groove_pad_note(void);
void groove_project_apply(void);
void groove_project_apply_edit(void);
void groove_project_arp_defaults(void);
void groove_project_clean(void);
void groove_project_dirty(void);
void groove_project_touch(void);
void groove_project_valid(void);
void groove_samples_stop(void);
void groove_sends_edit(void);
void groove_sends_get(void);
void groove_sequence_chord_name(void);
void groove_sequence_count_in(void);
void groove_sequence_defaults(void);
void groove_sequence_edit(void);
void groove_sequence_length(void);
void groove_sequence_length_set(void);
void groove_sequence_rewound(void);
void groove_sequence_setting(void);
void groove_sequence_setting_edit(void);
void groove_source_set(void);
void groove_source_type(void);
void groove_step_setting(void);
void groove_step_setting_edit(void);
void groove_tempo_edit(void);
void groove_tempo_tap(void);
void groove_timbre_export(void);
void groove_timbre_import(void);
void groove_tone_copy(void);
void groove_tone_count(void);
void groove_tone_edit(void);
void groove_tone_parameter(void);
void groove_track_role_set(void);
void groove_tuning_apply(void);
void groove_tuning_capture(void);
void sampledrum_bind(void);
void sampledrum_param(void);

const void *const brickwave_keep[] = {
    (const void *)drum808_nparams,
    (const void *)drum808_param,
    (const void *)fm1_ble_connect,
    (const void *)fm1_ble_disconnect,
    (const void *)fm1_ble_forget_bonds,
    (const void *)fm1_ble_get_device,
    (const void *)fm1_ble_get_status,
    (const void *)fm1_ble_scan,
    (const void *)fm1_ble_set_role,
    (const void *)fm1_bluetooth_configure,
    (const void *)fm1_bluetooth_get_status,
    (const void *)fm1_control_request,
    (const void *)fm1_engine_stats_read,
    (const void *)fm1_flash_erase,
    (const void *)fm1_flash_invalidate,
    (const void *)fm1_flash_map,
    (const void *)fm1_flash_read,
    (const void *)fm1_flash_size,
    (const void *)fm1_flash_write,
    (const void *)fm1_midi_input_source,
    (const void *)groove_adsr_edit,
    (const void *)groove_adsr_enable,
    (const void *)groove_arp_edit,
    (const void *)groove_arp_set,
    (const void *)groove_arp_setting,
    (const void *)groove_arrangement_capture,
    (const void *)groove_arrangement_tuning_apply,
    (const void *)groove_arrangement_tuning_capture,
    (const void *)groove_audio_stats,
    (const void *)groove_clock_set,
    (const void *)groove_control_edit,
    (const void *)groove_delay_ticks,
    (const void *)groove_drum_steps,
    (const void *)groove_edit_checkpoint,
    (const void *)groove_edit_clear,
    (const void *)groove_edit_clipboard_count,
    (const void *)groove_edit_copy,
    (const void *)groove_edit_cursor,
    (const void *)groove_edit_error,
    (const void *)groove_edit_paste,
    (const void *)groove_edit_record,
    (const void *)groove_edit_recording,
    (const void *)groove_edit_repeat,
    (const void *)groove_edit_seek,
    (const void *)groove_edit_undo,
    (const void *)groove_effects_edit,
    (const void *)groove_effects_get,
    (const void *)groove_fm_copy,
    (const void *)groove_fm_edit,
    (const void *)groove_fm_import,
    (const void *)groove_fm_name_edit,
    (const void *)groove_get_view,
    (const void *)groove_key_note,
    (const void *)groove_lfo_copy,
    (const void *)groove_lfo_edit,
    (const void *)groove_lfo_rate,
    (const void *)groove_lock_parameter_name,
    (const void *)groove_master_state,
    (const void *)groove_midi_activity,
    (const void *)groove_midi_input_set,
    (const void *)groove_mix_toggle,
    (const void *)groove_mono_set,
    (const void *)groove_operator_toggle,
    (const void *)groove_pad_index,
    (const void *)groove_pad_label,
    (const void *)groove_pad_name,
    (const void *)groove_pad_note,
    (const void *)groove_project_apply,
    (const void *)groove_project_apply_edit,
    (const void *)groove_project_arp_defaults,
    (const void *)groove_project_clean,
    (const void *)groove_project_dirty,
    (const void *)groove_project_touch,
    (const void *)groove_project_valid,
    (const void *)groove_samples_stop,
    (const void *)groove_sends_edit,
    (const void *)groove_sends_get,
    (const void *)groove_sequence_chord_name,
    (const void *)groove_sequence_count_in,
    (const void *)groove_sequence_defaults,
    (const void *)groove_sequence_edit,
    (const void *)groove_sequence_length,
    (const void *)groove_sequence_length_set,
    (const void *)groove_sequence_rewound,
    (const void *)groove_sequence_setting,
    (const void *)groove_sequence_setting_edit,
    (const void *)groove_source_set,
    (const void *)groove_source_type,
    (const void *)groove_step_setting,
    (const void *)groove_step_setting_edit,
    (const void *)groove_tempo_edit,
    (const void *)groove_tempo_tap,
    (const void *)groove_timbre_export,
    (const void *)groove_timbre_import,
    (const void *)groove_tone_copy,
    (const void *)groove_tone_count,
    (const void *)groove_tone_edit,
    (const void *)groove_tone_parameter,
    (const void *)groove_track_role_set,
    (const void *)groove_tuning_apply,
    (const void *)groove_tuning_capture,
    (const void *)sampledrum_bind,
    (const void *)sampledrum_param
};
const unsigned brickwave_keep_count = sizeof brickwave_keep / sizeof brickwave_keep[0];
#endif
