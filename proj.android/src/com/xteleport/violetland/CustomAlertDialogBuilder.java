package com.xteleport.violetland;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.graphics.Color;
import android.graphics.Typeface;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewParent;
import android.widget.Button;
import android.widget.TextView;

public final class CustomAlertDialogBuilder extends AlertDialog.Builder {
	
	private final Typeface mTypeface;
	private final boolean noColorButtons;
	
	public CustomAlertDialogBuilder(Context context, Typeface tf) {
		super(context);
		this.mTypeface = tf;
		this.noColorButtons = false;
	}

	public CustomAlertDialogBuilder(Context context, Typeface tf, boolean noColorButtons, int style) {
		super(context, style);
		this.mTypeface = tf;
		this.noColorButtons = noColorButtons;
	}
	
	@Override
	public AlertDialog show() {
		AlertDialog dialog = super.show();
		try {
			ViewGroup vg = getContentView(dialog);
			if (vg != null) {
				rcurlyChangeTypefaceAndColor(dialog, vg);
			}
		} catch(Throwable t) {
		}
		return dialog;
	}
	
	private final void rcurlyChangeTypefaceAndColor(AlertDialog dialog, ViewGroup vg) {
		int childCount = vg.getChildCount();
		for (int i = 0; i < childCount; ++i) {
			View v = vg.getChildAt(i);
			if (v instanceof ViewGroup) {
				rcurlyChangeTypefaceAndColor(dialog, (ViewGroup)v);
			} else if (v instanceof TextView) {
				TextView tv = (TextView)v;
				if (mTypeface != null) {
					tv.setTypeface(mTypeface);
				}
				if (tv instanceof Button) {
					if (!noColorButtons) {
						if (tv == dialog.getButton(DialogInterface.BUTTON_POSITIVE)) {
							tv.setTextColor(Color.YELLOW);
						} else if (tv == dialog.getButton(DialogInterface.BUTTON_NEGATIVE)) {
							tv.setTextColor(Color.RED);
						} else {
							tv.setTextColor(Color.LTGRAY);
						}
					}
				} else {
					tv.setTextColor(Color.rgb(0xFF, 0x99, 0x99));
				}
			}
		}
	}
		
	private final ViewGroup getContentView(AlertDialog dialog) {
		View button = dialog.getButton(DialogInterface.BUTTON1);
		if (button == null) {
			button = dialog.getButton(DialogInterface.BUTTON2);
		}
		if (button == null) {
			button = dialog.getButton(DialogInterface.BUTTON3);
		}
		if (button == null) {
			return null;
		}
		ViewParent vpUsed = null;
		ViewParent vp = null;
		View cur = button;
		do {
			vpUsed = vp;
			vp = cur.getParent();
			if (vp instanceof ViewGroup) {
				cur = (View)vp;
			} else {
				vp = null;
			}
		} while (vp != null);
		
		if (vpUsed instanceof ViewGroup) {
			return (ViewGroup)vpUsed;
		} else {
			return null;
		}
	}
}