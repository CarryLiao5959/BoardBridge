#include "SSocket.h"
#include "System.h"
#include "Singleton.h"
#include "SocketHandler.h"

int main()
{
    Singleton<System>::instance()->init();
    
    return 0;
}