package com.skplanet.dev.guide.helper;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public final class ParamsBuilder {
	public static final String KEY_APPID = "appid";
	public static final String KEY_PID = "product_id";
	public static final String KEY_PNAME = "product_name";
	public static final String KEY_TID = "tid";
	public static final String KEY_BPINFO = "bpinfo";
	
	private ConcurrentHashMap<String, String> mParams = null;

	public ParamsBuilder() {
		mParams = new ConcurrentHashMap<String, String>();
	}

	public ParamsBuilder put(Map<String, String> source) {
		for (Map.Entry<String, String> entry : source.entrySet()) {
			put(entry.getKey(), entry.getValue());
		}

		return this;
	}

	public ParamsBuilder put(String k, String v) {
		if (k != null && v != null) {
			if (mParams.contains(k)) {
				mParams.replace(k, v);
			} else {
				mParams.put(k, v);
			}
		}
		return this;
	}

	public ParamsBuilder remove(String k) {
		mParams.remove(k);
		return this;
	}

	public String build() {
		return toString();
	}

	@Override
	public String toString() {
		StringBuffer result = new StringBuffer();
		for (ConcurrentHashMap.Entry<String, String> entry : mParams.entrySet()) {
			if (result.length() > 0)
				result.append("&");

			result.append(entry.getKey());
			result.append("=");
			result.append(entry.getValue());
		}

		return result.toString();
	}
}
