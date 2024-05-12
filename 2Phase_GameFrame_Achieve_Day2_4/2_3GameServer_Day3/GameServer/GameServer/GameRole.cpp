#include "GameRole.h"
#include "GameProtocol.h"
#include "GameMsg.h"
#include "GameChannel.h"
#include "msg.pb.h"
#include <algorithm>
#include <random>
#include "ZinxTimer.h"

static std::default_random_engine random_engine(time(NULL));
static AOIWorld world(0, 400, 0, 400, 20, 20);

class Exit_Timer : public TimerOutProc
{
public:
    Exit_Timer()
    {
        //ZinxKernel::Zinx_Add_Channel(*new ZinxTimerChannel());
    }
    // 通过 TimerOutProc 继承
    void Proc() override
    {
        ZinxKernel::Zinx_Exit();
    }
    int GetTimeSec() override
    {
        return 7;
    }
};

Exit_Timer exit_Timer;

void GameRole::ViewAppear(GameRole* pRole)
{
    ZinxKernel::Zinx_SendOut(*CreateSelfPosition(), *pRole->m_proto);
    ZinxKernel::Zinx_SendOut(*pRole->CreateSelfPosition(), *m_proto);
}

void GameRole::ViewLost(GameRole* pRole)
{
    ZinxKernel::Zinx_SendOut(*CreateIDNameLogoff(), *pRole->m_proto);
    ZinxKernel::Zinx_SendOut(*pRole->CreateIDNameLogoff(), *m_proto);
}

void GameRole::ProcNewPos(float _x, float _y, float _z, float _v)
{
    auto s1 = world.GetSrdPlayerPosition(this);
    world.Del_Player(this);
    x = _x;
    y = _y;
    z = _z;
    v = _v;
    world.Add_Player(this);

    auto s2 = world.GetSrdPlayerPosition(this);

    for (auto single : s2)
    {
        if (s1.end() == std::find(s1.begin(), s1.end(), single))
        {
            ViewAppear(dynamic_cast<GameRole*>(single));
        }
    }

    for (auto single : s1)
    {
        if (s2.end() == std::find(s2.begin(), s2.end(), single))
        {
            // std::cout << "---------------------------------------------------------|\n|\n|\n|\n|\n|\n|**********************-----------------------------------------------------------------------------------*********\n" << std::endl;
            ViewLost(dynamic_cast<GameRole*>(single));
        }
    }
    
    auto player_list = world.GetSrdPlayerPosition(this);

	auto pmsg = new pb::BroadCast();
	auto new_pos = pmsg->mutable_p();
	new_pos->set_x(_x);
	new_pos->set_y(_y);
	new_pos->set_z(_z);
	new_pos->set_v(_v);
	pmsg->set_tp(2); // 不确定是不是第二个
	pmsg->set_pid(Pid);
	pmsg->set_username(szName);

    for (auto single : player_list)
    {
        auto single_player = dynamic_cast<GameRole*>(single);
        GameMsg* p = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pmsg);
        ZinxKernel::Zinx_SendOut(*p, *(single_player->m_proto));
    }
}

void GameRole::ProcChat(std::string content)
{
    auto player_list = world.GetSrdPlayerPosition(this);

	auto c = new pb::BroadCast();
	c->set_pid(Pid);
	c->set_username(szName);
	c->set_tp(1);
	c->set_content(content);

    for (auto single : player_list)
    {
        auto single_player = dynamic_cast<GameRole*>(single);
                GameMsg* pmsg = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, c);
        ZinxKernel::Zinx_SendOut(*pmsg, *single_player->m_proto);
    }
}

GameMsg* GameRole::CreateIDNameLogin()
{
    pb::SyncPid* pMsg = new pb::SyncPid();
    pMsg->set_pid(Pid);
    pMsg->set_username(szName);
    GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID_NAME, pMsg);
    return pRet;
}

GameMsg* GameRole::CreateSrdPlayerPosition()
{
    pb::SyncPlayers* pMsg = new pb::SyncPlayers();

    auto player_list = world.GetSrdPlayerPosition(this);

    for (auto single : player_list)
    {
        // pb::SyncPlayer* pMsg = new pb::SyncPlayer();
        auto pPlayer = pMsg->add_ps();

        auto pRole = dynamic_cast<GameRole*>(single);
        pPlayer->set_pid(pRole->Pid);
        pPlayer->set_username(pRole->szName);

        auto pPostion = pPlayer->mutable_p();
        pPostion->set_x(pRole->x);
        pPostion->set_y(pRole->y);
        pPostion->set_z(pRole->z);
        pPostion->set_v(pRole->v);

    }

    GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE_SRD_POSITION, pMsg);
    return pRet;
}

GameMsg* GameRole::CreateSelfPosition()
{
    pb::BroadCast* pMsg = new pb::BroadCast();
    pMsg->set_pid(Pid);
    pMsg->set_username(szName);
    pMsg->set_tp(2);

    pb::Position* pPostion = pMsg->mutable_p();
    pPostion->set_x(x);
    pPostion->set_y(y);
    pPostion->set_z(z);
    pPostion->set_v(v);

    GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pMsg);
    return pRet;
}

GameMsg* GameRole::CreateIDNameLogoff()
{
    auto pmsg = new pb::SyncPid();
    pmsg->set_pid(Pid);
    pmsg->set_username(szName);
    auto pRet = new GameMsg(GameMsg::MSG_TYPE_LOGOFF_ID_NAME, pmsg);
    return pRet;
}

GameRole::GameRole()
{
    szName = "Tom";
    x = 100;
    z = 100;
}

GameRole::~GameRole()
{
    
}

bool GameRole::Init()
{
    if (ZinxKernel::Zinx_GetAllRole().size() == 0)
    {
        TimerOutMng::getInstance().DelTask(&exit_Timer);
    }
    bool bRet = false;

    Pid = m_proto->m_channel->GetFd();

    szName = szName + std::to_string(Pid);
    x = x + random_engine() % 50;
    z = z + random_engine() % 50;

    bRet = world.Add_Player(this);

    if (bRet == true)
    {
		ZinxKernel::Zinx_SendOut(*CreateIDNameLogin(), *m_proto);
        ZinxKernel::Zinx_SendOut(*CreateSrdPlayerPosition(), *m_proto);
        for (auto single : world.GetSrdPlayerPosition(this))
		{
			ZinxKernel::Zinx_SendOut(*CreateSelfPosition(), *(dynamic_cast<GameRole*>(single)->m_proto));
        }
    }

    if (bRet == false)
    {
        std::cout << "GameRole::Init fail" << std::endl;
    }
    return bRet;
}

UserData* GameRole::ProcMsg(UserData& _poUserData)
{
    GET_REF2DATA(MultiMsg, multiMsg, _poUserData);
    for (auto single : multiMsg.m_msg_list)
    {
        /* 调试代码 */
        std::cout << "GameRole::ProcMsg() [google debug for multiMsg]" << std::endl;
        std::cout << single->pMsg->Utf8DebugString() << std::endl;

        // ZinxKernel::Zinx_SendOut(*single, *m_proto);
        // google::protobuf::Message* pmsg = NULL;

        if (single->enMsgType == GameMsg::MSG_TYPE_NEW_POITION)
        {
			auto pmsg_pos = dynamic_cast<pb::Position*>(single->pMsg);
			ProcNewPos(pmsg_pos->x(), pmsg_pos->y(), pmsg_pos->z(), pmsg_pos->v());

        }
        else if (single->enMsgType == GameMsg::MSG_TYPE_CHAT_CONTENT)
        {
			 auto pmsg_talk = dynamic_cast<pb::Talk*>(single->pMsg);
						 ProcChat(pmsg_talk->content());

        }
        /*switch (single->enMsgType)
        {
        case GameMsg::MSG_TYPE_NEW_POITION:
            auto pmsg_pos = dynamic_cast<pb::Position*>(single->pMsg);
            ProcNewPos(pmsg_pos->x(), pmsg_pos->y(), pmsg_pos->z(), pmsg_pos->v());
            break;*/
        // case GameMsg::MSG_TYPE_CHAT_CONTENT:
            // auto pmsg_talk = dynamic_cast<pb::Talk*>(single->pMsg);
            // ProcChat(pmsg_talk->content());
       /*     break;
        default:
            break;*/
        //}
    }

    return nullptr;
}

void GameRole::Fini()
{
    auto player_list = world.GetSrdPlayerPosition(this);
    world.Del_Player(this);
    for (auto single : player_list)
    {
        auto single_player = dynamic_cast<GameRole*>(single);
        ZinxKernel::Zinx_SendOut(*CreateIDNameLogoff(), *single_player->m_proto);
    }

    auto role_list = ZinxKernel::Zinx_GetAllRole();
    if (role_list.size() == 0)
    {
        TimerOutMng::getInstance().AddTask(&exit_Timer);
    }
}

float GameRole::GetX()
{
    return x;
}

float GameRole::GetY()
{
    return z;
}
