/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.security.GeneralSecurityException;
import java.security.spec.X509EncodedKeySpec;
import java.util.ArrayList;
import java.util.List;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.PendingIntent;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.net.Uri;

import java.security.KeyFactory;
import java.security.PublicKey;
import java.security.Signature;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.text.TextUtils;
import android.util.Base64;
import android.util.Log;

import com.skplanet.dev.guide.helper.ConverterFactory;
import com.skplanet.dev.guide.helper.ParamsBuilder;
import com.skplanet.dev.guide.pdu.Response;
//import com.skplanet.dev.guide.pdu.VerifyReceipt;
import com.skplanet.dodo.IapPlugin;
import com.skplanet.dodo.IapResponse;
//import com.skplanet.internal.dodo.dev.Logger;

import com.unity3d.ads.android.UnityAds;
import com.unity3d.ads.android.IUnityAdsListener;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.games.Games;
import com.google.example.games.basegameutils.BaseGameUtils;
import com.magmon.PlatformDefenseTstore.R;
//Naver
import com.naver.android.appstore.iap.InvalidProduct;
import com.naver.android.appstore.iap.NIAPHelper;
import com.naver.android.appstore.iap.NIAPHelper.ConsumeListener;
import com.naver.android.appstore.iap.NIAPHelper.GetProductDetailsListener;
import com.naver.android.appstore.iap.NIAPHelper.GetPurchasesListener;
import com.naver.android.appstore.iap.NIAPHelper.GetSinglePurchaseListener;
import com.naver.android.appstore.iap.NIAPHelper.OnInitializeFinishedListener;
import com.naver.android.appstore.iap.NIAPHelper.RequestPaymentListener;
import com.naver.android.appstore.iap.NIAPHelperErrorType;
import com.naver.android.appstore.iap.Product;
import com.naver.android.appstore.iap.Purchase;

public class AppActivity extends Cocos2dxActivity implements IUnityAdsListener ,
											GoogleApiClient.ConnectionCallbacks,
											GoogleApiClient.OnConnectionFailedListener {
	long callback;
	int STORE_TSTORE = 0;
	int STORE_NAVER = 1;
	int market = 0;

    public static native void iapSuccess();
    public static native void iapFailed();
    public static native void unityAdsCallback();
    public static native void playServiceConnected(boolean login);
    
	private static Handler handler;
	private final static int HANDLER_NAVER_PURCHASE = 0;
    static public AppActivity me;
    public GoogleApiClient mGoogleApiClient;
	
	// Naver
	private static final String BASE64_PUBLIC_KEY = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCmyerugOcWO2Cb2Ak23hPcyCUR2irQHtGYjLEyYpFf1+wyCsCMdM9W+8lN9WBoYvBexQYJgG/8+Cnm0Kmh1dtk2dK3zre72ehXXBIInFIZiRnYo0RMgiEWAkJm76ih61YZlcETHRxCFbix6ieZRQcdRbnG7WnXzgQkWYFary3mlQIDAQAB";
	protected static final String TAG = "naver helper";
	private NIAPHelper niapHelper = null;
	private static final String PRODUCT_CODE_SMALL_COINS = "1000020397";
	private static final String PRODUCT_CODE_MIDDLE_COINS = "1000020398";
	private static final String PRODUCT_CODE_BIG_COINS = "1000020399";
	private static final String PRODUCT_CODE_HUGE_COINS = "1000020400";
	private static ArrayList<String> PRODUCT_CODE_LIST;
	private static final int NIAP_REQUEST_CODE = 100;
	private Handler mHandler = new Handler();

	private String mShieldPaymentSeq = null;
	private String userId = "user001";

	private static String purchasingItem = "purchasing item";
	private final long mShieldDueTime = 60000L;

	//Naver end
 	
    @Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		UnityAds.init((Activity)this, "80617", (IUnityAdsListener)this);

		// Naver
//		niapHelper = new NIAPHelper(this, BASE64_PUBLIC_KEY);
//		niapHelper.initialize(onInitializeFinishedListener);
		// naver end
		
        // keep alive
        setKeepScreenOn(true);


		me = this;

		handler = new Handler() {
			public void handleMessage(Message msg) {
				switch (msg.what) {
				case HANDLER_NAVER_PURCHASE:
					handleNaverPurchase();
					break;	
				}	
			}
			// AppActivity anroid interstitial a1534e35a33c505
		};
		
        mGoogleApiClient = new GoogleApiClient.Builder(me)
                .addConnectionCallbacks(me)
                .addOnConnectionFailedListener(me)
                .addApi(Games.API).addScope(Games.SCOPE_GAMES)
                        // add other APIs and scopes here as needed
                .build();

        
		// TStore
		mPlugin = IapPlugin.getPlugin(this, "release");	// development release
		Log.d("TStore", "TStore init");
	}

    static public void showAchievements(){
        if(me.mGoogleApiClient.isConnected()){
            me.startActivityForResult(Games.Achievements.getAchievementsIntent(me.mGoogleApiClient), 2);
        }else{
            loginPlayService();
        }
    }
    static public void updateAchievement(String id, int percent){
        if(me.mGoogleApiClient.isConnected()) {
        	Log.e("PD Achieve", "Achievement: " + id + ", " + percent);
            Games.Achievements.setSteps(me.mGoogleApiClient, id, percent);
        }
    }

    static public void loginPlayService(){
        Log.e(me.TAG, "loginPlayService");
        me.signInClicked();
    }

    static public void logoutPlayService(){
        Log.e(me.TAG, "logoutPlayService");
        me.signOutclicked();
    }
    private static final int REQUEST_RESOLVE_ERROR = 1001;
    private boolean mResolvingError = false;
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent intent) {
        super.onActivityResult(requestCode, resultCode, intent);
        if (requestCode == REQUEST_RESOLVE_ERROR) {
            mResolvingError = false;
            if (resultCode == RESULT_OK) {
                // Make sure the app is not already connected or attempting to connect
                if (!mGoogleApiClient.isConnecting() &&
                        !mGoogleApiClient.isConnected()) {
                    mGoogleApiClient.connect();
                }
            }
        }
        if (requestCode == RC_SIGN_IN) {
            mSignInClicked = false;
            mResolvingConnectionFailure = false;
            if (resultCode == RESULT_OK) {
                mGoogleApiClient.connect();
            } else {
                // Bring up an error dialog to alert the user that sign-in
                // failed. The R.string.signin_failure should reference an error
                // string in your strings.xml file that tells the user they
                // could not be signed in, such as "Unable to sign in."
//                BaseGameUtils.showActivityResultError(this, requestCode, resultCode, com.magmon.PlatformDefense.R.string.signin_failure);
            }
        }
    }

    // Call when the sign-in button is clicked
    public void signInClicked() {
        mSignInClicked = true;
        mGoogleApiClient.connect();
    }

    // Call when the sign-out button is clicked
    public void signOutclicked() {
        mSignInClicked = false;
        Games.signOut(mGoogleApiClient);
        mGoogleApiClient.disconnect();
        playServiceConnected(false);
    }
    @Override
    protected void onStart() {
       super.onStart();
       mGoogleApiClient.connect();
   }

   @Override
   protected void onStop() {
       super.onStop();
       mGoogleApiClient.disconnect();
   }
   @Override
   public void onConnected(Bundle connectionHint) {
       // The player is signed in. Hide the sign-in button and allow the
       // player to proceed.
       Log.e(TAG, "onConnected:" + connectionHint);
       playServiceConnected(true);
   }
   private static int RC_SIGN_IN = 9001;

   private boolean mResolvingConnectionFailure = false;
   private boolean mAutoStartSignInflow = true;
   private boolean mSignInClicked = false;

   @Override
   public void onConnectionFailed(ConnectionResult connectionResult) {
       Log.e(TAG, "onConnectionFailed:" + connectionResult);
       playServiceConnected(false);

       if (mResolvingConnectionFailure) {
           // already resolving
           return;
       }

       // if the sign-in button was clicked or if auto sign-in is enabled,
       // launch the sign-in flow
       if (mSignInClicked || mAutoStartSignInflow) {
           mAutoStartSignInflow = false;
           mSignInClicked = false;
           mResolvingConnectionFailure = true;

           // Attempt to resolve the connection failure using BaseGameUtils.
           // The R.string.signin_other_error value should reference a generic
           // error string in your strings.xml file, such as "There was
           // an issue with sign-in, please try again later."
           if (!BaseGameUtils.resolveConnectionFailure(this,
                   mGoogleApiClient, connectionResult,
                   RC_SIGN_IN, this.getBaseContext().getString(R.string.signin_failure))) {
               mResolvingConnectionFailure = false;
           }
       }

       // Put code here to display the sign-in button
   }

   @Override
   public void onConnectionSuspended(int i) {
       // Attempt to reconnect
       mGoogleApiClient.connect();
   }

   static public void showLeaderboards(String leaderboardID) {
       Log.e(me.TAG, "showLeaderboards on AppActivity.java");

       if(me.mGoogleApiClient.isConnected()){
           me.startActivityForResult(Games.Leaderboards.getLeaderboardIntent(me.mGoogleApiClient,
                   leaderboardID), 2);//REQUEST_LEADERBOARD);
       }else{
           loginPlayService();
       }
   }
   static public void updateTopScoreLeaderboard(int score, String leaderboardID) {
       Log.e(me.TAG, "post score on AppActivity.java");
       if(me.mGoogleApiClient.isConnected()){
           Games.Leaderboards.submitScore(me.mGoogleApiClient, leaderboardID, score);
       }
   }

    static public void openUrl(String url){
        Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
        me.startActivity(browserIntent);
    }
    
    // Unity Ads
    public static void showUnityAdsVideo(){
    	Log.d("UnityAds", "Show Unity Ads Video");
    	if(UnityAds.canShow()) {
    		  UnityAds.show();
    	}
    }
    @Override
    public void onShow(){
    	
    }
    @Override
    public void onHide(){
    	
    }

    @Override
    public void onFetchCompleted() {

    }

    @Override
    public void onFetchFailed(){

    }
    @Override
    public void onVideoStarted(){
    	
    }
    @Override
    public void onVideoCompleted(String itemKey, boolean skipped){
    	if(!skipped){
    		unityAdsCallback();    		
    	}
    }
    
    @Override
    public void onResume() {
      super.onResume();
      UnityAds.changeActivity(this);
    }
    // Unity Ads Done
    
    // naver

	public void handleNaverPurchase(){
		niapHelper.requestPayment(AppActivity.this, purchasingItem, getPayLoad(userId), NIAP_REQUEST_CODE, mRequestPaymentListener);		
	}
	public static void naverPurchase(final String item){
//		showProgressDialog();
		// 소모성 상품 구매버튼 - 소모성 상품코드를 파라메터로 Helper클래스의 requestPayment 메소드를 호출함
		// Button for buying consumable product - call requestPayment API using consumable product code
		purchasingItem = item;
//		GoogleBilling.mCallback_address = callback_address;
		
		Message msg = new Message();
		msg.what = HANDLER_NAVER_PURCHASE;
		handler.sendMessage(msg);
	}
	/*
	 * - Payload 검증 메소드 (개발사에서 직접 구현한다.)
	 */
	/*
	 * - Payload validation method (You have to implement this method by yourself.)
	 */
	boolean verifyDeveloperPayload(String userId, Purchase purchase) {
		String payload = purchase.getDeveloperPayload();

		// Payload 검증 로직을 직접 구현한다.
		// Logic for Payload verification.
		if (getPayLoad(userId).equals(payload)) {
			return true;
		} else {
			return false;
		}
	}
	public static String getPayLoad(String userId) {
		return userId + "PayLoad!@#$%";
	}
	/*
	 * - 앱스토어 결제 signature 확인용 메소드
	 */
	/*
	 * - Method for verification Purchase signature.
	 */
	private static final String SIGNATURE_ALGORITHM = "SHA1withRSA";
	private static final String KEY_ALGORITHM = "RSA";

	private PublicKey generatePublicKey(String encodedPublicKeyString) throws Exception {
		byte[] publicKeyBytes = android.util.Base64.decode(encodedPublicKeyString, android.util.Base64.DEFAULT);
		X509EncodedKeySpec x509KeySpec = new X509EncodedKeySpec(publicKeyBytes);

		try {
			KeyFactory keyFactory = KeyFactory.getInstance(KEY_ALGORITHM);
			return keyFactory.generatePublic(x509KeySpec);
		} catch (GeneralSecurityException e) {
			throw new Exception("Fail to create public RSA key.", e);
		}
	}
	
	private boolean isValidSignature(String signatureStr, String signedDataStr) {
		try {
			PublicKey publicKey = generatePublicKey(BASE64_PUBLIC_KEY);

			Signature signature = Signature.getInstance(SIGNATURE_ALGORITHM);
			signature.initVerify(publicKey);
			signature.update(signedDataStr.getBytes());
			byte[] signatureBytes = Base64.decode(signatureStr, Base64.DEFAULT);

			boolean isVeryfied = signature.verify(signatureBytes);
			Log.d(TAG, "Signature Veryfied - " + isVeryfied);

			return isVeryfied;
		} catch (Exception e) {
			Log.e(TAG, "isValidSignature exception" + e.toString());
			return false;
		}
	}
	/*
	 * - NIAPHelper Initialize 콜백 리스너.
	 */
	/*
	 * - NIAPHelper Initialize Callback Listener.
	 */
	OnInitializeFinishedListener onInitializeFinishedListener = new OnInitializeFinishedListener() {
		@Override
		public void onSuccess() {

			// 리스너 호출 시점에 액티비티가 종료되었을경우 리스너도 종료한다.
			// If activity terminated during callback listener, quit it.
			if (niapHelper == null)
				return;

			// 사용자가 결제한 리스트를 불러와서 처리한다.
			/*
			 * 인앱결제Helper 클래스에서 제공하는 getPurchasesAsync를 사용하여 구매데이터를 셋팅한다.
			 * getPurchasesAsync의 콜백메소드인 getAllPurchasesListener 에서는
			 * 1. 사용자가 지금까지 결제했던 모든 상품정보를 읽어와서
			 * 2. 영구성 상품 구매내역이 있는경우 영구성 상품 구매 플래그를 셋팅하고
			 * 3. 기존 구매하고 consume으로 소진하지 않은 소모성 상품이 존재하는 경우 이를 consume한 후 혜택 지급과
			 * 같은 관한 비즈니스 로직을 실행한다.
			 */
			// When NIAPHelper initialize finished, call getPurchasesAsync API for Process logics that depends on user's purchased product.
			niapHelper.getPurchasesAsync(mGetPurchasesListener);

			// 상품 정보 조회를 위하여 리스트에 상품코드를 담는다.
			// Add product code to list for getProductDetailsAsync.
			PRODUCT_CODE_LIST = new ArrayList<String>();
			PRODUCT_CODE_LIST.add(PRODUCT_CODE_SMALL_COINS);
			PRODUCT_CODE_LIST.add(PRODUCT_CODE_MIDDLE_COINS);
			PRODUCT_CODE_LIST.add(PRODUCT_CODE_BIG_COINS);
			PRODUCT_CODE_LIST.add(PRODUCT_CODE_HUGE_COINS);

			// 상품 정보를 조회한다.
			// Call getProductDetailsAsync API if you want product's detail information.
			niapHelper.getProductDetailsAsync(PRODUCT_CODE_LIST, mGetProductDetailsListener);

			// 단건 상품의 구매정보를 조회한다.
			// 개별 구매정보를 조회할때는 구매완료후 전달되는 PaymentSeq가 필요하다.
			// 샘플 앱에서는 구현상의 편의를 위해 최초 앱 초기화 시에만 미리 설정된 기간제형 상품의 구매 정보를 조회하지만,
			// 실제 앱에서 구현시에는 필요한 주기마다 호출하여 만료 처리를 하도록 한다.
			// you can call specific purchase information directly by getSinglePurchaseAsync API.
			// When you call getSinglePurchaseAsync, you need PaymentSeq which has been returned from payment API result.
			// In this sample app, It calls this API at once on initializing for convenient implementation,
			// but your app should call it on demand to check duration of periodic product.
			if (mShieldPaymentSeq != null) {
				niapHelper.getSinglePurchaseAsync(mShieldPaymentSeq, getPurchaseListener);
			}

//			Toast.makeText(AppActivity.this, "NIAPHelper initialize finished", Toast.LENGTH_SHORT).show();
			Log.d(TAG, "initialize finished.");
		}

		@Override
		public void onFail(NIAPHelperErrorType errorType) {
			if (errorType == NIAPHelperErrorType.NEED_INSTALL_OR_UPDATE_APPSTORE) {
				// 사용자의 단말기에 네이버앱스토어앱이 설치되지 않았거나 구버전이 설치되어있을경우 설치를 유도한다.
				// If user does not install NAVER APPSTORE APP or has old version, induce user to install.
				niapHelper.updateOrInstallAppstore(AppActivity.this);
			} else {
				// 기타 Helper 초기화 오류가 발생할 경우 처리
				// When other NIAPHelper initial error has been occurred.
				
				complain("NIAPHelper initialize failed", errorType);
			}
			iapFailed();
			Log.d(TAG, "initialize failed.");
		}
	};

	/*
	 * - NIAPHelper 상품정보 조회API 콜백 리스너.
	 * 네이버 앱스토어 개발자센터에서 등록한 상품정보를 불러와 상품명이나 상품가격등을 셋팅하는데 사용한다.
	 */
	/*
	 * - NIAPHelper getProductDetailsAsync API Callback Listener.
	 * You can load product's detail information from NAVER APPSTORE DEV CENTER for getting product's name or price.
	 */
	GetProductDetailsListener mGetProductDetailsListener = new GetProductDetailsListener() {
		@Override
		public void onSuccess(List<Product> validProductList, List<InvalidProduct> inValidProductList) {
			// 상품정보를 성공적으로 불러오면 화면에 상품명과 가격을 표시한다.
			// When API call finished successfully, display product's name and price.
			for (Product validProduct : validProductList) {
				//itemListTxt.setText(itemListTxt.getText() + " " + validProduct.getProductName() + "(KRW" + validProduct.getSellPrice() + ") ");
				Log.d(TAG, "mGetProductDetailsListener success" + validProduct.getProductName() + "(KRW" + validProduct.getSellPrice() + ") ");
			}
			Log.d(TAG, "getProductDetailsListener success");
		}

		@Override
		public void onFail(NIAPHelperErrorType errorType) {
			iapFailed();
			complain("error has occurred on getProductDetails.", errorType);
			Log.d(TAG, "getProductDetailsListener faild");
		}
	};

	/*
	 * - NIAPHelper 상품소진(consumeAsync) API 콜백 리스너.
	 * 소모성 상품 소진요청 후 콜백으로 실행되며,
	 * 정상적으로 상품 소진이 완료되면 아이템 지급이나 혜택 적용 등의 비즈니스 코드를 실행한다.
	 */
	/*
	 * - NIAPHelper consumeAsunc API Callback Listener.
	 * This Listener runs as callback request exhausted after consuming products.
	 * From here, you can supply gifts or provide items that depends on business logic.
	 */
	ConsumeListener consumeListener = new ConsumeListener() {
		@Override
		public void onSuccess(Purchase purchase) {
			Log.d(TAG, "consumeListener success");
			// 정상적으로 상품이 소진된경우 아이템을 지급한다.
			// When consumption is successfully complete, provide items.
			
			if (purchase.getProductCode().equals(PRODUCT_CODE_SMALL_COINS) ||
					purchase.getProductCode().equals(PRODUCT_CODE_MIDDLE_COINS) ||
					purchase.getProductCode().equals(PRODUCT_CODE_BIG_COINS) ||
					purchase.getProductCode().equals(PRODUCT_CODE_HUGE_COINS)) {
				iapSuccess();
			}
		}

		@Override
		public void onFail(NIAPHelperErrorType errorType) {
			Log.d(TAG, "consumeListener failed");
			iapFailed();
			if (errorType == NIAPHelperErrorType.PRODUCT_NOT_OWNED) {
				// 유저가 상품을 소유하지 않아서 소진할 수 없는 경우.
				// Can not consume because user does not have product.
				complain("You don't have product.", errorType);

			} else {
				// 기타 상품 소진과정중 에러 발생시 로그출력
				// If error occurred while consuming, logging it.
				complain("consumeAsync failed", errorType);
			}
		}
	};

	/*
	 * - NIAPHelper 상품구매(requestPayment) API 콜백 리스너.
	 * 상품 구매가 완료되면 Payload를 체크하여 정상적인 구매 요청건인지 개발사 validation을 거친 후
	 * 1. 소모성 상품인 경우 바로 소진시킨다(consumeAsync API 사용)
	 * 2. 영구성 상품인 경우 혜택을 적용시킨다(비즈니스 로직)
	 */
	/*
	 * - NIAPHelper requestPayment API Callback Listener.
	 * This Listener runs as callback request exhausted after purchase products.
	 * From here, you have to validate Payload and
	 * 1. If purchased product is consumable product, consume it.
	 * 2. If purchased product is permanent product, supply gifts that depends on business logic.
	 */
	RequestPaymentListener mRequestPaymentListener = new RequestPaymentListener() {
		@Override
		public void onSuccess(Purchase purchase) {
			Log.d(TAG, "requestPaymentListener success");
			if (niapHelper == null)
				return;

			// payload를 체크하여 정상적인 거래요청인지 확인한다.
			// Verify Payload.
			if (!verifyDeveloperPayload(userId, purchase)) {
				complain("Error has occurred while purchasing. Payload verification failed.");
				return;
			}
			
			//결제 signature 확인 (niapHelper 내부 결제 과정중, 이미 같은 로직이 구현되어 있다. 이 코드는 signature 검증로직 참고용으로 실제 앱 구현시에는 제외해도 무관하다.)
			boolean isSignatureVerified = isValidSignature(purchase.getSignature(), purchase.getOriginalPurchaseAsJsonText());
			if (!isSignatureVerified) {
				complain("Error has occurred while purchasing. Signature verification failed.");
				return;
			}

			if (purchase.getProductCode().equals(PRODUCT_CODE_SMALL_COINS) ||
					purchase.getProductCode().equals(PRODUCT_CODE_MIDDLE_COINS)||
					purchase.getProductCode().equals(PRODUCT_CODE_BIG_COINS)||
					purchase.getProductCode().equals(PRODUCT_CODE_HUGE_COINS) ) {
				// 구매한 상품이 소모성 상품일 경우 바로 소진시킨다.
				// If purchased product is consumable product, consume it
				alert("Thank you for buying!");
				niapHelper.consumeAsync(purchase, consumeListener);
			} /*else if (purchase.getProductCode().equals(PRODUCT_CODE_PERMANENT_GOLDEN_ANVIL)) {
				// 구매한 상품이 영구성 상품일 경우 소진시키지 않고 혜택을 지급한다.
				// If purchased product is permanent product, supply gifts that depends on business logic. Do not consume it.
				alert("Thank you for buying Golden Anvil");
				
			} else if (purchase.getProductCode().equals(PRODUCT_CODE_PERIODIC_SHIELD)) {
				alert("Thank you for lending Shield");
				mShieldPaymentSeq = purchase.getPaymentSeq();
			}*/
		}

		@Override
		public void onFail(NIAPHelperErrorType errorType) {
			Log.d(TAG, "requestPaymentListener failed");
			iapFailed();
			if (errorType == NIAPHelperErrorType.SIGNATURE_VERIFICATION_ERROR) {
				// 구매 서명 검증키가 변조된 경우. 개발사 서버등으로 로그를 전송하여 검증키가 변조된 원인을 찾아서 대응하여야 한다.
				// If purchase signature verification has been failed, you should log it on your owned server for find why this error has been occurred.
				complain("Purchase signature verification has been failed", errorType);
			} else if (errorType == NIAPHelperErrorType.PURCHASE_PROCESS_NOT_WORKED) {
				// 구매 과정 중 네이버앱스토어앱 내부 문제로 결제가 실패한 경우. 개발사 서버등으로 로그를 전송하여 앱스토어 담당자에게 전달하여 원인을 파악한다.
				// This case may be caused by because of NAVER APPSTORE APP's internal error. You should log it on your own server and, deliver it to NAVER APPSTORE to find problem.
				complain("There is some problem on NAVER APPSTORE.", errorType);
			} else if (errorType == NIAPHelperErrorType.NETWORK_ERROR) {
				// 구매 과정 중 네트워크 오류로 결제에 실패한 경우
				// This case may be caused by because of network connection problem.
				complain("There is some network problem on purchasing. Please retry.", errorType);
			} else if (errorType == NIAPHelperErrorType.PRODUCT_ALREADY_OWNED) {
				// 이미 가지고 있는 상품을 재구매 할때 발생하는 오류. 영구성 상품인 경우 정상적인 오류이며, 소모성 상품일 경우에는 기존 상품을 consume 시켜야 재구매가 가능하다.
				// (이 경우 구매 로직 어딘가에서 소모성 상품구매후 바로 consume하고 혜택을 지급하는 부분이 빠져있다는 것을 의미한다.)
				// This case may be caused by because user already have same product.
				// If purchasing product was permanent product, this error worked correctly. but in case of consumable product, you have to consume user's already owned product for purchase for new one.
				// (In this case, there is some missing logic in your purchase code that is consuming consumable product after purchase.)
				complain("You already owned same product.", errorType);
			} else {
				// If other type of error occurred during purchase, log it.
				//complain("requestPayment fail", errorType);
				alert("네이버 앱스토어 설치가 필요합니다.");
			}
		}

		@Override
		public void onCancel() {
			iapFailed();
			Log.d(TAG, "request payment canceled.");
			// If user cancels purchase
//			Toast.makeText(AppActivity.this, "Purchase has been canceled.", Toast.LENGTH_SHORT).show();
		}
	};

	/*
	 * - NIAPHelper 구매상품 전체 조회 API 콜백 리스너.
	 * 앱 최초 로딩시 사용자의 구매상품리스트를 조회하여 기존에 구매후 소진되지 않은 소모성 상품이 존재하는 경우
	 * 바로 소진시키고 혜택을 지급하며, 영구성 상품 구매내역이 있으면 혜택을 적용한다.
	 */
	/*
	 * - NIAPHelper getPurchasesAsync API Callback Listener.
	 * While first initializing App's data, you should access user's purchases list and
	 * 1. If user has not-consumed consumable product, consume it.
	 * 2. If user has permanent product, supply gifts that depends on business logic.(Do not consume it.)
	 */
	GetPurchasesListener mGetPurchasesListener = new GetPurchasesListener() {
		@Override
		public void onSuccess(List<Purchase> purchases) {
			if (niapHelper == null)
				return;
			Log.d(TAG, "getPurchasesListener success.");
			for (Purchase purchase : purchases) {
				// payload를 체크하여 정상적인 거래요청인지 확인한다.
				// validate Payload.
				if (!verifyDeveloperPayload(userId, purchase)) {
					complain("Error has occurred while getting purchased list. Payload verification failed.");
					return;
				}

				if (purchase.getProductCode().equals(PRODUCT_CODE_SMALL_COINS)||
						purchase.getProductCode().equals(PRODUCT_CODE_MIDDLE_COINS)||
						purchase.getProductCode().equals(PRODUCT_CODE_BIG_COINS)||
						purchase.getProductCode().equals(PRODUCT_CODE_HUGE_COINS)) {
					niapHelper.consumeAsync(purchase, consumeListener);
				}/* else if (purchase.getProductCode().equals(PRODUCT_CODE_PERMANENT_GOLDEN_ANVIL)) {
					// 조회된 상품이 영구성 상품일 경우 소진시키지 않고 혜택을 지급한다.
					// If user has permanent product, supply gifts that depends on business logic. Do not consume it.
				}*/
			}
			
		}

		@Override
		public void onFail(NIAPHelperErrorType errorType) {
			Log.d(TAG, "getPurchasesListener fail.");
			iapFailed();
			if (errorType == NIAPHelperErrorType.SIGNATURE_VERIFICATION_ERROR) {
				// 구매 서명 검증키가 변조된 경우. 개발사 서버등으로 로그를 전송하여 검증키가 변조된 원인을 찾아서 대응하여야 한다.
				// If purchase signature verification has been failed, you should log it on your owned server for finding why this error has been occurred.
				complain("*Purchase signature verification has been failed", errorType);
			} else if (errorType == NIAPHelperErrorType.USER_NOT_LOGGED_IN) {
				// 네이버앱스토어앱 로그인이 되어있지 않아 결제내역을 불러올수 없는 경우.
				// This case may be caused by because user account has been logged off in NAVER APPSTORE APP.
				complain("getPurchasesListener - Please login NAVER APPSTORE.", errorType);
			} else {
				complain("getPurchasesAsync failed", errorType);
			}
		}
	};

	/*
	 * - NIAPHelper 구매상품 개별 조회 API 콜백 리스너.
	 * 구매상품 개별 조회 API는 기간제 상품의 처리에 주로 사용하며, 사용기간이 남아있는경우 혜택을 지급하고 사용기간이 만료된 경우는 해당 상품을 소진 시킨다.
	 * (소진이 완료되어야 해당 상품 재구매가 가능하다.)
	 * 기간제 상품의 사용기간에 대한 체크는 앱 자체에서 처리하고 NIAP에서는 단순히 만료된 상품을 소진시키는 역할만 담당한다.
	 */
	/*
	 * - NIAPHelper getSinglePurchaseAsync API Callback Listener.
	 * getSinglePurchaseAsync API tend to be used by periodic products.
	 * In this listener, you can process business logic if time remains for periodic product
	 * or expired, It must be consumed so that the user can buy this product again.
	 * You have to check duration of periodic product by yourself. NIAP only provides consume API for expired one.
	 */
	GetSinglePurchaseListener getPurchaseListener = new GetSinglePurchaseListener() {
		@Override
		public void onSuccess(Purchase purchase) {
			Log.d(TAG, "getSinglePurchasesListener success.");
			// payload를 체크하여 정상적인 거래요청인지 확인한다.
			// Verify Payload.
			if (!verifyDeveloperPayload(userId, purchase)) {
				complain("Error has occurred while getting purchased list. Payload verification failed.");
				return;
			}
/*
			// 기간제 상품
			// If purchase is periodic product,
			if (purchase.getProductCode().equals(PRODUCT_CODE_PERIODIC_SHIELD)) {
				// 기간제 상품 만료 체크.
				// Check the product's expiration period. In this sample app, It checks time locally for convenient implementation but, your app should check it on your owned server.
				if (purchase.getPaymentTime() + mShieldDueTime <= System.currentTimeMillis()) {
					// 만료된 경우 상품을 소진한다.
					// If expired, consume product.
					niapHelper.consumeAsync(purchase, consumeListener);

					// 비즈니스 로직 처리
					// Process your business logic.
					mShieldPaymentSeq = null;
					complain("Your Shield has been expired");
				} else {
					// 만료되지 않는경우 혜택 지급
					// If not expired yet, process only product's business logic.
					mShieldPaymentSeq = purchase.getPaymentSeq();
				}
			}*/
		}

		@Override
		public void onFail(NIAPHelperErrorType errorType) {
			Log.d(TAG, "getSinglePurchasesListener success.");
			if (errorType == NIAPHelperErrorType.SIGNATURE_VERIFICATION_ERROR) {
				// 구매 서명 검증키가 변조된 경우. 개발사 서버등으로 로그를 전송하여 검증키가 변조된 원인을 찾아서 대응하여야 한다.
				// If purchase signature verification has been failed, you should log it on your owned server for find why this error has been occurred.
				complain("Purchase signature verification has been failed", errorType);
			} else if (errorType == NIAPHelperErrorType.USER_NOT_LOGGED_IN) {
				// 네이버앱스토어앱 로그인이 되어있지 않아 결제내역을 불러올수 없는 경우.
				// This case may be caused by because user account has been logged off in NAVER APPSTORE APP.
				complain("getSinglePurchaseListener - Please login NAVER APPSTORE.", errorType);
			} else if (errorType == NIAPHelperErrorType.NON_EXISTENT_PURCHASE) {
				// 해당 paymentSeq에 대한 구매정보를 찾을수 없는 경우.
				// This case may be caused by because there is no purchase information for that paymentSeq.
				complain("There is no purchase informaion for that paymentSeq.", errorType);
			} else if (errorType == NIAPHelperErrorType.ALREADY_CONSUMED) {
				// 이미 소진 완료되어 데이터를 불러올 수 없는 경우.
				// This case may be caused by because target purchase had been already consumed.
				complain("Target purchase had been already consumed.", errorType);
			} else {
				complain("getPurchasesAsync failed", errorType);
			}
//			dismissProgressDialog();
		}
	};

	/*
	 * - 에러 다이얼로그 출력 메소드
	 */
	/*
	 * - Method for error AlertDialog
	 */
	private void complain(String message) {
		iapFailed();
//		String errorMessage = message + " error has occurred";
//		Log.e(TAG, errorMessage);
//		alert("Error: " + errorMessage);
	}
	private void complain(String message, NIAPHelperErrorType errorType) {
		iapFailed();
//		String errorMessage = message + " error has occurred \ncode : " + errorType.getErrorCode() + ", details : " + errorType.getErrorDetails(); 
//		Log.e(TAG, errorMessage);
//		alert(errorMessage);
	}
	private void alert(String message) {
		iapFailed();
		AlertDialog.Builder bld = new AlertDialog.Builder(this);
		bld.setMessage(message).setNeutralButton("OK", null).create().show();
	}

	// naver end


    @Override
	public void onDestroy() {
		super.onDestroy();
		// naver
		if (niapHelper != null) {
			Log.d(TAG, "release helper");
			niapHelper.terminate();
			niapHelper = null;
		}

		PRODUCT_CODE_LIST = null;
		// naver end
	}

    @Override
	public void onRestart() {
		Log.d("naver helper", "onRestart Activity");
		super.onRestart();

		//Naver
		/*
		if (niapHelper == null) {
			// 인앱결제 Helper 클래스 인스턴스 생성
			// Create NIAP Helper Instance
			Log.d(TAG, "onRestart - Recreate helper");
			niapHelper = new NIAPHelper(this, BASE64_PUBLIC_KEY);
		}

		if (niapHelper.isInitialized() == false) {
			// 인앱결제 Helper 클래스 초기화
			// Initialize NIAP Helper Instance
			Log.d(TAG, "onRestart - Initialize helper");
			niapHelper.initialize(onInitializeFinishedListener);
		}else{
			niapHelper.getPurchasesAsync(mGetPurchasesListener);
		}*/
		//Naver end
	}
	// TStore starts
	private static IapPlugin mPlugin = null;
    private static String mRequestId;
    private static String mAID = "OA00694460";
    public static void requestPaymentTStore(final String item) {
//		AppActivity.showProgressDialog();
    	Log.d("TStore", "Payment start");
        String parameter = makeRequest(item);
        mUiHandler.obtainMessage(100, parameter).sendToTarget();
        Bundle req = mPlugin.sendPaymentRequest(parameter,
                new IapPlugin.RequestCallback() {

                    @Override
                    public void onResponse(IapResponse data) {
                    	Log.d("TStore", "data " + data.toString());
                        if (data == null || data.getContentLength() <= 0) {
                            // TODO Unusual error
//                            mUiHandler.obtainMessage(100,
//                                    "onResponse() response data is null")
//                                    .sendToTarget();
                            return;
                        }

                        // 1. JSON 
                        Response response = ConverterFactory.getConverter().fromJson(
                                data.getContentToString());

                        if (response == null) {
                            // TODO invalid response data
//                            mUiHandler.obtainMessage(100,
//                                    "onResponse() invalid response data")
//                                    .sendToTarget();

                        	iapFailed();
							
							
                            return;
                        }

                        // TODO for logging
                        StringBuffer sb = new StringBuffer("onResponse() \n");
                        sb.append("From:" + data.getContentToString())
                                .append("\n")
                                .append("To:" + response.toString());
//                        mUiHandler.obtainMessage(100, sb.toString()).sendToTarget();

                        // TODO ���� �Ϸ� �� ó�� �� - ����
                        // C1. ��� �ڵ� üũ
                        // response.result.code
                        if (!response.result.code.equals("0000")) {
//                            mUiHandler.obtainMessage(200,
//                                    "Failed to request to purchase a item")
//                                    .sendToTarget();

                        	iapFailed();
							
                            return;
                        }

                        iapSuccess();
                    }

                    @Override
                    public void onError(String reqid, String errcode,
                            String errmsg) {
                        // TODO Error occurred
//                        mUiHandler.obtainMessage(100, "onError() identifier:" + reqid + " code:" + errcode + " msg:" + errmsg)
//                        .sendToTarget();
                    	
                    	iapFailed();
                    }
                });

        if (req == null) {
            // TODO request failure
        	iapFailed();
			
            return;
        }

        mRequestId = req.getString(IapPlugin.EXTRA_REQUEST_ID);
        if (mRequestId == null || mRequestId.length() == 0) {
            // TODO request failure
        	iapFailed();
//            return;
        }

//        return;
    }

    public static Handler mUiHandler = new Handler() {

        @Override
        public void handleMessage(Message msg) {
            if (msg.what == 100) {
//                this.mLog.setText((String) msg.obj);
            }
            else if (msg.what == 101) {
                final Response response = (Response) msg.obj;
                JSONObject json = new JSONObject();
                try {
                    json.put("appid", mAID.toUpperCase());
                    json.put("txid", response.result.txid);
                    Log.d("Receipt", response.result.receipt.length() + " is the receipt ");
                    json.put("signdata", response.result.receipt);
                } catch (JSONException e) {
                    e.printStackTrace();
//                    PaymentFragment.this.mLog
//                            .setText("Failed while composing json data for verification receipt.");
                }

                ReceiptConfirm rc = new ReceiptConfirm();
                rc.execute((String) json.toString());
            } else if (msg.what == 200) {
//                Toast.makeText(PaymentFragment.this.getActivity(), (String) msg.obj,
//                        Toast.LENGTH_SHORT).show();
            }
        }
    };

    public static class ReceiptConfirm extends AsyncTask<String, Void, String>
    {
        final String convertStreamToString(InputStream is) throws IOException {
            ByteArrayOutputStream bs = new ByteArrayOutputStream();
            int i = is.read();
            while (i != -1) {
                bs.write(i);
                i = is.read();
            }
            return bs.toString();
        }

        @Override
        protected void onPostExecute(String result) {
            // TODO
//            if (TextUtils.isEmpty(result)) {
//                PaymentFragment.this.mLog.append("Failed to request to verify receipt.");
//            } else {
//                VerifyReceipt r = ConverterFactory.getConverter().fromJson2VerifyReceipt(result);
//                PaymentFragment.this.mLog.append(r.toString());
//            }
        }

        @Override
        protected String doInBackground(String... params)
        {
            HttpClient client = new DefaultHttpClient();
            String DEV_SERVER_URL = "https://iapdev.tstore.co.kr/digitalsignconfirm.iap";
            HttpPost httpPostRequest = new HttpPost(DEV_SERVER_URL);
            StringEntity se;
            try {
                se = new StringEntity(params[0], "UTF-8");
                httpPostRequest.setEntity(se);
                httpPostRequest.setHeader("Content-type", "application/json");
            } catch (UnsupportedEncodingException e) {
                e.printStackTrace();
            }
            try {
                HttpResponse resp = client.execute(httpPostRequest);
                if (resp.getStatusLine().getStatusCode() == HttpStatus.SC_OK)
                {
                    HttpEntity respEntity = resp.getEntity();
                    if (respEntity != null)
                    {
                        try
                        {
                            InputStream ins = respEntity.getContent();
                            return convertStreamToString(ins);
                        } catch (IllegalStateException e)
                        {
                            e.printStackTrace();
                        } catch (IOException e)
                        {
                            e.printStackTrace();
                        }

                    }
                }
            } catch (ClientProtocolException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
            return null;
        }
    }

    public static String makeRequest(String pID) {
        ParamsBuilder pb = new ParamsBuilder();

        
        pb.put(ParamsBuilder.KEY_APPID,
                mAID.trim().toUpperCase()).put(
                ParamsBuilder.KEY_PID, pID.trim());

        String tmp = "";
        if (!TextUtils.isEmpty(tmp)) {
            pb.put(ParamsBuilder.KEY_PNAME, tmp);
        } else {
            pb.put(ParamsBuilder.KEY_PNAME, "");
        }

        tmp = "";//"TID";//mTid.getText().toString();
        if (!TextUtils.isEmpty(tmp)) {
            pb.put(ParamsBuilder.KEY_TID, tmp);
        } else {
            pb.put(ParamsBuilder.KEY_TID, "");
        }

        tmp = "";//"BPInfo";//mBpinfo.getText().toString();
        if (!TextUtils.isEmpty(tmp)) {
            pb.put(ParamsBuilder.KEY_BPINFO, tmp);
        } else {
            pb.put(ParamsBuilder.KEY_BPINFO, tmp);
        }

        return pb.build();
    }
	// TStore end
}
