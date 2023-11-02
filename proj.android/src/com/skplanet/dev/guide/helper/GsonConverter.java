package com.skplanet.dev.guide.helper;

import com.google.gson.Gson;
import com.skplanet.dev.guide.pdu.CommandRequest;
import com.skplanet.dev.guide.pdu.Response;
import com.skplanet.dev.guide.pdu.VerifyReceipt;

public class GsonConverter implements Converter {
	private final Gson mGson = new Gson();
	
	@Override
	public String toJson(CommandRequest r) {
		return mGson.toJson(r);
	}

	@Override
	public Response fromJson(String json) {		
		return mGson.fromJson(json, Response.class);
	}

    @Override
    public VerifyReceipt fromJson2VerifyReceipt(String json) { 
        return mGson.fromJson(json, VerifyReceipt.class);
    }

}
