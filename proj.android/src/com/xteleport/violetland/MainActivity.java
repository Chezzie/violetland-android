package com.xteleport.violetland;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import mp.MpUtils;
import mp.PaymentRequest;
import mp.PaymentResponse;

import org.libsdl.app.SDLActivity;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentSender.SendIntentException;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.content.res.AssetManager;
import android.graphics.Color;
import android.graphics.Typeface;
import android.hardware.Sensor;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.Handler;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.widget.RelativeLayout;

import com.appodeal.ads.Appodeal;
import com.appodeal.ads.BannerView;
import com.flurry.android.FlurryAgent;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GooglePlayServicesUtil;
import com.google.android.gms.common.SignInButton;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.GamesActivityResultCodes;
import com.google.android.gms.plus.Plus;
import com.xteleport.iab.IabHelper;
import com.xteleport.iab.IabResult;
import com.xteleport.iab.Inventory;
import com.xteleport.iab.Purchase;
import com.xteleport.iab.SkuDetails;

public final class MainActivity extends SDLActivity implements
	GoogleApiClient.ConnectionCallbacks, GoogleApiClient.OnConnectionFailedListener
{
	public static final String TAG = "VIOLET";
	
	private SensorManager senSensorManager;
	private Sensor senAccelerometer;
	
	private static volatile float accel_x;
	private static volatile long highscore;
	private static MainActivity mSingleton;
	
	private GoogleApiClient mGoogleApiClient = null;
	private boolean mIntentInProgress = false;
	private SharedPreferences localPrefs;
	private SignInButton signInButton = null;
	private boolean signInButtonVisible = false;
	private Handler mHandler;
	private boolean isDebugBuild;
	private Typeface mAlertDialogTypeface;
	private RelativeLayout mLayoutOverlay;
	private BannerView mBanner = null;
	
	private static final int RC_SIGN_IN = 123;
	private static final int RC_LEADERBOARD = 124;
	private static final int RC_GP_ERROR_DIALOG = 125;
	private static final int RC_FORTUMO_PAYMENT_REQUEST = 126;
	private static final int RC_IAB_PAYMENT_REQUEST = 127;
	private static final int RC_ABOUT = 127;
	
	private static final String KEY_WAS_SIGNED_IN = "WAS_SIGNED_IN";
	private static boolean googlePlusAgressive;
	private static boolean bannersEnabled;
	private static final boolean BANNERS_TEMPORARY_DISABLED = true;
	
	private IabHelper mIabHelper = null;
	
	private static final int IAB_SKU_1_PERK = 0;
	private static final int IAB_SKU_5_PERKS = 1;
	private static final int IAB_SKU_10_PERKS = 2;
	private static final int IAB_SKU_15_PERKS = 3;
	private static final List<String> IAB_SKU_IDS = Arrays.asList(new String[] { "1_perk", "5_perks", "10_perks", "15_perks" });
	
	private static boolean isKindle = false;
	private static boolean isSlideME = false;
	
    private boolean adsVisible = false;
    private ProgressDialog mProgressDialog = null;
    private boolean purchaseFortumoLocked = false;
    
    private Translations mTranslations;
    
	@Override
	public void onCreate(Bundle savedInstanceState) {
		
		mSingleton = this;
		mTranslations = new Translations();  
		super.onCreate(savedInstanceState);
		
		if (isDebugBuild) {
        	FlurryAgent.setLogEnabled(false);
		}
        FlurryAgent.setVersionName("1.0.5");
        FlurryAgent.init(getApplicationContext(), getString(R.string.flurry_app_id));
        
		mProgressDialog = new ProgressDialog(this);
		mProgressDialog.setIndeterminate(true);
		mProgressDialog.setMessage(getString(R.string.wait_please));
		mProgressDialog.show();
		
		isDebugBuild = Tools.isDebugBuild(this);
		isKindle = Tools.isKindle(this);
		isSlideME = Tools.isSlideME(this);
		
		mLayout.setBackgroundColor(Color.BLACK);
		
		mLayoutOverlay = new RelativeLayout(this);
		mLayoutOverlay.setBackgroundColor(Color.TRANSPARENT);
		addContentView(mLayoutOverlay, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
		
        localPrefs = getSharedPreferences("PS", MODE_PRIVATE);
        mHandler = new Handler();
        
        try {
        	mAlertDialogTypeface = Typeface.createFromAsset(getAssets(), "archangelsk.ttf");
        } catch(Exception e) {
        }
		
        if (!isKindle) {
        	MpUtils.enablePaymentBroadcast(this, Manifest.permission.PAYMENT_BROADCAST_PERMISSION);
        }
		
        mLayout.setBackgroundColor(Color.BLACK);
        
        if (isDebugBuild) {
        	Appodeal.setTesting(true);
        }
        Appodeal.initialize(this, getResources().getString(R.string.appodeal_app_key), Appodeal.BANNER | Appodeal.INTERSTITIAL | Appodeal.VIDEO);
    	
		if (!isKindle) {
        	googlePlusAgressive = localPrefs.getBoolean(KEY_WAS_SIGNED_IN, false);
	        
        	mGoogleApiClient = new GoogleApiClient.Builder(this)
	        	.addConnectionCallbacks(this)
	        	.addOnConnectionFailedListener(this)
	        	.addApi(Plus.API).addScope(Plus.SCOPE_PLUS_LOGIN)
	        	.addApi(Games.API).addScope(Games.SCOPE_GAMES)
	        	.build();
        
	        signInButton = new SignInButton(this);
	        signInButton.setStyle(SignInButton.SIZE_STANDARD, SignInButton.COLOR_LIGHT);
	        signInButton.setVisibility(View.INVISIBLE);
	        signInButton.setOnClickListener(new View.OnClickListener() {
				@Override
				public void onClick(View v) {
					connectGooglePlay();
				}
			});
        
	        RelativeLayout.LayoutParams lp = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
	        lp.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
	        lp.addRule(RelativeLayout.ALIGN_PARENT_TOP);
	        lp.setMargins(0, Tools.dpToPx(this, 10), Tools.dpToPx(this, 10), 0);
	        mLayoutOverlay.addView(signInButton, lp);
        
	        if (!isSlideME) {
	        	initializeIabAndConsumeItems();
	        }
        }
	}
	
    @Override
    public void onPause() {
    	super.onPause();
		// long startTime = System.currentTimeMillis();
		do { // hack to avoid context losing
	    	while (!isInPoll()) {
	    		try {
	    			Thread.sleep(10);
	    		} catch(Exception ex) {}
	    	}
			try {
				Thread.sleep(150);
			} catch(Exception ex) {}
		} while (!isInPoll());
		try {
			Thread.sleep(10);
		} catch(Exception ex) {}
		// long endTime = System.currentTimeMillis();
		// Log.d(TAG, "onPause took " + (endTime - startTime) + "ms.");
    }
    
    @Override
    protected void onResume() {
    	setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        super.onResume();
        Appodeal.onResume(this, Appodeal.BANNER);
    }
    
	@Override
	protected void onStart() {
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		super.onStart();
		FlurryAgent.onStartSession(this);
		if (!isKindle) {
			if (googlePlusAgressive) {
				connectGooglePlay();
			}
			updateSignInButtonVisibility();
		}
	}
	
	@Override
	protected void onStop() {
		FlurryAgent.onEndSession(this);
		if (!isKindle) {
			googlePlusAgressive |= isGoogleClientSignedIn();
	    	localPrefs.edit().putBoolean(KEY_WAS_SIGNED_IN, googlePlusAgressive).commit();
	        if (mGoogleApiClient.isConnected()) {
	            mGoogleApiClient.disconnect();
	        }
		}
		super.onStop();
	}
	
    @Override
    protected void onDestroy() {
    	Log.d(TAG, "MainActivity.onDestroy()");
    	if (!isKindle) {
	    	if (mIabHelper != null) {
	    		mIabHelper.dispose();
	    		mIabHelper = null;
	    	}
    	}
    	super.onDestroy();
    	mSingleton = null;
    }
    
    @Override
    protected String[] getLibraries() {
        return new String[] {
            "SDL2",
            "mikmod",
            "smpeg2",
            "SDL2_image",
            "SDL2_mixer",
            // "SDL2_net",
            "SDL2_ttf",
            "main"
        };
    }
    
    public static final void setSignInButtonVisible(final boolean visible) {
    	if (!isKindle) {
	    	mSingleton.signInButtonVisible = visible;
	    	updateSignInButtonVisibility();
    	}
    }
    
    private static void updateSignInButtonVisibility() {
    	 if (!isKindle) {
	    	mSingleton.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					boolean shouldBeVisible = mSingleton.signInButtonVisible && !mSingleton.mIntentInProgress;
					if (shouldBeVisible && isGoogleClientSignedIn()) {
						shouldBeVisible = false;
					}
					SignInButton button = mSingleton.signInButton;
					boolean isNowVisible = button.getVisibility() == View.VISIBLE;
					// Log.d("SignInButton state transition: %d", isNowVisible + " -> " + shouldBeVisible);
					if (isNowVisible != shouldBeVisible) {
						button.setVisibility(shouldBeVisible ? View.VISIBLE : View.INVISIBLE);
					}
				}
			});
    	}
    }
    
	public static final String getDataPath() {
		File dir = mSingleton.getFilesDir();
		int version = -1;
		File vers = new File(dir, "vers.txt");
		try {
			FileInputStream fis = new FileInputStream(vers);
			try {
				version = fis.read();
			} finally {
				fis.close();
			}
		} catch(IOException ex) {
		}
		if ((char)version != '1') {
			byte[] buffer = new byte[1024 * 64];
			AssetManager am = mSingleton.getAssets();
			try {
				InputStream is = am.open("assets.zip");
				try {
					ZipInputStream zi = new ZipInputStream(is);
					try {
						ZipEntry z;
						while ((z = zi.getNextEntry()) != null) {
							String name = z.getName();
							if (z.isDirectory()) {
								new File(dir, name).mkdirs();
							} else {
								File f = new File(dir, name);
								File fdir = f.getParentFile();
								if (fdir != null) {
									fdir.mkdirs();
								}
								FileOutputStream fout = new FileOutputStream(f);
								try {
									do {
										int nread = zi.read(buffer);
										if (nread >= 0) {
											fout.write(buffer, 0, nread);
										} else {
											break;
										}
									} while (true);
								} finally {
									fout.close();
								}
							}
						}
					} finally {
						zi.close();
					}
				} finally {
					is.close();
				}
			} catch(IOException ex) {
				Log.e(TAG, "Cannot unpack assets!", ex);
				new File(dir, "vers.txt").delete();
				mSingleton.runOnUiThread(new Runnable() {
					@Override
					public void run() {
						if (mSingleton.mProgressDialog != null) {
							mSingleton.mProgressDialog.dismiss();
							mSingleton.mProgressDialog = null;
						}
						newAlertDialogBuilder(true)
							.setMessage("Not enough space in device memory to run application. Please clear space and try to run application again.")
							.setPositiveButton("OK...", null)
							.show()
							.setOnDismissListener(new DialogInterface.OnDismissListener() {
								@Override
								public void onDismiss(DialogInterface dialog) {
									System.exit(1);
								}
							});
					}
				});
				try {
					while (true) {
						Thread.sleep(10000);
					}
				} catch(Exception ex2) {
				}
			}
		}

		if (mSingleton.mProgressDialog != null) {
			mSingleton.mProgressDialog.dismiss();
			mSingleton.mProgressDialog = null;
		}
		
		return dir.getAbsolutePath();
	}

	@Override
	protected void onActivityResult(int requestCode, int responseCode, Intent intent) {
		super.onActivityResult(requestCode, responseCode, intent);
		if (requestCode == RC_SIGN_IN) {
			mIntentInProgress = false;
			if (responseCode == RESULT_OK) {
				if (!mGoogleApiClient.isConnecting() && !mGoogleApiClient.isConnected()) {
					mGoogleApiClient.connect();
				}
			} else {
				 BaseGameUtils.showActivityResultError(this, requestCode, responseCode, R.string.signin_other_error);
				 updateSignInButtonVisibility();
			}
		} else if (requestCode == RC_LEADERBOARD) {
			leaderboardLocked = false;
			if (responseCode == GamesActivityResultCodes.RESULT_RECONNECT_REQUIRED) {
				mGoogleApiClient.disconnect();
				localPrefs.edit().putBoolean(KEY_WAS_SIGNED_IN, false).commit();
				updateSignInButtonVisibility();
			}
		} else if (requestCode == RC_FORTUMO_PAYMENT_REQUEST) {
			purchaseFortumoLocked = false;
			if (intent != null && responseCode == RESULT_OK) {
				PaymentResponse response = new PaymentResponse(intent);
				int billingStatus = response.getBillingStatus();
				switch (billingStatus) {
				case MpUtils.MESSAGE_STATUS_NOT_SENT: {
						newAlertDialogBuilder()
							.setTitle(R.string.error)
							.setMessage(R.string.we_cannot_process_your_purchase)
							.setNegativeButton("OK", null)
							.show();
						break;
					}
				};
			}
		} else if (requestCode == RC_ABOUT) {
			mShowAboutLocked = false;
		} else if (mIabHelper != null && mIabHelper.handleActivityResult(requestCode, responseCode, intent)) {
		}
	}
	
	// Google APIs
	public static final int mmToPx(int mm) {
		float px = Tools.mmToPx(mSingleton, mm);
		return (int)Math.round(px);
	}
	
 	// Google callbacks
	@Override
	public void onConnectionFailed(ConnectionResult result) {
		 Log.e(TAG, "onConnectionFailed: " + result);
		 if (!mIntentInProgress && result.hasResolution()) {
			 try {
				 mIntentInProgress = true;
				 startIntentSenderForResult(result.getResolution().getIntentSender(), RC_SIGN_IN, null, 0, 0, 0);
			 } catch (SendIntentException e) {
 				 // The intent was canceled before it was sent.  Return to the default
 				 // state and attempt to connect to get an updated ConnectionResult.
			     mIntentInProgress = false;
			     mGoogleApiClient.connect();
			 }
		}
		updateSignInButtonVisibility();
	}

	@Override
	public void onConnected(Bundle arg0) {
		Log.i(TAG, "onConnected: " + arg0);
		updateSignInButtonVisibility();
		MpUtils.fetchPaymentData(this, getResources().getString(R.string.fortumo_service_id), getResources().getString(R.string.fortumo_in_app_secret));
		postLocalHighScores();
	}

	@Override
	public void onConnectionSuspended(int arg0) {
		Log.i(TAG, "onConnectionSuspended: " + arg0);
		updateSignInButtonVisibility();
		mGoogleApiClient.connect();
	}
	
    public static final boolean isGoogleClientSignedIn() {
        return (mSingleton.mGoogleApiClient != null && mSingleton.mGoogleApiClient.isConnected());
    }

    private boolean mShowAboutLocked = false;
	public static final void showAbout() {
		if (!mSingleton.mShowAboutLocked) {
			mSingleton.mShowAboutLocked = true;
			mSingleton.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					Intent intent = new Intent(mSingleton, AboutActivity.class);
					mSingleton.startActivityForResult(intent, RC_ABOUT);
				}
			});
		}
	}
	
	public static final void purchasePoints() {
		mSingleton.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				mSingleton.showPurchaseMethodDialog();
			}
		});
	}
	
	private boolean mShowPurchaseDialogLocked = false;
	public final void showPurchaseMethodDialog() {
		if (mIabHelper != null) {
			if (!mShowPurchaseDialogLocked) {
				mShowPurchaseDialogLocked = true;
				Dialog chooseMethodDlg = newAlertDialogBuilder(true)
					.setTitle(R.string.choose_purchase_method)
					.setMessage(R.string.you_can_purchase_perk_points_using)
					.setNeutralButton("Google", new DialogInterface.OnClickListener() {
						@Override
						public void onClick(DialogInterface dialog, int which) {
							purchaseIab();
						}
					})
					.setPositiveButton(R.string.phone, new DialogInterface.OnClickListener() {
						@Override
						public void onClick(DialogInterface dialog, int which) {
							purchaseFortumo();
						}
					})
					.show();
				chooseMethodDlg.setOnDismissListener(new DialogInterface.OnDismissListener() {
					@Override
					public void onDismiss(DialogInterface dialog) {
						mShowPurchaseDialogLocked = false;
					}
				});
				chooseMethodDlg.setCanceledOnTouchOutside(false);
				chooseMethodDlg.show();
			}
		} else {
			purchaseFortumo();
		}
	}
	
	private final void initializeIabAndConsumeItems() { // returns true if wallet messages will be processed inside
        mIabHelper = new IabHelper(this, getKey());
        if (isDebugBuild) {
        	mIabHelper.enableDebugLogging(true, TAG);
        }
        mIabHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
			@Override
			public void onIabSetupFinished(final IabResult result) {
				if (result != null && result.isSuccess()) {
					mIabHelper.queryInventoryAsync(new IabHelper.QueryInventoryFinishedListener() {
						@Override
						public void onQueryInventoryFinished(final IabResult result, final Inventory inv) {
							if (result == null || result.isFailure()) {
								return;
							}
							ArrayList<Purchase> purchases = new ArrayList<Purchase>();
							for (String skuId : IAB_SKU_IDS) {
								if (inv.hasPurchase(skuId)) {
									purchases.add(inv.getPurchase(skuId));
								}
							}
							if (purchases.isEmpty()) {
								return;
							}
							mIabHelper.consumeAsync(purchases, new IabHelper.OnConsumeMultiFinishedListener() {
								@Override
								public void onConsumeMultiFinished(List<Purchase> purchases, List<IabResult> results) {
									int i = 0;
									for (IabResult result : results) {
										if (result != null && result.isSuccess()) {
											Purchase purchase = purchases.get(i);
											if (purchase != null) {
												int items = translateIabSkuId(purchase.getSku());
												if (items != 0) {
													Wallet.addCredits(MainActivity.this, items);
												}
											}
										}
										++i;
									}
								}
							});
						}
					});
				} else {
					mIabHelper = null;
				}
			}
		});
	}
	
	private final void purchaseIab() {
		if (mIabHelper == null) {
			return;
		}
		final ProgressDialog pd = ProgressDialog.show(this, null, getString(R.string.wait_please), true, false);
		pd.setCanceledOnTouchOutside(false);
		mIabHelper.queryInventoryAsync(true, IAB_SKU_IDS, new IabHelper.QueryInventoryFinishedListener() {
			@Override
			public void onQueryInventoryFinished(final IabResult result, final Inventory inv) {
				pd.dismiss();
				if (result == null || result.isFailure()) {
					newAlertDialogBuilder()
						.setTitle(R.string.error)
						.setMessage(R.string.we_cannot_process_your_purchase)
						.setNegativeButton("OK", null)
						.show();
					return;
				}

				final ArrayList<String> items = new ArrayList<String>();
				final ArrayList<String> itemIds = new ArrayList<String>();
				for (String skuId : IAB_SKU_IDS) {
					if (inv.hasDetails(skuId) && !inv.hasPurchase(skuId)) {
						SkuDetails details = inv.getSkuDetails(skuId);
						if (details != null) {
							String title = details.getTitle();
							String price = details.getPrice();
							items.add(title + ": " + price);
							itemIds.add(skuId);
						}
					}
				}
				
				if (items.isEmpty()) {
					return;
				}
	
				final String[] skuId = new String[1];
				String[] itemsArray = items.toArray(new String[items.size()]);
				AlertDialog.Builder skuSelectDialogBld = newAlertDialogBuilder();
				skuSelectDialogBld.setSingleChoiceItems(itemsArray, itemsArray.length/2, new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
						skuId[0] = itemIds.get(which);
					}
				});
				skuId[0] = itemIds.get(itemIds.size()/2);
					
				skuSelectDialogBld
					.setTitle(R.string.how_many_perk_points)
					.setPositiveButton(R.string.purchase, new DialogInterface.OnClickListener() {
						@Override
						public void onClick(DialogInterface dialog, int which) {
							mIabHelper.launchPurchaseFlow(MainActivity.this, skuId[0], RC_IAB_PAYMENT_REQUEST, new IabHelper.OnIabPurchaseFinishedListener() {
								@Override
								public void onIabPurchaseFinished(final IabResult result, final Purchase info) {
									if (result == null || result.isFailure()) {
										return;
									}
									final ProgressDialog pd = ProgressDialog.show(MainActivity.this, null, getString(R.string.wait_please), true, false);
									pd.setCanceledOnTouchOutside(false);
									mIabHelper.consumeAsync(info, new IabHelper.OnConsumeFinishedListener() {
										@Override
										public void onConsumeFinished(final Purchase purchase, final IabResult result) {
											pd.dismiss();
											if (result != null && result.isSuccess()) {
												String skuId = purchase.getSku();
												int items = translateIabSkuId(skuId);
												Wallet.addCredits(MainActivity.this, items);
											} else {
												newAlertDialogBuilder()
													.setTitle(R.string.error)
													.setMessage(R.string.unfortunately_purchase_delivery_failed)
													.setNegativeButton("OK", null)
													.show();
											}
										}
									});
								}
							});
						}
					})
					.setNegativeButton(R.string.back, null)
					.show();
			}
		});
	}
	
	private static final int translateIabSkuId(String skuId) {
		if (IAB_SKU_IDS.get(IAB_SKU_1_PERK).equals(skuId)) {
			return 1;
		} else if (IAB_SKU_IDS.get(IAB_SKU_5_PERKS).equals(skuId)) {
			return 5;
		} else if (IAB_SKU_IDS.get(IAB_SKU_10_PERKS).equals(skuId)) {
			return 10;
		} else if (IAB_SKU_IDS.get(IAB_SKU_15_PERKS).equals(skuId)) {
			return 25;
		} else {
			return 0;
		}
	}
	
	private final void purchaseFortumo() {
		if (purchaseFortumoLocked) {
			return;
		}
		purchaseFortumoLocked = true;
		PaymentRequest.PaymentRequestBuilder builder = new PaymentRequest.PaymentRequestBuilder();
		String serviceId = getResources().getString(R.string.fortumo_service_id);
		String inAppSecret = getResources().getString(R.string.fortumo_in_app_secret);
	    builder.setService(serviceId, inAppSecret);
        builder.setDisplayString(getString(R.string.purchase_perk_points));
        builder.setProductName("perk points");
        builder.setType(MpUtils.PRODUCT_TYPE_CONSUMABLE);
        builder.setIcon(R.drawable.icon);
        PaymentRequest pr = builder.build();
        startActivityForResult(pr.toIntent(this), RC_FORTUMO_PAYMENT_REQUEST);		
	}
	
	public static final int getPurchasedPoints() {
		return Wallet.getCredits(mSingleton);
	}
	
	public static final void decreasePurchasedPoints() {
		Wallet.decreaseCredits(mSingleton);
	}
	
	public static final void showBanner(final boolean show) {
		final boolean change = (show ? 1 : 0) != (mSingleton.adsVisible ? 1 : 0);
		if (change) {
			mSingleton.adsVisible = show; 
			mSingleton.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					if (show) {
						if (mSingleton.mBanner == null) {
							mSingleton.mBanner = Appodeal.getBannerView(mSingleton);
					        RelativeLayout.LayoutParams lp = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
					        lp.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
					        lp.setMargins(0, 0, 0, 0);
							mSingleton.mLayoutOverlay.addView(mSingleton.mBanner, lp);
						}
						Appodeal.show(mSingleton, Appodeal.BANNER);
						setSignInButtonVisible(true);
					} else {
						Appodeal.hide(mSingleton, Appodeal.BANNER);
						setSignInButtonVisible(false);
					}
				}
			});
		}
	}

	public static final void showInterstitial() {
		mSingleton.mHandler.postDelayed(new Runnable() {
			@Override
			public void run() {
				Appodeal.show(mSingleton, Appodeal.INTERSTITIAL | Appodeal.VIDEO);
			}
		}, 2000);
	}
	
	private boolean leaderboardLocked = false;
	public static final boolean showLeaderboard(final boolean attackWaves) {
		if (!isKindle && isGoogleClientSignedIn()) {
			mSingleton.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					String leaderboardId = getLeaderboardId(attackWaves);
					if (leaderboardId != null) {
						try {
							mSingleton.startActivityForResult(
								Games.Leaderboards.getLeaderboardIntent(mSingleton.mGoogleApiClient, leaderboardId),
								RC_LEADERBOARD);
							mSingleton.leaderboardLocked = true;
						} catch(Exception ex) {
							Log.e(TAG, "showLeaderboard error: " + ex);
						}
					}
				}
			});
			return true;
		} else {
			return false;
		}
	}
	
	public static final void reportScore(final int xp, final boolean attackWaves) {
		if (!isKindle && xp > 0) {
			mSingleton.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					String leaderboardId = getLeaderboardId(attackWaves);
					if (leaderboardId != null) {
						try {
							if (isGoogleClientSignedIn()) {
								Games.Leaderboards.submitScore(mSingleton.mGoogleApiClient, leaderboardId, xp);
							} else {
								int maxScore = mSingleton.localPrefs.getInt(leaderboardId, -1);
								if (maxScore < xp) {
									mSingleton.localPrefs.edit().putInt(leaderboardId, xp).commit();
								}
							}
						} catch(Exception ex) {
							Log.e(TAG, "addScore error: " + ex);
						}
					}
				}
			});
		}
	}
	
	public final void postLocalHighScores() {
		String leaderboardId = getLeaderboardId(false);
		if (leaderboardId != null) {
			int xp = localPrefs.getInt(leaderboardId, -1);
			if (xp > 0) {
				Games.Leaderboards.submitScore(mGoogleApiClient, leaderboardId, xp);
				localPrefs.edit().putInt(leaderboardId, -1).commit();
			}
		}
		leaderboardId = getLeaderboardId(true);
		if (leaderboardId != null) {
			int xp = localPrefs.getInt(leaderboardId, -1);
			if (xp > 0) {
				Games.Leaderboards.submitScore(mGoogleApiClient, leaderboardId, xp);
				localPrefs.edit().putInt(leaderboardId, -1).commit();
			}
		}
	}
	
	public static final boolean isKindle() {
		return isKindle;
	}
	
	public static final String getTranslatedString(String key) {
		return mSingleton.mTranslations.getString(mSingleton, key);
	}
	
	private static final String getLeaderboardId(boolean attackWaves) {
		if (attackWaves) {
			return mSingleton.getResources().getString(R.string.leaderboard_attackwaves);
		} else {
			return mSingleton.getResources().getString(R.string.leaderboard_survival);
		}
	}
	
	private static AlertDialog.Builder newAlertDialogBuilder() {
		return newAlertDialogBuilder(false);
	}
	
	private static AlertDialog.Builder newAlertDialogBuilder(boolean noColorButtons) {
		return new CustomAlertDialogBuilder(mSingleton, mSingleton.mAlertDialogTypeface, noColorButtons, AlertDialog.THEME_HOLO_DARK);
	}
	
	public static final void connectGooglePlay() {
		mSingleton.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				try {
					if (checkGooglePlayServices()) {
						mSingleton.mGoogleApiClient.connect();
					}
				} catch(Exception ex) {
					Log.e(TAG, "connectGooglePlay error: " + ex);
				}
			}
		});
	}
	
	public static final boolean checkGooglePlayServices() {
	    int resultCode = GooglePlayServicesUtil.isGooglePlayServicesAvailable(mSingleton);
	    if (resultCode != ConnectionResult.SUCCESS) {
	        if (GooglePlayServicesUtil.isUserRecoverableError(resultCode)) {
	            Dialog dialog = GooglePlayServicesUtil.getErrorDialog(resultCode, mSingleton, RC_GP_ERROR_DIALOG);
	            dialog.show();
	        }
	        return false;
	    }
	    return true;
	}
	
    public static final long getScreenSize() {
		DisplayMetrics dm = mSingleton.getResources().getDisplayMetrics();
        final int height = dm.heightPixels;
        final int width = dm.widthPixels;
        long result = (long)width << 32 | height;
        return result;
    }
    
	private native boolean isInPoll();
	private native String getKey();
}
