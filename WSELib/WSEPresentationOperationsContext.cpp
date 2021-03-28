#include "WSEPresentationOperationsContext.h"

#include "WSE.h"
#include "warband.h"

int OverlayGetValue(WSEPresentationOperationsContext *context)
{
#if defined WARBAND
	int overlay_no;

	context->ExtractOverlayNo(overlay_no);

	rgl::widget *overlay = warband->cur_presentation->overlays[overlay_no];

	switch (overlay->get_type())
	{
	case rgl::wt_slider:
	case rgl::wt_fill_slider:
		return rglRound(overlay->get_value_f());
	case rgl::wt_combo_button:
	case rgl::wt_number_box:
	case rgl::wt_list_box:
	case rgl::wt_combo_label:
		return overlay->get_value_i();
	}

	return -1;
#else
	return -1;
#endif
}

bool PresentationActivate(WSEPresentationOperationsContext *context)
{
#if defined WARBAND
	int presentation_no;

	context->ExtractPresentationNo(presentation_no);

	if (warband->game_screen.open_windows.back() != wb::gwt_tactical)
		return false;

	wb::tactical_window *tactical_window = (wb::tactical_window *)warband->game_screen.game_windows[wb::gwt_tactical];

	for (int i = 0; i < tactical_window->presentations.size(); ++i)
	{
		if (tactical_window->presentations[i]->presentation_no == presentation_no)
		{
			warband->cur_presentation = tactical_window->presentations[i];
			return true;
		}
	}

	return false;
#else
	return false;
#endif
}

void OverlayButtonSetType(WSEPresentationOperationsContext *context)
{
#if defined WARBAND
	int overlay_no;
	bool showed, enabled;

	context->ExtractOverlayNo(overlay_no);
	context->ExtractBoolean(showed);
	context->ExtractBoolean(enabled);

	rgl::widget *overlay = warband->cur_presentation->overlays[overlay_no];

	if (overlay->get_type() == rgl::wt_simple_button || overlay->get_type() == rgl::wt_image_button || overlay->get_type() == rgl::wt_game_button)
		overlay->flags = (showed ? 0 : rgl::wf_hidden) | (enabled ? 0 : rgl::wf_disabled);
#endif
}

int OverlayGetScrollPos(WSEPresentationOperationsContext *context)
{
#if defined WARBAND
	int overlay_no;

	context->ExtractOverlayNo(overlay_no);

	rgl::scrollable_widget *overlay = (rgl::scrollable_widget *)warband->cur_presentation->overlays[overlay_no];

	if (overlay->get_type() == rgl::wt_scrollable)
	{
		rgl::scrollbar_widget *scrollbar = overlay->scrollbar_widgets[1];

		return (int)((scrollbar->value_f - scrollbar->value_f_min) / (scrollbar->value_f_max - scrollbar->value_f_min) * (scrollbar->upper_right_offset[scrollbar->direction] - scrollbar->range) * warband->basic_game.fixed_point_multiplier);
	}
	else
	{
		return -1;
	}
#else
	return -1;
#endif
}

void OverlaySetScrollPos(WSEPresentationOperationsContext *context)
{
#if defined WARBAND
	int overlay_no;
	float value;

	context->ExtractOverlayNo(overlay_no);
	context->ExtractFixedPoint(value);

	rgl::scrollable_widget *overlay = (rgl::scrollable_widget *)warband->cur_presentation->overlays[overlay_no];

	if (overlay->get_type() == rgl::wt_scrollable)
	{
		rgl::scrollbar_widget *scrollbar = overlay->scrollbar_widgets[1];

		scrollbar->value_f = rglClamp(value / (scrollbar->upper_right_offset[scrollbar->direction] - scrollbar->range) * (scrollbar->value_f_max - scrollbar->value_f_min) + scrollbar->value_f_min, scrollbar->value_f_min, scrollbar->value_f_max);
	}
#endif
}


WSEPresentationOperationsContext::WSEPresentationOperationsContext() : WSEOperationContext("presentation", 4900, 4999)
{
}

void WSEPresentationOperationsContext::OnLoad()
{
	RegisterOperation("overlay_get_val", OverlayGetValue, Client, Lhs, 2, 2,
		"Stores <1>'s value into <0>",
		"destination", "overlay_no");

	RegisterOperation("presentation_activate", PresentationActivate, Client, Cf, 1, 1,
		"Activates <0>. Fails if <0> is not running",
		"presentation_no");

	RegisterOperation("overlay_button_set_type", OverlayButtonSetType, Client, None, 3, 3,
		"Sets <0>'s <1> and <2>",
		"overlay_no", "toggle_or_not", "deselectable_or_not");

	RegisterOperation("overlay_get_scroll_pos", OverlayGetScrollPos, Client, Lhs, 2, 2,
		"Stores <1>'s scroll pos into <0>",
		"destination_fixed_point", "overlay_no");

	RegisterOperation("overlay_set_scroll_pos", OverlaySetScrollPos, Client, None, 2, 2,
		"Sets <0>'s scroll pos <1>",
		"overlay_no", "value_fixed_point");
}
