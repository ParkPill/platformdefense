package com.skplanet.dev.guide.helper;

import com.skplanet.dev.guide.pdu.Command;
import com.skplanet.dev.guide.pdu.CommandRequest;

public final class CommandBuilder {
	private final Converter mConverter;

	public CommandBuilder() {
		mConverter = ConverterFactory
				.getConverter(ConverterFactory.GSON);
	}

	public CommandBuilder(final String type) {
		mConverter = ConverterFactory.getConverter(type);
	}

	public final String requestPurchaseHistory(String appid,
			String... product_ids) {
		CommandRequest r = CommandRequest.makeRequest(
				Command.request_purchase_history.method(),
				CommandRequest.Parameter.makeParam(appid, product_ids));
		return mConverter.toJson(r);
	}

	public final String requestProductInfo(String appid) {
		CommandRequest r = CommandRequest.makeRequest(
				Command.request_product_info.method(),
				CommandRequest.Parameter.makeParam(appid));
		return mConverter.toJson(r);
	}

	public final String checkPurchasability(String appid, String... product_ids) {
		CommandRequest r = CommandRequest.makeRequest(
				Command.check_purchasability.method(),
				CommandRequest.Parameter.makeParam(appid, product_ids));
		return mConverter.toJson(r);
	}

	public final String chnageProductProperties(String appid, String action,
			String... product_ids) {
		CommandRequest r = CommandRequest.makeRequest(
				Command.change_product_properties.method(),
				CommandRequest.Parameter.makeParam(appid, action, product_ids));
		return mConverter.toJson(r);
	}

	public final String authItem(String appid, String... product_ids) {
		CommandRequest r = CommandRequest.makeRequest(
				Command.auth_item.method(),
				CommandRequest.Parameter.makeParam(appid, product_ids));
		return mConverter.toJson(r);
	}

	public final String wholeAuthItem(String appid) {
		CommandRequest r = CommandRequest.makeRequest(
				Command.whole_auth_item.method(),
				CommandRequest.Parameter.makeParam(appid));
		return mConverter.toJson(r);
	}

	public final String itemUse(String appid, String... product_ids) {
		CommandRequest r = CommandRequest.makeRequest(
				Command.item_use.method(),
				CommandRequest.Parameter.makeParam(appid, product_ids));
		return mConverter.toJson(r);
	}

	public final String monthlyWithdraw(String appid, String... product_ids) {
		CommandRequest r = CommandRequest.makeRequest(
				Command.monthly_withdraw.method(),
				CommandRequest.Parameter.makeParam(appid, product_ids));
		return mConverter.toJson(r);
	}
}
