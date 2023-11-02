/****************************************************************************
Copyright (c) 2015 Chukong Technologies Inc.
 
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

import android.app.ProgressDialog;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Message;
import android.os.Handler;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.InterstitialAd;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.common.api.PendingResult;
import com.google.android.gms.common.api.ResultCallback;
import com.google.android.gms.drive.Drive;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.GamesStatusCodes;
import com.google.android.gms.games.snapshot.Snapshot;
import com.google.android.gms.games.snapshot.SnapshotMetadata;
import com.google.android.gms.games.snapshot.SnapshotMetadataChange;
import com.google.android.gms.games.snapshot.Snapshots;
import com.google.android.gms.plus.Plus;
import com.magmon.PlatformDefense.R;
import com.unity3d.ads.android.IUnityAdsListener;
import com.unity3d.ads.android.UnityAds;

import org.cocos2dx.lib.Cocos2dxActivity;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Random;

import com.heyzap.sdk.ads.HeyzapAds;
import com.heyzap.sdk.ads.IncentivizedAd;
import com.heyzap.sdk.ads.HeyzapAds.OnIncentiveResultListener;


public class AppActivity extends Cocos2dxActivity implements IUnityAdsListener,
        GoogleApiClient.ConnectionCallbacks,
        GoogleApiClient.OnConnectionFailedListener {
    String TAG = "Platform_Defense";
    Boolean isFreeVersion = true;
    int videoAdsKind = 0;
    InterstitialAd mInterstitialAd;
    Boolean showImmediatly = false;
    static public AppActivity me;
    public GoogleApiClient mGoogleApiClient;
    public static native void unityAdsCallback();
    public static native void playServiceConnected(boolean login);
    private static native void setLoadedData(String key, String value);
    private static native void resetGameData();
    private boolean insteadOfUnityAds = false;
    private static Handler handler;
    private String strArg0;
    private String strArg1;
    private int intArg0;
    private int intArg1;
    private final static int HANDLER_LOAD_INTERSTITIAL = 0;
    private final static int HANDLER_SHOW_INTERSTITIAL = 1;
    private final static int HANDLER_SHOW_ACHIEVEMENTS = 2;
    private final static int HANDLER_UPDATE_ACHIEVEMENT = 3;
    private final static int HANDLER_UNLOCK_ACHIEVEMENT = 4;
    private final static int HANDLER_SHARE_PHOTO = 5;
    private final static int HANDLER_SHARE_LINK = 6;
    private final static int HANDLER_LOGIN_PLAY_SERVICE = 7;
    private final static int HANDLER_LOGOUT_PLAY_SERVICE = 8;
    private final static int HANDLER_OPEN_URL = 9;
    private final static int HANDLER_SHOW_UNITY_ADS_VIDEO = 10;
    private final static int HANDLER_SHOW_LEADERBOARDS = 11;
    private final static int HANDLER_UPDATE_TOP_SCORE = 12;
    private final static int HANDLER_SAVE_SNAPSHOT = 13;
    private final static int HANDLER_LOAD_SNAPSHOT = 14;

    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        Log.e(TAG, "onCreate.");

        me = this;

        // free 85204
        // paid 80617
        if (isFreeVersion){
            UnityAds.init(me, "85204", this);
        }else{
            UnityAds.init(me, "80617", this);
        }

//        UnityAds.setDebugMode(true);
//        UnityAds.setTestMode(true);

        // keep alive
        setKeepScreenOn(true);

        mGoogleApiClient = null;

        mGoogleApiClient = new GoogleApiClient.Builder(me)
                .addConnectionCallbacks(me)
                .addOnConnectionFailedListener(me)
                .addApi(Plus.API).addScope(Plus.SCOPE_PLUS_LOGIN)
                .addApi(Games.API).addScope(Games.SCOPE_GAMES)
                .addApi(Drive.API).addScope(Drive.SCOPE_APPFOLDER)
                        // add other APIs and scopes here as needed
                .build();


        handler = new Handler() {
            public void handleMessage(Message msg) {
                switch (msg.what) {
                    case HANDLER_LOAD_INTERSTITIAL:
                        requestNewInterstitial();
                        break;
                    case HANDLER_SHOW_INTERSTITIAL:
                        me.showInterstitial();
                        break;
                    case HANDLER_SHOW_ACHIEVEMENTS:
                        showAchievementsHandler();
                        break;
                    case HANDLER_UPDATE_ACHIEVEMENT:
                        updateAchievementHandler();
                        break;
                    case HANDLER_UNLOCK_ACHIEVEMENT:
                        unlockAchievementHandler();
                        break;
                    case HANDLER_SHARE_PHOTO:
                        sharePhotoHandler();
                        break;
                    case HANDLER_SHARE_LINK:
                        shareLinkHandler();
                        break;
                    case HANDLER_LOGIN_PLAY_SERVICE:
                        loginPlayServiceHandler();
                        break;
                    case HANDLER_LOGOUT_PLAY_SERVICE:
                        logoutPlayServiceHandler();
                        break;
                    case HANDLER_OPEN_URL:
                        openUrlHandler();
                        break;
                    case HANDLER_SHOW_UNITY_ADS_VIDEO:
                        showUnityAdsVideoHandler();
                        break;
                    case HANDLER_SHOW_LEADERBOARDS:
                        showLeaderboardsHandler();
                        break;
                    case HANDLER_UPDATE_TOP_SCORE:
                        updateTopScoreLeaderboardHandler();
                        break;
                    case HANDLER_SAVE_SNAPSHOT:
                        savedGamesUpdate();
                        break;
                    case HANDLER_LOAD_SNAPSHOT:
                        savedGamesSelect();
                        break;
                }
            }
        };
        mInterstitialAd = new InterstitialAd(this);
        if (isFreeVersion){
            mInterstitialAd.setAdUnitId("ca-app-pub-4597682064467102/9705624671");
        }else{
            mInterstitialAd.setAdUnitId("ca-app-pub-4597682064467102/1413315075");
        }

        mInterstitialAd.setAdListener(new AdListener() {
            @Override
            public void onAdClosed() {
                Message msg = new Message();
                msg.what = HANDLER_LOAD_INTERSTITIAL;
                handler.sendMessage(msg);
            }

            @Override
            public void onAdLoaded() {

                if (showImmediatly) {
                    Log.e("Admob", "ads loaded");
                    Message msg = new Message();
                    msg.what = HANDLER_SHOW_INTERSTITIAL;
                    handler.sendMessage(msg);

                    showImmediatly = false;
                }
            }
        });

        if (isFreeVersion){
            Message msg = new Message();
            msg.what = HANDLER_LOAD_INTERSTITIAL;
            handler.sendMessage(msg);
        }

        // Your Publisher ID is: 715ca6d56576512b62f94fd564cd0f09
        HeyzapAds.start("715ca6d56576512b62f94fd564cd0f09", this);
        IncentivizedAd.fetch();

        IncentivizedAd.setOnIncentiveResultListener(new OnIncentiveResultListener() {
            @Override
            public void onComplete(String tag) {
                Log.e(me.TAG, "Heyzap ads complete");
                unityAdsCallback();
            }

            @Override
            public void onIncomplete(String tag) {
                // Don't give the player their reward, and tell them why
            }
        });

        Log.e(TAG, "onCreate done");
    }

    static public void showAds(){
        Log.e("Admob", "Show ads");
        Message msg = new Message();
        msg.what = HANDLER_SHOW_INTERSTITIAL;
        handler.sendMessage(msg);
    }
    private void showInterstitial(){
        if (me.mInterstitialAd.isLoaded()) {
            Log.e("Admob", "try to show ads");
            me.mInterstitialAd.show();
        }else{
            Log.e("Admob", "ads not loaded");
        }
    }
    private void requestNewInterstitial() {
        Log.e("Admob", "requestNewInterstitial");
        AdRequest adRequest = new AdRequest.Builder()
                .addTestDevice("SEE_YOUR_LOGCAT_TO_GET_YOUR_DEVICE_ID")
                .build();

        mInterstitialAd.loadAd(adRequest);

        if (insteadOfUnityAds){
            insteadOfUnityAds = false;
            unityAdsCallback();
        }
    }

    private void showAchievementsHandler(){
        if(me.mGoogleApiClient.isConnected()){
            me.startActivityForResult(Games.Achievements.getAchievementsIntent(me.mGoogleApiClient), 2);
        }else{
            loginPlayService();
        }
    }
    static public void showAchievements(){
        Message msg = new Message();
        msg.what = HANDLER_SHOW_ACHIEVEMENTS;
        handler.sendMessage(msg);
    }
    private void updateAchievementHandler(){
        if(me.mGoogleApiClient.isConnected()) {
            Log.e("PD Achieve", "Achievement: " + strArg0 + ", " + intArg0);
            if(intArg0 > 0){
                Games.Achievements.setSteps(me.mGoogleApiClient, strArg0, intArg0);
            }
        }
    }
    static public void updateAchievement(String id, int count) {
        me.strArg0 = id;
        me.intArg0 = count;
        Message msg = new Message();
        msg.what = HANDLER_UPDATE_ACHIEVEMENT;
        handler.sendMessage(msg);
    }
    private void unlockAchievementHandler(){
        if(me.mGoogleApiClient.isConnected()) {
            Log.e("PD Unlock Achieve", "Achievement: " +  strArg0);
            Games.Achievements.unlock(me.mGoogleApiClient, strArg0);
        }
    }
    static public void unlockAchievement(String id){
        me.strArg0 = id;
        Message msg = new Message();
        msg.what = HANDLER_UNLOCK_ACHIEVEMENT;
        handler.sendMessage(msg);
    }
    private void sharePhotoHandler(){
        Log.e(me.TAG, "zabob sharePhoto: " + strArg0 + ", text: " + strArg1);

//        final String path = fileName;
//        final String tweetMessage = text +  "http://onelink.to/platformdefense";

        me.ShareSNS(strArg1, strArg0);
//        Intent share = new Intent(android.content.Intent.ACTION_SEND);
//        share.setType("image/jpeg");
//
//        File imageFileToShare = new File(fileName);
//        Uri uri = Uri.fromFile(imageFileToShare);
//
//        share.putExtra(Intent.EXTRA_STREAM, uri);
//        me.startActivity(Intent.createChooser(share, "Share Image!"));

//        share.putExtra(Intent.EXTRA_TEXT, "http://onelink.to/platformdefense");
//        share.putExtra(Intent.EXTRA_TITLE, "http://onelink.to/platformdefense");
//        share.putExtra(Intent.EXTRA_SUBJECT,  "http://onelink.to/platformdefense");
    }
    // Zabob sns
    static public void sharePhoto(final String fileName, String text) {
        me.strArg0 = fileName;
        me.strArg1 = text;
        Message msg = new Message();
        msg.what = HANDLER_SHARE_PHOTO;
        handler.sendMessage(msg);
    }

    //SNS공유
    public void ShareSNS(String p_str, String p_img_path)
    {
        final String post_str = p_str;
        final String post_img_path = p_img_path;

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                shareIntent(post_str, post_img_path);
            }
        });
    }


    private Intent shareIntent(String msg_str, String post_img_path) {
        //  List<Intent> targetedShareIntents = new ArrayList<Intent>();
        Intent shareIntent = new Intent(Intent.ACTION_SEND);
        shareIntent.setType("image/jpeg");


        shareIntent.putExtra(android.content.Intent.EXTRA_SUBJECT,
                "Platform Defense");
        shareIntent.putExtra(android.content.Intent.EXTRA_TEXT,
                msg_str);

        this.deletePrevImage();

        if (!"".equals(post_img_path)) {
            this.addImageIntent(shareIntent, post_img_path);
        }

        Intent chooserIntent = Intent.createChooser(shareIntent, "");

        startActivity(chooserIntent);
        return shareIntent;
    }

    private String tempImagePath = "";

    private void deletePrevImage()
    {
        if (!"".equals(tempImagePath)) {
            try
            {
                File file = new File(tempImagePath);
                if(file.exists()) {
                    boolean result = file.delete();
                    if (!result){
                        Log.e(me.TAG, "deletePrevImage failed");
                    }
                }

                getContentResolver().delete(Uri.parse(tempImagePath), null, null);
            }
            catch (Exception e)
            {
                Log.e(me.TAG, "deletePrevImage: " + e.toString());
            }
        }
    }

    //이미지를 인텐트에 붙입니다~!
    private void addImageIntent(Intent intent,String img_Path)
    {
        File imgFile = new  File(img_Path);
        if(imgFile.exists()){
            try {
                Bitmap myBitmap = BitmapFactory.decodeFile(imgFile.getAbsolutePath());

                ByteArrayOutputStream bytes = new ByteArrayOutputStream();
                myBitmap.compress(Bitmap.CompressFormat.JPEG, 100, bytes);
                String path = MediaStore.Images.Media.insertImage(getContentResolver(), myBitmap, "ScreenShot", null);

                tempImagePath = path;

                intent.putExtra(Intent.EXTRA_STREAM,
                        Uri.parse(path));

            } catch (Exception e) {
                Log.e(me.TAG, "addImageIntent: " + e.toString());
            }
        }
    }
    // Zabob sns end

    private void shareLinkHandler(){
        Log.e(me.TAG, "shareLink: " + strArg0 + ", link: " + strArg1);
        Intent share = new Intent(android.content.Intent.ACTION_SEND);
        share.setType("text/plain");

        share.putExtra(Intent.EXTRA_SUBJECT, strArg0);
        share.putExtra(Intent.EXTRA_TITLE, strArg0);
        share.putExtra(Intent.EXTRA_TEXT, strArg1);

        me.startActivity(Intent.createChooser(share, "Share Game!"));
    }
    static public void shareLink(final String title, String link) {
        me.strArg0 = title;
        me.strArg1 = link;

        Message msg = new Message();
        msg.what = HANDLER_SHARE_LINK;
        handler.sendMessage(msg);
    }

    public static byte[] convertFileToByteArray(File f)
    {
        byte[] byteArray = null;
        try
        {
            InputStream inputStream = new FileInputStream(f);
            ByteArrayOutputStream bos = new ByteArrayOutputStream();
            byte[] b = new byte[1024*8];
            int bytesRead =0;

            while ((bytesRead = inputStream.read(b)) != -1)
            {
                bos.write(b, 0, bytesRead);
            }

            byteArray = bos.toByteArray();
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
        return byteArray;
    }
    private void loginPlayServiceHandler(){
        Log.e(me.TAG, "loginPlayService");
        me.signInClicked();
    }
    static public void loginPlayService(){
        Message msg = new Message();
        msg.what = HANDLER_LOGIN_PLAY_SERVICE;
        handler.sendMessage(msg);
    }
    private void logoutPlayServiceHandler(){
        Log.e(me.TAG, "logoutPlayService");
        me.signOutclicked();
    }
    static public void logoutPlayService(){
        Message msg = new Message();
        msg.what = HANDLER_LOGOUT_PLAY_SERVICE;
        handler.sendMessage(msg);
    }

    // saved game
    private ProgressDialog mProgressDialog;
    private static final int RC_SELECT_SNAPSHOT = 9002;
    private static final int APP_STATE_KEY = 0;
    SaveGame mSaveGame = new SaveGame();
    private int dataCount = 0;
    public static void setIntForSavedGame(String key, int value){
        Log.e("SavedGame", "setInt: " + key + ", " + value);
        me.mSaveGame.setIntValue(key, value);
    }
    public static void setStringForSavedGame(String key, String value) {
        Log.e("SavedGame", "setString: " + key + ", " + value);
        me.mSaveGame.setStringValue(key, value);
    }
    public static void saveGame() {
        Message msg = new Message();
        msg.what = HANDLER_SAVE_SNAPSHOT;
        handler.sendMessage(msg);
    }
    public static void loadGame() {
        Message msg = new Message();
        msg.what = HANDLER_LOAD_SNAPSHOT;
        handler.sendMessage(msg);
    }
    private void savedGamesLoad(String snapshotName) {
        if(!mGoogleApiClient.isConnected()){
            signInClicked();
            return;
        }
        PendingResult<Snapshots.OpenSnapshotResult> pendingResult = Games.Snapshots.open(
                mGoogleApiClient, snapshotName, false);

        showProgressDialog("Loading Saved Game");
        ResultCallback<Snapshots.OpenSnapshotResult> callback =
                new ResultCallback<Snapshots.OpenSnapshotResult>() {
                    @Override
                    public void onResult(Snapshots.OpenSnapshotResult openSnapshotResult) {
                        if (openSnapshotResult.getStatus().isSuccess()) {
                            Log.e("SavedGame", "saved_games_load_success");
                            byte[] data = new byte[0];
                            try {
                                data = openSnapshotResult.getSnapshot().getSnapshotContents().readFully();
                            } catch (IOException e) {
                                Log.e("SavedGame", "Exception reading snapshot: " + e.getMessage());
                            }
                            mSaveGame.loadFromJson(new String(data));

//                            resetGameData();

                            dataCount = 0;
                            for (String key : mSaveGame.mKeyValueDic.keySet()) {
                                setLoadedData(key, mSaveGame.mKeyValueDic.get(key));
                                dataCount++;
                            }
                            Log.e("SavedGame", "Total Data:" + dataCount + ", Length: " + mSaveGame.toString().length());
//                            setData(new String(data));
//                            displaySnapshotMetadata(openSnapshotResult.getSnapshot().getMetadata());
                        } else {
//                            displayMessage(getString(R.string.saved_games_load_failure), true);
//                            clearDataUI();
                        }

                        dismissProgressDialog();
                    }
                };
        pendingResult.setResultCallback(callback);
    }
    private String makeSnapshotName(int appStateKey) {
        return "Snapshot-" + String.valueOf(appStateKey);
    }
    private void savedGamesUpdate() {
        if(!mGoogleApiClient.isConnected()){
            signInClicked();
            return;
        }
        dataCount = 0;
        for (String key : mSaveGame.mKeyValueDic.keySet()) {
            Log.e("SavedGame", ".setLoadedData: " + key + " ," + mSaveGame.mKeyValueDic.get(key));
            dataCount++;
        }
        Log.e("SavedGame", "Total Data:" + dataCount + ", Length: " + mSaveGame.toString().length());

        final String snapshotName = makeSnapshotName(APP_STATE_KEY);
        final boolean createIfMissing = true;

        // Use the data from the EditText as the new Snapshot data.
        final byte[] data = mSaveGame.toBytes();
        Log.e("SavedGame", "doInBackground 22");

        AsyncTask<Void, Void, Boolean> updateTask = new AsyncTask<Void, Void, Boolean>() {
            @Override
            protected void onPreExecute() {
                showProgressDialog("Updating Saved Game");
                Log.e("SavedGame", "Updating Saved Game");
            }

            @Override
            protected Boolean doInBackground(Void... params) {

                Snapshots.OpenSnapshotResult open = Games.Snapshots.open(
                        mGoogleApiClient, snapshotName, createIfMissing).await();

                Log.e("SavedGame", "data: " + mSaveGame.toString());

                if (!open.getStatus().isSuccess()) {
                    Log.e("SavedGame", "Could not open Snapshot for update.");
                    return false;
                }
                // Change data but leave existing metadata
                Snapshot snapshot = open.getSnapshot();
                snapshot.getSnapshotContents().writeBytes(data);

                Snapshots.CommitSnapshotResult commit = Games.Snapshots.commitAndClose(
                        mGoogleApiClient, snapshot, SnapshotMetadataChange.EMPTY_CHANGE).await();

                if (!commit.getStatus().isSuccess()) {
                    Log.e("SavedGame", "Failed to commit Snapshot.");
                    return false;
                }
                // No failures
                return true;
            }

            @Override
            protected void onPostExecute(Boolean result) {
                if (result) {
                    Log.e("SavedGame", "saved_games_update_success");
                } else {
                    Log.e("SavedGame", "saved_games_update_failure");
                }

                dismissProgressDialog();
//                clearDataUI();
            }
        };
        updateTask.execute();
    }
    private void savedGamesSelect() {
        if(!mGoogleApiClient.isConnected()){
            signInClicked();
            return;
        }
        final boolean allowAddButton = false;
        final boolean allowDelete = false;
        Intent intent = Games.Snapshots.getSelectSnapshotIntent(
                mGoogleApiClient, "Saved Games", allowAddButton, allowDelete,
                Snapshots.DISPLAY_LIMIT_NONE);

        showProgressDialog("Loading");
        startActivityForResult(intent, RC_SELECT_SNAPSHOT);
    }
    private void showProgressDialog(String msg) {
        if (mProgressDialog == null) {
            mProgressDialog = new ProgressDialog(this);
            mProgressDialog.setIndeterminate(true);
        }

        mProgressDialog.setMessage(msg);
        mProgressDialog.show();
    }
    private void dismissProgressDialog() {
        if (mProgressDialog != null && mProgressDialog.isShowing()) {
            mProgressDialog.dismiss();
        }
    }
    // saved game end
    private static final int REQUEST_RESOLVE_ERROR = 1001;
    private boolean mResolvingError = false;
    private String mCurrentSaveName = "snapshotTemp";
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent intent) {
        if (requestCode == RC_SELECT_SNAPSHOT) {
            dismissProgressDialog();
            Log.d(TAG, "onActivityResult: RC_SELECT_SNAPSHOT, resultCode = " + resultCode);
            if (resultCode == RESULT_OK) {
                // Successfully returned from Snapshot selection UI
                if (intent != null) {
                    Bundle bundle = intent.getExtras();
                    SnapshotMetadata selected = Games.Snapshots.getSnapshotFromBundle(bundle);
                    if (selected == null) {
                        // No snapshot in the Intent bundle, display error message
                        Log.e("SavedGame", "saved_games_select_failure");
                        //setData(null); text box data
                        //displaySnapshotMetadata(null); brief mata data for the snapshot
                    } else {
                        // Found Snapshot Metadata in Intent bundle.  Load Snapshot by name.
                        String snapshotName = selected.getUniqueName();
                        savedGamesLoad(snapshotName);
                    }
                }
            } else {
                // User canceled the select intent or it failed for some other reason
                Log.e("SavedGame","saved_games_select_cancel");
                //setData(null); text box data
                //displaySnapshotMetadata(null); brief mata data for the snapshot
            }
        }
        // saved game
      /*  boolean signedIn = (mGoogleApiClient != null && mGoogleApiClient.isConnected());
        if (intent != null && signedIn) {
            if (intent.hasExtra(Snapshots.EXTRA_SNAPSHOT_METADATA)) {
                // Load a snapshot.
                SnapshotMetadata snapshotMetadata = (SnapshotMetadata)
                        intent.getParcelableExtra(Snapshots.EXTRA_SNAPSHOT_METADATA);
                currentSaveName = snapshotMetadata.getUniqueName();


                this.loadFromSnapshot(snapshotMetadata);

                // Load the game data from the Snapshot
                // ...
            } else if (intent.hasExtra(Snapshots.EXTRA_SNAPSHOT_NEW)) {
                // Create a new snapshot named with a unique string
                String unique = new BigInteger(281, new Random()).toString(13);
                currentSaveName = "snapshotTemp-" + unique;


                // Create the new snapshot
                // ...
                // Load a snapshot.
                String conflictId = intent.getStringExtra(SelectSnapshotActivity.CONFLICT_ID);
                int retryCount = intent.getIntExtra(SelectSnapshotActivity.RETRY_COUNT,
                        3);
                SnapshotMetadata snapshotMetadata =
                        intent.getParcelableExtra(SelectSnapshotActivity.SNAPSHOT_METADATA);
                if (conflictId == null) {
                    saveSnapshot(null);
                } else {
                    Log.d(TAG,"resolving " + snapshotMetadata);
                    resolveSnapshotConflict(requestCode, conflictId, retryCount, snapshotMetadata);
                }
            }
        }
*/

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
    private void openUrlHandler(){
        Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(strArg0));
        me.startActivity(browserIntent);
    }
    static public void openUrl(String url){
        me.strArg0 = url;
        Message msg = new Message();
        msg.what = HANDLER_OPEN_URL;
        handler.sendMessage(msg);
    }
    private void showUnityAdsVideoHandler(){
        if(UnityAds.canShow()) {
            Log.e(me.TAG, "showVideo");
            UnityAds.show();
        }else if (IncentivizedAd.isAvailable()) {
            IncentivizedAd.display(this);
        }else{
            insteadOfUnityAds = true;
            showImmediatly = true;
            if(mInterstitialAd.isLoaded()){
                showInterstitial();
            }else{
                requestNewInterstitial();
            }

            Log.e(me.TAG, "cannot show");
        }
    }
    static public void showUnityAdsVideo(){
        Message msg = new Message();
        msg.what = HANDLER_SHOW_UNITY_ADS_VIDEO;
        handler.sendMessage(msg);
    }
    @Override
    public void onVideoCompleted(String itemKey, boolean skipped){
        if(!skipped){
            Log.e(TAG, "video complete");
            unityAdsCallback();;
        }else{
            Log.e(TAG, "video skipped");
        }
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
    public void onShow(){

    }

    @Override
    public void onHide(){

    }
    private void showLeaderboardsHandler(){
        Log.e(me.TAG, "showLeaderboards on AppActivity.java" + strArg0);

        if(me.mGoogleApiClient.isConnected()){
            me.startActivityForResult(Games.Leaderboards.getLeaderboardIntent(me.mGoogleApiClient,
                    strArg0), 2);//REQUEST_LEADERBOARD);
        }else{
            loginPlayService();
        }
    }
    static public void showLeaderboards(String leaderboardID) {
        me.strArg0 = leaderboardID;
        Message msg = new Message();
        msg.what = HANDLER_SHOW_LEADERBOARDS;
        handler.sendMessage(msg);
    }
    private void updateTopScoreLeaderboardHandler(){
        Log.e(me.TAG, "post score on AppActivity.java");
        if(me.mGoogleApiClient.isConnected()){
            Games.Leaderboards.submitScore(me.mGoogleApiClient, strArg0, intArg0);
        }
    }
    static public void updateTopScoreLeaderboard(int score, String leaderboardID) {
        me.intArg0 = score;
        me.strArg0 = leaderboardID;
        Message msg = new Message();
        msg.what = HANDLER_UPDATE_TOP_SCORE;
        handler.sendMessage(msg);
    }
    @Override
    protected void onResume() {
        super.onResume();
        UnityAds.changeActivity(this);
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
        Log.e("Admob", "1");
        playServiceConnected(false);
        Log.e("Admob", "2");
        if (mResolvingConnectionFailure) {
            // already resolving
            return;
        }
        Log.e("Admob", "3");


        // Put code here to display the sign-in button
    }

    @Override
    public void onConnectionSuspended(int i) {
        // Attempt to reconnect
        mGoogleApiClient.connect();
    }
}
