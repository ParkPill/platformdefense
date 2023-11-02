package com.skplanet.dev.guide.helper;

import com.skplanet.dev.guide.pdu.CommandRequest;
import com.skplanet.dev.guide.pdu.Response;
import com.skplanet.dev.guide.pdu.VerifyReceipt;

public interface Converter {
    public String toJson(final CommandRequest r);

    public Response fromJson(final String json);

    public VerifyReceipt fromJson2VerifyReceipt(final String json);
}
