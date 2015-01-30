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
		return (int)overlay->get_value_f();
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
}
