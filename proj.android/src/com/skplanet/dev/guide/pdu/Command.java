package com.skplanet.dev.guide.pdu;

public enum Command {
	auth_item {
		@Override
		public String method() {
			return "auth_item";
		}
	},
	whole_auth_item {
		@Override
		public String method() {
			return "whole_auth_item";
		}
	},
	monthly_withdraw {
		@Override
		public String method() {
			return "monthly_withdraw";
		}
	},
	item_use {
		@Override
		public String method() {
			return "item_use";
		}
	},
	request_purchase_history {
		@Override
		public String method() {
			return "request_purchase_history";
		}
	},
	check_purchasability {
		@Override
		public String method() {
			return "check_purchasability";
		}
	},
	request_product_info {
		@Override
		public String method() {
			return "request_product_info";
		}
	},
	change_product_properties {
		@Override
		public String method() {
			return "change_product_properties";
		}
	};
	public abstract String method();
}
