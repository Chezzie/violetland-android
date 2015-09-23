package com.xteleport.violetland;

import mp.MpUtils;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;

public class PaymentStatusReceiver extends BroadcastReceiver {
	
	public static final String TAG = "AXT_Payment";
	
	@Override
	public void onReceive(Context context, Intent intent) {
		Bundle extras = intent.getExtras(); 
		if (extras == null) {
			return;
		}
		
	    /* Log.d(TAG, "- billing_status:  " + extras.getInt("billing_status"));
	    Log.d(TAG, "- credit_amount:   " + extras.getString("credit_amount"));
	    Log.d(TAG, "- credit_name:     " + extras.getString("credit_name"));
	    Log.d(TAG, "- message_id:      " + extras.getString("message_id") );
	    Log.d(TAG, "- payment_code:    " + extras.getString("payment_code"));
	    Log.d(TAG, "- price_amount:    " + extras.getString("price_amount"));
	    Log.d(TAG, "- price_currency:  " + extras.getString("price_currency"));
	    Log.d(TAG, "- product_name:    " + extras.getString("product_name"));
	    Log.d(TAG, "- service_id:      " + extras.getString("service_id"));
	    Log.d(TAG, "- user_id:         " + extras.getString("user_id")); */

	    int billingStatus = extras.getInt("billing_status");
	    if (billingStatus == MpUtils.MESSAGE_STATUS_BILLED || billingStatus == MpUtils.MESSAGE_STATUS_FAILED) {
	    	int amount = -1;
    	    String s_amount = extras.getString("credit_amount");
    	    if (s_amount != null && !s_amount.isEmpty()) {
	    	    s_amount = s_amount.replaceAll(",", ".");
	    	    try {
	    	    	double d_amount = Double.parseDouble(s_amount);
	    	    	amount = (int)Math.round(d_amount);
	    	    } catch(NumberFormatException ex) {
	    	    }
    	    }
	    	if (billingStatus == MpUtils.MESSAGE_STATUS_BILLED) {
	    	    if (amount > 0) {
	    	    	Wallet.addCredits(context, amount);
	    	    }
	    	}
	    }
	}
}
