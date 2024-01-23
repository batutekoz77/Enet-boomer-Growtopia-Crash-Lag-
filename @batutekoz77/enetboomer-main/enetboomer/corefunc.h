#pragma once
#include "enet/enet.h"
#include <string>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>

using namespace std;
string SERVER_HOST = "127.0.0.1"; // Server IP.
int SERVER_PORT = 17777; // Server Udp Port.
bool SERVER_CRASH = false; // True means send crash packets to server.
bool SERVER_LAG = false;  // True means send lag packets to server.

/*********  hidden vars  ********/
class GrowtopiaBot {
public:
	ENetPeer *peer;
	ENetHost * client;

	long long lastreq = 0;
	bool connected = false;
	/********** user sutff ***************/

	enum {
		PACKET_STATE = 0,
		PACKET_CALL_FUNCTION,
		PACKET_UPDATE_STATUS,
		PACKET_TILE_CHANGE_REQUEST,
		PACKET_SEND_MAP_DATA,
		PACKET_SEND_TILE_UPDATE_DATA,
		PACKET_SEND_TILE_UPDATE_DATA_MULTIPLE,
		PACKET_TILE_ACTIVATE_REQUEST,
		PACKET_TILE_APPLY_DAMAGE,
		PACKET_SEND_INVENTORY_STATE,
		PACKET_ITEM_ACTIVATE_REQUEST,
		PACKET_ITEM_ACTIVATE_OBJECT_REQUEST,
		PACKET_SEND_TILE_TREE_STATE,
		PACKET_MODIFY_ITEM_INVENTORY,
		PACKET_ITEM_CHANGE_OBJECT,
		PACKET_SEND_LOCK,
		PACKET_SEND_ITEM_DATABASE_DATA,
		PACKET_SEND_PARTICLE_EFFECT,
		PACKET_SET_ICON_STATE,
		PACKET_ITEM_EFFECT,
		PACKET_SET_CHARACTER_STATE,
		PACKET_PING_REPLY,
		PACKET_PING_REQUEST,
		PACKET_GOT_PUNCHED,
		PACKET_APP_CHECK_RESPONSE,
		PACKET_APP_INTEGRITY_FAIL,
		PACKET_DISCONNECT,
		PACKET_BATTLE_JOIN,
		PACKET_BATTLE_EVEN,
		PACKET_USE_DOOR,
		PACKET_SEND_PARENTAL,
		PACKET_GONE_FISHIN,
		PACKET_STEAM,
		PACKET_PET_BATTLE,
		PACKET_NPC,
		PACKET_SPECIAL,
		PACKET_SEND_PARTICLE_EFFECT_V2,
		GAME_ACTIVE_ARROW_TO_ITEM,
		GAME_SELECT_TILE_INDEX
	};
	enum {
		NET_MESSAGE_UNKNOWN = 0,
		NET_MESSAGE_SERVER_HELLO,
		NET_MESSAGE_GENERIC_TEXT,
		NET_MESSAGE_GAME_MESSAGE,
		NET_MESSAGE_GAME_PACKET,
		NET_MESSAGE_ERROR,
		NET_MESSAGE_TRACK,
		NET_MESSAGE_CLIENT_LOG_REQUEST,
		NET_MESSAGE_CLIENT_LOG_RESPONSE,
	};

	/*********** structs declaration *********/
	struct gameupdatepacket_t {
		uint8_t m_type;
		uint8_t m_netid;
		uint8_t m_jump_amount;
		uint8_t m_count;
		int32_t m_player_flags;
		int32_t m_item;
		int32_t m_packet_flags;
		float m_struct_flags;
		int32_t m_int_data;
		float m_vec_x;
		float m_vec_y;
		float m_vec2_x;
		float m_vec2_y;
		float m_particle_time;
		uint32_t m_state1;
		uint32_t m_state2;
		uint32_t m_data_size;
		uint32_t m_data;
	};

	/********* user funcs  *********/
	void userLoop();
	void userInit();
	//void onLoginRequested();
	void WhenConnected();
	void WhenDisconnected();
	/********* user funcs  *********/

	/***************    debug stuff   ***************/
	void dbgPrint(string text)
	{
		cout << "[DBG] " + text << endl;
	}
	/***************    debug stuff   ***************/

	typedef struct gametankpacket_t {
		int32_t m_type;
		char m_data;
	} gametextpacket_t;
	/******************* enet core *********************/
	void SendPacket2(int32_t type, uint8_t* data, int32_t len) {
		
		auto packet = enet_packet_create(0, len + 5, ENET_PACKET_FLAG_RELIABLE);
		auto game_packet = (gametextpacket_t*)packet->data;
		game_packet->m_type = type;
		if (data)
			memcpy(&game_packet->m_data, data, len);

		memset(&game_packet->m_data + len, 0, 1);
		enet_peer_send(peer, 0, packet);
		
	}

	
	void SendPacket(int a1, string a2, ENetPeer* enetPeer)
	{
		if (enetPeer)
		{
			ENetPacket* v3 = enet_packet_create(0, a2.length() + 5, 1);
			memcpy(v3->data, &a1, 4);
			//*(v3->data) = (DWORD)a1;
			memcpy((v3->data) + 4, a2.c_str(), a2.length());

			//cout << std::hex << (int)(char)v3->data[3] << endl;
			enet_peer_send(enetPeer, 0, v3);
		}
	}
	

	// Connect with default value
	void connectClient() {
		connectClient(SERVER_HOST, SERVER_PORT);
		/*
			* Connecting to Server.
		*/
	}

	void connectClient(string hostName, int port)
	{
		client = enet_host_create(NULL /* create a client host */,
			1 /* only allow 1 outgoing connection */,
			2 /* allow up 2 channels to be used, 0 and 1 */,
			0 /* 56K modem with 56 Kbps downstream bandwidth */,
			0 /* 56K modem with 14 Kbps upstream bandwidth */);
		if (client == NULL)
		{
			cout << "An error occurred while trying to create an ENet client host.\n";
			exit(EXIT_FAILURE);
		}
		ENetAddress address;

		client->checksum = enet_crc32;
		enet_host_compress_with_range_coder(client);
		enet_address_set_host(&address, hostName.c_str());
		address.port = port;

		/* Initiate the connection, allocating the two channels 0 and 1. */
		peer = enet_host_connect(client, &address, 2, 0);
		if (peer == NULL)
		{
			cout << "No available peers for initiating an ENet connection.\n";
			exit(EXIT_FAILURE);
		}
		enet_host_flush(client);
	}
	/******************* enet core *********************/


	void eventLoop()
	{
		while (true) {
			ENetEvent event;
			while (enet_host_service(client, &event, 0) > 0)
			{
				switch (event.type)
				{
				case ENET_EVENT_TYPE_NONE:
					cout << "No event???" << endl;
					break;
				case ENET_EVENT_TYPE_CONNECT:
					WhenConnected();
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					WhenDisconnected();
					break;
				case ENET_EVENT_TYPE_RECEIVE:
					//ProcessPacket(&event, peer);
					enet_packet_destroy(event.packet);
					break;
				default:
					break;
				}
			}
			userLoop();

			Sleep(50); 

			/*
				* Adding Sleep function will lower your CPU usage.
				* If you think that your pc is powerfull, you can lower sleep ms.
			*/
		}
	}
};

void init() {
	if (enet_initialize() != 0) {
		fprintf(stderr, "An error occurred while initializing ENet.\n");
		exit(0);
	}
	atexit(enet_deinitialize);
	srand(time(NULL));
}
