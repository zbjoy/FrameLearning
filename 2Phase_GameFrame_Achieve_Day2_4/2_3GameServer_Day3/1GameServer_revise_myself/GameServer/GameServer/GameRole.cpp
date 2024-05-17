#include "GameRole.h"
#include "GameProtocol.h"
#include "GameChannel.h"
#include "GameMsg.h"
// #include "AOIWorld.h"

static AOIWorld world(0, 400, 0, 400, 20, 20);

GameMsg* GameRole::CreateLoginNameID()
{
    auto pMsg = new pb::SyncPid();
    pMsg->set_pid(m_Pid);
    pMsg->set_username(m_Name);
    GameMsg* pRetMsg = new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID_NAME, pMsg);
    std::cout << "CreateLoginNameID(): google debug information:" << std::endl;
    std::cout << pMsg->Utf8DebugString();
    return pRetMsg;
}

GameMsg* GameRole::CreateSrdPlayersPosition()
{
    pb::SyncPlayers* pMsg = new pb::SyncPlayers();

    std::list<Player*> playerList = world.GetSrdPlayersPosition(this);

    for (auto single : playerList)
    {
		auto player = pMsg->add_ps();
        GameRole* tempRole = dynamic_cast<GameRole*>(single);
        player->set_pid(tempRole->m_Pid);
        player->set_username(tempRole->m_Name);
        auto pos = player->mutable_p();

        /* 为什么这里又没有y和z互换 */
        pos->set_x(tempRole->x);
        pos->set_y(tempRole->y);
        pos->set_z(tempRole->z);
        pos->set_v(tempRole->v);

    }
    GameMsg* pRetMsg = new GameMsg(GameMsg::MSG_TYPE_SRD_POSITION, pMsg);
    return pRetMsg;
}

GameMsg* GameRole::CreateSelfPosition()
{
    pb::BroadCast* pMsg = new pb::BroadCast();
    pMsg->set_pid(m_Pid);
    pMsg->set_username(m_Name);
    pMsg->set_tp(2);
    auto pos = pMsg->mutable_p();
    pos->set_x(x);
    pos->set_y(y);
    pos->set_z(z);
    pos->set_v(v);

    GameMsg* pRetMsg = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pMsg);
    return pRetMsg;
}

GameMsg* GameRole::CreateLogoffNameID()
{
    pb::SyncPid* pSyncPid = new pb::SyncPid();
    pSyncPid->set_pid(m_Pid);
    pSyncPid->set_username(m_Name);

    GameMsg* pRetMsg = new GameMsg(GameMsg::MSG_TYPE_LOGOFF_ID_NAME, pSyncPid);
    return pRetMsg;
}

void GameRole::ViewAppear(GameRole* _pRole)
{
    // ZinxKernel::Zinx_SendOut(*CreateLoginNameID(), *_pRole->m_proto);
    ZinxKernel::Zinx_SendOut(*CreateSelfPosition(), *_pRole->m_proto);
    // ZinxKernel::Zinx_SendOut(*_pRole->CreateLoginNameID(), *m_proto);
    ZinxKernel::Zinx_SendOut(*_pRole->CreateSelfPosition(), *m_proto);
}

void GameRole::ViewLost(GameRole* _pRole)
{
    ZinxKernel::Zinx_SendOut(*CreateLogoffNameID(), *_pRole->m_proto);
    ZinxKernel::Zinx_SendOut(*_pRole->CreateLogoffNameID(), *m_proto);
}

// void GameRole::ProcChatMsg(pb::Talk* pTalk)
void GameRole::ProcChatMsg(google::protobuf::Message* pMsg)
{
    pb::Talk* pTalk = dynamic_cast<pb::Talk*>(pMsg);
    std::string content = pTalk->content();

    std::cout << "GameRole::ProcChatMsg(): google debug:" << std::endl;
    std::cout << pTalk->Utf8DebugString();

    auto pGive = new pb::BroadCast();
    pGive->set_pid(m_Pid);
    pGive->set_username(m_Name);
    /* 聊天 */
    pGive->set_tp(1);
    pGive->set_content(content);

    /* 第一次不小心发成 */
    // GameMsg* pGameMsg = new GameMsg(GameMsg::MSG_TYPE_CHAT_CONTENT, pGive);
    // GameMsg* pGameMsg = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pGive);

    // ZinxKernel::Zinx_SendOut(*pGameMsg , *m_proto);
    auto player_list = world.GetSrdPlayersPosition(this);
    std::cout << "Pid: " << m_Pid << " 有" << player_list.size() << "个人在周围" << std::endl;
    std::cout << pGive->Utf8DebugString();
    for (auto single : player_list)
    {
        /* 为什么必须要把 pGameMsg new在 for 里面, 在外面服务器一发消息就挂了*/
		GameMsg* pGameMsg = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pGive);
        /*
            在 ZinxKernel::Zinx_SendOut() 中, 发完之后应该会 delete 掉pGameMsg, 再发就是空指针了 
        */
        auto pPlayer = dynamic_cast<GameRole*>(single);
        std::cout << "Pid: " << pPlayer->m_Pid << std::endl;
        std::cout << "姓名: " << pPlayer->m_Name << std::endl;;
        //if (pPlayer == NULL)
        //{
            //std::cout << "pPlayer == NULL" << std::endl;
        //}
        //if (pPlayer->m_proto == NULL)
        //{
        //    std::cout << "pPlayer->m_proto == NULL" << std::endl;
        //}
        ZinxKernel::Zinx_SendOut(*pGameMsg, *pPlayer->m_proto);
    }
}

void GameRole::ProcNewPos(google::protobuf::Message* pMsg)
{
    /* 处理新位置 */
    std::cout << m_Name << "移动了" << std::endl;

    /* 对比旧位置得到要消失的视野 */
    auto s1 = world.GetSrdPlayersPosition(this);
    
    pb::Position* pPos = dynamic_cast<pb::Position*>(pMsg);

    world.Del_Player(this);
    x = pPos->x();
    y = pPos->y();
    z = pPos->z();
    v = pPos->v();

    world.Add_Player(this);

    /* 获得新位置 */
    auto s2 = world.GetSrdPlayersPosition(this);

    /* 要消失的 */
    for (auto single : s1)
    {
        if (std::find(s2.begin(), s2.end(), single) == s2.end())
        {
            ViewLost(dynamic_cast<GameRole*>(single));
        }
    }

    /* 要出现的 */
    for (auto single : s2)
    {
        if (std::find(s1.begin(), s1.end(), single) == s1.end())
        {
            ViewAppear(dynamic_cast<GameRole*>(single));
        }
    }

    pb::BroadCast* pBroadCast = new pb::BroadCast();
    pBroadCast->set_pid(m_Pid);
    pBroadCast->set_username(m_Name);
    pBroadCast->set_tp(2);
    auto p = pBroadCast->mutable_p();
    p->set_x(x);
    p->set_y(y);
    p->set_z(z);
    p->set_v(v);


    for (auto single : world.GetSrdPlayersPosition(this))
    {
        /* 应该发pBroadCast, 发成了pPos */
		auto pGameMsg = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pBroadCast);
        auto pPlayer = dynamic_cast<GameRole*>(single);
        std::cout << "向" << pPlayer->m_Name << "发送了移动消息" << std::endl;
        ZinxKernel::Zinx_SendOut(*pGameMsg, *pPlayer->m_proto);
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

bool GameRole::Init()
{
    m_Pid = m_proto->m_channel->GetFd();
    // m_Name = "Tom" + m_Pid 是错的, 但不知道为什么
    m_Name = std::string("Tom") + std::to_string(m_Pid);

    /* 初始化位置 */
    x = 100 + 3 * m_Pid;
    z = 100 + 3 * m_Pid;

    world.Add_Player(this);

    std::cout << "玩家: " << m_Name << "Pid: " << m_Pid << std::endl;
    std::cout << "x: " << x << " y: " << z << std::endl;
    std::cout << "应在的grid: " << ((int)GetX() - 0) / 20 + ((int)GetY() - 0) / 20 * 20 << std::endl;

    /* 发送登录时的姓名和Pid给客户端 */
    ZinxKernel::Zinx_SendOut(*CreateLoginNameID(), *m_proto);
    
    /* 发送周围玩家的上线信息给客户端 */
    ZinxKernel::Zinx_SendOut(*CreateSrdPlayersPosition(), *m_proto);

    /* 把自己上线的消息发给周围玩家 */
    std::list<Player*> playerList = world.GetSrdPlayersPosition(this);
    for (auto single : playerList)
    {
        auto player = dynamic_cast<GameRole*>(single);
        ZinxKernel::Zinx_SendOut(*CreateSelfPosition(), *player->m_proto);
    }

    return true;
}

UserData* GameRole::ProcMsg(UserData& _poUserData)
{
    GameMsg* pRetMsg = nullptr;
    GET_REF2DATA(MultiMsg, multiMsg, _poUserData);

    /* 同步消息 */
    for (auto single : multiMsg.m_msg_list)
    {
        google::protobuf::Message* pMsg = single->pMsg;
        std::cout << "GameRole::ProcMsg(): google debug:" << std::endl;
        std::cout << pMsg->Utf8DebugString();
        switch (single->enMsgType)
        {
        case GameMsg::MSG_TYPE_NEW_POITION:
            ProcNewPos(pMsg);
            break;
        case GameMsg::MSG_TYPE_CHAT_CONTENT:
            ProcChatMsg(pMsg);
            break;
        default:
            break;
        }
    }

    return NULL;
}

void GameRole::Fini()
{
    std::list<Player*> player_list = world.GetSrdPlayersPosition(this);
    world.Del_Player(this);
    for (auto single : player_list)
    {
        auto pPlayer = dynamic_cast<GameRole*>(single);
		ZinxKernel::Zinx_SendOut(*CreateLogoffNameID(), *(pPlayer->m_proto));
    }
}
