#include "GameRole.h"
#include "msg.pb.h"
#include "GameMsg.h"

bool GameRole::Init()
{
    return true;
}

UserData* GameRole::ProcMsg(UserData& _poUserData)
{
    GET_REF2DATA(MultiMsg, multiMsg, _poUserData);
    
    return nullptr;
}

void GameRole::Fini()
{
}
