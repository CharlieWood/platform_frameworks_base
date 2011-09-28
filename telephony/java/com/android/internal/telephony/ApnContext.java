/*
 * Copyright (C) 2006 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.internal.telephony;

import android.app.PendingIntent;

import android.util.Log;
import android.os.SystemProperties;
import java.util.ArrayList;
import com.android.internal.telephony.gsm.GsmDataConnection;

/**
 * Maintain the Apn context
 */
public class ApnContext {

    public static final int PENDING_ACTION_NONE = 1;
    public static final int PENDING_ACTION_RECONNECT = 2;
    public static final int PENDING_ACTION_APN_DISABLE = 3;

    public static final int DATA_NOT_USED = 0;
    public static final int DATA_ENABLED = 1;
    public static final int DATA_DISABLED = 2;

    public final String LOG_TAG;

    int mPendingAction;

    protected static final boolean DBG = true;

    String mApnType;

    DataConnectionTracker.State mState;

    ArrayList<ApnSetting> mWaitingApns = null;

    private int mWaitingApnsPermanentFailureCountDown = 0;

    ApnSetting mApnSetting;

    GsmDataConnection mDataConnection;

    String mReason;

    PendingIntent mReconnectIntent;

    int mDataEnabled;

    /**
    * Whether this APN depned on another APN or the some status
    * For VZW LTE requiremnts, default and CBS is depend on the IMS PDN
    * If whenever IMS PDN disconnected or IMS registration failed,
    * we need set mApnDependencyMet to false for default and CBS PDN
    */
    boolean mApnDependencyMet = true;

    public ApnContext(String apnType, String logTag) {
        mApnType = apnType;
        mState = DataConnectionTracker.State.IDLE;
        setReason(Phone.REASON_DATA_ENABLED);
        mPendingAction = PENDING_ACTION_NONE;
        mApnDependencyMet = !SystemProperties.getBoolean(TelephonyProperties.PROPERTY_APN_DEPENDENCY_ON, false);
        mDataEnabled = DATA_NOT_USED;
        LOG_TAG = logTag;
    }

    public int getPendingAction() {
        return mPendingAction;
    }

    public void setPendingAction(int pa) {
        mPendingAction = pa;
    }

    public int getPermFailCount() {
        return mWaitingApnsPermanentFailureCountDown;
    }

    public void decPermFailCount() {
        mWaitingApnsPermanentFailureCountDown--;
    }

    public String getApnType() {
        return mApnType;
    }

    public GsmDataConnection getDataConnection() {
        return mDataConnection;
    }

    public void setDataConnection(GsmDataConnection dataConnection) {
        mDataConnection = dataConnection;
    }

    public ApnSetting getApnSetting() {
        return mApnSetting;
    }

    public void setApnSetting(ApnSetting apnSetting) {
        mApnSetting = apnSetting;
    }

    public void setWaitingApns(ArrayList<ApnSetting> waitingApns) {
        mWaitingApns = waitingApns;
    }

    public ApnSetting getNextApn() {
        ArrayList<ApnSetting> list = mWaitingApns;
        ApnSetting apn = null;

        if (list != null) {
            if (!list.isEmpty()) {
                apn = list.get(0);
            }
        }
        return apn;
    }

    public void removeNextApn() {
        if ((mWaitingApns != null) && (!mWaitingApns.isEmpty())) {
            mWaitingApns.remove(0);
        }
    }

    public ArrayList<ApnSetting> getWaitingApns() {
        return mWaitingApns;
    }

    public void setState(DataConnectionTracker.State s) {
        if (DBG)
            log("setState: " + s + " for type " + mApnType + ", previous state:" + mState);

        mState = s;

        if (mState == DataConnectionTracker.State.FAILED) {
            if (mWaitingApns != null)
                mWaitingApns.clear(); // when teardown the connection and set to IDLE
        }
    }

    public DataConnectionTracker.State getState() {
        return mState;
    }

    public void setReason(String reason) {
        if (DBG)
            log("set reason as " + reason + ", for type " + mApnType + ",current state " + mState);
        mReason = reason;
    }

    public String getReason() {
        return mReason;
    }

    public void setReconnectIntent(PendingIntent intent) {
        if (DBG)
            log("set ReconnectIntent for type " + mApnType);
        mReconnectIntent = intent;
    }

    public PendingIntent getReconnectIntent() {
        return mReconnectIntent;
    }

    public void setEnabled(boolean enabled) {
        if(DBG)log("set enabled as "+enabled+", for type "+mApnType+",current state "+mState);
        if(enabled) mDataEnabled = DATA_ENABLED;
        else mDataEnabled = DATA_DISABLED;
    }

    public void setApnDependencyMet(boolean depended) {
        if(DBG)log("set ApnDependencyMet as "+depended+", for type "+mApnType+",current state "+mState);
        mApnDependencyMet = depended;
    }

    public boolean isEnabled() {
        return ((mDataEnabled == DATA_ENABLED) && mApnDependencyMet);
    }

    // Returns only those that are DISABLED. Skips NOT_USED.
    public boolean isDisabled() {
        return (mDataEnabled == DATA_DISABLED) ;
    }

    public boolean isDependencyMet() {
        return mApnDependencyMet;
    }

    protected void log(String s) {
        Log.d(LOG_TAG, "[ApnContext] " + s);
    }
}
