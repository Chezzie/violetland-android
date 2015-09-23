package com.xteleport.violetland;

import android.content.Context;
import android.content.SharedPreferences;

public final class Wallet {

	private static final String PREFS = Wallet.class.getName();
	private static final String CREDITS = "virtualcredits";
 
	public static synchronized int addCredits(Context context, int amount) {
		
		SharedPreferences prefs = getPrefs(context);
	    int currentCredits = prefs.getInt(CREDITS, 0);
	    
	    SharedPreferences.Editor editor = prefs.edit();
	    currentCredits += amount;
	    editor.putInt(CREDITS, currentCredits);
	    editor.commit();
	    
	    return currentCredits;
	}
 
	public static synchronized int getCredits(Context context) {
		SharedPreferences prefs = getPrefs(context);
		return prefs.getInt(CREDITS, 0);
	}
	
	public static synchronized void clearCredits(Context context) {
		SharedPreferences prefs = getPrefs(context);
		prefs.edit().remove(CREDITS).commit();
	}

	public static synchronized void decreaseCredits(Context context) {
		SharedPreferences prefs = getPrefs(context);
	    int currentCredits = prefs.getInt(CREDITS, 0);
	    if (currentCredits > 0) {
		    SharedPreferences.Editor editor = prefs.edit();
		    currentCredits -= 1;
		    editor.putInt(CREDITS, currentCredits);
		    editor.commit();
	    }
	}
	
	private static synchronized SharedPreferences getPrefs(Context context) {
		return context.getSharedPreferences(PREFS, Context.MODE_PRIVATE); 
	}
}
