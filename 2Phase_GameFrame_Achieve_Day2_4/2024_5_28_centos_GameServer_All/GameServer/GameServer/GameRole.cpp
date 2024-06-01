#include "GameRole.h"
#include "GameProtocol.h"
#include "GameChannel.h"
#include "GameMsg.h"
#include "RandomName.h"
#include <random>

static AOIWorld world(0, 400, 0, 400, 20, 20);
RandomName randomName;

/* 随机位置 */
std::default_random_engine random_engine(time(NULL));
/* 随机姓名 */

bool GameRole::Init()
{
    /* 初始化姓名和Pid */
    m_Pid = m_protocol->m_channel->GetFd();
    // m_Name = std::string("Tom") + std::to_string(m_Pid); 改为随机姓名
    m_Name = randomName.GetName();
    // x = 50 + 10 * m_Pid; 改为随机位置
    // z = 50 + 10 * m_Pid;
    x = 100 + random_engine() % 50;
    z = 100 + random_engine() % 50;

    /* 将玩家添加到世界类 */
    world.Add_Player(this);

    /* 发送上线消息 */
    ZinxKernel::Zinx_SendOut(*CreateLoginIDName(), *m_protocol); // 有什么用 ??? 

    auto player_list = world.GetSrdPlayersPosition(this);



    /* 发送周围玩家位置给自己 */
    ZinxKernel::Zinx_SendOut(*CreateSrdPosition(), *m_protocol);

    /* ------------------------------------------------ */
    // 用上面的方法发不出去, 但这个方法就可以
    //for (auto single : player_list)
    //{
    //    auto player = dynamic_cast<GameRole*>(single);
    //    ZinxKernel::Zinx_SendOut(*player->CreateSelfPosition(), *m_protocol);
    //}
    /* ------------------------------------------------ */


    
    /* 发送自己位置给周围玩家 */
    for (auto single : player_list)
    {
        auto player = dynamic_cast<GameRole*>(single);
        ZinxKernel::Zinx_SendOut(*CreateSelfPosition(), *player->m_protocol);
    }

    return true;
}

UserData* GameRole::ProcMsg(UserData& _poUserData)
{
    /* 解析协议层传来的消息 */
    GET_REF2DATA(MultiMsg, _multiMsg, _poUserData);
    
    for (auto pGameMsg : _multiMsg.m_msg_list)
    {
        /* 处理GameMsg传来的对应的业务消息 */
        switch (pGameMsg->enMsgType)
        {
            /* 处理聊天 */
        case GameMsg::MSG_TYPE_CHAT_CONTENT:
            ProcChatMsg(pGameMsg->pMsg);
            break;
            /* 处理移动 */
        case GameMsg::MSG_TYPE_NEW_POSITION:
            ProcMoveMsg(pGameMsg->pMsg);
            break;
        default:
            break;
        }
    }

    return nullptr;
}

void GameRole::Fini()
{
    /* 释放姓名 */
    std::cout << "释放了一个姓名: " << m_Name << std::endl;
    randomName.ReleaseName(m_Name);
}

GameMsg* GameRole::CreateLoginIDName()
{
    pb::SyncPid* pMsg = new pb::SyncPid();
    pMsg->set_pid(m_Pid);
    pMsg->set_username(m_Name);

    std::cout << "(玩家上线) GameMsg::CreateLoginIDName (google debug): " << std::endl;
    std::cout << pMsg->Utf8DebugString() << std::endl;

    GameMsg* pRetMsg = new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID_NAME, pMsg);
    return pRetMsg;
}

GameMsg* GameRole::CreateSrdPosition()
{
    auto player_list = world.GetSrdPlayersPosition(this);
    pb::SyncPlayers* pMsg = new pb::SyncPlayers();
    auto msg_players = pMsg->mutable_ps();

    std::cout << "发送给" << m_Name << "的所有玩家" << std::endl;
    //for (auto single : player_list)
    //{
    //    auto player = dynamic_cast<GameRole*>(single);
    //    std::cout << "姓名: " << player->m_Name << std::endl;
    //}
    for (auto single : player_list)
    {
        auto player = dynamic_cast<GameRole*>(single);
        // pb::Player* msg_player = msg_players->Add();
        pb::Player* msg_player = msg_players->Add();
        msg_player->set_pid(player->m_Pid);
        msg_player->set_username(player->m_Name);
        pb::Position* msg_pos = msg_player->mutable_p();
        msg_pos->set_x(player->x);
        msg_pos->set_y(player->y);
        /* ------------------------------------------------------- */
        //msg_pos->set_z(player->v);
        //msg_pos->set_v(player->z);

        // 抽象拉满了, z 和 v 又搞错了
        msg_pos->set_z(player->z);
        msg_pos->set_v(player->v);
        /* ------------------------------------------------------- */

        std::cout << "(发送给自己的玩家) GameRole::CreateSrdPosition() google debug:" << std::endl;
        std::cout << msg_player->Utf8DebugString() << std::endl;
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
    auto msg_pos = pMsg->mutable_p();
    msg_pos->set_x(x);
    msg_pos->set_y(y);
    msg_pos->set_z(z);
    msg_pos->set_v(v);

    GameMsg* pRetMsg = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pMsg);
    return pRetMsg;
}

// void GameRole::ProcChatContent(pb::Talk* pMsg)
void GameRole::ProcChatMsg(google::protobuf::Message* _pMsg)
{
    auto pChatMsg = dynamic_cast<pb::Talk*>(_pMsg);
    std::string content = pChatMsg->contect();

    pb::BroadCast* pMsg = new pb::BroadCast();
    pMsg->set_pid(m_Pid);
    pMsg->set_username(m_Name);
    pMsg->set_tp(1);
    pMsg->set_content(content);

    auto player_list = world.GetSrdPlayersPosition(this);
    for (auto single : player_list)
    {
        auto player = dynamic_cast<GameRole*>(single);
        GameMsg* pGameMsg = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pMsg);
        ZinxKernel::Zinx_SendOut(*pGameMsg, *player->m_protocol);
    }
}

void GameRole::ProcMoveMsg(google::protobuf::Message* _pMsg)
{
    auto pMoveMsg = dynamic_cast<pb::Position*>(_pMsg);
    

    world.Del_Player(this);
    x = pMoveMsg->x();
    y = pMoveMsg->y();
    z = pMoveMsg->z();
    v = pMoveMsg->v();
    world.Add_Player(this);

    pb::BroadCast* pMsg = new pb::BroadCast();
    pMsg->set_pid(m_Pid);
    pMsg->set_username(m_Name);
    pMsg->set_tp(2);

    auto msg_pos = pMsg->mutable_p();
    msg_pos->set_x(x);
    msg_pos->set_y(y);
    msg_pos->set_z(z);
    msg_pos->set_v(v);

    auto player_list = world.GetSrdPlayersPosition(this);
    for (auto single : player_list)
    {
        auto player = dynamic_cast<GameRole*>(single);
        GameMsg* pGameMsg = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pMsg);
        ZinxKernel::Zinx_SendOut(*pGameMsg, *player->m_protocol);
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
