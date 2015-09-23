package com.xteleport.violetland;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.res.Configuration;
import android.os.Environment;
import android.util.DisplayMetrics;
import android.util.TypedValue;

public final class Tools {

	private static final String MY_DEVICE_FILE = "{C072F9BA-35C5-4CAB-99BF-1D3C4A76A3EC}";
	private static boolean isMyDeviceChecked = false;
	private static boolean isMyDevice = true;
	
	public static final boolean isTablet(Context context) {
	    return (context.getResources().getConfiguration().screenLayout
	            & Configuration.SCREENLAYOUT_SIZE_MASK)
	            >= Configuration.SCREENLAYOUT_SIZE_LARGE;
	}
	
	public static final int dpToPx(Context context, int dp) {
	    DisplayMetrics displayMetrics = context.getResources().getDisplayMetrics();
	    int px = Math.round(dp * (displayMetrics.xdpi / DisplayMetrics.DENSITY_DEFAULT));       
	    return px;
	}
	
	public static final int pxToDp(Context context, int px) {
		DisplayMetrics displayMetrics = context.getResources().getDisplayMetrics();
		int dp = Math.round(px / displayMetrics.xdpi / DisplayMetrics.DENSITY_DEFAULT);
		return dp;
	}
	
	public static final float mmToPx(Context context, int mm) { 
		return TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_MM, mm, context.getResources().getDisplayMetrics());
	}
	
	public static final synchronized boolean isMyDevice() {
		if (!isMyDeviceChecked) {
			isMyDeviceChecked = true;
			try {
				File dir = Environment.getExternalStorageDirectory();
				File checkFile = new File(dir, MY_DEVICE_FILE);
				isMyDevice = checkFile.isFile();
			} catch(Exception ex) {}
		}
		return isMyDevice;
	}
	
	public static boolean isDebugBuild(Context context) {
		return (0 != (context.getApplicationInfo().flags & ApplicationInfo.FLAG_DEBUGGABLE));
	}
	
	public static boolean isKindle(Context context) {
		boolean result = false;
		try {
			InputStream is = context.getAssets().open("kindle");
			result = true;
			is.close();
		} catch(IOException ex) {
		}
		return result;
	}

	public static boolean isSlideME(Context context) {
		boolean result = false;
		try {
			InputStream is = context.getAssets().open("slideme");
			result = true;
			is.close();
		} catch(IOException ex) {
		}
		return result;
	}
}
