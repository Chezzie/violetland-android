package com.xteleport.violetland;

import java.io.InputStream;

import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.webkit.WebView;

public final class AboutActivity extends Activity
{
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		WebView webView = new WebView(this);
		webView.setVerticalScrollBarEnabled(true);
		webView.setBackgroundColor(Color.rgb(0x88, 0x88, 0x88));
		setContentView(webView);
		try {
			byte[] buffer = new byte[4096];
			int length = -1;
			InputStream is = getAssets().open("about.html");
			try {
				length = is.read(buffer);
			} finally {
				is.close();
			}
			if (length <= 0) {
				throw new IllegalStateException();
			}
			String data = new String(buffer, 0, length);
			webView.loadDataWithBaseURL("file:///android_asset/", data, "text/html", "UTF-8", null);
		} catch(Exception e) {
		}
	}
	
}
