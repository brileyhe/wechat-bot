#pragma once
#include <Windows.h>
#include <memory>
#include <list>
#include <vector>
#include <nlohmann/json.hpp>
#include <arris/util/json_util.hpp>
#include <arris/util/log_util.hpp>
#include <arris/util/str_util.hpp>
#include  <arris/util/read_mem.hpp>
#include <arris/wechat/constant.hpp>
#include <arris/wechat/offset_version.hpp>

using nlohmann::json;
using namespace arris::util;
using namespace arris::wechat;

namespace arris {
	namespace wechat {
		class contact_tree {
		public:
			contact_tree() :header_node_(0){
				header_node_ = header_node();
				contact_json_["type"] = MsgType::kUserListFromContact;
			}

			DWORD node() {
				DWORD result = read_dword(NULL, header_node());
				return result;
			}

			std::string to_string(const std::string& id,int type) {
				std::string result;
				try {
					result = contact_json_.dump();
				}
				catch (json::exception& e) {
					tinyjson tiny;
					result = tiny.ret_msg(id, kMsgFailedStatus, e.what(),type);
				}
				return result;
			}


			void recurse(DWORD node) {
				if (node == header_node_) {
					return;
				}

				for (auto item : node_list_){
					if (item == node) return;
				}
				node_list_.push_back(node);
				
				std::wstring id = get_wxid(node);
				std::wstring nick = get_nick(node);
				json j;
				j["wxid"]= wstring_to_string(id);
				j["name"] = ucs2_to_utf8(wstring_to_wchar(nick));
				contact_json_["content"].push_back(j);

				DWORD node1 = read_dword(NULL, node);
				DWORD node2 = read_dword(NULL, node + 0x04);
				DWORD node3 = read_dword(NULL, node + 0x08);

				recurse(node1);
				recurse(node2);
				recurse(node3);

			}
		private:

			DWORD header_node() {
				DWORD result = read_dword(NULL, base_node() + static_cast<DWORD>(version::contact_data_offset::contact_addr_offset));
				return result;
			}
			DWORD base_node() {
				DWORD base_addr = get_wx_baseaddr();
				DWORD addr = read_dword(NULL, base_addr + static_cast<DWORD>(version::contact_data_offset::contact_offset));
				return addr;
			}
			std::wstring get_wxid(DWORD node) {
				DWORD sign = read_dword(NULL,node+ static_cast<DWORD>(version::contact_data_offset::wxid_sign));
				
				if (sign == 0) {
					int size = read_dword(NULL,node + static_cast<DWORD>(version::contact_data_offset::wxid1_len));
					DWORD addr = read_dword(NULL,node + static_cast<DWORD>(version::contact_data_offset::wxid1_offset));
					std::string wxid = read_char(NULL,addr , size);
					std::wstring w_wxid = string_to_wstring(wxid);
					return w_wxid;
				}
				else {
					int size = read_dword(NULL, node + static_cast<DWORD>(version::contact_data_offset::wxid2_len));
					DWORD addr = read_dword(NULL, node + static_cast<DWORD>(version::contact_data_offset::wxid2_offset));
					std::wstring wxid = read_wchar(NULL,addr,size);
					return wxid;
				}
			}
			std::wstring get_nick(DWORD node) {
				int size = read_dword(NULL, node + static_cast<DWORD>(version::contact_data_offset::wxnick_len));
				DWORD addr = read_dword(NULL, node + static_cast<DWORD>(version::contact_data_offset::wxnick_offset));
				std::wstring nick = read_wchar(NULL, addr, size);
				return nick;
			}
		private:
			DWORD header_node_;
			json contact_json_;
			std::list<DWORD> node_list_;
		};//class btree

		using contact_tree_ptr = std::unique_ptr<contact_tree>;
	}//namespace util
}//namespace arris