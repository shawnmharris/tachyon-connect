
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

	if (udpcon->Bind("127.0.0.1", 5000) != 0)
	{
		std::cerr << "error binding" << std::endl;
		exit(0);
	}

	msgBuf m;
	memset(&m, 0, sizeof(msgBuf));
	std::cout << "listening for message on port 5000" << std::endl;
	
	if (udpcon->Recv(&m) > 0)
	{
		std::cout << "got message:" << std::endl;
		std::cout << m.data << std::endl;
	}

}

