#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cctype>
#include <thread>
#include <chrono>
#include "mqtt/async_client.h"
#include <stdio.h>
#include <sqlite3.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
std::string jj, fr, api, rqi, rn, to, aei;
int ty, op;
bool lcl, rr;

using namespace std;
const string SERVER_ADDRESS	{ "tcp://192.168.0.113:1883" };
const string CLIENT_ID		{ "Asim_SUB" };
const string TOPIC 			{ "#" };
const int  QOS = 1;

int main(int argc, char* argv[])
{
	mqtt::connect_options connOpts;
	connOpts.set_keep_alive_interval(20);
	connOpts.set_clean_session(true);
	mqtt::async_client cli(SERVER_ADDRESS, CLIENT_ID);
	try {
		cout << "Connecting to the MQTT server..." << flush;
		cli.connect(connOpts)->wait();
		cli.start_consuming();
		cli.subscribe(TOPIC, QOS)->wait();
		cout << "OK" << endl;
		// Consume messages
		while (true) {
			auto msg = cli.consume_message();
			if (!msg) break;
			cout << msg->get_topic() << ": " << msg->to_string() << endl;
			jj = msg->to_string(); //payload is saving in the jj variable

//-------------------------> MY ADITIONAL PART <-----------------------------//
					
			//auto response = json::parse(jj.c_str());{
			auto response = json::parse(jj);{
			op=response["op"];
			rqi=response["rqi"];
			api=response["pc"]["m2m:ae"]["api"];
			rn=response["pc"]["m2m:ae"]["rn"];
			rr=response["pc"]["m2m:ae"]["rr"];
			to=response["to"];
			fr=response["fr"];

			std::cout << "Operation(op): " << op << std::endl;
			std::cout << "Request Identity(rqi): " << rqi << std::endl;
			std::cout << "Application Identity(api): " << api << std::endl;
			std::cout << "Resource Name(rn): " << rn << std::endl;
			std::cout << "Request Reachability(rr): " << rr << std::endl;
			std::cout << "Sending Towards(to): " << to << std::endl;
			std::cout << "Sent From(fr): " << fr << std::endl;}
    	
//---------------------------------------------------------------------------//


		}
		// Disconnect
		cout << "\nShutting down and disconnecting from the MQTT server..." << flush;
		cli.unsubscribe(TOPIC)->wait();
		cli.stop_consuming();
		cli.disconnect()->wait();
		cout << "OK" << endl;
	}
	catch (const mqtt::exception& exc) {
		cerr << exc.what() << endl;
		return 1;
	}
 	return 0;
}
