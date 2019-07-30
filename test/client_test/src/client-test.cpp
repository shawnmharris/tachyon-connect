
#include <iostream>
#include <tachyon/component.hpp>
#include <iconnect.hpp>


using namespace tachyon;


int main()
{
	MasterFactory &mf = MasterFactory::Instance();
	mf.Manage("tachyon-connect");

	sp<IConnect> udpcon = mf.Create<IConnect>("IConnect.UDPConnect");

	if (!udpcon.isValid())
	{
		std::cerr << "Invalid connection, program abort" << std::endl;
		exit(0);
	}

	if (!udpcon->PostInit())
	{
		std::cerr << "PostInit failure, program abort" << std::endl;
		exit(0);
	}

	msgBuf m;
	memset(&m, 0, sizeof(msgBuf));
	char s[] = "spiderman is awesome!";
	memcpy(m.data, s, strlen(s));
	m.dataLen = strlen(s);
	udpcon->Send(m, "127.0.0.1", 5000);
}

